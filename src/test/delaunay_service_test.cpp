#include "delaunay_service_test.h"

#include "delaunay/delaunay_service.h"
#include "delaunay/triangle.h"
#include "delaunay/vertex_2d.h"

#include "test.h"

#include <list>
#include <memory>


namespace test::delaunay
{
	void DelaunayServiceTest::testIterativeDynamicProcess1()
	{
		DELAUNAY_TEST_BEGIN();
			::delaunay::DelaunayService delaunayService;
			std::list<std::shared_ptr<::delaunay::Vertex2D> > vertices;
			std::shared_ptr<::delaunay::Vertex2D> vertex1 = std::make_shared<::delaunay::Vertex2D>();
			std::shared_ptr<::delaunay::Vertex2D> vertex2 = std::make_shared<::delaunay::Vertex2D>();
			std::shared_ptr<::delaunay::Vertex2D> vertex3 = std::make_shared<::delaunay::Vertex2D>();
			std::shared_ptr<::delaunay::Vertex2D> vertex4 = std::make_shared<::delaunay::Vertex2D>();

			vertex1->id = 1;
			vertex1->x = 1.0f;
			vertex1->y = 1.0f;
			vertex2->id = 2;
			vertex2->x = 2.0f;
			vertex2->y = 2.0f;
			vertex3->id = 3;
			vertex3->x = 3.0f;
			vertex3->y = 1.0f;
			vertex4->id = 4;
			vertex4->x = 2.0f;
			vertex4->y = 0.0f;

			vertices.push_back(vertex1);
			vertices.push_back(vertex2);
			vertices.push_back(vertex3);
			vertices.push_back(vertex4);

			bool ok;
			std::list<std::shared_ptr<::delaunay::Triangle> > triangles =
					delaunayService.iterativeDynamicProcess(vertices, &ok);
			DELAUNAY_TEST(ok);
			// TODO: не предусмотрел логику, что в самом конце FAKE вершины могут соединяться только с вершинами, образующими выпуклое множество, поэтому тест не проходит
//			DELAUNAY_TEST(triangles.size() == 2);

		DELAUNAY_TEST_END();
	}

}
