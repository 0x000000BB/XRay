#pragma warning(disable:4996)

#include "Core/Window.h"

#include "Renderer/RayTracer.h"
#include "Renderer/Scene.h"
#include "Renderer/Sphere.h"
#include "Renderer/Material.h"
#include "Renderer/Camera.h"
#include "Renderer/MovingSphere.h"
#include "Renderer/Texture.h"
#include "Scenes.h"

#include "GUI/ImGuiRendererer.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"


#include <thread>
#include <vector>
#include <iostream>
#include <stdio.h>
#include <filesystem>
#include <string>

XRay::RayTracer renderer;

bool rendering1 = false;
bool rendering2 = false;
bool rendering3 = false;
bool rendering4 = false;
bool saving = false;
bool hasRendered = false;

std::string ReplaceAll(std::string str, const std::string& from, const std::string& to) {
    size_t start_pos = 0;
    while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
    }
    return str;
}

float max(float value, float max) {

    if (value < max) {
        return value;
    }

    return max;
}

void setRendering(int index, bool value) {
    if (index == 1)
        rendering1 = value;
    if (index == 2)
        rendering2 = value;
    if (index == 3)
        rendering3 = value;
    if (index == 4)
        rendering4 = value;
}

void render(Framebuffer fb, int width, int height, int startHeight, int endHeight, int samples, const XRay::Scene& scene, const XRay::Camera& camera, int depth, int threadIndex, const Color& background) {
    setRendering(threadIndex, true);
    hasRendered = true;
    renderer.render(fb, width, height, startHeight, endHeight, samples, scene, camera, depth, background);
    setRendering(threadIndex, false);
}

void saveImage(Framebuffer fb, std::string filePath, int nx, int ny) {
    FILE* file = fopen(filePath.c_str(), "w");

    std::string x = std::to_string(nx);
    char const* nxchar = x.c_str();
    std::string y = std::to_string(ny);
    char const* nychar = y.c_str();

    fprintf(file, "P3\n");
    fprintf(file, nxchar);
    fprintf(file, " ");
    fprintf(file, nychar);
    fprintf(file, "\n255\n");
    for (int j = ny - 1; j >= 0; j--) {
        for (int i = 0; i < nx; i++) {
            size_t pixel_index = j * nx + i;
            float r = max(fb[pixel_index].x(), 1);
            float g = max(fb[pixel_index].y(), 1);
            float b = max(fb[pixel_index].z(), 1);
            int ir = int(255.99 * r);
            int ig = int(255.99 * g);
            int ib = int(255.99 * b);
            std::string rs = std::to_string(ir);
            char const* rchar = rs.c_str();
            std::string gs = std::to_string(ig);
            char const* gchar = gs.c_str();
            std::string bs = std::to_string(ib);
            char const* bchar = bs.c_str();
            //std::cout << ir << " " << ig << " " << ib << "\n";
            fprintf(file, rchar);
            fprintf(file, " ");
            fprintf(file, gchar);
            fprintf(file, " ");
            fprintf(file, bchar);
            fprintf(file, "\n");
        }
    }

    std::cout << "finished saving!" << std::endl;
}

int main() {

	XRay::Window window(1600, 900, "X-Ray");
    XRay::ImGuiRenderer imGuiRenderer;

    imGuiRenderer.init(window);

    auto aspect_ratio = 3.0 / 2.0;
    int image_width = 1200;
    int image_height = static_cast<int>(image_width / aspect_ratio);
    int samples_per_pixel = 500;
    int max_depth = 50;

    //auto material_ground = make_shared<XRay::Lambertian>(Color(0.8, 0.8, 0.0));
    //auto material_center = make_shared<XRay::Lambertian>(Color(0.1, 0.2, 0.5));
    //auto material_left = make_shared<XRay::Dielectric>(1.5);
    //auto material_right = make_shared<XRay::Metal>(Color(0.8, 0.6, 0.2), 0.0);
    //
    //scene.add(make_shared<XRay::Sphere>(vec3(0.0, -100.5, -1.0), 100.0, material_ground));
    //scene.add(make_shared<XRay::Sphere>(vec3(0.0, 0.0, -1.0), 0.5, material_center));
    //scene.add(make_shared<XRay::Sphere>(vec3(-1.0, 0.0, -1.0), 0.5, material_left));
    //scene.add(make_shared<XRay::Sphere>(vec3(-1.0, 0.0, -1.0), -0.4, material_left));
    //scene.add(make_shared<XRay::Sphere>(vec3(1.0, 0.0, -1.0), 0.5, material_right));


    //vec3 lookfrom(13, 2, 3);
    //vec3 lookat(0, 0, 0);
    //vec3 vup(0, 1, 0);
    //auto dist_to_focus = 10.0;
    //auto aperture = 0.1;

    
    XRay::Camera cam;
    Color background = Color(0.70, 0.80, 1.00);

    XRay::Scene scene = simple_light(cam, background);

    Framebuffer fb = renderer.createFrameBuffer(image_width, image_height);

    const unsigned int threadCount = std::thread::hardware_concurrency() / 2;
    std::thread renderThread1;
    std::thread renderThread2;
    std::thread renderThread3;
    std::thread renderThread4;

    std::thread saveThread;


    while (!window.shouldClose())
    {

        if (renderThread1.joinable() && !rendering1) {
            renderThread1.join();
        }
        if (renderThread2.joinable() && !rendering2) {
            renderThread2.join();
        }
        if (renderThread3.joinable() && !rendering3) {
            renderThread3.join();
        }
        if (renderThread4.joinable() && !rendering4) {
            renderThread4.join();
        }

        if (saveThread.joinable() && !saving) {
            saveThread.join();
        }

        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image_width, image_height, 0, GL_RGB, GL_FLOAT, fb);
        glGenerateMipmap(GL_TEXTURE_2D);

        imGuiRenderer.Begin();
        imGuiRenderer.renderViewport((ImTextureID)texture, image_width, image_height);
        bool renderPressed = false;
        bool savePressed = false;
        char* filename = new char[512]; 
        strcpy(filename, "image.ppm");
        imGuiRenderer.renderSettings(renderPressed, savePressed, filename, samples_per_pixel, max_depth, scene);
        if (renderPressed) {
            if(renderThread1.joinable() && !rendering1)
                renderThread1.detach();
            if (renderThread2.joinable() && !rendering2)
                renderThread2.detach();
            if (renderThread3.joinable() && !rendering3)
                renderThread3.detach();
            if (renderThread4.joinable() && !rendering4)
                renderThread4.detach();
            fb = renderer.createFrameBuffer(image_width, image_height);
            int height_per_thread = image_height / 4;
            renderThread1 = std::thread(render, fb, image_width, image_height, 0, height_per_thread, samples_per_pixel, scene , cam, max_depth, 1, background);
            renderThread2 = std::thread(render, fb, image_width, image_height, height_per_thread, 2 * height_per_thread, samples_per_pixel, scene, cam, max_depth, 2, background);
            renderThread3 = std::thread(render, fb, image_width, image_height, 2 * height_per_thread, 3 * height_per_thread, samples_per_pixel, scene, cam, max_depth, 3, background);
            renderThread4 = std::thread(render, fb, image_width, image_height, image_height - 3 * height_per_thread, image_height, samples_per_pixel, scene, cam, max_depth, 4, background);

        }
        if (savePressed && !saving) {
            if (saveThread.joinable())
                saveThread.detach();
            std::filesystem::path path = std::filesystem::current_path(); 
            path += "/Examples/lights.ppm";
            saveThread = std::thread(saveImage, fb, path.generic_string(), image_width, image_height);
        }

        imGuiRenderer.End(window);

        window.swapBuffers();

        window.pollEvents();
    }

    if (renderThread1.joinable() ) {
        renderThread1.detach();
    }
    if (renderThread2.joinable()) {
        renderThread2.detach();
    }
    if (renderThread3.joinable()) {
        renderThread3.detach();
    }
    if (renderThread4.joinable()) {
        renderThread4.detach();
    }
    if (saveThread.joinable()) {
        saveThread.detach();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    window.terminate();

    return 0;
}