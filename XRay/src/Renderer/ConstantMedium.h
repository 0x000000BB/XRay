#pragma once
#include "../Core/XRay.h"
#include "Hittable.h"
#include "Texture.h"
#include "Material.h"

namespace XRay {

    class ConstantMedium : public Hittable {
    public:
        ConstantMedium(std::shared_ptr<Hittable> b, double d, std::shared_ptr<Texture> a, std::string n)
            : boundary(b),
            neg_inv_density(-1 / d),
            phase_function(std::make_shared<Isotropic>(a))
        {
            name = n;
            mat_ptr = b.get()->mat_ptr;
        }

        ConstantMedium(std::shared_ptr<Hittable> b, double d, Color c, std::string n)
            : boundary(b),
            neg_inv_density(-1 / d),
            phase_function(std::make_shared<Isotropic>(c))
        {
            name = n;
            mat_ptr = phase_function;
        }

        bool hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const override {

            HitRecord rec1, rec2;

            if (!boundary->hit(r, -infinity, infinity, rec1))
                return false;

            if (!boundary->hit(r, rec1.t + 0.0001, infinity, rec2))
                return false;

            if (rec1.t < t_min) rec1.t = t_min;
            if (rec2.t > t_max) rec2.t = t_max;

            if (rec1.t >= rec2.t)
                return false;

            if (rec1.t < 0)
                rec1.t = 0;

            const auto ray_length = r.direction().length();
            const auto distance_inside_boundary = (rec2.t - rec1.t) * ray_length;
            const auto hit_distance = neg_inv_density * log(random_double());

            if (hit_distance > distance_inside_boundary)
                return false;

            rec.t = rec1.t + hit_distance / ray_length;
            rec.p = r.pointAtParameter(rec.t);

            rec.normal = vec3(1, 0, 0);
            rec.front_face = true;
            rec.mat_ptr = phase_function;

            return true;
        }

        bool boundingBox(double time0, double time1, AABB& output_box) const override {
            return boundary->boundingBox(time0, time1, output_box);
        }

    public:
        std::shared_ptr<Hittable> boundary;
        std::shared_ptr<Material> phase_function;
        double neg_inv_density;
    };

}