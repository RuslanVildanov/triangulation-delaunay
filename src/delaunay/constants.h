#ifndef DELAUNAY_CONSTANTS_H
#define DELAUNAY_CONSTANTS_H

#include <limits>


namespace delaunay
{
	static const float EPSILON = 0.00001f;
	static const float FAKE_RECTANGLE_OFFSET = 1.0f;
	static const int FAKE_ID = std::numeric_limits<int>::min();

}

#endif //DELAUNAY_CONSTANTS_H
