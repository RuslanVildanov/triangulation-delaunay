#ifndef TEST_TRIANGLE_TEST_H
#define TEST_TRIANGLE_TEST_H


namespace test::delaunay
{
	class TriangleTest
	{
	public:
		TriangleTest() = default;
		~TriangleTest() = default;
		TriangleTest(const TriangleTest&) = delete;
		TriangleTest(TriangleTest&&) = delete;
		TriangleTest & operator=(const TriangleTest&) = delete;
		TriangleTest & operator=(TriangleTest&&) = delete;

		void testVertexRelation();

	};

}

#endif //TEST_TRIANGLE_TEST_H
