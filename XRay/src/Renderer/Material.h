#pragma once
#include "../Core/XRay.h"
#include "Hittable.h"
#include "Texture.h"
#include "ONB.h"

namespace XRay {

    enum MaterialTyp {
        LambertianT = 0,
        MetalT, 
        DielectricT,
        DiffuseLightT,
        IsotropicT
    };

    class Material {
    public:
        virtual bool scatter(const Ray& r_in, const HitRecord& rec, Color& albedo, Ray& scattered, double& pdf) const {
            return false;
        }

        virtual double scattering_pdf(const Ray& r_in, const HitRecord& rec, const Ray& scattered) const {
            return 0;
        }


        virtual Color emitted(const Ray& r_in, const HitRecord& rec, double u, double v, const vec3& p) const {
            return Color(0, 0, 0);
        }

        MaterialTyp materialTyp;
    };

    class Lambertian : public Material {
    public:
        Lambertian(const Color& a) : albedo(make_shared<SolidColor>(a)) {
            materialTyp = MaterialTyp::LambertianT;
        }
        Lambertian(shared_ptr<Texture> a) : albedo(a) {
            materialTyp = MaterialTyp::LambertianT;
        }

        bool scatter(const Ray& r_in, const HitRecord& rec, Color& alb, Ray& scattered, double& pdf) const override {
            auto scatter_direction = rec.normal + random_unit_vector();

            if (scatter_direction.near_zero())
                scatter_direction = rec.normal;

            ONB uvw;
            uvw.build_from_w(rec.normal);
            auto direction = uvw.local(random_cosine_direction());

            scattered = Ray(rec.p, unit_vector(direction), r_in.time());
            alb = albedo->value(rec.u, rec.v, rec.p);
            pdf = 0.5 / pi;
            return true;
        }

        double scattering_pdf(const Ray& r_in, const HitRecord& rec, const Ray& scattered) const {
            auto cosine = dot(rec.normal, unit_vector(scattered.direction()));
            return cosine < 0 ? 0 : cosine / pi;
        }

    public:
        shared_ptr<Texture> albedo;
    };

    class Metal : public Material {
    public:
        Metal(const Color& a, float f) : albedo(a), fuzz(f < 1 ? f : 1) {
            materialTyp = MaterialTyp::MetalT;
        }

        bool scatter(const Ray& r_in, const HitRecord& rec, Color& attenuation, Ray& scattered, double& pdf) const override {
            vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
            scattered = Ray(rec.p, reflected + fuzz * random_in_unit_sphere(), r_in.time());
            attenuation = albedo;
            return (dot(scattered.direction(), rec.normal) > 0);
        }

    public:
        Color albedo;
        float fuzz;
    };

    class Dielectric : public Material {
    public:
        Dielectric(double index_of_refraction) : ir(index_of_refraction) {
            materialTyp = MaterialTyp::DielectricT;
        }

        virtual bool scatter(const Ray& r_in, const HitRecord& rec, Color& attenuation, Ray& scattered, double& pdf) const override {
            attenuation = Color(1.0, 1.0, 1.0);
            double refraction_ratio = rec.front_face ? (1.0 / ir) : ir;

            vec3 unit_direction = unit_vector(r_in.direction());
            double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
            double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

            bool cannot_refract = refraction_ratio * sin_theta > 1.0;
            vec3 direction;

            if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_double())
                direction = reflect(unit_direction, rec.normal);
            else
                direction = refract(unit_direction, rec.normal, refraction_ratio);

            scattered = Ray(rec.p, direction, r_in.time());
            return true;
        }

    public:
        double ir;

    private:
        static double reflectance(double cosine, double ref_idx) {
            // Use Schlick's approximation for reflectance.
            auto r0 = (1 - ref_idx) / (1 + ref_idx);
            r0 = r0 * r0;
            return r0 + (1 - r0) * pow((1 - cosine), 5);
        }
    };

    class DiffuseLight : public Material {
    public:
        DiffuseLight(shared_ptr<Texture> a, float intens) : emit(a), intensity(intens) {
            materialTyp = DiffuseLightT;
        }
        DiffuseLight(Color c, float intens) : emit(make_shared<SolidColor>(c)), intensity(intens) {
            materialTyp = DiffuseLightT;
        }

        virtual bool scatter(const Ray& r_in, const HitRecord& rec, Color& attenuation, Ray& scattered, double& pdf) const override {
            return false;
        }

        Color emitted(const Ray& r_in, const HitRecord& rec, double u, double v, const vec3& p) const override {

            if (rec.front_face)
                return emit->value(u, v, p) * intensity;
            else
                return Color(0, 0, 0);
        }

    public:
        shared_ptr<Texture> emit;
        float intensity;
    };

    class Isotropic : public Material {
    public:
        Isotropic(Color c) : albedo(make_shared<SolidColor>(c)) {
            materialTyp = IsotropicT;
        }
        Isotropic(shared_ptr<Texture> a) : albedo(a) {
            materialTyp = IsotropicT;
        }

        bool scatter(const Ray& r_in, const HitRecord& rec, Color& attenuation, Ray& scattered, double& pdf) const override {
            scattered = Ray(rec.p, random_in_unit_sphere(), r_in.time());
            attenuation = albedo->value(rec.u, rec.v, rec.p);
            return true;
        }

    public:
        shared_ptr<Texture> albedo;
    };

    
}