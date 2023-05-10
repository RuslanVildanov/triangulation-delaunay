#include "delaunay_service.h"

#include "delaunay_factory.h"
#include "triangle.h"
#include "triangulation_cache.h"
#include "utils.h"
#include "vertex_2d.h"
#include <cassert>


namespace delaunay
{
	/**
	 * Функция инициализации триангуляционного кэша, если бы он был сделан в
	 * классическом виде - т.е. с обработкой ситуации когда новая вершина не попадает
	 * в существующий треугольник в кэше, а происходит поиск таких вершин с которыми
	 * можно создать треугольники, так чтобы получилось выпуклая область триангуляции.
	 * @param cache
	 * @param delaunayFactory
	 * @param vertices
	 * @param ok
	 * @return
	 */
	std::list<std::shared_ptr<Vertex2D> > defaultStep1(
			TriangulationCache* cache,
			DelaunayFactory* delaunayFactory,
			const std::list<std::shared_ptr<Vertex2D> >& vertices,
			bool* ok)
	{
		std::shared_ptr<Triangle> firstTriangle = Utils::createFirstTriangle(
				vertices, delaunayFactory);
		if (!firstTriangle || !firstTriangle->isValid()) {
			if (ok) {
				*ok = false;
			}
			return {};
		}
		if (ok) {
			*ok = true;
		}

		bool thisOk = cache->insertFirstTrianglesWithSameRectangle({firstTriangle});
		assert(thisOk);
		return {firstTriangle->vertex1, firstTriangle->vertex2, firstTriangle->vertex3};
	}

	/**
	 * Функция необходима для инициализации триангуляционного кэша FAKE треугольниками.
	 * Сделана, чтобы ускорить результат.
	 * @param cache
	 * @param delaunayFactory
	 * @param vertices
	 * @param ok
	 * @return
	 */
	std::list<std::shared_ptr<Vertex2D> > fakeSquareStep1(
			TriangulationCache* cache,
			DelaunayFactory* delaunayFactory,
			const std::list<std::shared_ptr<Vertex2D> >& vertices,
			bool* ok)
	{
		if (vertices.size() < 3) {
			if (ok) {
				*ok = false;
			}
			return {};
		}
		if (ok) {
			*ok = true;
		}
		std::vector<std::shared_ptr<Vertex2D> > fakeVertices =
				Utils::createVerticesAsRectangle(vertices);
		assert(fakeVertices.size() == 4);

		std::shared_ptr<Triangle> firstTriangle = Utils::createFirstTriangle(
			{fakeVertices.cbegin(), fakeVertices.cend()}, delaunayFactory);
		assert(firstTriangle && firstTriangle->isValid());

		std::shared_ptr<Triangle> secondTriangle = delaunayFactory->createTriangle(
				fakeVertices[3], fakeVertices[2], fakeVertices[1]);
		assert(secondTriangle->isValid());
		std::shared_ptr<Triangle> emptyTriangle;
		bool thisOk = Utils::bindTriangles(
				firstTriangle, secondTriangle, emptyTriangle, emptyTriangle);
		assert(thisOk);

		thisOk = cache->insertFirstTrianglesWithSameRectangle(
				{firstTriangle, secondTriangle});
		assert(thisOk);

		// Возвращаем пустой список так как добавлены fake вершины, а не из входного списка
		return {};
	}

	std::list<std::shared_ptr<Triangle> > DelaunayService::iterativeDynamicProcess(
			const std::list<std::shared_ptr<Vertex2D> >& vertices,
			bool* ok)
	{
		std::list<std::shared_ptr<Triangle> > result =
				generalIterativeDynamicProcess(fakeSquareStep1, vertices, ok);

		// TODO: не корректно просто удалять треугольники с FAKE_ID,
		//  сначала по вершинам без FAKE_ID нужно построить выпуклое множество вершин
		//  и перестроить некоторые треугольники так, чтобы после удаления треугольников
		//  с FAKE_ID осталось только выпуклое множество
		auto i = result.begin();
		while (i != result.end()) {
			std::shared_ptr<Triangle>& triangle = *i;
			if (triangle->vertex1->id == FAKE_ID
					|| triangle->vertex2->id == FAKE_ID
					|| triangle->vertex3->id == FAKE_ID) {
				i = result.erase(i);
			}
			else {
				++i;
			}
		}
		return result;
	}

	std::list<std::shared_ptr<Triangle> > DelaunayService::generalIterativeDynamicProcess(
			const std::function<std::list<std::shared_ptr<Vertex2D> >(
					TriangulationCache*,
					DelaunayFactory*,
					const std::list<std::shared_ptr<Vertex2D> >&,
					bool*) >& step1,
			const std::list<std::shared_ptr<Vertex2D> >& vertices,
			bool* ok)
	{
		if (ok) {
			*ok = true;
		}

		bool thisOk;
		std::list<std::shared_ptr<Triangle> > createdTriangles;
		DelaunayFactory delaunayFactory;
		TriangulationCache cache;

		// Шаг 1. На первых трёх исходных точках строим один треугольник (предполагается, что
		// точки не лежат на одной прямой, иначе надо выбрать другие точки).
		std::list<std::shared_ptr<Vertex2D> > addedVertices = step1(
				&cache, &delaunayFactory, vertices, &thisOk);
		if (!thisOk) {
			return {};
		}

		// Шаг 2. В цикле по n для всех остальных точек выполняем шаги 3–5
		for(const auto& nextVertex : vertices) {
			if (!nextVertex) {
				continue;
			}
			bool found = false;
			for (const auto& addedVertex : addedVertices) {
				if (nextVertex->equals(*addedVertex)) {
					found = true;
					break;
				}
			}
			if (found) {
				continue;
			}

			// Шаг 3. Очередная n-я точка добавляется в уже построенную структуру триангуляции
			// следующим образом. Вначале производится локализация точки, т.е. находится
			// треугольник (построенный ранее), в который попадает очередная точка. Либо,
			// если точка не попадает внутрь триангуляции, находится треугольник на границе
			// триангуляции, ближайший к очередной точке.
			std::shared_ptr<Triangle> foundTriangle =
					cache.findAppropriateTriangle(nextVertex);
			if (!foundTriangle) {
				// Не найден треугольник когда nextVertex обнаружена в уже добавленном
				// треугольнике
				continue;
			}

			// Шаг 4. Если точка попала на ранее вставленный узел триангуляции, то такая
			// точка обычно отбрасывается (сделано в условии выше), иначе точка
			// вставляется в триангуляцию в виде нового узла. При этом если точка попала
			// на некоторое ребро, то оно разбивается на два новых, а оба смежных с
			// ребром треугольника также делятся на два меньших. Если точка попала строго
			// внутрь какого-нибудь треугольника, он разбивается на три новых. Если точка
			// попала вне триангуляции, то строится один или более треугольников.
			VertexRelation vertexRelation = foundTriangle->vertexRelation(nextVertex, &thisOk);
			assert(thisOk);
			switch (vertexRelation) {
				case VertexRelation::VertexOutOfTriangle:
					// Если точка попала вне триангуляции, то строится один или более
					// треугольников.
					createdTriangles = cache.createNewSeveralTrianglesNearOuterVertex(
							foundTriangle, nextVertex, &thisOk);
					if(!thisOk) {
						if (ok) {
							*ok = false;
						}
						return {};
					}
					break;
				case VertexRelation::VertexInTriangle:
					// Если точка попала строго внутрь какого-нибудь треугольника, он
					// разбивается на три новых.
					createdTriangles = cache.createNewThreeTrianglesNearInnerVertex(
							foundTriangle, nextVertex, &delaunayFactory);
					break;
				case VertexRelation::VertexOnRib12:
					// При этом если точка попала на некоторое ребро, то оно разбивается
					// на два новых, а оба смежных с ребром треугольника также делятся на
					// два меньших.
					createdTriangles = cache.createNewTwoOrFourTrianglesNearVertexOnRib(
							foundTriangle,
							nextVertex,
							VertexRelation::VertexOnRib12,
							&delaunayFactory);
					break;
				case VertexRelation::VertexOnRib13:
					// При этом если точка попала на некоторое ребро, то оно разбивается
					// на два новых, а оба смежных с ребром треугольника также делятся на
					// два меньших.
					createdTriangles = cache.createNewTwoOrFourTrianglesNearVertexOnRib(
							foundTriangle,
							nextVertex,
							VertexRelation::VertexOnRib13,
							&delaunayFactory);
					break;
				case VertexRelation::VertexOnRib23:
					// При этом если точка попала на некоторое ребро, то оно разбивается
					// на два новых, а оба смежных с ребром треугольника также делятся на
					// два меньших.
					createdTriangles = cache.createNewTwoOrFourTrianglesNearVertexOnRib(
							foundTriangle,
							nextVertex,
							VertexRelation::VertexOnRib23,
							&delaunayFactory);
					break;
			}

			// Шаг 5. Проводятся локальные проверки вновь полученных треугольников на
			// соответствие условию Делоне и выполняются необходимые перестроения.
			cache.checkDelaunayConditionAndRebuildIfNeed(createdTriangles);
		}

		// TODO: Для отладки хорошо бы сделать полную проверку всех полученных
		//  треугольников на то что вообще нет ни одного пересечения рёбер

		return cache.triangles();
	}

	std::set<int> DelaunayService::vertexIds(
			const std::list<std::shared_ptr<Triangle> >& triangles)
	{
		std::set<int> result;
		for (const auto& triangle : triangles) {
			assert(triangle);
			const std::array<int, 3>& vertexIds = triangle->vertexIds();
			result.insert(vertexIds.begin(), vertexIds.end());
		}
		return result;
	}

}
