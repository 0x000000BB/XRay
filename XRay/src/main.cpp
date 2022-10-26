#pragma warning(disable:4996)

#include "Core/Window.h"

#include "Renderer/RayTracer.h"
#include "Renderer/Scene.h"
#include "Renderer/Sphere.h"
#include "Renderer/Material.h"
#include "Renderer/Camera.h"
#include "Renderer/MovingSphere.h"
#include "Renderer/Texture.h"

#include "GUI/ImGuiRendererer.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <thread>
#include <vector>
#include <iostream>
#include <stdio.h>

XRay::RayTracer renderer;

bool rendering1 = false;
bool rendering2 = false;
bool rendering3 = false;
bool rendering4 = false;
bool saving = false;
bool hasRendered = false;

XRay::Scene random_scene() {
    XRay::Scene scene;

    auto checker = make_shared<XRay::CheckerTexture>(Color(0.2, 0.3, 0.1), Color(0.9, 0.9, 0.9));
    scene.add(make_shared<XRay::Sphere>(vec3(0, -1000, 0), 1000, make_shared<XRay::Lambertian>(checker), "Ground"));

    int sphereCount = 0;

    for (int a = 0; a < 6; a++) {
        for (int b = 0; b < 6; b++) {
            auto choose_mat = random_double();
            vec3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

            if ((center - vec3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<XRay::Material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = Color::random() * Color::random();
                    sphere_material = make_shared<XRay::Lambertian>(albedo);
                    auto center2 = center + vec3(0, random_double(0, .5), 0);
                    scene.add(make_shared<XRay::MovingSphere>(
                        center, center2, 0.0, 1.0, 0.2, sphere_material, std::string("Sphere " + std::to_string(sphereCount))));
                    sphereCount++;
                }
                else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = Color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<XRay::Metal>(albedo, fuzz);
                    scene.add(make_shared<XRay::Sphere>(center, 0.2, sphere_material, std::string("Sphere " + std::to_string(sphereCount))));
                    sphereCount++;
                }
                else {
                    // glass
                    sphere_material = make_shared<XRay::Dielectric>(1.5);
                    scene.add(make_shared<XRay::Sphere>(center, 0.2, sphere_material, std::string("Sphere " + std::to_string(sphereCount))));
                    sphereCount++;
                }
            }
        }
    }

    auto material1 = make_shared<XRay::Dielectric>(1.5);
    scene.add(make_shared<XRay::Sphere>(vec3(0, 1, 0), 1.0, material1, "Dielectric Sphere"));

    auto material2 = make_shared<XRay::Lambertian>(Color(0.4, 0.2, 0.1));
    scene.add(make_shared<XRay::Sphere>(vec3(-4, 1, 0), 1.0, material2, "Lambertian Sphere"));

    //auto material3 = make_shared<XRay::Metal>(Color(0.7, 0.6, 0.5), 0.0);
    //scene.add(make_shared<XRay::Sphere>(vec3(4, 1, 0), 1.0, material3, "Metal Sphere"));
    auto earth_texture = make_shared<XRay::ImageTexture>("E:/Dev/C++/XRay/XRay/src/Renderer/earthmap.jpg");
    auto earth_surface = make_shared<XRay::Lambertian>(earth_texture);
    auto globe = make_shared<XRay::Sphere>(vec3(4, 1, 0), 1.0, earth_surface, "globe");
    scene.add(globe);

    return scene;
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

void render(Framebuffer fb, int width, int height, int startHeight, int endHeight, int samples, const XRay::Scene& scene, const XRay::Camera& camera, int depth, int threadIndex) {
    setRendering(threadIndex, true);
    hasRendered = true;
    renderer.render(fb, width, height, startHeight, endHeight, samples, scene, camera, depth);
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
            float r = fb[pixel_index].x();
            float g = fb[pixel_index].y();
            float b = fb[pixel_index].z();
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

    
    XRay::Scene scene = random_scene();
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


    vec3 lookfrom(13, 2, 3);
    vec3 lookat(0, 0, 0);
    vec3 vup(0, 1, 0);
    auto dist_to_focus = 10.0;
    auto aperture = 0.1;

    
    XRay::Camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus, 0.0, 1.0);

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
        imGuiRenderer.renderSettings(renderPressed, savePressed, samples_per_pixel, max_depth, scene);
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
            renderThread1 = std::thread(render, fb, image_width, image_height, 0, height_per_thread, samples_per_pixel, scene , cam, max_depth, 1);
            renderThread2 = std::thread(render, fb, image_width, image_height, height_per_thread, 2 * height_per_thread, samples_per_pixel, scene, cam, max_depth, 2);
            renderThread3 = std::thread(render, fb, image_width, image_height, 2 * height_per_thread, 3 * height_per_thread, samples_per_pixel, scene, cam, max_depth, 3);
            renderThread4 = std::thread(render, fb, image_width, image_height, image_height - 3 * height_per_thread, image_height, samples_per_pixel, scene, cam, max_depth, 4);

        }
        if (savePressed && !saving) {
            if (saveThread.joinable())
                saveThread.detach();
            saveThread = std::thread(saveImage, fb, "C:/Users/Anwender/Desktop/images.ppm", image_width, image_height);
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