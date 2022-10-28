#pragma once

#include "Hittable.h"
#include "Camera.h"

#include <memory>
#include <vector>

using std::shared_ptr;
using std::make_shared;

namespace XRay {
    class Scene : public Hittable {
    public:
        //Scene() {}
        Scene(Camera& cam, Color& background, int width, float aspect_ratio) {
            camera = cam;
            backgroundColor = background;
            this->aspect_ratio = aspect_ratio;
            this->width = width;
            this->height = static_cast<int>(width / aspect_ratio);
        }
        Scene(shared_ptr<Hittable> object, Camera& cam) { 
            add(object); 
            camera = cam;
        }

        void clear() { objects.clear(); }
        void add(shared_ptr<Hittable> object) { objects.push_back(object); }

        bool hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const override {
            HitRecord temp_rec;
            bool hit_anything = false;
            auto closest_so_far = t_max;

            for (const auto& object : objects) {
                if (object->hit(r, t_min, closest_so_far, temp_rec)) {
                    hit_anything = true;
                    closest_so_far = temp_rec.t;
                    rec = temp_rec;
                }
            }

            return hit_anything;
        }

        bool boundingBox(double time0, double time1, AABB& output_box) const {
            if (objects.empty()) return false;

            AABB temp_box;
            bool first_box = true;

            for (const auto& object : objects) {
                if (!object->boundingBox(time0, time1, temp_box)) return false;
                output_box = first_box ? temp_box : surroundingBox(output_box, temp_box);
                first_box = false;
            }

            return true;
        }

    public:
        std::vector<shared_ptr<Hittable>> objects;
        Camera camera;
        Color backgroundColor;
        int width;
        float aspect_ratio;
        int height;
    };

}