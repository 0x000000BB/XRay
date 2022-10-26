#pragma once

#include "Hittable.h"

namespace XRay {

    class Material;

    class Sphere : public Hittable {
    public:
        Sphere() = default;
        Sphere(vec3 cen, double r, std::shared_ptr<Material> m, std::string n) : center(cen), radius(r) {
            mat_ptr = m;
            name = n;
        };

        bool hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const override{
            vec3 oc = r.origin() - center;
            auto a = r.direction().squared_length();
            auto half_b = dot(oc, r.direction());
            auto c = oc.squared_length() - radius * radius;

            auto discriminant = half_b * half_b - a * c;
            if (discriminant < 0) return false;
            auto sqrtd = sqrt(discriminant);

            auto root = (-half_b - sqrtd) / a;
            if (root < t_min || t_max < root) {
                root = (-half_b + sqrtd) / a;
                if (root < t_min || t_max < root)
                    return false;
            }

            rec.t = root;
            rec.p = r.pointAtParameter(rec.t);
            rec.normal = (rec.p - center) / radius;
            vec3 outward_normal = (rec.p - center) / radius;
            rec.set_face_normal(r, outward_normal);
            get_sphere_uv(outward_normal, rec.u, rec.v);
            rec.mat_ptr = mat_ptr;

            return true;
        }

        bool boundingBox(double time0, double time1, AABB& output_box) const {
            output_box = AABB(
                center - vec3(radius, radius, radius),
                center + vec3(radius, radius, radius));
            return true;
        }

    public:
        vec3 center;
        double radius;

    private:
        static void get_sphere_uv(const vec3& p, double& u, double& v) {

            auto theta = acos(-p.y());
            auto phi = atan2(-p.z(), p.x()) + pi;

            u = phi / (2 * pi);
            v = theta / pi;
        }
    };
}