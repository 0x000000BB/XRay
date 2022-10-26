#pragma once

#include "../Core/Window.h"

#include "../Renderer/Scene.h"
#include "../Renderer/Material.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

namespace XRay {

	class ImGuiRenderer {
	public:
		void init(Window window);
		void Begin();
		void renderViewport(ImTextureID texture, int image_width, int image_height);
		void renderSettings(bool& renderButtonPressed, bool& saveButtonPressed, int& samples, int& depth, Scene& scene);
		void End(Window window);
	};

}