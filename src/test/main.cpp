#include "delaunay_service_test.h"
#include "utils_test.h"
#include "triangulation_cache_test.h"
#include "triangle_test.h"


int main()
{
	test::delaunay::UtilsTest utilsTest;
	utilsTest.testSinAlpha();
	utilsTest.testCosAlpha();
	utilsTest.testFirstTriangleCreation();
	utilsTest.testSumOppositeAngles();
	utilsTest.testCircleEquation();
	utilsTest.testCenterOfMass();
	utilsTest.testDistance();
	utilsTest.testBindTriangles();
	utilsTest.testDirection();
	utilsTest.testSegmentsIntersect();
	utilsTest.testCreateVerticesAsRectangle();

	test::delaunay::TriangleTest triangleTest;
	triangleTest.testVertexRelation();

	test::delaunay::TriangulationCacheTest triangulationCacheTest;
	triangulationCacheTest.testFindAppropriateTriangle();
	triangulationCacheTest.testCreateNewThreeTrianglesNearInnerVertex();
	triangulationCacheTest.testCreateNewTwoOrFourTrianglesNearVertexOnRib();
	triangulationCacheTest.testCheckDelaunayConditionAndRebuildIfNeed();

	test::delaunay::DelaunayServiceTest delaunayServiceTest;
	delaunayServiceTest.testIterativeDynamicProcess1();

	return 0;
}
