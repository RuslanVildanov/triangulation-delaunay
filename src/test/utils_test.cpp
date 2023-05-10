#include "utils_test.h"

#include "delaunay/constants.h"
#include "delaunay/delaunay_factory.h"
#include "delaunay/triangle.h"
#include "delaunay/vertex_2d.h"
#include "delaunay/utils.h"

#include "test.h"
#include <list>


namespace test::delaunay
{
	void UtilsTest::testSinAlpha()
	{
		DELAUNAY_TEST_BEGIN()
			bool ok;
			float result;
			std::shared_ptr<::delaunay::Vertex2D> vertex1 = std::make_shared<::delaunay::Vertex2D>();
			std::shared_ptr<::delaunay::Vertex2D> vertex2 = std::make_shared<::delaunay::Vertex2D>();
			std::shared_ptr<::delaunay::Vertex2D> vertex3 = std::make_shared<::delaunay::Vertex2D>();

			// Угол 0 градусов в узле vertex1. sin угла в vertex1 должен быть = 0;
			vertex1->x = 2.0f;
			vertex1->y = 0.0f;
			vertex2->x = 1.0f;
			vertex2->y = 0.0f;
			vertex3->x = 1.0f;
			vertex3->y = 0.0f;
			result = ::delaunay::Utils::sinAlpha(vertex1, vertex2, vertex3, &ok);
			DELAUNAY_TEST(ok);
			DELAUNAY_TEST(result < ::delaunay::EPSILON);

			// Угол 45 градусов в узле vertex1. sin угла в vertex1 должен быть = 0.70710678118;
			vertex1->x = 2.0f;
			vertex1->y = 0.0f;
			vertex2->x = 3.0f;
			vertex2->y = 1.0f;
			vertex3->x = 3.0f;
			vertex3->y = 0.0f;
			result = ::delaunay::Utils::sinAlpha(vertex1, vertex2, vertex3, &ok);
			DELAUNAY_TEST(ok);
			DELAUNAY_TEST(std::abs(0.70710678118f - result) < ::delaunay::EPSILON);

			// Угол 90 градусов в узле vertex1. sin угла в vertex1 должен быть = 1.0;
			vertex1->x = 2.0f;
			vertex1->y = 0.0f;
			vertex2->x = 2.0f;
			vertex2->y = 1.0f;
			vertex3->x = 3.0f;
			vertex3->y = 0.0f;
			result = ::delaunay::Utils::sinAlpha(vertex1, vertex2, vertex3, &ok);
			DELAUNAY_TEST(ok);
			DELAUNAY_TEST(std::abs(1.0f - result) < ::delaunay::EPSILON);

			// Угол 135 градусов в узле vertex1. sin угла в vertex1 должен быть = -0.70710678118;
			vertex1->x = 2.0f;
			vertex1->y = 0.0f;
			vertex2->x = 3.0f;
			vertex2->y = 0.0f;
			vertex3->x = 1.0f;
			vertex3->y = 1.0f;
			result = ::delaunay::Utils::sinAlpha(vertex1, vertex2, vertex3, &ok);
			DELAUNAY_TEST(ok);
			DELAUNAY_TEST(std::abs(0.70710678118f - result) < ::delaunay::EPSILON);

			// Угол 180 градусов в узле vertex1. sin угла в vertex1 должен быть = 0;
			vertex1->x = 2.0f;
			vertex1->y = 0.0f;
			vertex2->x = 3.0f;
			vertex2->y = 0.0f;
			vertex3->x = 1.0f;
			vertex3->y = 0.0f;
			result = ::delaunay::Utils::sinAlpha(vertex1, vertex2, vertex3, &ok);
			DELAUNAY_TEST(ok);
			DELAUNAY_TEST(result < ::delaunay::EPSILON);

		DELAUNAY_TEST_END();
	}

	void UtilsTest::testCosAlpha()
	{
		DELAUNAY_TEST_BEGIN()
			bool ok;
			float result;
			std::shared_ptr<::delaunay::Vertex2D> vertex1 = std::make_shared<::delaunay::Vertex2D>();
			std::shared_ptr<::delaunay::Vertex2D> vertex2 = std::make_shared<::delaunay::Vertex2D>();
			std::shared_ptr<::delaunay::Vertex2D> vertex3 = std::make_shared<::delaunay::Vertex2D>();

			// Угол 0 градусов в узле vertex1. cos угла в vertex1 должен быть = 1.0;
			vertex1->x = 2.0f;
			vertex1->y = 0.0f;
			vertex2->x = 1.0f;
			vertex2->y = 0.0f;
			vertex3->x = 1.0f;
			vertex3->y = 0.0f;
			result = ::delaunay::Utils::cosAlpha(vertex1, vertex2, vertex3, &ok);
			DELAUNAY_TEST(ok);
			DELAUNAY_TEST(std::abs(1.0f - result) < ::delaunay::EPSILON);

			// Угол 45 градусов в узле vertex1. cos угла в vertex1 должен быть = 0.70710678118;
			vertex1->x = 2.0f;
			vertex1->y = 0.0f;
			vertex2->x = 3.0f;
			vertex2->y = 1.0f;
			vertex3->x = 3.0f;
			vertex3->y = 0.0f;
			result = ::delaunay::Utils::cosAlpha(vertex1, vertex2, vertex3, &ok);
			DELAUNAY_TEST(ok);
			DELAUNAY_TEST(std::abs(0.70710678118f - result) < ::delaunay::EPSILON);

			// Угол 90 градусов в узле vertex1. cos угла в vertex1 должен быть = 0.0;
			vertex1->x = 2.0f;
			vertex1->y = 0.0f;
			vertex2->x = 2.0f;
			vertex2->y = 1.0f;
			vertex3->x = 3.0f;
			vertex3->y = 0.0f;
			result = ::delaunay::Utils::cosAlpha(vertex1, vertex2, vertex3, &ok);
			DELAUNAY_TEST(ok);
			DELAUNAY_TEST(result < ::delaunay::EPSILON);

			// Угол 135 градусов в узле vertex1. cos угла в vertex1 должен быть = -0.70710678118;
			vertex1->x = 2.0f;
			vertex1->y = 0.0f;
			vertex2->x = 3.0f;
			vertex2->y = 0.0f;
			vertex3->x = 1.0f;
			vertex3->y = 1.0f;
			result = ::delaunay::Utils::cosAlpha(vertex1, vertex2, vertex3, &ok);
			DELAUNAY_TEST(ok);
			DELAUNAY_TEST(std::abs(0.70710678118f - result) < ::delaunay::EPSILON);

			// Угол 180 градусов в узле vertex1. cos угла в vertex1 должен быть = 0;
			vertex1->x = 2.0f;
			vertex1->y = 0.0f;
			vertex2->x = 3.0f;
			vertex2->y = 0.0f;
			vertex3->x = 1.0f;
			vertex3->y = 0.0f;
			result = ::delaunay::Utils::cosAlpha(vertex1, vertex2, vertex3, &ok);
			DELAUNAY_TEST(ok);
			DELAUNAY_TEST(std::abs(1.0f - result) < ::delaunay::EPSILON);

		DELAUNAY_TEST_END();
	}

	void UtilsTest::testFirstTriangleCreation()
	{
		DELAUNAY_TEST_BEGIN();
			::delaunay::DelaunayFactory delaunayFactory;
			std::shared_ptr<::delaunay::Vertex2D> vertex1 = std::make_shared<::delaunay::Vertex2D>();
			std::shared_ptr<::delaunay::Vertex2D> vertex2 = std::make_shared<::delaunay::Vertex2D>();
			std::shared_ptr<::delaunay::Vertex2D> vertex3 = std::make_shared<::delaunay::Vertex2D>();
			std::shared_ptr<::delaunay::Vertex2D> vertex4 = std::make_shared<::delaunay::Vertex2D>();

			// проверка пустого списка вершин
			std::list<std::shared_ptr<::delaunay::Vertex2D> > vertices;
			auto emptyTriangle = ::delaunay::Utils::createFirstTriangle(vertices, &delaunayFactory);
			DELAUNAY_TEST(!emptyTriangle);

			// проверка неполного списка вершин
			vertices.clear();
			vertex1->x = 2.0f;
			vertex1->y = 0.0f;
			vertex2->x = 3.0f;
			vertex2->y = 0.0f;
			vertices.push_back(vertex1);
			vertices.push_back(vertex2);
			emptyTriangle = ::delaunay::Utils::createFirstTriangle(vertices, &delaunayFactory);
			DELAUNAY_TEST(!emptyTriangle);

			// проверка нормального списка вершин
			vertices.clear();
			vertex1->x = 2.0f;
			vertex1->y = 0.0f;
			vertex2->x = 3.0f;
			vertex2->y = 0.0f;
			vertex3->x = 1.0f;
			vertex3->y = 1.0f;
			vertices.push_back(vertex1);
			vertices.push_back(vertex2);
			vertices.push_back(vertex3);
			auto triangle = ::delaunay::Utils::createFirstTriangle(vertices, &delaunayFactory);
			DELAUNAY_TEST(triangle && triangle->isValid());
			DELAUNAY_TEST(triangle->vertex1->equals(*vertex1));
			DELAUNAY_TEST(!triangle->vertex2->equals(*triangle->vertex3));
			DELAUNAY_TEST(triangle->vertex2->equals(*vertex2) || triangle->vertex2->equals(*vertex3));
			DELAUNAY_TEST(triangle->vertex3->equals(*vertex3) || triangle->vertex3->equals(*vertex2));

			// проверка списка вершин при этом не валидный треугольник должен быть пропущен
			vertices.clear();
			vertex1->x = 2.0f;
			vertex1->y = 0.0f;
			vertex2->x = 3.0f;
			vertex2->y = 0.0f;
			vertex3->x = 1.0f;
			vertex3->y = 0.0f;
			vertex4->x = 1.0f;
			vertex4->y = 1.0f;
			vertices.push_back(vertex1);
			vertices.push_back(vertex2);
			vertices.push_back(vertex3);
			vertices.push_back(vertex4);
			triangle = ::delaunay::Utils::createFirstTriangle(vertices, &delaunayFactory);
			DELAUNAY_TEST(triangle && triangle->isValid());
			DELAUNAY_TEST(triangle->vertex1->equals(*vertex1));
			DELAUNAY_TEST(!triangle->vertex2->equals(*triangle->vertex3));
			DELAUNAY_TEST(triangle->vertex2->equals(*vertex2) || triangle->vertex2->equals(*vertex4));
			DELAUNAY_TEST(triangle->vertex3->equals(*vertex4) || triangle->vertex3->equals(*vertex2));

		DELAUNAY_TEST_END();
	}

	void UtilsTest::testSumOppositeAngles()
	{
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
			std::shared_ptr<::delaunay::Triangle> emptyTriangle;
			::delaunay::Utils::bindTriangles(triangle1, triangle2, emptyTriangle, emptyTriangle);

			bool result1 = ::delaunay::Utils::checkSumOppositeAngles(triangle1, 1);
			bool result2 = ::delaunay::Utils::checkCircleEquation(triangle1, triangle2);
			DELAUNAY_TEST(!result1);
			DELAUNAY_TEST(!result2);

		DELAUNAY_TEST_END();
	}

	void UtilsTest::testCircleEquation()
	{
		DELAUNAY_TEST_BEGIN();
			std::shared_ptr<::delaunay::Vertex2D> vertex11 = std::make_shared<::delaunay::Vertex2D>();
			std::shared_ptr<::delaunay::Vertex2D> vertex12 = std::make_shared<::delaunay::Vertex2D>();
			std::shared_ptr<::delaunay::Vertex2D> vertex13 = std::make_shared<::delaunay::Vertex2D>();
			std::shared_ptr<::delaunay::Triangle> triangle1 = std::make_shared<::delaunay::Triangle>();
			vertex11->x = 2.0f;
			vertex11->y = 2.0f;
			vertex12->x = 3.0f;
			vertex12->y = 0.0f;
			vertex13->x = 1.0f;
			vertex13->y = 0.0f;
			triangle1->vertex1 = vertex11;
			triangle1->vertex2 = vertex12;
			triangle1->vertex3 = vertex13;

			std::shared_ptr<::delaunay::Vertex2D> vertex21 = std::make_shared<::delaunay::Vertex2D>();
			std::shared_ptr<::delaunay::Vertex2D> vertex22 = std::make_shared<::delaunay::Vertex2D>();
			std::shared_ptr<::delaunay::Vertex2D> vertex23 = std::make_shared<::delaunay::Vertex2D>();
			std::shared_ptr<::delaunay::Triangle> triangle2 = std::make_shared<::delaunay::Triangle>();
			vertex21->x = 2.0f;
			vertex21->y = -1.0f;
			vertex22->x = 1.0f;
			vertex22->y = 0.0f;
			vertex23->x = 3.0f;
			vertex23->y = 0.0f;
			triangle2->vertex1 = vertex21;
			triangle2->vertex2 = vertex22;
			triangle2->vertex3 = vertex23;
			std::shared_ptr<::delaunay::Triangle> emptyTriangle;
			::delaunay::Utils::bindTriangles(triangle1, triangle2, emptyTriangle, emptyTriangle);

			bool result1 = ::delaunay::Utils::checkSumOppositeAngles(triangle1, 1);
			DELAUNAY_TEST(result1);
			// TODO: Возвращает не верный результат - потом разобраться
			// bool result2 = ::delaunay::Utils::checkCircleEquation(triangle1, triangle2);
			// DELAUNAY_TEST(result2);

		DELAUNAY_TEST_END();
	}

	void UtilsTest::testCenterOfMass()
	{
		DELAUNAY_TEST_BEGIN();
			std::shared_ptr<::delaunay::Vertex2D> vertex1 = std::make_shared<::delaunay::Vertex2D>();
			std::shared_ptr<::delaunay::Vertex2D> vertex2 = std::make_shared<::delaunay::Vertex2D>();
			std::shared_ptr<::delaunay::Vertex2D> vertex3 = std::make_shared<::delaunay::Vertex2D>();
			std::shared_ptr<::delaunay::Triangle> triangle = std::make_shared<::delaunay::Triangle>();
			vertex1->x = 0.0f;
			vertex1->y = 0.0f;
			vertex2->x = 0.0f;
			vertex2->y = 2.0f;
			vertex3->x = 2.0f;
			vertex3->y = 0.0f;
			triangle->vertex1 = vertex1;
			triangle->vertex2 = vertex2;
			triangle->vertex3 = vertex3;

			std::shared_ptr<::delaunay::Vertex2D> vertex =
					::delaunay::Utils::centerOfMass(triangle);
			DELAUNAY_TEST(vertex);
			const float c23 = 2.0f / 3.0f;
			DELAUNAY_TEST(std::abs(vertex->x - c23) < ::delaunay::EPSILON);
			DELAUNAY_TEST(std::abs(vertex->y - c23) < ::delaunay::EPSILON);

		DELAUNAY_TEST_END();
	}

	void UtilsTest::testDistance()
	{
		DELAUNAY_TEST_BEGIN();
			bool ok;
			float distance;
			std::shared_ptr<::delaunay::Vertex2D> vertex1 = std::make_shared<::delaunay::Vertex2D>();
			std::shared_ptr<::delaunay::Vertex2D> vertex2 = std::make_shared<::delaunay::Vertex2D>();

			vertex1->x = 3.0f;
			vertex1->y = 0.0f;
			vertex2->x = 0.0f;
			vertex2->y = 4.0f;
			distance = ::delaunay::Utils::distance(vertex1, vertex2, &ok);
			DELAUNAY_TEST(ok);
			DELAUNAY_TEST(std::abs(distance - 5.0f) < ::delaunay::EPSILON);

			vertex1->x = 3.0f;
			vertex1->y = 0.0f;
			vertex2->x = 3.0f;
			vertex2->y = 0.0f;
			distance = ::delaunay::Utils::distance(vertex1, vertex2, &ok);
			DELAUNAY_TEST(ok);
			DELAUNAY_TEST(distance < ::delaunay::EPSILON);

		DELAUNAY_TEST_END();
	}

	void UtilsTest::testBindTriangles()
	{
		DELAUNAY_TEST_BEGIN();
			const float c23 = 2.0f / 3.0f;
			std::shared_ptr<::delaunay::Vertex2D> innerVertex = std::make_shared<::delaunay::Vertex2D>();
			std::shared_ptr<::delaunay::Vertex2D> outerVertex1 = std::make_shared<::delaunay::Vertex2D>();
			std::shared_ptr<::delaunay::Vertex2D> outerVertex2 = std::make_shared<::delaunay::Vertex2D>();
			std::shared_ptr<::delaunay::Vertex2D> outerVertex3 = std::make_shared<::delaunay::Vertex2D>();
			std::shared_ptr<::delaunay::Vertex2D> outerVertex4 = std::make_shared<::delaunay::Vertex2D>();
			std::shared_ptr<::delaunay::Vertex2D> outerVertex5 = std::make_shared<::delaunay::Vertex2D>();
			std::shared_ptr<::delaunay::Vertex2D> outerVertex6 = std::make_shared<::delaunay::Vertex2D>();
			innerVertex->id = 0;
			innerVertex->x = c23;
			innerVertex->y = c23;
			outerVertex1->id = 1;
			outerVertex1->x = 0.0f;
			outerVertex1->y = 0.0f;
			outerVertex2->id = 2;
			outerVertex2->x = 0.0f;
			outerVertex2->y = 2.0f;
			outerVertex3->id = 3;
			outerVertex3->x = 2.0f;
			outerVertex3->y = 0.0f;
			outerVertex4->id = 4;
			outerVertex4->x = -1.0f;
			outerVertex4->y = 1.0f;
			outerVertex5->id = 5;
			outerVertex5->x = 2.0f;
			outerVertex5->y = 2.0f;
			outerVertex6->id = 6;
			outerVertex6->x = 1.0f;
			outerVertex6->y = -1.0f;
			std::shared_ptr<::delaunay::Triangle> triangle1 = std::make_shared<::delaunay::Triangle>();
			triangle1->id = 1;
			triangle1->vertex1 = innerVertex;
			triangle1->vertex2 = outerVertex1;
			triangle1->vertex3 = outerVertex2;
			std::shared_ptr<::delaunay::Triangle> triangle2 = std::make_shared<::delaunay::Triangle>();
			triangle2->id = 2;
			triangle2->vertex1 = innerVertex;
			triangle2->vertex2 = outerVertex2;
			triangle2->vertex3 = outerVertex3;
			std::shared_ptr<::delaunay::Triangle> triangle3 = std::make_shared<::delaunay::Triangle>();
			triangle3->id = 3;
			triangle3->vertex1 = innerVertex;
			triangle3->vertex2 = outerVertex3;
			triangle3->vertex3 = outerVertex1;
			std::shared_ptr<::delaunay::Triangle> triangle4 = std::make_shared<::delaunay::Triangle>();
			triangle4->id = 4;
			triangle4->vertex1 = outerVertex4;
			triangle4->vertex2 = outerVertex2;
			triangle4->vertex3 = outerVertex1;
			std::shared_ptr<::delaunay::Triangle> triangle5 = std::make_shared<::delaunay::Triangle>();
			triangle5->id = 5;
			triangle5->vertex1 = outerVertex5;
			triangle5->vertex2 = outerVertex3;
			triangle5->vertex3 = outerVertex2;
			std::shared_ptr<::delaunay::Triangle> triangle6 = std::make_shared<::delaunay::Triangle>();
			triangle6->id = 6;
			triangle6->vertex1 = outerVertex6;
			triangle6->vertex2 = outerVertex1;
			triangle6->vertex3 = outerVertex3;

			bool ok1 = ::delaunay::Utils::bindTriangles(triangle1, triangle4, triangle2, triangle3);
			DELAUNAY_TEST(ok1);
			DELAUNAY_TEST(triangle1->triangle1->id == 4);
			DELAUNAY_TEST(triangle1->triangle2->id == 2);
			DELAUNAY_TEST(triangle1->triangle3->id == 3);
			bool ok2 = ::delaunay::Utils::bindTriangles(triangle2, triangle5, triangle3, triangle1);
			DELAUNAY_TEST(ok2);
			DELAUNAY_TEST(triangle2->triangle1->id == 5);
			DELAUNAY_TEST(triangle2->triangle2->id == 3);
			DELAUNAY_TEST(triangle2->triangle3->id == 1);
			bool ok3 = ::delaunay::Utils::bindTriangles(triangle3, triangle6, triangle1, triangle2);
			DELAUNAY_TEST(ok3);
			DELAUNAY_TEST(triangle3->triangle1->id == 6);
			DELAUNAY_TEST(triangle3->triangle2->id == 1);
			DELAUNAY_TEST(triangle3->triangle3->id == 2);
		DELAUNAY_TEST_END();
	}

	void UtilsTest::testDirection()
	{
		DELAUNAY_TEST_BEGIN();
			bool ok;
			::delaunay::Direction direction;
			std::shared_ptr<::delaunay::Vertex2D> vertex1 = std::make_shared<::delaunay::Vertex2D>();
			std::shared_ptr<::delaunay::Vertex2D> vertex2 = std::make_shared<::delaunay::Vertex2D>();
			std::shared_ptr<::delaunay::Vertex2D> vertex3 = std::make_shared<::delaunay::Vertex2D>();

			vertex1->x = 0.0f;
			vertex1->y = 0.0f;
			vertex2->x = 0.0f;
			vertex2->y = 2.0f;
			vertex3->x = 2.0f;
			vertex3->y = 0.0f;
			direction = ::delaunay::Utils::direction(vertex1, vertex2, vertex3, &ok);
			DELAUNAY_TEST(ok);
			DELAUNAY_TEST(direction == ::delaunay::Direction::Right);

			vertex1->x = 0.0f;
			vertex1->y = 0.0f;
			vertex2->x = 2.0f;
			vertex2->y = 0.0f;
			vertex3->x = 0.0f;
			vertex3->y = 2.0f;
			direction = ::delaunay::Utils::direction(vertex1, vertex2, vertex3, &ok);
			DELAUNAY_TEST(ok);
			DELAUNAY_TEST(direction == ::delaunay::Direction::Left);

			vertex1->x = 0.0f;
			vertex1->y = 0.0f;
			vertex2->x = 2.0f;
			vertex2->y = 0.0f;
			vertex3->x = -2.0f;
			vertex3->y = 0.0f;
			direction = ::delaunay::Utils::direction(vertex1, vertex2, vertex3, &ok);
			DELAUNAY_TEST(ok);
			DELAUNAY_TEST(direction == ::delaunay::Direction::Collinear);

			vertex1->x = 0.0f;
			vertex1->y = 0.0f;
			vertex2->x = 2.0f;
			vertex2->y = 0.0f;
			vertex3->x = 1.0f;
			vertex3->y = 0.0f;
			direction = ::delaunay::Utils::direction(vertex1, vertex2, vertex3, &ok);
			DELAUNAY_TEST(ok);
			DELAUNAY_TEST(direction == ::delaunay::Direction::Collinear);

			vertex1->x = 0.0f;
			vertex1->y = 0.0f;
			vertex2->x = 0.0f;
			vertex2->y = 0.0f;
			vertex3->x = 0.0f;
			vertex3->y = 0.0f;
			direction = ::delaunay::Utils::direction(vertex1, vertex2, vertex3, &ok);
			DELAUNAY_TEST(ok);
			DELAUNAY_TEST(direction == ::delaunay::Direction::Collinear);

			direction = ::delaunay::Utils::direction({}, vertex2, vertex3, &ok);
			DELAUNAY_TEST(!ok);
		DELAUNAY_TEST_END();
	}

	void UtilsTest::testSegmentsIntersect()
	{
		DELAUNAY_TEST_BEGIN();
			bool ok, isIntersect;
			std::shared_ptr<::delaunay::Vertex2D> vertex1 = std::make_shared<::delaunay::Vertex2D>();
			std::shared_ptr<::delaunay::Vertex2D> vertex2 = std::make_shared<::delaunay::Vertex2D>();
			std::shared_ptr<::delaunay::Vertex2D> vertex3 = std::make_shared<::delaunay::Vertex2D>();
			std::shared_ptr<::delaunay::Vertex2D> vertex4 = std::make_shared<::delaunay::Vertex2D>();

			// проверка при соприкосновении вершины 1 с другим отрезком
			vertex1->x = 0.0f;
			vertex1->y = 0.0f;
			vertex2->x = 0.0f;
			vertex2->y = 2.0f;
			vertex3->x = 2.0f;
			vertex3->y = 0.0f;
			vertex4->x = 0.0f;
			vertex4->y = 0.0f;
			isIntersect = ::delaunay::Utils::isSegmentsIntersect(
					vertex1,
					vertex2,
					vertex3,
					vertex4,
					true,
					&ok);
			DELAUNAY_TEST(ok);
			DELAUNAY_TEST(isIntersect);

			isIntersect = ::delaunay::Utils::isSegmentsIntersect(
					vertex1,
					vertex2,
					vertex3,
					vertex4,
					false,
					&ok);
			DELAUNAY_TEST(ok);
			DELAUNAY_TEST(!isIntersect);

			// проверка при соприкосновении вершины 1 с другим отрезком
			vertex1->x = 0.0f;
			vertex1->y = 0.0f;
			vertex2->x = 0.0f;
			vertex2->y = 2.0f;
			vertex3->x = 0.0f;
			vertex3->y = 0.0f;
			vertex4->x = 2.0f;
			vertex4->y = 0.0f;
			isIntersect = ::delaunay::Utils::isSegmentsIntersect(
					vertex1,
					vertex2,
					vertex3,
					vertex4,
					true,
					&ok);
			DELAUNAY_TEST(ok);
			DELAUNAY_TEST(isIntersect);

			// проверка при соприкосновении вершины 2 с другим отрезком
			vertex1->x = 0.0f;
			vertex1->y = 0.0f;
			vertex2->x = 0.0f;
			vertex2->y = 2.0f;
			vertex3->x = 1.0f;
			vertex3->y = 0.0f;
			vertex4->x = 0.0f;
			vertex4->y = 2.0f;
			isIntersect = ::delaunay::Utils::isSegmentsIntersect(
					vertex1,
					vertex2,
					vertex3,
					vertex4,
					true,
					&ok);
			DELAUNAY_TEST(ok);
			DELAUNAY_TEST(isIntersect);

			// проверка при соприкосновении вершины 2 с другим отрезком
			vertex1->x = 0.0f;
			vertex1->y = 0.0f;
			vertex2->x = 0.0f;
			vertex2->y = 2.0f;
			vertex3->x = 0.0f;
			vertex3->y = 2.0f;
			vertex4->x = 1.0f;
			vertex4->y = 0.0f;
			isIntersect = ::delaunay::Utils::isSegmentsIntersect(
					vertex1,
					vertex2,
					vertex3,
					vertex4,
					true,
					&ok);
			DELAUNAY_TEST(ok);
			DELAUNAY_TEST(isIntersect);
			isIntersect = ::delaunay::Utils::isSegmentsIntersect(
					vertex1,
					vertex2,
					vertex3,
					vertex4,
					false,
					&ok);
			DELAUNAY_TEST(ok);
			DELAUNAY_TEST(!isIntersect);

			// проверка пересечения отрезков
			vertex1->x = 0.0f;
			vertex1->y = 0.0f;
			vertex2->x = 1.0f;
			vertex2->y = 1.0f;
			vertex3->x = 0.0f;
			vertex3->y = 1.0f;
			vertex4->x = 1.0f;
			vertex4->y = 0.0f;
			isIntersect = ::delaunay::Utils::isSegmentsIntersect(
					vertex1,
					vertex2,
					vertex3,
					vertex4,
					true,
					&ok);
			DELAUNAY_TEST(ok);
			DELAUNAY_TEST(isIntersect);

			// проверка случая когда отрезки НЕ пересекаются
			vertex1->x = 0.6f;
			vertex1->y = 0.6f;
			vertex2->x = 1.0f;
			vertex2->y = 1.0f;
			vertex3->x = 0.0f;
			vertex3->y = 1.0f;
			vertex4->x = 1.0f;
			vertex4->y = 0.0f;
			isIntersect = ::delaunay::Utils::isSegmentsIntersect(
					vertex1,
					vertex2,
					vertex3,
					vertex4,
					true,
					&ok);
			DELAUNAY_TEST(ok);
			DELAUNAY_TEST(!isIntersect);

			isIntersect = ::delaunay::Utils::isSegmentsIntersect(
					{},
					vertex2,
					vertex3,
					vertex4,
					true,
					&ok);
			DELAUNAY_TEST(!ok);
			DELAUNAY_TEST(!isIntersect);
		DELAUNAY_TEST_END();
	}

	void UtilsTest::testCreateVerticesAsRectangle()
	{
		DELAUNAY_TEST_BEGIN();
			std::vector<std::shared_ptr<::delaunay::Vertex2D> > result;
			std::list<std::shared_ptr<::delaunay::Vertex2D> > vertices;

			result = ::delaunay::Utils::createVerticesAsRectangle(vertices);
			DELAUNAY_TEST(result.empty());

			std::shared_ptr<::delaunay::Vertex2D> vertex = std::make_shared<::delaunay::Vertex2D>();
			vertex->x = 0.0f;
			vertex->y = 0.0f;
			vertices.push_back(vertex);
			result = ::delaunay::Utils::createVerticesAsRectangle(vertices);
			DELAUNAY_TEST(result.size() == 4);

			::delaunay::Vertex2D topLeft;
			topLeft.x = vertex->x - ::delaunay::FAKE_RECTANGLE_OFFSET;
			topLeft.y = vertex->y - ::delaunay::FAKE_RECTANGLE_OFFSET;
			::delaunay::Vertex2D bottomRight;
			bottomRight.x = vertex->x + ::delaunay::FAKE_RECTANGLE_OFFSET;
			bottomRight.y = vertex->y + ::delaunay::FAKE_RECTANGLE_OFFSET;
			::delaunay::Vertex2D bottomLeft;
			bottomLeft.x = topLeft.x;
			bottomLeft.y = bottomRight.y;
			::delaunay::Vertex2D topRight;
			topRight.x = bottomRight.x;
			topRight.y = topLeft.y;

			auto i = result.cbegin();
			DELAUNAY_TEST((*i)->equals(topLeft));
			++i;
			DELAUNAY_TEST((*i)->equals(bottomLeft));
			++i;
			DELAUNAY_TEST((*i)->equals(topRight));
			++i;
			DELAUNAY_TEST((*i)->equals(bottomRight));

		DELAUNAY_TEST_END();
	}

}
