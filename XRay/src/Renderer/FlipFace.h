#pragma once
#include "Hittable.h"

namespace XRay {

    class FlipFace : public Hittable {
    public:
        FlipFace(std::shared_ptr<Hittable> p) : ptr(p) {
            mat_ptr = p.get()->mat_ptr;
            name = p.get()->name;
        }

        bool hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const override {

            if (!ptr->hit(r, t_min, t_max, rec))
                return false;

            rec.front_face = !rec.front_face;
            return true;
        }

        virtual bool boundingBox(double time0, double time1, AABB& output_box) const override {
            return ptr->boundingBox(time0, time1, output_box);
        }

    public:
        std::shared_ptr<Hittable> ptr;
    };

}