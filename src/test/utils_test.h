#ifndef TEST_UTILS_TEST_H
#define TEST_UTILS_TEST_H


namespace test::delaunay
{
	class UtilsTest
	{
	public:
		UtilsTest() = default;
		~UtilsTest() = default;
		UtilsTest(const UtilsTest&) = delete;
		UtilsTest(UtilsTest&&) = delete;
		UtilsTest & operator=(const UtilsTest&) = delete;
		UtilsTest & operator=(UtilsTest&&) = delete;

		void testSinAlpha();
		void testCosAlpha();
		void testFirstTriangleCreation();
		void testSumOppositeAngles();
		void testCircleEquation();
		void testCenterOfMass();
		void testDistance();
		void testBindTriangles();
		void testDirection();
		void testSegmentsIntersect();
		void testCreateVerticesAsRectangle();

	};
}

#endif //TEST_UTILS_TEST_H
