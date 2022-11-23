#pragma once
#include "../../Renderer/Scene.h"
#include "../../Renderer/Sphere.h"

#include <string>
#include <iostream>
#include <fstream>

namespace XRay {

	std::vector<std::string> split(char* s, const char* delimiter) {
		std::cout << "input: " << s << std::endl;
		char* str = s;

		// Returns first token
		char* token = strtok(str, delimiter);

		std::vector<std::string> res = std::vector<std::string>();

		while (token != NULL)
		{
			res.emplace_back(std::string(token));
			token = strtok(NULL, delimiter);
		}
		std::cout << "output: " << res.at(0) << ", " << res.at(1) << ", " << res.at(2) << ", " << std::endl;

		return res;
	}

	class Deserializer {
	public:
		Scene deserialize(std::string filepath) {
			//vec3 lookfrom(13, 2, 3);
			//vec3 lookat(0, 0, 0);
			//auto vfov = 20.0;
			//auto dist_to_focus = 10.0;
			//auto aperture = 0.1;
			//auto aspect_ratio = 3.0 / 2.0;
			//vec3 vup(0, 1, 0);
			//
			//XRay::Camera cam(lookfrom, lookat, vup, vfov, aspect_ratio, aperture, dist_to_focus, 0.0, 1.0);
			std::shared_ptr<XRay::Hittable> lights = nullptr;
			

			std::ifstream file("scene.xray");

			std::string* lines = new std::string[1000];
			std::string line = "";
			int lineCounter = 0;

			if (file.is_open()) {
				while (file.good()) {
					file >> line;
					//std::cout << line << std::endl;
					lines[lineCounter] = line;
					lineCounter++;
				}
			}
			else {
				std::cout << "unable to open file" << std::endl;
			}
			int index = 0;
			Color background = deserializeVec3(lines, index);
			Camera cam = deserializeCamera(lines, index);

			XRay::Scene scene(cam, background, 1200, cam.aspect_ratio, lights);

			for (int i = index; i < lineCounter; i++) {
				std::string line = lines[i];
				if (line == "Sphere") {
					i++;
					scene.add(std::make_shared<Sphere>(deserializeObject(lines, i)));
				}
			}

			return scene;
		}
	private:

		static Camera deserializeCamera(std::string* lines, int& lineIndex) {

			vec3 lookfrom = deserializeVec3(lines, lineIndex);
			vec3 lookat = deserializeVec3(lines, lineIndex);
			vec3 vup = deserializeVec3(lines, lineIndex);
			double vfov = deserializeDouble(lines[lineIndex]);
			lineIndex++;
			double aspect_ratio = deserializeDouble(lines[lineIndex]);
			lineIndex++;
			double aperture = deserializeDouble(lines[lineIndex]);
			lineIndex++;
			double focus_dist = deserializeDouble(lines[lineIndex]);
			lineIndex++;
			double time0 = deserializeDouble(lines[lineIndex]);
			lineIndex++;
			double time1 = deserializeDouble(lines[lineIndex]);
			lineIndex++;
			Camera cam(lookfrom, lookat, vup, vfov, aspect_ratio, aperture, focus_dist, time0, time1);
			return cam;
		}

		static Sphere deserializeObject(std::string* lines, int& lineIndex) {
			vec3 center = deserializeVec3(lines, lineIndex);
			double radius = deserializeDouble(lines[lineIndex]);
			lineIndex++;
			std::string name = lines[lineIndex];
			std::cout << "name: " << name << std::endl;
			lineIndex++;
			std::shared_ptr<Material> material;
			if (lines[lineIndex] == "Lambertian") {
				std::cout << lines[lineIndex] << std::endl;
				lineIndex++;
				material = deserializeLambertian(lines, lineIndex);
			}
			if (lines[lineIndex] == "Metal") {
				lineIndex++;
				material = deserializeMetal(lines, lineIndex);
			}
			if (lines[lineIndex] == "Dielectric") {
				lineIndex++;
				material = deserializeDielectric(lines, lineIndex);
			}
			lineIndex--;
			return Sphere(center, radius, material, name);
		}

		static double deserializeDouble(std::string line) {
			std::cout << line << std::endl;
			return std::stof(line);
		}

		static float deserializeFloat(std::string line) {
			return std::stof(line);
		}

		static vec3 deserializeVec3(std::string* lines, int& lineIndex) {
			float x = std::stof(lines[lineIndex]);
			lineIndex++;
			float y = std::stof(lines[lineIndex]);
			lineIndex++;
			float z = std::stof(lines[lineIndex]);
			lineIndex++;
			std::cout << "vec3: " << x << ", " << y << ", " << z << std::endl;
			return vec3(x, y, z);
		}

		static std::shared_ptr<Lambertian> deserializeLambertian(std::string* lines, int& lineIndex) {
			return std::make_shared<Lambertian>(deserializeVec3(lines, lineIndex));
		}

		static std::shared_ptr<Metal> deserializeMetal(std::string* lines, int& lineIndex) {
			vec3 albedo = deserializeVec3(lines, lineIndex);
			float fuzz = deserializeFloat(lines[lineIndex]);
			lineIndex++;
			return std::make_shared<Metal>(albedo, fuzz);
		}

		static std::shared_ptr<Dielectric> deserializeDielectric(std::string* lines, int& lineIndex) {
			auto result = std::make_shared<Dielectric>(deserializeDouble(lines[lineIndex]));
			lineIndex++;
			return result;
		}

	};
}