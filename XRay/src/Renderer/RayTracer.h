#pragma once

#include "../Core/XRay.h"

#include "Sphere.h"
#include "Scene.h"
#include "Camera.h"
#include "Material.h"


namespace XRay {


	class RayTracer {

	public:
		RayTracer() = default;

		void render(Framebuffer fb, int width, int height, int startHeight, int endHeight, int samples, const Scene& scene, int depth);
		Framebuffer createFrameBuffer(int width, int height);

	private:
		Color RayColor(const Ray& r, const Color& background, const Scene& scene, int depth);

	};

}