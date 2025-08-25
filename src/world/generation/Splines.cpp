#include "world/generation/Splines.h"

using namespace World::Generation::Splines;

// Calculates the linear segments between each pair of spline points
// Finds the index of the segment that contains the specified t value
int FindSegmentIndex(const std::vector<SplinePoint>& points, float t) noexcept {
	// Handle out-of-bounds t values
	if (t < points[0].t) {
		return 0;
	}
	else if (t >= points.back().t) {
		return points.size() - 2;
	}

	// Find the segment that contains t
	// Since points are sorted by t, and there are usually few points, a linear search is sufficient
	for (int i = 1; i < points.size(); i++) {
		if (t <= points[i].t)
			return i - 1;
	}
}

// Calculates the linear segments between each pair of spline points
std::vector<LinearSegment> CalculateLinearSegments(const std::vector<SplinePoint>& points) {
	std::vector<LinearSegment> segments;
	segments.reserve(points.size() - 1);

	for (int i = 0; i < points.size() - 1; i++) {
		const SplinePoint& p0 = points[i];
		const SplinePoint& p1 = points[i + 1];

		float slope = (p1.v - p0.v) / (p1.t - p0.t);
		float intercept = p0.v - slope * p0.t;
		segments.emplace_back(slope, intercept);
	}

	return segments;
}

namespace World::Generation::Splines {

	// Constructs a linear spline from the given points. Sorts points by t value.
	LinearSpline::LinearSpline(std::vector<SplinePoint> points_) :
		points(std::move(points_))
	{
		assert(points.size() >= 2 && "A spline must have at least two points.");

		std::sort(points.begin(), points.end(),
			[](const SplinePoint& a, const SplinePoint& b) { return a.t < b.t; });

		segments = CalculateLinearSegments(points);
	}

	// Samples the spline at the specified t value
	float LinearSpline::Sample(float t) const noexcept {
		int segmentIndex = FindSegmentIndex(points, t);
		const LinearSegment& segment = segments[segmentIndex];
		return std::fma(segment.slope, t, segment.intercept);
	}

	// Calculates the first derivative (slope) at the specified t value.
	// Non-continuous at spline points (returns slope of segment to the right of the point).
	float LinearSpline::FirstDerivative(float t) const noexcept {
		int segmentIndex = FindSegmentIndex(points, t);
		return segments[segmentIndex].slope;
	}

}
