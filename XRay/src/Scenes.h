#pragma once
#include "Core/XRay.h"
#include "Renderer/Scene.h"
#include "Renderer/Material.h"
#include "Renderer/Sphere.h"
#include "Renderer/MovingSphere.h"
#include "Renderer/AARect.h"
#include "Renderer/Box.h"
#include "Renderer/Translate.h"
#include "Renderer/ConstantMedium.h"
#include "Renderer/FlipFace.h"

#include <iostream>
#include <filesystem>

XRay::Scene random_scene() {
    //vec3 lookfrom(13, 2, 3);
    //vec3 lookat(0, 0, 0);
    //vec3 vup(0, 1, 0);
    //auto dist_to_focus = 10.0;
    //auto aperture = 0.1;
    //auto aspect_ratio = 3.0 / 2.0;
    //std::shared_ptr<XRay::Hittable> lights = nullptr;
    //XRay::Camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus);
    //Color background(0, 0, 0);
    Color background = Color(0.1, 0.1, 0.1);
    vec3 lookfrom(13, 2, 3);
    vec3 lookat(0, 0, 0);
    auto vfov = 20.0;
    auto dist_to_focus = 10.0;
    auto aperture = 0.1;
    auto aspect_ratio = 3.0 / 2.0;
    vec3 vup(0, 1, 0);

    XRay::Camera cam(lookfrom, lookat, vup, vfov, aspect_ratio, aperture, dist_to_focus, 0.0, 1.0);
    std::shared_ptr<XRay::Hittable> lights = nullptr;
    XRay::Scene scene(cam, background, 1200, aspect_ratio, lights);



    auto checker = make_shared<XRay::SolidColor>(Color(0.2, 0.3, 0.1));
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
                    scene.add(make_shared<XRay::Sphere>(
                        center, 0.2, sphere_material, std::string("Sphere" + std::to_string(sphereCount))));
                    sphereCount++;
                }
                else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = Color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<XRay::Metal>(albedo, fuzz);
                    scene.add(make_shared<XRay::Sphere>(center, 0.2, sphere_material, std::string("Sphere" + std::to_string(sphereCount))));
                    sphereCount++;
                }
                else {
                    // glass
                    sphere_material = make_shared<XRay::Dielectric>(1.5);
                    scene.add(make_shared<XRay::Sphere>(center, 0.2, sphere_material, std::string("Sphere" + std::to_string(sphereCount))));
                    sphereCount++;
                }
            }
        }
    }

    auto material1 = make_shared<XRay::Dielectric>(1.5);
    scene.add(make_shared<XRay::Sphere>(vec3(0, 1, 0), 1.0, material1, "DielectricSphere"));

    auto material2 = make_shared<XRay::Lambertian>(Color(0.4, 0.2, 0.1));
    scene.add(make_shared<XRay::Sphere>(vec3(-4, 1, 0), 1.0, material2, "LambertianSphere"));

    auto material3 = make_shared<XRay::Metal>(Color(0.7, 0.6, 0.5), 0.0);
    scene.add(make_shared<XRay::Sphere>(vec3(4, 1, 0), 1.0, material3, "MetalSphere"));


    return scene;
}

XRay::Scene random_scene_moving() {
    vec3 lookfrom(13, 2, 3);
    vec3 lookat(0, 0, 0);
    vec3 vup(0, 1, 0);
    auto dist_to_focus = 10.0;
    auto aperture = 0.1;
    auto aspect_ratio = 3.0 / 2.0;
    std::shared_ptr<XRay::Hittable> lights;
    XRay::Camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus);
    Color background(0, 0, 0);
    XRay::Scene scene(cam, background, 1200, aspect_ratio, lights);

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

    auto material3 = make_shared<XRay::Metal>(Color(0.7, 0.6, 0.5), 0.0);
    scene.add(make_shared<XRay::Sphere>(vec3(4, 1, 0), 1.0, material3, "Metal Sphere"));

    return scene;
}

XRay::Scene random_scene_texture() {
    vec3 lookfrom(13, 2, 3);
    vec3 lookat(0, 0, 0);
    vec3 vup(0, 1, 0);
    auto dist_to_focus = 10.0;
    auto aperture = 0.1;
    auto aspect_ratio = 3.0 / 2.0;
    std::shared_ptr<XRay::Hittable> lights;
    XRay::Camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus);
    Color background(0, 0, 0);
    XRay::Scene scene(cam, background, 1200, aspect_ratio, lights);

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
                    scene.add(make_shared<XRay::Sphere>(
                        center, 0.2, sphere_material, std::string("Sphere " + std::to_string(sphereCount))));
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
    std::filesystem::path path = std::filesystem::current_path();
    path += "/src/Renderer/earthmap.jpg";
    auto earth_texture = make_shared<XRay::ImageTexture>(path.generic_string().c_str());
    auto earth_surface = make_shared<XRay::Lambertian>(earth_texture);
    auto globe = make_shared<XRay::Sphere>(vec3(4, 1, 0), 1.0, earth_surface, "globe");
    scene.add(globe);

    return scene;
}

XRay::Scene random_scene_moving_texture() {
    vec3 lookfrom(13, 2, 3);
    vec3 lookat(0, 0, 0);
    vec3 vup(0, 1, 0);
    auto dist_to_focus = 10.0;
    auto aperture = 0.1;
    auto aspect_ratio = 3.0 / 2.0;
    std::shared_ptr<XRay::Hittable> lights;
    XRay::Camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus);
    Color background(0, 0, 0);
    XRay::Scene scene(cam, background, 1200, aspect_ratio, lights);

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
    std::filesystem::path path = std::filesystem::current_path();
    path += "/src/Renderer/earthmap.jpg";
    auto earth_texture = make_shared<XRay::ImageTexture>(path.generic_string().c_str());
    auto earth_surface = make_shared<XRay::Lambertian>(earth_texture);
    auto globe = make_shared<XRay::Sphere>(vec3(4, 1, 0), 1.0, earth_surface, "globe");
    scene.add(globe);

    return scene;
}

XRay::Scene simple_light() {

    Color background = Color(0.1, 0.1, 0.1);
    vec3 lookfrom = vec3(26, 3, 6);
    vec3 lookat = vec3(0, 2, 0);
    auto vfov = 20.0;

    auto aspect_ratio = 3.0 / 2.0;

    vec3 vup(0, 1, 0);
    auto dist_to_focus = 23.0;
    auto aperture = 0.1;
    
    XRay::Camera cam(lookfrom, lookat, vup, vfov, aspect_ratio, aperture, dist_to_focus, 0.0, 1.0);
    std::shared_ptr<XRay::Hittable> lights = nullptr;
    XRay::Scene scene(cam, background, 1200, aspect_ratio, lights);

    auto texture = make_shared<XRay::SolidColor>(Color(0.5, 0.2, 0.7));
    scene.add(make_shared<XRay::Sphere>(vec3(0, -1000, 0), 1000, make_shared<XRay::Lambertian>(texture), "Sphere 1"));
    scene.add(make_shared<XRay::Sphere>(vec3(0, 2, 0), 2, make_shared<XRay::Lambertian>(texture), "Sphere 2"));

    auto difflight = make_shared<XRay::DiffuseLight>(Color(1, 1, 1), 4);
    scene.add(make_shared<XRay::XYRect>(3, 5, 1, 3, -2, difflight, "Light"));

    return scene;
}

XRay::Scene cornell_box() {

    vec3 lookfrom(278, 278, -800);
    vec3 lookat(278, 278, 0);
    vec3 vup(0, 1, 0);
    auto dist_to_focus = (lookfrom - lookat).length();
    auto aperture = 0.1;
    auto aspect_ratio = 1.0;

    std::shared_ptr<XRay::Hittable> lights =
        make_shared<XRay::XZRect>(213, 343, 227, 332, 554, shared_ptr<XRay::Material>(), "");

    XRay::Camera cam(lookfrom, lookat, vup, 40, aspect_ratio, aperture, dist_to_focus);
    Color background(0, 0, 0);
    XRay::Scene scene(cam, background, 500, aspect_ratio, lights);

    auto red = make_shared<XRay::Lambertian>(Color(.65, .05, .05));
    auto white = make_shared<XRay::Lambertian>(Color(.73, .73, .73));
    auto green = make_shared<XRay::Lambertian>(Color(.12, .45, .15));
    auto light = make_shared<XRay::DiffuseLight>(Color(1, 1, 1), 15);

    scene.add(make_shared<XRay::YZRect>(0, 555, 0, 555, 555, green, "Green"));
    scene.add(make_shared<XRay::YZRect>(0, 555, 0, 555, 0, red, "Red"));
    scene.add(make_shared<XRay::FlipFace>(make_shared<XRay::XZRect>(213, 343, 227, 332, 554, light, "Light")));
    scene.add(make_shared<XRay::XZRect>(0, 555, 0, 555, 0, white, "White"));
    scene.add(make_shared<XRay::XZRect>(0, 555, 0, 555, 555, white, "White"));
    scene.add(make_shared<XRay::XYRect>(0, 555, 0, 555, 555, white, "White"));

    std::shared_ptr<XRay::Hittable> box1 = std::make_shared<XRay::Box>(vec3(0, 0, 0), vec3(165, 330, 165), white);
    box1 = std::make_shared<XRay::RotateY>(box1, 15);
    box1 = std::make_shared<XRay::Translate>(box1, vec3(265, 0, 295), "Box 1");
    scene.add(box1);

    std::shared_ptr<XRay::Hittable> box2 = std::make_shared<XRay::Box>(vec3(0, 0, 0), vec3(165, 165, 165), white);
    box2 = std::make_shared<XRay::RotateY>(box2, -18);
    box2 = std::make_shared<XRay::Translate>(box2, vec3(130, 0, 65), "Box 2");
    scene.add(box2);

    return scene;
}

XRay::Scene cornell_smoke() {

    vec3 lookfrom(278, 278, -800);
    vec3 lookat(278, 278, 0);
    vec3 vup(0, 1, 0);
    auto dist_to_focus = (lookfrom - lookat).length();
    auto aperture = 0.1;
    auto aspect_ratio = 1.0;
    std::shared_ptr<XRay::Hittable> lights =
        make_shared<XRay::XZRect>(213, 343, 227, 332, 554, shared_ptr<XRay::Material>(), "");

    XRay::Camera cam(lookfrom, lookat, vup, 40, aspect_ratio, aperture, dist_to_focus);
    Color background(0, 0, 0);
    XRay::Scene scene(cam, background, 600, aspect_ratio, lights);

    auto red = make_shared<XRay::Lambertian>(Color(.65, .05, .05));
    auto white = make_shared<XRay::Lambertian>(Color(.73, .73, .73));
    auto whiteSmoke = make_shared<XRay::Lambertian>(Color(.73, .73, .73));
    auto blackSmoke = make_shared<XRay::Lambertian>(Color(.73, .73, .73));
    auto green = make_shared<XRay::Lambertian>(Color(.12, .45, .15));
    auto light = make_shared<XRay::DiffuseLight>(Color(1, 1, 1), 7);

    scene.add(make_shared<XRay::YZRect>(0, 555, 0, 555, 555, green, "Green"));
    scene.add(make_shared<XRay::YZRect>(0, 555, 0, 555, 0, red, "Red"));
    scene.add(make_shared<XRay::XZRect>(113, 443, 127, 432, 554, light, "Light"));
    scene.add(make_shared<XRay::XZRect>(0, 555, 0, 555, 0, white, "White"));
    scene.add(make_shared<XRay::XZRect>(0, 555, 0, 555, 555, white, "White"));
    scene.add(make_shared<XRay::XYRect>(0, 555, 0, 555, 555, white, "White"));

    std::shared_ptr<XRay::Hittable> box1 = std::make_shared<XRay::Box>(vec3(0, 0, 0), vec3(165, 330, 165), whiteSmoke);
    box1 = std::make_shared<XRay::RotateY>(box1, 15);
    box1 = std::make_shared<XRay::Translate>(box1, vec3(265, 0, 295), "");

    std::shared_ptr<XRay::Hittable> box2 = std::make_shared<XRay::Box>(vec3(0, 0, 0), vec3(165, 165, 165), blackSmoke);
    box2 = std::make_shared<XRay::RotateY>(box2, -18);
    box2 = std::make_shared<XRay::Translate>(box2, vec3(130, 0, 65), "");

    scene.add(make_shared<XRay::ConstantMedium>(box1, 0.01, Color(0, 0, 0), "Black Smoke"));
    scene.add(make_shared<XRay::ConstantMedium>(box2, 0.01, Color(1, 1, 1), "White Smoke"));

    return scene;
}

XRay::Scene test_scene()
{
    vec3 lookfrom(13, 2, 3);
    vec3 lookat(0, 0, 0);
    vec3 vup(0, 1, 0);
    auto dist_to_focus = 10.0;
    auto aperture = 0.1;
    auto aspect_ratio = 3.0 / 2.0;
    std::shared_ptr<XRay::Hittable> lights;
    XRay::Camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus);
    Color background(0, 0, 0);
    XRay::Scene scene(cam, background, 1200, aspect_ratio, lights);

    auto material2 = make_shared<XRay::Lambertian>(Color(0.4, 0.2, 0.1));
    scene.add(make_shared<XRay::Sphere>(vec3(-4, 1, 0), 1.0, material2, "LambertianSphere"));

    return scene;
}
