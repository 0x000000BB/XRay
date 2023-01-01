#include "ImGuiRendererer.h"

namespace XRay {

	void ImGuiRenderer::init(Window window) {
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.DisplaySize = ImVec2(window.width, window.height);

        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Window
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

	void ImGuiRenderer::renderViewport(ViewportInfo viewportInfo) {
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar;
        bool open = true;
        ImGui::Begin("Viewport", &open, window_flags);
        ImGui::Image(viewportInfo.texture, ImVec2(viewportInfo.width, viewportInfo.height), ImVec2(0, 1), ImVec2(1, 0));
        ImGui::End();
	}

    std::array<std::string, 5> items = { "Lambertian", "Metal", "Dielectric", "Diffuse Light", "Isotropic" };

    MaterialTyp stringToTyp(std::string s) {
        if (s == "Lambertian") return LambertianT;
        else if (s == "Metal") return MetalT;
        else if (s == "Dielectric") return DielectricT;
        else if (s == "Diffuse Light") return DiffuseLightT;
        else if (s == "Isotropic") return IsotropicT;
        return LambertianT;
    }

    void ImGuiRenderer::renderObjectSettings() {
        ImGui::PushID(selectedIndex);
        ImGui::Begin("Object");
        if (selectedObject) {
            std::shared_ptr<Hittable> object = selectedObject;
            Material* material = object->mat_ptr.get();
            ImGui::Text(object->name.c_str());
            if (material) {
                std::string current_item = items[material->materialTyp];

                if (ImGui::BeginCombo("##combo", current_item.c_str())) 
                {
                    for (int n = 0; n < items.size(); n++)
                    {
                        bool is_selected = (current_item == items[n]);
                        if (ImGui::Selectable(items[n].c_str(), is_selected)) {
                            current_item = items[n];
                            is_selected = (current_item == items[n]);
                            if (is_selected) {
                                switch (stringToTyp(current_item)) {
                                case LambertianT:
                                    object->mat_ptr = make_shared<Lambertian>();
                                    break;
                                case MetalT:
                                    object->mat_ptr = make_shared<Metal>();
                                    break;
                                case DielectricT:
                                    object->mat_ptr = make_shared<Dielectric>();
                                    break;
                                case DiffuseLightT:
                                    object->mat_ptr = make_shared<DiffuseLight>();
                                    break;
                                case IsotropicT:
                                    object->mat_ptr = make_shared<Isotropic>();
                                    break;
                                default:
                                    break;
                                }
                                ImGui::SetItemDefaultFocus();
                            }
                        }
                    }
                    ImGui::EndCombo();
                }
                if (material->materialTyp == LambertianT) {
                    
                    Lambertian* m = static_cast<Lambertian*>(material);
                    Texture* texture = static_cast<Texture*>(m->albedo.get());
                    if (texture->typ == SolidColorT) {
                        SolidColor* t = static_cast<SolidColor*>(m->albedo.get());
                        Color color = t->colorValue;
                        float c[3] = { color.r(), color.g(), color.b() };
                        ImGui::ColorPicker3("Albedo", c);
                        t->colorValue = Color(c[0], c[1], c[2]);
                    }

                    else if (texture->typ == ImageTextureT) {
                        ImageTexture* t = static_cast<ImageTexture*>(m->albedo.get());
                        std::string filepath = t->filepath;
                        ImGui::Text(filepath.c_str());
                    }

                    else if (texture->typ == CheckerT) {
                        CheckerTexture* t = static_cast<CheckerTexture*>(m->albedo.get());
                        Texture* oddTexture = static_cast<Texture*>(t->odd.get());
                        Texture* evenTexture = static_cast<Texture*>(t->even.get());
                        if (ImGui::CollapsingHeader("Texture 1")) {
                            if (oddTexture->typ == SolidColorT) {
                                SolidColor* t = static_cast<SolidColor*>(oddTexture);
                                Color color = t->colorValue;
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
                                float c[3] = { color.r(), color.g(), color.b() };
                                ImGui::ColorPicker3("Texture even", c);
                                t->colorValue = Color(c[0], c[1], c[2]);
                            }
                        }
                    }
                    

                }

                else if (material->materialTyp == MetalT) {
                    Metal* m = static_cast<Metal*>(material);
                    Color color = m->albedo;
                    float c[3] = { color.r(), color.g(), color.b() };
                    ImGui::ColorPicker3("Albedo ", c);
                    m->albedo = Color(c[0], c[1], c[2]);
                    ImGui::SliderFloat("Fuzz", &(m->fuzz), 0, 1);
                    
                }

                else if (material->materialTyp == DielectricT) {
                    
                    Dielectric* m = static_cast<Dielectric*>(material);
                    float ir = m->ir;
                    ImGui::SliderFloat("Ir", &ir, 0, 5);
                    
                }

                else if (material->materialTyp == DiffuseLightT) {
                   
                    DiffuseLight* m = static_cast<DiffuseLight*>(material);
                    Texture* texture = static_cast<Texture*>(m->emit.get());
                    if (texture->typ == SolidColorT) {
                        SolidColor* t = static_cast<SolidColor*>(m->emit.get());
                        Color color = t->colorValue;
                        float c[3] = { color.r(), color.g(), color.b() };
                        ImGui::ColorPicker3("Albedo", c);
                        t->colorValue = Color(c[0], c[1], c[2]);
                        ImGui::SliderFloat("Intensity", &(m->intensity), 0.1, 20);
                    }
                    
                }

                else if (material->materialTyp == IsotropicT && object) {
                    Isotropic* m = static_cast<Isotropic*>(material);
                    Texture* texture = static_cast<Texture*>(m->albedo.get());
                    if (texture->typ == SolidColorT) {
                        SolidColor* t = static_cast<SolidColor*>(m->albedo.get());
                        Color color = t->colorValue;
                        float c[3] = { color.r(), color.g(), color.b() };
                        ImGui::ColorPicker3("Albedo", c);
                        t->colorValue = Color(c[0], c[1], c[2]);
                    }
                }
            }
        }
        ImGui::End();
        ImGui::PopID();
    }

    void ImGuiRenderer::renderObjects(XRay::Scene& scene) {
        ImGui::Begin("Objects");
        for (int i = 0; i < scene.objects.size(); i++) {
            std::shared_ptr<Hittable> hittable = scene.objects.at(i);
            bool selected = false;
            if (hittable == selectedObject) {
                selected = true;
            }
            ImGui::Selectable(hittable.get()->name.c_str(), &selected);
            if (selected) {
                selectedObject = hittable;
                selectedIndex = i;
            }
        }
        ImGui::End();
    }

    GuiInfo ImGuiRenderer::render(ViewportInfo viewportInfo, Window window, XRay::Scene& scene) {
        GuiInfo result;
        Begin();
        renderViewport(viewportInfo);
        renderMenuBar();
        renderObjects(scene);
        
        renderObjectSettings();
        End(window);
        return result;
    }


    void ImGuiRenderer::renderSettings(bool& renderButtonPressed, bool& saveButtonPressed, bool& openButtonPressed, char* filename, int& samples, int& depth, Scene& scene) {

        ImGui::Begin("Settings");
        renderButtonPressed = ImGui::Button("Render");
        saveButtonPressed = ImGui::Button("Save");
        openButtonPressed = ImGui::Button("Open");
        ImGui::Separator();
        ImGui::InputInt("Samples", &samples);
        ImGui::SliderInt("Depth", &depth, 1, 50);
        ImGui::Separator();
        for (int i = 0; i < scene.objects.size(); i++) {
            ImGui::PushID(i);
            
            ImGui::PopID();
            ImGui::Separator();
            ImGui::Spacing();
        }
        ImGui::End();
    }

    void ImGuiRenderer::renderMenuBar() {
        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Open"))
                {
                    //Do something
                }
                if (ImGui::MenuItem("Save"))
                {
                    //Do something
                }
                if (ImGui::MenuItem("New"))
                {
                    //Do something
                }
                ImGui::EndMenu();
            }

            ImGui::EndMainMenuBar();
        }
        //ImGui::NewLine();
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