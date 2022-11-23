#pragma once
#include "../Core/XRay.h"
#include "ONB.h"
#include "Hittable.h"

namespace XRay {
    class Pdf {
    public:
        virtual ~Pdf() {}

        virtual double value(const vec3& direction) const = 0;
        virtual vec3 generate() const = 0;
    };

    class CosinePdf : public Pdf {
    public:
        CosinePdf(const vec3& w) { uvw.build_from_w(w); }

        virtual double value(const vec3& direction) const override {
            auto cosine = dot(unit_vector(direction), uvw.w());
            return (cosine <= 0) ? 0 : cosine / pi;
        }

        virtual vec3 generate() const override {
            return uvw.local(random_cosine_direction());
        }

    public:
        ONB uvw;
    };

    class HittablePdf : public Pdf {
    public:
        HittablePdf(shared_ptr<Hittable> p, const vec3& origin) : ptr(p), o(origin) {}

        virtual double value(const vec3& direction) const override {
            return ptr->pdf_value(o, direction);
        }

        virtual vec3 generate() const override {
            return ptr->random(o);
        }

    public:
        vec3 o;
        shared_ptr<Hittable> ptr;
    };

    class MixturePdf : public Pdf {
    public:
        MixturePdf(shared_ptr<Pdf> p0, shared_ptr<Pdf> p1) {
            p[0] = p0;
            p[1] = p1;
        }

        virtual double value(const vec3& direction) const override {
            return 0.5 * p[0]->value(direction) + 0.5 * p[1]->value(direction);
        }

        virtual vec3 generate() const override {
            if (random_double() < 0.5)
                return p[0]->generate();
            else
                return p[1]->generate();
        }

    public:
        shared_ptr<Pdf> p[2];
    };

}