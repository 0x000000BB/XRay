#pragma once
#include "Core/XRay.h"
#include "Renderer/Scene.h"
#include "Renderer/Material.h"
#include "Renderer/Sphere.h"
#include "Renderer/MovingSphere.h"
#include "Renderer/AARect.h"

#include <iostream>
#include <filesystem>

XRay::Scene random_scene(XRay::Camera&, Color& background) {
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

    auto material3 = make_shared<XRay::Metal>(Color(0.7, 0.6, 0.5), 0.0);
    scene.add(make_shared<XRay::Sphere>(vec3(4, 1, 0), 1.0, material3, "Metal Sphere"));


    return scene;
}

XRay::Scene random_scene_moving(XRay::Camera&, Color& background) {
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

    auto material3 = make_shared<XRay::Metal>(Color(0.7, 0.6, 0.5), 0.0);
    scene.add(make_shared<XRay::Sphere>(vec3(4, 1, 0), 1.0, material3, "Metal Sphere"));

    return scene;
}

XRay::Scene random_scene_texture(XRay::Camera&, Color& background) {
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

XRay::Scene random_scene_moving_texture(XRay::Camera&, Color& background) {
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
    std::filesystem::path path = std::filesystem::current_path();
    path += "/src/Renderer/earthmap.jpg";
    auto earth_texture = make_shared<XRay::ImageTexture>(path.generic_string().c_str());
    auto earth_surface = make_shared<XRay::Lambertian>(earth_texture);
    auto globe = make_shared<XRay::Sphere>(vec3(4, 1, 0), 1.0, earth_surface, "globe");
    scene.add(globe);

    return scene;
}

XRay::Scene simple_light(XRay::Camera& cam, Color& background) {
    XRay::Scene scene;

    auto texture = make_shared<XRay::SolidColor>(Color(0.5, 0.2, 0.7));
    scene.add(make_shared<XRay::Sphere>(vec3(0, -1000, 0), 1000, make_shared<XRay::Lambertian>(texture), "Sphere 1"));
    scene.add(make_shared<XRay::Sphere>(vec3(0, 2, 0), 2, make_shared<XRay::Lambertian>(texture), "Sphere 2"));

    auto difflight = make_shared<XRay::DiffuseLight>(Color(4, 4, 4));
    scene.add(make_shared<XRay::XYRect>(3, 5, 1, 3, -2, difflight));

    background = Color(0, 0, 0);
    vec3 lookfrom = vec3(26, 3, 6);
    vec3 lookat = vec3(0, 2, 0);
    auto vfov = 20.0;
    
    auto aspect_ratio = 3.0 / 2.0;

    vec3 vup(0, 1, 0);
    auto dist_to_focus = 10.0;
    auto aperture = 0.1;


    cam = XRay::Camera(lookfrom, lookat, vup, vfov, aspect_ratio, aperture, dist_to_focus, 0.0, 1.0);


    return scene;
}