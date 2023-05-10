#ifndef DELAUNAY_TRIANGULATION_CACHE_H
#define DELAUNAY_TRIANGULATION_CACHE_H

#include "vertex_relation.h"

#include <list>
#include <unordered_map>
#include <memory>
#include <vector>


namespace delaunay
{
	struct Vertex2D;
	struct Triangle;
	class DelaunayFactory;

	struct CacheInfo
	{
		std::list<std::shared_ptr<Triangle> > triangles;
	};

	// Триангуляционный кэш - это двумерная таблица в ячейках, которой находится
	// информация о треугольниках, чтобы по координате (x, y) получить информацию
	// о рядом расположенных треугольниках
	class TriangulationCache
	{
	public:
		TriangulationCache() = default;
		~TriangulationCache() = default;
		TriangulationCache(const TriangulationCache&) = delete;
		TriangulationCache(TriangulationCache&&) = delete;
		TriangulationCache & operator=(const TriangulationCache&) = delete;
		TriangulationCache & operator=(TriangulationCache&&) = delete;

		bool insertFirstTrianglesWithSameRectangle(
				const std::list<std::shared_ptr<Triangle> >& triangles);

		[[nodiscard]] const std::list<std::shared_ptr<Triangle> >& triangles() const noexcept;
		[[nodiscard]] std::shared_ptr<Triangle> findAppropriateTriangle(
				const std::shared_ptr<Vertex2D>& vertex) const;

		/**
		 * НЕ РЕАЛИЗОВАНО!!!
		 * Создаёт новые треугольники так, чтобы вся область триангуляции была выпуклой.
		 *
		 * Использовать только когда vertex попадает вне области текущей триангуляции.

		 * @param nearestTriangle - треугольник, центр которого ближе всего к новой вершине
		 * @param vertex - новая вершина
		 * @param ok
		 * @return
		 */
		std::list<std::shared_ptr<Triangle> > createNewSeveralTrianglesNearOuterVertex(
				std::shared_ptr<Triangle>& nearestTriangle,
				const std::shared_ptr<Vertex2D>& vertex,
				bool* ok);

		/**
		 * Создаёт три новых треугольника вместо существующего треугольника.
		 *
		 * Использовать только когда vertex попадает внутрь треугольника triangle.
		 *
		 * @param triangle - треугольник, который будет разбит на три новых
		 * @param innerVertex - новая вершина внутри triangle
		 * @param delaunayFactory
		 * @return
		 */
		std::list<std::shared_ptr<Triangle> > createNewThreeTrianglesNearInnerVertex(
				std::shared_ptr<Triangle>& triangle,
				const std::shared_ptr<Vertex2D>& innerVertex,
				DelaunayFactory* delaunayFactory);

		/**
		 * Создаёт два или четыре новых треугольника вместо существующего треугольника.
		 *
		 * Использовать только когда vertex попадает на какую-то грань triangle.
		 *
		 * @param triangle - треугольник, который будет разбит на три новых
		 * @param vertex - новая вершина внутри triangle
		 * @param vertexRelation
		 * @param delaunayFactory
		 * @return
		 */
		std::list<std::shared_ptr<Triangle> > createNewTwoOrFourTrianglesNearVertexOnRib(
				std::shared_ptr<Triangle>& triangle,
				const std::shared_ptr<Vertex2D>& vertex,
				VertexRelation vertexRelation,
				DelaunayFactory* delaunayFactory);

		/**
		 * В списке заданных треугольников проверяет условие Делоне и при необходимости
		 * перестраивает их
		 * @param createdTriangles
		 */
		void checkDelaunayConditionAndRebuildIfNeed(
				std::list<std::shared_ptr<Triangle> >& createdTriangles);

		/**
		 * В заданном треугольнике проверяет условие Делоне со всеми его граничащими
		 * треугольниками
		 * @param triangle
		 */
		void checkTriangleDelaunayConditionAndRebuildIfNeed(
				std::shared_ptr<Triangle>& triangle);

	private:
		bool initCache(float minX, float minY, float maxX, float maxY, int m);
		bool needReInitCacheBySize() const noexcept;
		void reInitCache();
		void cacheTriangle(const std::shared_ptr<Triangle>& triangle);
		void addToCache(const std::shared_ptr<Triangle>& triangle);
		void removeFromCache(const std::shared_ptr<Triangle>& triangle);
		/**
		 * По координатам x, y возвращает индекс в кэше
		 */
		void index(float x, float y, int& i, int& j) const noexcept;
		bool insertNewTriangleWithoutBoundaryRebuilding(
				const std::shared_ptr<Triangle>& triangle);
		void flipTriangles(
				std::shared_ptr<Triangle>& triangle,
				int vertexNumber);

		std::vector<std::vector<CacheInfo> > _cache;
		std::list<std::shared_ptr<Triangle> > _triangles;
//		std::unordered_map<int, std::shared_ptr<Vertex2D>> _vertices;
		// минимум / максимум
		float _minX = 0.0f;
		float _maxX = 0.0f;
		// коэффициенты перевода из [_minX, _maxX] в [0, _m - 1]
		float _aX = 0.0f;	// (_m - 1) / (_maxX - _minX)
		float _bX = 0.0f;	// -_minX * _aX

		float _minY = 0.0f;
		float _maxY = 0.0f;
		// коэффициенты перевода из [_minY, _maxY] в [0, _m - 1]
		float _aY = 0.0f;	// (_m - 1) / (_maxY - _minY)
		float _bY = 0.0f;	// -_minY * _aY

		int _m = 0;

	};
}

#endif //DELAUNAY_TRIANGULATION_CACHE_H
