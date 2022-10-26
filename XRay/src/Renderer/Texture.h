#pragma once
#include "../Core/XRay.h"
#include "XRay_stb_image.h"

namespace XRay{

    enum TextureTyp {
        SolidColorT = 0,
        CheckerT,
        ImageTextureT,
    };

    class Texture {
    public:
        virtual Color value(double u, double v, const vec3& p) const = 0;

        TextureTyp typ;
    };
    
    class SolidColor : public Texture {
    public:
        SolidColor() {
            typ = SolidColorT;
        }
        SolidColor(Color c) : colorValue(c) {
            typ = SolidColorT;
        }
    
        SolidColor(double red, double green, double blue)
            : colorValue(Color(red, green, blue)) {}
    
        virtual Color value(double u, double v, const vec3& p) const override {
            return colorValue;
        }
    
    public :
        Color colorValue;
    };

    class CheckerTexture : public Texture {
    public:
        CheckerTexture() {
            typ = CheckerT;
        }

        CheckerTexture(shared_ptr<Texture> _even, shared_ptr<Texture> _odd)
            : even(_even), odd(_odd) {
            typ = CheckerT;
        }

        CheckerTexture(Color c1, Color c2)
            : even(make_shared<SolidColor>(c1)), odd(make_shared<SolidColor>(c2)) {
            typ = CheckerT;
        }

        virtual Color value(double u, double v, const vec3& p) const override {
            auto sines = sin(10 * p.x()) * sin(10 * p.y()) * sin(10 * p.z());
            if (sines < 0)
                return odd->value(u, v, p);
            else
                return even->value(u, v, p);
        }

    public:
        shared_ptr<Texture> odd;
        shared_ptr<Texture> even;
    };

    class ImageTexture : public Texture {
    public:
        const static int bytes_per_pixel = 3;

        ImageTexture()
            : data(nullptr), width(0), height(0), bytes_per_scanline(0) {
            typ = ImageTextureT;
        }

        ImageTexture(const char* filename) {
            filepath = filename;
            auto components_per_pixel = bytes_per_pixel;

            data = stbi_load(
                filename, &width, &height, &components_per_pixel, components_per_pixel);

            if (!data) {
                std::cerr << "ERROR: Could not load texture image file '" << filename << "'.\n";
                width = height = 0;
            }

            bytes_per_scanline = bytes_per_pixel * width;

            typ = ImageTextureT;
        }

        ~ImageTexture() {
            delete data;
        }

        virtual Color value(double u, double v, const vec3& p) const override {
            // If we have no texture data, then return solid cyan as a debugging aid.
            if (data == nullptr)
                return Color(0, 1, 1);

            // Clamp input texture coordinates to [0,1] x [1,0]
            u = clamp(u, 0.0, 1.0);
            v = 1.0 - clamp(v, 0.0, 1.0);  // Flip V to image coordinates

            auto i = static_cast<int>(u * width);
            auto j = static_cast<int>(v * height);

            // Clamp integer mapping, since actual coordinates should be less than 1.0
            if (i >= width)  i = width - 1;
            if (j >= height) j = height - 1;

            const auto color_scale = 1.0 / 255.0;
            auto pixel = data + j * bytes_per_scanline + i * bytes_per_pixel;

            return Color(color_scale * pixel[0], color_scale * pixel[1], color_scale * pixel[2]);
        }

    public:
        std::string filepath;

    private:
        unsigned char* data;
        int width, height;
        int bytes_per_scanline;
    };

}