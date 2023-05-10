#include "triangle_test.h"

#include "delaunay/vertex_2d.h"
#include "delaunay/triangle.h"
#include "test.h"
#include <memory>


namespace test::delaunay
{
	void TriangleTest::testVertexRelation()
	{
		DELAUNAY_TEST_BEGIN();
			bool ok;
			const float c23 = 2.0 / 3.0;
			::delaunay::VertexRelation vertexRelation;
			std::shared_ptr<::delaunay::Triangle> triangle =
					std::make_shared<::delaunay::Triangle>();
			std::shared_ptr<::delaunay::Vertex2D> vertex1 = std::make_shared<::delaunay::Vertex2D>();
			std::shared_ptr<::delaunay::Vertex2D> vertex2 = std::make_shared<::delaunay::Vertex2D>();
			std::shared_ptr<::delaunay::Vertex2D> vertex3 = std::make_shared<::delaunay::Vertex2D>();
			std::shared_ptr<::delaunay::Vertex2D> vertex = std::make_shared<::delaunay::Vertex2D>();
			triangle->vertex1 = vertex1;
			triangle->vertex2 = vertex2;
			triangle->vertex3 = vertex3;

			vertex1->x = 0.0f;
			vertex1->y = 0.0f;
			vertex2->x = 0.0f;
			vertex2->y = 2.0f;
			vertex3->x = 2.0f;
			vertex3->y = 0.0f;

			vertex->x = c23;
			vertex->y = c23;
			vertexRelation = triangle->vertexRelation(vertex, &ok);
			DELAUNAY_TEST(ok);
			DELAUNAY_TEST(vertexRelation == ::delaunay::VertexRelation::VertexInTriangle);

			vertex->x = 0.0;
			vertex->y = 1.0;
			vertexRelation = triangle->vertexRelation(vertex, &ok);
			DELAUNAY_TEST(ok);
			DELAUNAY_TEST(vertexRelation == ::delaunay::VertexRelation::VertexOnRib12);

			vertex->x = 1.0;
			vertex->y = 0.0;
			vertexRelation = triangle->vertexRelation(vertex, &ok);
			DELAUNAY_TEST(ok);
			DELAUNAY_TEST(vertexRelation == ::delaunay::VertexRelation::VertexOnRib13);

			vertex->x = 1.0;
			vertex->y = 1.0;
			vertexRelation = triangle->vertexRelation(vertex, &ok);
			DELAUNAY_TEST(ok);
			DELAUNAY_TEST(vertexRelation == ::delaunay::VertexRelation::VertexOnRib23);

			vertex->x = 1.1;
			vertex->y = 1.1;
			vertexRelation = triangle->vertexRelation(vertex, &ok);
			DELAUNAY_TEST(ok);
			DELAUNAY_TEST(vertexRelation == ::delaunay::VertexRelation::VertexOutOfTriangle);

		DELAUNAY_TEST_END();
	}
}
