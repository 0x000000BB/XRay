#include "RayTracer.h"

namespace XRay {

    Color RayTracer::RayColor(const Ray& r, const Color& background, const Scene& scene, std::shared_ptr<Hittable>& lights, int depth) {
        HitRecord rec;

        // If we've exceeded the ray bounce limit, no more light is gathered.
        if (depth <= 0)
            return Color(0, 0, 0);

        // If the ray hits nothing, return the background color.
        if (!scene.hit(r, 0.001, infinity, rec))
            return background;

        Ray scattered;
        Color emitted = rec.mat_ptr->emitted(r, rec, rec.u, rec.v, rec.p);
        double pdf_val;
        Color albedo;
        if (!rec.mat_ptr->scatter(r, rec, albedo, scattered, pdf_val))
            return emitted;

        if (lights == nullptr) {
            return emitted + albedo * RayColor(scattered, background, scene, lights, depth - 1);
        }

        auto p0 = make_shared<HittablePdf>(lights, rec.p);
        auto p1 = make_shared<CosinePdf>(rec.normal);
        MixturePdf mixed_pdf(p0, p1);

        scattered = Ray(rec.p, mixed_pdf.generate(), r.time());
        pdf_val = mixed_pdf.value(scattered.direction());


        return emitted
            + albedo * rec.mat_ptr->scattering_pdf(r, rec, scattered)
            * RayColor(scattered, background, scene, lights, depth - 1) / pdf_val;
    }

	void RayTracer::render(Framebuffer fb, int width, int height, int startHeight, int endHeight, int samples, const Scene& scene, int depth, std::shared_ptr<XRay::Hittable> lights) {
        Camera camera = scene.camera;
        Color background = scene.backgroundColor;
        for (int j = endHeight; j > startHeight; --j) {
            for (int i = 0; i < width; ++i) {
                Color color(0, 0, 0);
                for (int s = 0; s < samples; s++) {
                    auto u = (double(i) + random_double()) / (width - 1);
                    auto v = (double(j) + random_double()) / (height - 1);
                    Ray r = camera.get_ray(u, v);
                    color += RayColor(r, background, scene, lights, depth);
                }
                auto scale = 1.0 / samples;
                color.setR(sqrt(scale * color.r()));
                color.setG(sqrt(scale * color.g()));
                color.setB(sqrt(scale * color.b()));
                fb[(j- 1) * width + i] = color;
            }
        }

	}

	Framebuffer RayTracer::createFrameBuffer(int width, int heigth) {
		int fb_size = width * heigth * sizeof(Color);
		Framebuffer fb = (Framebuffer)malloc(fb_size);
		return fb;
	}

}