#include "RayTracer.h"

namespace XRay {

    Color RayTracer::RayColor(const Ray& r, const Scene& scene, int depth) {
        HitRecord rec;

        if (depth <= 0)
            return Color(0, 0, 0);

        if (scene.hit(r, 0.001, infinity, rec)) {
            Ray scattered;
            Color attenuation;
            if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
                return attenuation * RayColor(scattered, scene, depth - 1);
            return Color(0, 0, 0);
        }
        vec3 unit_direction = unit_vector(r.direction());
        auto t = 0.5 * (unit_direction.y() + 1.0);
        return (1.0 - t) * Color(1.0, 1.0, 1.0) + t * Color(0.5, 0.7, 1.0);
    }

	void RayTracer::render(Framebuffer fb, int width, int height, int startHeight, int endHeight, int samples, const Scene& scene, const Camera& camera, int depth) {
        for (int j = endHeight; j > startHeight; --j) {
            for (int i = 0; i < width; ++i) {
                Color color(0, 0, 0);
                for (int s = 0; s < samples; s++) {
                    auto u = (double(i) + random_double()) / (width - 1);
                    auto v = (double(j) + random_double()) / (height - 1);
                    Ray r = camera.get_ray(u, v);
                    color += RayColor(r, scene, depth);                  
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