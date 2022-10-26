#pragma once

#include "Hittable.h"

namespace XRay {

    class MovingSphere : public Hittable {
    public:
        MovingSphere() {}
        MovingSphere(
            vec3 cen0, vec3 cen1, double _time0, double _time1, double r, std::shared_ptr<Material> m, std::string n)
            : center0(cen0), center1(cen1), time0(_time0), time1(_time1), radius(r)
        {
            mat_ptr = m;
            name = n;
        };

        bool hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const override {
            vec3 oc = r.origin() - center(r.time());
            auto a = r.direction().squared_length();
            auto half_b = dot(oc, r.direction());
            auto c = oc.squared_length() - radius * radius;

            auto discriminant = half_b * half_b - a * c;
            if (discriminant < 0) return false;
            auto sqrtd = sqrt(discriminant);

            // Find the nearest root that lies in the acceptable range.
            auto root = (-half_b - sqrtd) / a;
            if (root < t_min || t_max < root) {
                root = (-half_b + sqrtd) / a;
                if (root < t_min || t_max < root)
                    return false;
            }

            rec.t = root;
            rec.p = r.pointAtParameter(rec.t);
            auto outward_normal = (rec.p - center(r.time())) / radius;
            rec.set_face_normal(r, outward_normal);
            rec.mat_ptr = mat_ptr;

            return true;
        }

        bool boundingBox(double _time0, double _time1, AABB& output_box) const {
            AABB box0(
                center(_time0) - vec3(radius, radius, radius),
                center(_time0) + vec3(radius, radius, radius));
            AABB box1(
                center(_time1) - vec3(radius, radius, radius),
                center(_time1) + vec3(radius, radius, radius));
            output_box = surroundingBox(box0, box1);
            return true;
        }

        vec3 center(double time) const;

    public:
        vec3 center0, center1;
        double time0, time1;
        double radius;
    };

    vec3 MovingSphere::center(double time) const {
        return center0 + ((time - time0) / (time1 - time0)) * (center1 - center0);
    }


}