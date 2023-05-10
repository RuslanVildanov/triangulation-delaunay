#include "triangulation_cache.h"

#include "delaunay_factory.h"
#include "triangle.h"
#include "vertex_2d.h"
#include "utils.h"

#include <algorithm>
#include <iostream>
#include <cassert>
#include <cmath>


namespace delaunay
{
	// коэффициент роста динамического кэша
	static const int R = 5;
	// Начальный размер кэша - минимум 2
	static const int M = 2;

	bool TriangulationCache::insertFirstTrianglesWithSameRectangle(
			const std::list<std::shared_ptr<Triangle> >& triangles)
	{
		if (triangles.empty()) {
			return false;
		}
		for (const auto& triangle : triangles) {
			if (!triangle || !triangle->isValid()) {
				return false;
			}
		}

		float minX, maxX, minY, maxY;
		const bool ok = Utils::triangleRect(triangles.front(), minX, minY, maxX, maxY);
		assert(ok);
		initCache(minX - EPSILON, minY - EPSILON, maxX + EPSILON, maxY + EPSILON, M);

		for (const auto& triangle : triangles) {
			_triangles.push_back(triangle);
		}
		for(int i = 0; i < _m; i++) {
			for(int j = 0; j < _m; j++) {
				CacheInfo &cacheInfo = _cache.at(i).at(j);
				for (const auto& triangle : triangles) {
					cacheInfo.triangles.push_back(triangle);
				}
			}
		}
		return true;
	}

	const std::list<std::shared_ptr<Triangle> >& TriangulationCache::triangles() const noexcept
	{
		return _triangles;
	}

	bool TriangulationCache::initCache(
			float minX,
			float minY,
			float maxX,
			float maxY,
			int m)
	{
		if (m < 2) {
			return false;
		}

		_m = m;
		_minX = minX;
		_maxX = maxX;
		// коэффициенты перевода из [_minX, _maxX] в [0, _m - 1]
		_aX = static_cast<float>(_m - 1) / (_maxX - _minX);
		_bX = -_minX * _aX;

		_minY = minY;
		_maxY = maxY;
		// коэффициенты перевода из [_minY, _maxY] в [0, _m - 1]
		_aY = static_cast<float>(_m - 1) / (_maxY - _minY);
		_bY = -_minY * _aY;

		_cache.clear();
		_cache.resize(_m);
		for(int i = 0; i < _m; i++) {
			_cache[i].resize(_m);
		}
		return true;
	}

	bool TriangulationCache::needReInitCacheBySize() const noexcept
	{
		size_t vertexNumber = _triangles.size() * 3;
		if (vertexNumber > R * _m * _m) {
			return true;
		}
		return false;
	}

	void TriangulationCache::reInitCache() {
		bool ok = initCache(_minX, _minY, _maxX, _maxY, _m * 2);
		assert(ok);
		for (const auto& triangle : _triangles) {
			cacheTriangle(triangle);
		}
	}

	void TriangulationCache::cacheTriangle(const std::shared_ptr<Triangle>& triangle)
	{
		assert(triangle);
		float minX, maxX, minY, maxY;
		const bool ok = Utils::triangleRect(triangle, minX, minY, maxX, maxY);
		assert(ok);
		int minI, minJ, maxI, maxJ;
		index(minX, minY, minI, minJ);
		index(maxX, maxY, maxI, maxJ);
		assert(minI >= 0 && minI < _m);
		assert(minJ >= 0 && minJ < _m);
		assert(maxI >= 0 && maxI < _m);
		assert(maxJ >= 0 && maxJ < _m);
		assert(minI <= maxI && minJ <= maxJ);
		for(int i = minI; i <= maxI; i++) {
			for(int j = minJ; j <= maxJ; j++) {
				_cache.at(i).at(j).triangles.push_back(triangle);
			}
		}
	}

	void TriangulationCache::addToCache(const std::shared_ptr<Triangle>& triangle)
	{
		assert(triangle);
		if (needReInitCacheBySize()) {
			reInitCache();
		}
		else {
			cacheTriangle(triangle);
		}
	}

	void TriangulationCache::removeFromCache(const std::shared_ptr<Triangle>& triangle)
	{
		float minX, maxX, minY, maxY;
		const bool ok = Utils::triangleRect(triangle, minX, minY, maxX, maxY);
		assert(ok);
		int minI, minJ, maxI, maxJ;
		index(minX, minY, minI, minJ);
		index(maxX, maxY, maxI, maxJ);
		assert(minI >= 0 && minI < _m);
		assert(minJ >= 0 && minJ < _m);
		assert(maxI >= 0 && maxI < _m);
		assert(maxJ >= 0 && maxJ < _m);
		assert(minI <= maxI && minJ <= maxJ);
		for(int i = minI; i <= maxI; i++) {
			for(int j = minJ; j <= maxJ; j++) {
				auto& cachedTriangles = _cache.at(i).at(j).triangles;
				auto k = cachedTriangles.begin();
				while(k != cachedTriangles.end()) {
					auto& cachedTriangle = *k;
					if (cachedTriangle.get() == triangle.get()) {
						k = cachedTriangles.erase(k);
					}
					else {
						++k;
					}
				}
			}
		}
	}

	std::shared_ptr<Triangle> TriangulationCache::findAppropriateTriangle(
			const std::shared_ptr<Vertex2D>& vertex) const
	{
		if (_triangles.size() == 1) {
			const std::shared_ptr<Triangle>& triangle = _triangles.front();
			if (triangle->vertex1->equals(*vertex)
					|| triangle->vertex2->equals(*vertex)
					|| triangle->vertex3->equals(*vertex)) {
				return {};
			}
			return triangle;
		}

		int i, j;
		index(vertex->x, vertex->y, i, j);
		assert(i >= 0 && i < _m);
		assert(j >= 0 && j < _m);
		const CacheInfo& cacheInfo = _cache.at(i).at(j);
		if (cacheInfo.triangles.size() == 1) {
			const std::shared_ptr<Triangle>& triangle = cacheInfo.triangles.front();
			if (triangle->vertex1->equals(*vertex)
				|| triangle->vertex2->equals(*vertex)
				|| triangle->vertex3->equals(*vertex)) {
				return {};
			}
			return triangle;
		}

		// Найти самый близкий треугольник к vertex или вернуть пустой треугольник
		// если vertex обнаружена как уже добавленная вершина
		bool ok;
		bool skipFirst = true;
		float distance;
		float minDistance;
		VertexRelation vertexRelation;
		std::shared_ptr<Vertex2D> centerOfMass;
		std::shared_ptr<Triangle> result;
		for (const auto& triangle : cacheInfo.triangles) {
			if (triangle->vertex1->equals(*vertex)
				|| triangle->vertex2->equals(*vertex)
				|| triangle->vertex3->equals(*vertex)) {
				return {};
			}
			vertexRelation = triangle->vertexRelation(vertex, &ok);
			assert(ok);
			// Попадание внутрь треугольника или на его ребро приоритетнее, чем искать
			// самый близкий треугольник
			if (vertexRelation != VertexRelation::VertexOutOfTriangle) {
				return triangle;
			}

			centerOfMass = Utils::centerOfMass(triangle);
			distance = Utils::distance(centerOfMass, vertex, &ok);
			if (!ok) {
				continue;
			}
			if (skipFirst) {
				result = triangle;
				minDistance = distance;
				skipFirst = false;
				continue;
			}
			if (distance < minDistance) {
				result = triangle;
				minDistance = distance;
			}
		}
		// TODO: сюда попадаем только когда вместо FAKE треугольников, которые
		//  закрывают всю область триангуляции, используется логика
		//  createNewSeveralTrianglesNearOuterVertex
		assert(false);
		return result;
	}

	std::list<std::shared_ptr<Triangle> > TriangulationCache::createNewSeveralTrianglesNearOuterVertex(
			std::shared_ptr<Triangle>& nearestTriangle,
			const std::shared_ptr<Vertex2D>& vertex,
			bool* ok)
	{
		if (!nearestTriangle || !nearestTriangle->isValid()) {
			if (ok) {
				*ok = false;
			}
			return {};
		}

		// TODO: реализация отложена
		(void)vertex;
		assert(false);

		if (ok) {
			*ok = false;
		}
		return {};
	}

	std::list<std::shared_ptr<Triangle> > TriangulationCache::createNewThreeTrianglesNearInnerVertex(
			std::shared_ptr<Triangle>& triangle,
			const std::shared_ptr<Vertex2D>& innerVertex,
			DelaunayFactory* delaunayFactory)
	{
		bool ok;
		assert(triangle->vertexRelation(innerVertex, &ok) == VertexRelation::VertexInTriangle);

		// Добавим два новых треугольника + текущий преобразуем в третий новый треугольник
		std::shared_ptr<Vertex2D> outerVertex1 = triangle->vertex1;
		std::shared_ptr<Vertex2D> outerVertex2 = triangle->vertex2;
		std::shared_ptr<Vertex2D> outerVertex3 = triangle->vertex3;
		// треугольники напротив соответствующих вершин
		std::shared_ptr<Triangle> oppositeTriangle1 = triangle->triangle1;
		std::shared_ptr<Triangle> oppositeTriangle2 = triangle->triangle2;
		std::shared_ptr<Triangle> oppositeTriangle3 = triangle->triangle3;

		// Проверяем, а возможно ли вообще делать преобразование
		Direction newDirection = Utils::direction(innerVertex, outerVertex1, outerVertex2, &ok);
		if (newDirection != Direction::Right) {
			std::cout << "Skipped vertex: " << innerVertex->id << " (validation problem)." << std::endl;
			return {};
		}
		newDirection = Utils::direction(innerVertex, outerVertex2, outerVertex3, &ok);
		if (newDirection != Direction::Right) {
			std::cout << "Skipped vertex: " << innerVertex->id << " (validation problem)." << std::endl;
			return {};
		}
		newDirection = Utils::direction(innerVertex, outerVertex3, outerVertex1, &ok);
		if (newDirection != Direction::Right) {
			std::cout << "Skipped vertex: " << innerVertex->id << " (validation problem)." << std::endl;
			return {};
		}

		removeFromCache(triangle);

		std::shared_ptr<Triangle>& newTriangle1 = triangle;
		newTriangle1->vertex1 = innerVertex;
		newTriangle1->vertex2 = outerVertex1;
		newTriangle1->vertex3 = outerVertex2;
		std::shared_ptr<Triangle> newTriangle2 = delaunayFactory->createTriangle(
				innerVertex, outerVertex2, outerVertex3);
		std::shared_ptr<Triangle> newTriangle3 = delaunayFactory->createTriangle(
				innerVertex, outerVertex3, outerVertex1);

		ok = Utils::bindTriangles(newTriangle1, oppositeTriangle3, newTriangle2, newTriangle3);
		assert(ok);
		ok = Utils::bindTriangles(newTriangle2, oppositeTriangle1, newTriangle3, newTriangle1);
		assert(ok);
		ok = Utils::bindTriangles(newTriangle3, oppositeTriangle2, newTriangle1, newTriangle2);
		assert(ok);

		addToCache(newTriangle1);
		ok = insertNewTriangleWithoutBoundaryRebuilding(newTriangle2);
		assert(ok);
		ok = insertNewTriangleWithoutBoundaryRebuilding(newTriangle3);
		assert(ok);

		std::list<std::shared_ptr<Triangle> > result;
		result.push_back(newTriangle1);
		result.push_back(newTriangle2);
		result.push_back(newTriangle3);
		return result;
	}

	std::list<std::shared_ptr<Triangle> > TriangulationCache::createNewTwoOrFourTrianglesNearVertexOnRib(
			std::shared_ptr<Triangle>& triangle,
			const std::shared_ptr<Vertex2D>& vertex,
			VertexRelation vertexRelation,
			DelaunayFactory* delaunayFactory)
	{
		// Сначала находим соседний треугольник у которого общее ребро с заданным
		// triangle, а также находим совместные вершины заданного треугольника, чтобы
		// построить два треугольника внутри заданного. При этом заданный треугольник
		// преобразуется в один из новых.
		std::shared_ptr<Vertex2D> newVertex; // вершина triangle потом станет вершиной 1 в двух новых
		std::shared_ptr<Vertex2D> adjacentVertex1; // вершина ребра triangle на которой vertex
		std::shared_ptr<Vertex2D> adjacentVertex2; // вершина ребра triangle на которой vertex
		std::shared_ptr<Triangle> oppositeTriangle; // треугольник напротив vertex (на ребре) относительно заданного triangle
		std::shared_ptr<Triangle> adjacentTriangle1; // треугольник напротив adjacentVertex1 относительно заданного triangle
		std::shared_ptr<Triangle> adjacentTriangle2; // треугольник напротив adjacentVertex1 относительно заданного triangle
		if (vertexRelation == VertexRelation::VertexOnRib12) {
			oppositeTriangle = triangle->triangle3;
			adjacentTriangle1 = triangle->triangle1;
			adjacentTriangle2 = triangle->triangle2;
			newVertex = triangle->vertex3;
			adjacentVertex1 = triangle->vertex1;
			adjacentVertex2 = triangle->vertex2;
		}
		else if (vertexRelation == VertexRelation::VertexOnRib13) {
			oppositeTriangle = triangle->triangle2;
			adjacentTriangle1 = triangle->triangle3;
			adjacentTriangle2 = triangle->triangle1;
			newVertex = triangle->vertex2;
			adjacentVertex1 = triangle->vertex3;
			adjacentVertex2 = triangle->vertex1;
		}
		else if (vertexRelation == VertexRelation::VertexOnRib23) {
			oppositeTriangle = triangle->triangle1;
			adjacentTriangle1 = triangle->triangle2;
			adjacentTriangle2 = triangle->triangle3;
			newVertex = triangle->vertex1;
			adjacentVertex1 = triangle->vertex2;
			adjacentVertex2 = triangle->vertex3;
		}
		else {
			// Не корректный параметр функции
			assert(false);
		}
		assert(!adjacentVertex1->equals(*adjacentVertex2));

		// В треугольнике, который напротив заданного triangle также нужно найти
		// совмещённые треугольники и вершины, чтобы корректно разбить
		std::shared_ptr<Vertex2D> adjacentNewVertex; // вершина oppositeTriangle потом станет вершиной 1 в двух новых
		std::shared_ptr<Vertex2D> adjacentOppositeVertex1; // вершина ребра oppositeTriangle на которой vertex
		std::shared_ptr<Vertex2D> adjacentOppositeVertex2; // вершина ребра oppositeTriangle на которой vertex
		std::shared_ptr<Triangle> adjacentOppositeTriangle1; // треугольник напротив adjacentOppositeVertex1 относительно oppositeTriangle
		std::shared_ptr<Triangle> adjacentOppositeTriangle2; // треугольник напротив adjacentOppositeVertex1 относительно oppositeTriangle
		if (oppositeTriangle) {
			int oppositeTriangleRib = triangle->commonRib(oppositeTriangle);
			assert(oppositeTriangleRib >= 1 && oppositeTriangleRib <=3);
			if (oppositeTriangleRib == 1) {
				adjacentOppositeTriangle1 = oppositeTriangle->triangle2;
				adjacentOppositeTriangle2 = oppositeTriangle->triangle3;
				adjacentNewVertex = oppositeTriangle->vertex1;
				adjacentOppositeVertex1 = oppositeTriangle->vertex2;
				adjacentOppositeVertex2 = oppositeTriangle->vertex3;
			}
			else if (oppositeTriangleRib == 2) {
				adjacentOppositeTriangle1 = oppositeTriangle->triangle3;
				adjacentOppositeTriangle2 = oppositeTriangle->triangle1;
				adjacentNewVertex = oppositeTriangle->vertex2;
				adjacentOppositeVertex1 = oppositeTriangle->vertex3;
				adjacentOppositeVertex2 = oppositeTriangle->vertex1;
			}
			else if (oppositeTriangleRib == 3) {
				adjacentOppositeTriangle1 = oppositeTriangle->triangle1;
				adjacentOppositeTriangle2 = oppositeTriangle->triangle2;
				adjacentNewVertex = oppositeTriangle->vertex3;
				adjacentOppositeVertex1 = oppositeTriangle->vertex1;
				adjacentOppositeVertex2 = oppositeTriangle->vertex2;
			}
		}

		bool ok;
		// Проверяем, а возможно ли вообще делать преобразование
		Direction newDirection = Utils::direction(newVertex, adjacentVertex1, vertex, &ok);
		if (newDirection != Direction::Right) {
			std::cout << "Skipped vertex: " << vertex->id << " (validation problem)." << std::endl;
			return {};
		}
		newDirection = Utils::direction(newVertex, vertex, adjacentVertex2, &ok);
		if (newDirection != Direction::Right) {
			std::cout << "Skipped vertex: " << vertex->id << " (validation problem)." << std::endl;
			return {};
		}
		if (oppositeTriangle) {
			newDirection = Utils::direction(adjacentNewVertex, adjacentOppositeVertex1, vertex, &ok);
			if (newDirection != Direction::Right) {
				std::cout << "Skipped vertex: " << vertex->id << " (validation problem)." << std::endl;
				return {};
			}
			newDirection = Utils::direction(adjacentNewVertex, vertex, adjacentOppositeVertex2, &ok);
			if (newDirection != Direction::Right) {
				std::cout << "Skipped vertex: " << vertex->id << " (validation problem)." << std::endl;
				return {};
			}
		}

		removeFromCache(triangle);
		removeFromCache(oppositeTriangle);

		// Далее по очереди нужно перестроить сначала triangle, а потом oppositeTriangle
		// 1. triangle
		triangle->vertex1 = newVertex;
		triangle->vertex2 = adjacentVertex1;
		triangle->vertex3 = vertex;
		std::shared_ptr<Triangle> newTriangle = delaunayFactory->createTriangle(
				newVertex, vertex, adjacentVertex2);

		// 2. oppositeTriangle
		std::shared_ptr<Triangle> newOppositeTriangle;
		if (oppositeTriangle) {
			oppositeTriangle->vertex1 = adjacentNewVertex;
			oppositeTriangle->vertex2 = adjacentOppositeVertex1;
			oppositeTriangle->vertex3 = vertex;
			newOppositeTriangle = delaunayFactory->createTriangle(
					adjacentNewVertex, vertex, adjacentOppositeVertex2);
		}
		ok = Utils::bindTriangles(triangle, newOppositeTriangle, newTriangle, adjacentTriangle2);
		assert(ok);
		ok = Utils::bindTriangles(newTriangle, oppositeTriangle, adjacentTriangle1, triangle);
		assert(ok);
		if (oppositeTriangle) {
			ok = Utils::bindTriangles(oppositeTriangle, newTriangle, newOppositeTriangle, adjacentOppositeTriangle2);
			assert(ok);
			ok = Utils::bindTriangles(newOppositeTriangle, triangle, adjacentOppositeTriangle1, oppositeTriangle);
			assert(ok);
		}

		addToCache(triangle);
		ok = insertNewTriangleWithoutBoundaryRebuilding(newTriangle);
		assert(ok);
		if (oppositeTriangle) {
			addToCache(oppositeTriangle);
			ok = insertNewTriangleWithoutBoundaryRebuilding(newOppositeTriangle);
			assert(ok);
		}

		std::list<std::shared_ptr<Triangle> > result;
		result.push_back(triangle);
		result.push_back(newTriangle);
		if (oppositeTriangle) {
			result.push_back(oppositeTriangle);
			result.push_back(newOppositeTriangle);
		}
		return result;
	}

	void TriangulationCache::checkDelaunayConditionAndRebuildIfNeed(
			std::list<std::shared_ptr<Triangle> >& createdTriangles)
	{
		if (createdTriangles.empty()) {
			return;
		}
		for (auto& triangle : createdTriangles) {
			checkTriangleDelaunayConditionAndRebuildIfNeed(triangle);
		}
	}

	void TriangulationCache::checkTriangleDelaunayConditionAndRebuildIfNeed(
			std::shared_ptr<Triangle>& triangle)
	{
		if (!triangle) {
			return;
		}
		if (triangle->triangle1) {
			if (!Utils::checkSumOppositeAngles(triangle, 1)) {
				flipTriangles(triangle, 1);
			}
		}
		if (triangle->triangle2) {
			if (!Utils::checkSumOppositeAngles(triangle, 2)) {
				flipTriangles(triangle, 2);
			}
		}
		if (triangle->triangle3) {
			if (!Utils::checkSumOppositeAngles(triangle, 3)) {
				flipTriangles(triangle, 3);
			}
		}
	}

	void TriangulationCache::index(float x, float y, int& i, int& j) const noexcept
	{
		i = static_cast<int>(std::round(_aX * x + _bX));
		j = static_cast<int>(std::round(_aY * y + _bY));
	}

	bool TriangulationCache::insertNewTriangleWithoutBoundaryRebuilding(
			const std::shared_ptr<Triangle>& triangle)
	{
		if (!triangle || !triangle->isValid()) {
			return false;
		}
		_triangles.push_back(triangle);
		addToCache(triangle);
		return true;
	}

	void TriangulationCache::flipTriangles(
			std::shared_ptr<Triangle>& triangle,
			int vertexNumber)
	{
		// подготовка набора вершин для разных случаев, чтобы обобщить разные варианты
		std::shared_ptr<Vertex2D> vertex1;
		std::shared_ptr<Vertex2D> vertex2;
		std::shared_ptr<Vertex2D> vertex3;
		std::shared_ptr<Vertex2D> oppositeVertex1;
		std::shared_ptr<Vertex2D> oppositeVertex2;
		std::shared_ptr<Vertex2D> oppositeVertex3;
		std::shared_ptr<Triangle> triangle12;
		std::shared_ptr<Triangle> triangle13;
		std::shared_ptr<Triangle> oppositeTriangle;
		std::shared_ptr<Triangle> oppositeTriangle12;
		std::shared_ptr<Triangle> oppositeTriangle13;
		if (vertexNumber == 1) {
			oppositeTriangle = triangle->triangle1;
			triangle12 = triangle->triangle3;
			triangle13 = triangle->triangle2;
			vertex1 = triangle->vertex1;
			vertex2 = triangle->vertex2;
			vertex3 = triangle->vertex3;
		}
		else if (vertexNumber == 2) {
			oppositeTriangle = triangle->triangle2;
			triangle12 = triangle->triangle1;
			triangle13 = triangle->triangle3;
			vertex1 = triangle->vertex2;
			vertex2 = triangle->vertex3;
			vertex3 = triangle->vertex1;
		}
		else if (vertexNumber == 3) {
			oppositeTriangle = triangle->triangle3;
			triangle12 = triangle->triangle2;
			triangle13 = triangle->triangle1;
			vertex1 = triangle->vertex3;
			vertex2 = triangle->vertex1;
			vertex3 = triangle->vertex2;
		}
		else {
			return;
		}
		if (!oppositeTriangle) {
			return;
		}
		int oppositeVertexNumber = triangle->commonRib(oppositeTriangle);
		if (oppositeVertexNumber == 1) {
			oppositeTriangle12 = oppositeTriangle->triangle3;
			oppositeTriangle13 = oppositeTriangle->triangle2;
			oppositeVertex1 = oppositeTriangle->vertex1;
			oppositeVertex2 = oppositeTriangle->vertex2;
			oppositeVertex3 = oppositeTriangle->vertex3;
		}
		else if (oppositeVertexNumber == 2) {
			oppositeTriangle12 = oppositeTriangle->triangle1;
			oppositeTriangle13 = oppositeTriangle->triangle3;
			oppositeVertex1 = oppositeTriangle->vertex2;
			oppositeVertex2 = oppositeTriangle->vertex3;
			oppositeVertex3 = oppositeTriangle->vertex1;
		}
		else if (oppositeVertexNumber == 3) {
			oppositeTriangle12 = oppositeTriangle->triangle2;
			oppositeTriangle13 = oppositeTriangle->triangle1;
			oppositeVertex1 = oppositeTriangle->vertex3;
			oppositeVertex2 = oppositeTriangle->vertex1;
			oppositeVertex3 = oppositeTriangle->vertex2;
		}
		else {
			return;
		}
		assert(vertex2->equals(*oppositeVertex3));
		assert(vertex3->equals(*oppositeVertex2));

		bool ok;
		Direction newDirection = Utils::direction(vertex1, oppositeVertex1, vertex3, &ok);
		if (newDirection != Direction::Right) {
			// невозможно выполнить flip (с большой вероятностью треугольник с одной или двумя FAKE вершин)
			return;
		}
		newDirection = Utils::direction(oppositeVertex1, vertex1, oppositeVertex3, &ok);
		if (newDirection != Direction::Right) {
			// невозможно выполнить flip (с большой вероятностью треугольник с одной или двумя FAKE вершин)
			return;
		}

		removeFromCache(triangle);
		removeFromCache(oppositeTriangle);

		triangle->vertex1 = vertex1;
		triangle->vertex2 = oppositeVertex1;
		triangle->vertex3 = vertex3;
		oppositeTriangle->vertex1 = oppositeVertex1;
		oppositeTriangle->vertex2 = vertex1;
		oppositeTriangle->vertex3 = oppositeVertex3;

		ok = Utils::bindTriangles(
				triangle, oppositeTriangle12, triangle13, oppositeTriangle);
		assert(ok);
		ok = Utils::bindTriangles(
				oppositeTriangle, triangle12, oppositeTriangle13, triangle);
		assert(ok);

		addToCache(triangle);
		addToCache(oppositeTriangle);
	}

}
