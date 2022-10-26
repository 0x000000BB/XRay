#pragma once
#include "../Core/vec3.h"

namespace XRay {

	class Ray {
	public:
		Ray() {}
		Ray(const vec3& a, const vec3& b, double time = 0.0) : A(a), B(b), tm(time) { }

		vec3 origin() const { return A; }
		vec3 direction() const { return B; }
		double time() const { return tm; }
		vec3 pointAtParameter(float t) const { return A + t * B; }

	private:
		vec3 A;
		vec3 B;
		double tm;
	};

}