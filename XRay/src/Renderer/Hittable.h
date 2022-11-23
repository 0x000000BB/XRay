#pragma once

#include "../Core/XRay.h"
#include "AABB.h"
#include<memory>
#include<string>

namespace XRay {

    class Material;

    enum ObjectTyp {
        SphereT = 0,
    };

    struct HitRecord {
        vec3 p;
        vec3 normal;
        std::shared_ptr<Material> mat_ptr;
        double t;
        double u;
        double v;
        bool front_face;

        inline void set_face_normal(const Ray& r, const vec3& outward_normal) {
            front_face = dot(r.direction(), outward_normal) < 0;
            normal = front_face ? outward_normal : -outward_normal;
        }
    };

    class Hittable {
    public:
        virtual bool hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const = 0;
        virtual bool boundingBox(double time0, double time1, AABB& output_box) const = 0;
        virtual double pdf_value(const vec3& o, const vec3& v) const {
            return 0.0;
        }

        virtual vec3 random(const vec3& o) const {
            return vec3(1, 0, 0);
        }
        std::shared_ptr<Material> mat_ptr;
        std::string name;
        ObjectTyp typ;
    };

}
