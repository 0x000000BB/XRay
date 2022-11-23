#pragma once

#include "../Core/XRay.h"

#include "Sphere.h"
#include "Scene.h"
#include "Camera.h"
#include "Material.h"
#include "AARect.h"
#include "Pdf.h"


namespace XRay {


	class RayTracer {

	public:
		RayTracer() = default;

		void render(Framebuffer fb, int width, int height, int startHeight, int endHeight, int samples, const Scene& scene, int depth, std::shared_ptr<XRay::Hittable> lights);
		Framebuffer createFrameBuffer(int width, int height);

	private:
		Color RayColor(const Ray& r, const Color& background, const Scene& scene, std::shared_ptr<Hittable>& lights, int depth);

	};

}