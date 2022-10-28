#include "ImGuiRendererer.h"

namespace XRay {

	void ImGuiRenderer::init(Window window) {
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.DisplaySize = ImVec2(window.width, window.height);

        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
        io.ConfigViewportsNoAutoMerge = true;
        io.ConfigViewportsNoTaskBarIcon = true;

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        //ImGui::StyleColorsLight();

        // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
        ImGuiStyle& style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        // Setup Platform/Renderer backends
        ImGui_ImplGlfw_InitForOpenGL(window.handel, true);
        ImGui_ImplOpenGL3_Init();
	}

	void ImGuiRenderer::renderViewport(ImTextureID texture, int image_width, int image_height) {
        ImGuiIO& io = ImGui::GetIO();
        ImGui::Begin("Viewport");
        ImGui::Image(texture, ImVec2(image_width, image_height), ImVec2(0, 1), ImVec2(1, 0));
        ImGui::End();
	}


    void ImGuiRenderer::renderSettings(bool& renderButtonPressed, bool& saveButtonPressed, char* filename, int& samples, int& depth, Scene& scene) {
        ImGui::Begin("Settings");
        renderButtonPressed = ImGui::Button("Render");
        saveButtonPressed = ImGui::Button("Save");
        ImGui::Separator();
        ImGui::SliderInt("Samples", &samples, 1, 1000);
        ImGui::SliderInt("Depth", &depth, 1, 50);
        ImGui::Separator();
        Material* material;
        for (int i = 0; i < scene.objects.size(); i++) {
            ImGui::PushID(i);
            std::shared_ptr<Hittable> object = scene.objects.at(i);
            material = object->mat_ptr.get();
            if (ImGui::CollapsingHeader(object->name.c_str())) {
                if (material) {
                    ImGui::Dummy(ImVec2(25.0f, 10.0f));
                    ImGui::SameLine();
                    if (material->materialTyp == LambertianT) {
                        if (ImGui::CollapsingHeader("Lambertian")) {
                            Lambertian* m = static_cast<Lambertian*>(material);
                            Texture* texture = static_cast<Texture*>(m->albedo.get());
                            if (texture->typ == SolidColorT) {
                                SolidColor* t = static_cast<SolidColor*>(m->albedo.get());
                                Color color = t->colorValue;
                                ImGui::Dummy(ImVec2(45.0f, 10.0f));
                                ImGui::SameLine();
                                float c[3] = { color.r(), color.g(), color.b() };
                                ImGui::ColorPicker3("Albedo", c);
                                t->colorValue = Color(c[0], c[1], c[2]);
                            }

                            else if (texture->typ == ImageTextureT) {
                                ImageTexture* t = static_cast<ImageTexture*>(m->albedo.get());
                                std::string filepath = t->filepath;
                                ImGui::Dummy(ImVec2(45.0f, 10.0f));
                                ImGui::SameLine();
                                ImGui::Text(filepath.c_str());
                            }
                        
                            else if (texture->typ == CheckerT) {
                                CheckerTexture* t = static_cast<CheckerTexture*>(m->albedo.get());
                                Texture* oddTexture = static_cast<Texture*>(t->odd.get());
                                Texture* evenTexture = static_cast<Texture*>(t->even.get());
                                ImGui::Dummy(ImVec2(33.0f, 10.0f));
                                ImGui::SameLine();
                                if (ImGui::CollapsingHeader("Texture 1")) {
                                    if (oddTexture->typ == SolidColorT) {
                                        SolidColor* t = static_cast<SolidColor*>(oddTexture);
                                        Color color = t->colorValue;
                                        ImGui::Dummy(ImVec2(45.0f, 10.0f));
                                        ImGui::SameLine();
                                        float c[3] = { color.r(), color.g(), color.b() };
                                        ImGui::ColorPicker3("Texture odd", c);
                                        t->colorValue = Color(c[0], c[1], c[2]);
                                    }
                                }
                                ImGui::Dummy(ImVec2(33.0f, 10.0f));
                                ImGui::SameLine();
                                if (ImGui::CollapsingHeader("Texture 2")) {
                                    if (evenTexture->typ == SolidColorT) {
                                        SolidColor* t = static_cast<SolidColor*>(evenTexture);
                                        Color color = t->colorValue;
                                        ImGui::Dummy(ImVec2(45.0f, 10.0f));
                                        ImGui::SameLine();
                                        float c[3] = { color.r(), color.g(), color.b() };
                                        ImGui::ColorPicker3("Texture even", c);
                                        t->colorValue = Color(c[0], c[1], c[2]);
                                    }
                                }
                            }
                        }

                    }

                    else if (material->materialTyp == MetalT) {
                        if (ImGui::CollapsingHeader("Metal")) {
                            Metal* m = static_cast<Metal*>(material);
                            Color color = m->albedo;
                            ImGui::Dummy(ImVec2(45.0f, 10.0f));
                            ImGui::SameLine();
                            float c[3] = { color.r(), color.g(), color.b() };
                            ImGui::ColorPicker3("Albedo " + i, c);
                            m->albedo = Color(c[0], c[1], c[2]);
                            ImGui::Dummy(ImVec2(45.0f, 10.0f));
                            ImGui::SameLine();
                            ImGui::SliderFloat("Fuzz", &(m->fuzz), 0, 1);
                        }
                    }

                    else if (material->materialTyp == DielectricT) {
                        if (ImGui::CollapsingHeader("Dielectric")) {
                            Dielectric* m = static_cast<Dielectric*>(material);
                            float ir = m->ir;
                            ImGui::Dummy(ImVec2(45.0f, 10.0f));
                            ImGui::SameLine();
                            ImGui::SliderFloat("Ir", &ir, 0, 5);
                        }
                    }
                }
            }
            ImGui::PopID();
            ImGui::Separator();
            ImGui::Spacing();
        }
        ImGui::End();
    }

    void ImGuiRenderer::Begin() {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::DockSpaceOverViewport();
    }

    void ImGuiRenderer::End(Window window) {
        ImGuiIO& io = ImGui::GetIO();
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window.handel, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.8f, 0.5f, 0.3f, 1.0f);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        //
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
    }

}