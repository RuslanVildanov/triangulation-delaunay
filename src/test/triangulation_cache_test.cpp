#include "triangulation_cache_test.h"

#include "delaunay/delaunay_factory.h"
#include "delaunay/triangulation_cache.h"
#include "delaunay/triangle.h"
#include "delaunay/utils.h"
#include "delaunay/vertex_2d.h"

#include "test.h"

#include <list>
#include <memory>
#include <cassert>


namespace test::delaunay
{
	void TriangulationCacheTest::testFindAppropriateTriangle()
	{
		DELAUNAY_TEST_BEGIN();
			std::shared_ptr<::delaunay::Vertex2D> vertex = std::make_shared<::delaunay::Vertex2D>();
			std::shared_ptr<::delaunay::Vertex2D> fakeVertex1 = std::make_shared<::delaunay::Vertex2D>();
			std::shared_ptr<::delaunay::Vertex2D> fakeVertex2 = std::make_shared<::delaunay::Vertex2D>();
			std::shared_ptr<::delaunay::Vertex2D> fakeVertex3 = std::make_shared<::delaunay::Vertex2D>();
			std::shared_ptr<::delaunay::Vertex2D> fakeVertex4 = std::make_shared<::delaunay::Vertex2D>();

			fakeVertex1->id = ::delaunay::FAKE_ID;
			fakeVertex1->x = 0.0f;
			fakeVertex1->y = -1.0f;
			fakeVertex2->id = ::delaunay::FAKE_ID;
			fakeVertex2->x = 0.0f;
			fakeVertex2->y = 3.0f;
			fakeVertex3->id = ::delaunay::FAKE_ID;
			fakeVertex3->x = 4.0f;
			fakeVertex3->y = -1.0f;
			fakeVertex4->id = ::delaunay::FAKE_ID;
			fakeVertex4->x = 4.0f;
			fakeVertex4->y = 3.0f;

			std::shared_ptr<::delaunay::Triangle> firstTriangle = std::make_shared<::delaunay::Triangle>();
			std::shared_ptr<::delaunay::Triangle> secondTriangle = std::make_shared<::delaunay::Triangle>();

			firstTriangle->id = 1;
			firstTriangle->vertex1 = fakeVertex1;
			firstTriangle->vertex2 = fakeVertex2;
			firstTriangle->vertex3 = fakeVertex3;
			DELAUNAY_TEST(firstTriangle->isValid());
			secondTriangle->id = 2;
			secondTriangle->vertex1 = fakeVertex4;
			secondTriangle->vertex2 = fakeVertex3;
			secondTriangle->vertex3 = fakeVertex2;
			DELAUNAY_TEST(secondTriangle->isValid());
			std::shared_ptr<::delaunay::Triangle> emptyTriangle;
			bool thisOk = ::delaunay::Utils::bindTriangles(
					firstTriangle, secondTriangle, emptyTriangle, emptyTriangle);
			assert(thisOk);

			std::shared_ptr<::delaunay::Triangle> triangle;
			::delaunay::TriangulationCache cache;
			cache.insertFirstTrianglesWithSameRectangle({firstTriangle, secondTriangle});

			// Тест findAppropriateTriangle при точном попадании vertex внутрь треугольника
			vertex->x = 1.0f;
			vertex->y = 1.0f;
			triangle = cache.findAppropriateTriangle(vertex);
			DELAUNAY_TEST(triangle->id == firstTriangle->id);

			// Тест findAppropriateTriangle при точном попадании vertex на вершину треугольника
			vertex->x = 0.0f;
			vertex->y = -1.0f;
			triangle = cache.findAppropriateTriangle(vertex);
			DELAUNAY_TEST(!triangle);

			// Тест findAppropriateTriangle при поиске ближнего треугольника
			vertex->x = -1.0f;
			vertex->y = -1.0f;
			triangle = cache.findAppropriateTriangle(vertex);
			DELAUNAY_TEST(triangle->id == firstTriangle->id);

		DELAUNAY_TEST_END();
	}

	void TriangulationCacheTest::testCreateNewThreeTrianglesNearInnerVertex()
	{
		DELAUNAY_TEST_BEGIN();
			bool ok;
			::delaunay::DelaunayFactory delaunayFactory;
			std::shared_ptr<::delaunay::Vertex2D> vertex = std::make_shared<::delaunay::Vertex2D>();
			std::shared_ptr<::delaunay::Vertex2D> fakeVertex1 = std::make_shared<::delaunay::Vertex2D>();
			std::shared_ptr<::delaunay::Vertex2D> fakeVertex2 = std::make_shared<::delaunay::Vertex2D>();
			std::shared_ptr<::delaunay::Vertex2D> fakeVertex3 = std::make_shared<::delaunay::Vertex2D>();
			std::shared_ptr<::delaunay::Vertex2D> fakeVertex4 = std::make_shared<::delaunay::Vertex2D>();

			fakeVertex1->id = ::delaunay::FAKE_ID;
			fakeVertex1->x = 0.0f;
			fakeVertex1->y = -1.0f;
			fakeVertex2->id = ::delaunay::FAKE_ID;
			fakeVertex2->x = 0.0f;
			fakeVertex2->y = 3.0f;
			fakeVertex3->id = ::delaunay::FAKE_ID;
			fakeVertex3->x = 4.0f;
			fakeVertex3->y = -1.0f;
			fakeVertex4->id = ::delaunay::FAKE_ID;
			fakeVertex4->x = 4.0f;
			fakeVertex4->y = 3.0f;

			std::shared_ptr<::delaunay::Triangle> firstTriangle = std::make_shared<::delaunay::Triangle>();
			std::shared_ptr<::delaunay::Triangle> secondTriangle = std::make_shared<::delaunay::Triangle>();

			firstTriangle->id = 1;
			firstTriangle->vertex1 = fakeVertex1;
			firstTriangle->vertex2 = fakeVertex2;
			firstTriangle->vertex3 = fakeVertex3;
			DELAUNAY_TEST(firstTriangle->isValid());
			secondTriangle->id = 2;
			secondTriangle->vertex1 = fakeVertex4;
			secondTriangle->vertex2 = fakeVertex3;
			secondTriangle->vertex3 = fakeVertex2;
			DELAUNAY_TEST(secondTriangle->isValid());
			std::shared_ptr<::delaunay::Triangle> emptyTriangle;
			bool thisOk = ::delaunay::Utils::bindTriangles(
					firstTriangle, secondTriangle, emptyTriangle, emptyTriangle);
			assert(thisOk);

			std::shared_ptr<::delaunay::Triangle> triangle;
			::delaunay::TriangulationCache cache;
			cache.insertFirstTrianglesWithSameRectangle({firstTriangle, secondTriangle});

			// vertex внутри треугольника
			vertex->x = 1.0f;
			vertex->y = 1.0f;
			triangle = cache.findAppropriateTriangle(vertex);
			DELAUNAY_TEST(triangle->id == firstTriangle->id);
			DELAUNAY_TEST(triangle->vertexRelation(vertex, &ok) == ::delaunay::VertexRelation::VertexInTriangle);

			std::list<std::shared_ptr<::delaunay::Triangle> >createdTriangles =
					cache.createNewThreeTrianglesNearInnerVertex(triangle, vertex, &delaunayFactory);
			DELAUNAY_TEST(createdTriangles.size() == 3);
			auto i = createdTriangles.begin();
			std::shared_ptr<::delaunay::Triangle> triangle1 = *i;
			++i;
			std::shared_ptr<::delaunay::Triangle> triangle2 = *i;
			++i;
			std::shared_ptr<::delaunay::Triangle> triangle3 = *i;

			DELAUNAY_TEST(triangle1->vertex1 == vertex);
			DELAUNAY_TEST(triangle1->vertex2 == fakeVertex1);
			DELAUNAY_TEST(triangle1->vertex3 == fakeVertex2);
			DELAUNAY_TEST(!triangle1->triangle1);
			DELAUNAY_TEST(triangle1->triangle2.get() == triangle2.get());
			DELAUNAY_TEST(triangle1->triangle3.get() == triangle3.get());

			DELAUNAY_TEST(triangle2->vertex1 == vertex);
			DELAUNAY_TEST(triangle2->vertex2 == fakeVertex2);
			DELAUNAY_TEST(triangle2->vertex3 == fakeVertex3);
			DELAUNAY_TEST(triangle2->triangle1.get() == secondTriangle.get());
			DELAUNAY_TEST(triangle2->triangle2.get() == triangle3.get());
			DELAUNAY_TEST(triangle2->triangle3.get() == triangle1.get());

			DELAUNAY_TEST(triangle3->vertex1 == vertex);
			DELAUNAY_TEST(triangle3->vertex2 == fakeVertex3);
			DELAUNAY_TEST(triangle3->vertex3 == fakeVertex1);
			DELAUNAY_TEST(!triangle3->triangle1);
			DELAUNAY_TEST(triangle3->triangle2.get() == triangle1.get());
			DELAUNAY_TEST(triangle3->triangle3.get() == triangle2.get());

		DELAUNAY_TEST_END();
	}

	void TriangulationCacheTest::testCreateNewTwoOrFourTrianglesNearVertexOnRib()
	{
		DELAUNAY_TEST_BEGIN();
			bool ok;
			::delaunay::DelaunayFactory delaunayFactory;
			std::shared_ptr<::delaunay::Vertex2D> vertex = std::make_shared<::delaunay::Vertex2D>();
			std::shared_ptr<::delaunay::Vertex2D> fakeVertex1 = std::make_shared<::delaunay::Vertex2D>();
			std::shared_ptr<::delaunay::Vertex2D> fakeVertex2 = std::make_shared<::delaunay::Vertex2D>();
			std::shared_ptr<::delaunay::Vertex2D> fakeVertex3 = std::make_shared<::delaunay::Vertex2D>();
			std::shared_ptr<::delaunay::Vertex2D> fakeVertex4 = std::make_shared<::delaunay::Vertex2D>();

			fakeVertex1->id = ::delaunay::FAKE_ID;
			fakeVertex1->x = 0.0f;
			fakeVertex1->y = -1.0f;
			fakeVertex2->id = ::delaunay::FAKE_ID;
			fakeVertex2->x = 0.0f;
			fakeVertex2->y = 3.0f;
			fakeVertex3->id = ::delaunay::FAKE_ID;
			fakeVertex3->x = 4.0f;
			fakeVertex3->y = -1.0f;
			fakeVertex4->id = ::delaunay::FAKE_ID;
			fakeVertex4->x = 4.0f;
			fakeVertex4->y = 3.0f;

			std::shared_ptr<::delaunay::Triangle> firstTriangle = std::make_shared<::delaunay::Triangle>();
			std::shared_ptr<::delaunay::Triangle> secondTriangle = std::make_shared<::delaunay::Triangle>();

			firstTriangle->id = 1;
			firstTriangle->vertex1 = fakeVertex1;
			firstTriangle->vertex2 = fakeVertex2;
			firstTriangle->vertex3 = fakeVertex3;
			DELAUNAY_TEST(firstTriangle->isValid());
			secondTriangle->id = 2;
			secondTriangle->vertex1 = fakeVertex4;
			secondTriangle->vertex2 = fakeVertex3;
			secondTriangle->vertex3 = fakeVertex2;
			DELAUNAY_TEST(secondTriangle->isValid());
			std::shared_ptr<::delaunay::Triangle> emptyTriangle;
			bool thisOk = ::delaunay::Utils::bindTriangles(
					firstTriangle, secondTriangle, emptyTriangle, emptyTriangle);
			assert(thisOk);

			std::shared_ptr<::delaunay::Triangle> triangle;
			::delaunay::TriangulationCache cache;
			cache.insertFirstTrianglesWithSameRectangle({firstTriangle, secondTriangle});

			// vertex на рёбрах двух треугольников
			vertex->x = 2.0f;
			vertex->y = 1.0f;
			triangle = cache.findAppropriateTriangle(vertex);
			DELAUNAY_TEST(triangle->id == firstTriangle->id);
			DELAUNAY_TEST(triangle->vertexRelation(vertex, &ok) == ::delaunay::VertexRelation::VertexOnRib23);
			DELAUNAY_TEST(secondTriangle->vertexRelation(vertex, &ok) == ::delaunay::VertexRelation::VertexOnRib23);

			std::list<std::shared_ptr<::delaunay::Triangle> >createdTriangles =
					cache.createNewTwoOrFourTrianglesNearVertexOnRib(
							triangle,
							vertex,
							::delaunay::VertexRelation::VertexOnRib23,
							&delaunayFactory);
			DELAUNAY_TEST(createdTriangles.size() == 4);

			auto i = createdTriangles.begin();
			std::shared_ptr<::delaunay::Triangle> triangle1 = *i;
			++i;
			std::shared_ptr<::delaunay::Triangle> triangle2 = *i;
			++i;
			std::shared_ptr<::delaunay::Triangle> triangle3 = *i;
			++i;
			std::shared_ptr<::delaunay::Triangle> triangle4 = *i;

			DELAUNAY_TEST(triangle1->vertex1 == fakeVertex1);
			DELAUNAY_TEST(triangle1->vertex2 == fakeVertex2);
			DELAUNAY_TEST(triangle1->vertex3 == vertex);
			DELAUNAY_TEST(triangle1->triangle1.get() == triangle4.get());
			DELAUNAY_TEST(triangle1->triangle2.get() == triangle2.get());
			DELAUNAY_TEST(!triangle1->triangle3);

			DELAUNAY_TEST(triangle2->vertex1 == fakeVertex1);
			DELAUNAY_TEST(triangle2->vertex2 == vertex);
			DELAUNAY_TEST(triangle2->vertex3 == fakeVertex3);
			DELAUNAY_TEST(triangle2->triangle1.get() == triangle3.get());
			DELAUNAY_TEST(!triangle2->triangle2);
			DELAUNAY_TEST(triangle2->triangle3.get() == triangle1.get());

			DELAUNAY_TEST(triangle3->vertex1 == fakeVertex4);
			DELAUNAY_TEST(triangle3->vertex2 == fakeVertex3);
			DELAUNAY_TEST(triangle3->vertex3 == vertex);
			DELAUNAY_TEST(triangle3->triangle1.get() == triangle2.get());
			DELAUNAY_TEST(triangle3->triangle2.get() == triangle4.get());
			DELAUNAY_TEST(!triangle3->triangle3);

			DELAUNAY_TEST(triangle4->vertex1 == fakeVertex4);
			DELAUNAY_TEST(triangle4->vertex2 == vertex);
			DELAUNAY_TEST(triangle4->vertex3 == fakeVertex2);
			DELAUNAY_TEST(triangle4->triangle1.get() == triangle1.get());
			DELAUNAY_TEST(!triangle4->triangle2);
			DELAUNAY_TEST(triangle4->triangle3.get() == triangle3.get());

		DELAUNAY_TEST_END();
	}
	void TriangulationCacheTest::testCheckDelaunayConditionAndRebuildIfNeed()
	{
		// TODO: потом переписать тест
		/*
		DELAUNAY_TEST_BEGIN();
			std::shared_ptr<::delaunay::Vertex2D> vertex11 = std::make_shared<::delaunay::Vertex2D>();
			std::shared_ptr<::delaunay::Vertex2D> vertex12 = std::make_shared<::delaunay::Vertex2D>();
			std::shared_ptr<::delaunay::Vertex2D> vertex13 = std::make_shared<::delaunay::Vertex2D>();
			std::shared_ptr<::delaunay::Triangle> triangle1 = std::make_shared<::delaunay::Triangle>();
			vertex11->x = 1.0f;
			vertex11->y = 0.0f;
			vertex12->x = 2.0f;
			vertex12->y = 2.0f;
			vertex13->x = 2.0f;
			vertex13->y = -1.0f;
			triangle1->vertex1 = vertex11;
			triangle1->vertex2 = vertex12;
			triangle1->vertex3 = vertex13;

			std::shared_ptr<::delaunay::Vertex2D> vertex21 = std::make_shared<::delaunay::Vertex2D>();
			std::shared_ptr<::delaunay::Vertex2D> vertex22 = std::make_shared<::delaunay::Vertex2D>();
			std::shared_ptr<::delaunay::Vertex2D> vertex23 = std::make_shared<::delaunay::Vertex2D>();
			std::shared_ptr<::delaunay::Triangle> triangle2 = std::make_shared<::delaunay::Triangle>();
			vertex21->x = 3.0f;
			vertex21->y = 0.0f;
			vertex22->x = 2.0f;
			vertex22->y = -1.0f;
			vertex23->x = 2.0f;
			vertex23->y = 2.0f;
			triangle2->vertex1 = vertex21;
			triangle2->vertex2 = vertex22;
			triangle2->vertex3 = vertex23;
			bool ok;
			std::shared_ptr<::delaunay::Triangle> emptyTriangle;
			ok = ::delaunay::Utils::bindTriangles(triangle1, triangle2, emptyTriangle, emptyTriangle);
			DELAUNAY_TEST(ok);

			::delaunay::TriangulationCache cache;
			cache.insertFirstTrianglesWithSameRectangle({triangle1, triangle2});

			bool result1 = ::delaunay::Utils::checkSumOppositeAngles(triangle1, 1);
			DELAUNAY_TEST(!result1);

			// Сразу проверяется работа ::delaunay::Utils::flipTriangles
			cache.checkTriangleDelaunayConditionAndRebuildIfNeed(triangle1);
			DELAUNAY_TEST(triangle1->isValid());
			DELAUNAY_TEST(triangle1->vertex1->equals(*vertex11));
			DELAUNAY_TEST(triangle1->vertex2->equals(*vertex21));
			DELAUNAY_TEST(triangle1->vertex3->equals(*vertex13));
			DELAUNAY_TEST(!triangle1->triangle1 && !triangle1->triangle2 && triangle1->triangle3.get() == triangle2.get());

			DELAUNAY_TEST(triangle2->isValid());
			DELAUNAY_TEST(triangle2->vertex1->equals(*vertex21));
			DELAUNAY_TEST(triangle2->vertex2->equals(*vertex11));
			DELAUNAY_TEST(triangle2->vertex3->equals(*vertex23));
			DELAUNAY_TEST(!triangle2->triangle1 && !triangle2->triangle2 && triangle2->triangle3.get() == triangle1.get());

		DELAUNAY_TEST_END();
		*/
	}
}
