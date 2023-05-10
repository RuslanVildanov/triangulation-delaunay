#ifndef TEST_TRIANGULATION_CACHE_TEST_H
#define TEST_TRIANGULATION_CACHE_TEST_H


namespace test::delaunay
{
	class TriangulationCacheTest
	{
	public:
		TriangulationCacheTest() = default;
		~TriangulationCacheTest() = default;
		TriangulationCacheTest(const TriangulationCacheTest&) = delete;
		TriangulationCacheTest(TriangulationCacheTest&&) = delete;
		TriangulationCacheTest & operator=(const TriangulationCacheTest&) = delete;
		TriangulationCacheTest & operator=(TriangulationCacheTest&&) = delete;

		void testFindAppropriateTriangle();
		void testCreateNewThreeTrianglesNearInnerVertex();
		void testCreateNewTwoOrFourTrianglesNearVertexOnRib();
		void testCheckDelaunayConditionAndRebuildIfNeed();

	};
}

#endif //TEST_TRIANGULATION_CACHE_TEST_H
