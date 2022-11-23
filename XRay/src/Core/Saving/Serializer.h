#pragma once
#include "../../Renderer/Scene.h"
#include "../../Renderer/Sphere.h"

namespace XRay {

	class Serializer {
	public:
		static void serialize(const Scene& scene, std::string filepath) {
			FILE* file = fopen(filepath.c_str(), "w");
			serializeVec3(scene.backgroundColor, file);
			serializeCamera(scene.camera, file);
			for (std::shared_ptr<Hittable> object : scene.objects) {
				if (object.get()->typ == SphereT) {
					Sphere* sphere = static_cast<Sphere*>(object.get());
					serializeObject(*sphere, file);
				}
			}
		}
	private:

		static void serializeCamera(const Camera& cam, FILE* file) {
			serializeVec3(cam.lookfrom, file);
			serializeVec3(cam.lookat, file);
			serializeVec3(cam.vup, file);
			serializeDouble(cam.vfov, file);
			serializeDouble(cam.aspect_ratio, file);
			serializeDouble(cam.aperture, file);
			serializeDouble(cam.focus_dist, file);
			serializeDouble(cam.time0, file);
			serializeDouble(cam.time1, file);
		}

		static void serializeVec3(vec3 vec, FILE* file) {
			std::string value = "";
			value += std::to_string(vec.x());
			value += " ";
			value += std::to_string(vec.y());
			value += " ";
			value += std::to_string(vec.z());
			value += "\n";
			fprintf(file, value.c_str());
		}

		static void serializeDouble(double doub, FILE* file) {
			std::string value = "";
			value += std::to_string(doub);
			value += "\n";
			fprintf(file, value.c_str());
		}

		static void serializeFloat(float floa, FILE* file) {
			std::string value = "";
			value += std::to_string(floa);
			value += "\n";
			fprintf(file, value.c_str());
		}

		static void serializeString(std::string string, FILE* file) {
			string += "\n";
			fprintf(file, string.c_str());
		}

		static void serializeObject(Sphere& sphere, FILE* file) {
			fprintf(file, "Sphere\n");
			serializeVec3(sphere.center, file); 
			serializeDouble(sphere.radius, file);
			serializeString(sphere.name, file);
			if (sphere.mat_ptr.get()->materialTyp == LambertianT) {
				Lambertian* material = static_cast<Lambertian*>(sphere.mat_ptr.get());
				serializeMaterial(*material, file);
			}
			if (sphere.mat_ptr.get()->materialTyp == MetalT) {
				Metal* material = static_cast<Metal*>(sphere.mat_ptr.get());
				serializeMaterial(*material, file);
			}
			if (sphere.mat_ptr.get()->materialTyp == DielectricT) {
				Dielectric* material = static_cast<Dielectric*>(sphere.mat_ptr.get());
				serializeMaterial(*material, file);
			}
			
		}

		static void serializeMaterial(Lambertian& lambertian, FILE* file) {
			fprintf(file, "Lambertian\n");
			if (lambertian.albedo.get()->typ == SolidColorT) {
				SolidColor* texture = static_cast<SolidColor*>(lambertian.albedo.get());
				serializeTexture(*texture, file);
			}
		}

		static void serializeMaterial(Metal& metal, FILE* file) {
			fprintf(file, "Metal\n");
			serializeVec3(metal.albedo, file);
			serializeFloat(metal.fuzz, file);
		}

		static void serializeMaterial(Dielectric& dielectric, FILE* file) {
			fprintf(file, "Dielectric\n");
			serializeDouble(dielectric.ir, file);
		}

		static void serializeTexture(SolidColor& texture, FILE* file) {
			serializeVec3(texture.colorValue, file);
		}

	};

}