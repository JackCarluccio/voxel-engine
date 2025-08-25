#pragma once

#include <vector>
#include <algorithm>

namespace World::Generation::Splines {

	struct SplinePoint {
		float t, v;
	};

	struct LinearSegment {
		float slope, intercept;
	};

	class LinearSpline {
	public:
		LinearSpline(std::vector<SplinePoint>& points);

		// Allow default copy constructors and assignment operators
		LinearSpline(const LinearSpline&) = default;
		LinearSpline& operator=(const LinearSpline&) = default;

		// Allow default move constructors and assignment operators
		LinearSpline(LinearSpline&&) = default;
		LinearSpline& operator=(LinearSpline&&) = default;

		float Sample(float t) const noexcept;

		float FirstDerivative(float t) const noexcept;
		// Second derivative is always 0 for linear splines
		constexpr float SecondDerivative() const noexcept { return 0; };

	private:
		std::vector<LinearSegment> segments;
		std::vector<SplinePoint> points;
	};

}
