#pragma once

#include "../Core/Window.h"

#include "../Renderer/Scene.h"
#include "../Renderer/Material.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <array>

namespace XRay {

	typedef struct {

	}GuiInfo;

	typedef struct {
		ImTextureID texture;
		int width;
		int height;
	}ViewportInfo;

	class ImGuiRenderer {
	public:
		GuiInfo render(ViewportInfo info, Window window, XRay::Scene& scene);
		void init(Window window);

		void renderSettings(bool& renderButtonPressed, bool& saveButtonPressed, bool& openButtonPressed, char* filename, int& samples, int& depth, Scene& scene);

	private:
		void renderViewport(ViewportInfo viewportInfo);
		void Begin();
		void renderMenuBar();
		void End(Window window);
		void renderObjects(XRay::Scene& scene);
		void renderObjectSettings();

		std::shared_ptr<Hittable> selectedObject;
		int selectedIndex;
	};

}