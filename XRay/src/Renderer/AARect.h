#pragma once
#include "../Core/XRay.h"
#include "Hittable.h"


namespace XRay {

    class XYRect : public Hittable {
    public:
        XYRect() {}

        XYRect(double _x0, double _x1, double _y0, double _y1, double _k,
            std::shared_ptr<Material> mat)
            : x0(_x0), x1(_x1), y0(_y0), y1(_y1), k(_k) {
            mat_ptr = mat;
        };

        bool hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const override{
            auto t = (k - r.origin().z()) / r.direction().z();
            if (t < t_min || t > t_max)
                return false;
            auto x = r.origin().x() + t * r.direction().x();
            auto y = r.origin().y() + t * r.direction().y();
            if (x < x0 || x > x1 || y < y0 || y > y1)
                return false;
            rec.u = (x - x0) / (x1 - x0);
            rec.v = (y - y0) / (y1 - y0);
            rec.t = t;
            auto outward_normal = vec3(0, 0, 1);
            rec.set_face_normal(r, outward_normal);
            rec.mat_ptr = mat_ptr;
            rec.p = r.pointAtParameter(t);
            return true;
        }

        bool boundingBox(double time0, double time1, AABB& output_box) const override {
            // The bounding box must have non-zero width in each dimension, so pad the Z
            // dimension a small amount.
            output_box = AABB(vec3(x0, y0, k - 0.0001), vec3(x1, y1, k + 0.0001));
            return true;
        }

    public:
        double x0, x1, y0, y1, k;
    };

}