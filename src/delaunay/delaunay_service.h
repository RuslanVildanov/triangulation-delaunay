#ifndef DELAUNAY_DELAUNAY_SERVICE_H
#define DELAUNAY_DELAUNAY_SERVICE_H

#include <functional>
#include <list>
#include <memory>
#include <set>


namespace delaunay
{
	struct Vertex2D;
	struct Triangle;
	class TriangulationCache;
	class DelaunayFactory;

	class DelaunayService
	{
	public:
		DelaunayService() = default;
		~DelaunayService() = default;
		DelaunayService(const DelaunayService&) = delete;
		DelaunayService(DelaunayService&&) = delete;
		DelaunayService & operator=(const DelaunayService&) = delete;
		DelaunayService & operator=(DelaunayService&&) = delete;

		static std::set<int> vertexIds(
				const std::list<std::shared_ptr<Triangle> >& triangles);

		/**
		 * В данной реализации сделано упрощение: сначала создаются fake вершины для двух
		 * треугольников, которые образуют квадрат покрывающий всю область триангуляции.
		 * По окончании работы алгоритма все треугольники содержащие fake вершины
		 * удаляются.
		 *
		 * ВАЖНО!!! у всех вершин из списка vertices должен быть задан id и он не должен
		 * повторяться.
		 * @param vertices
		 * @param ok
		 * @return
		 */
		std::list<std::shared_ptr<Triangle> > iterativeDynamicProcess(
				const std::list<std::shared_ptr<Vertex2D> >& vertices,
				bool* ok);

	private:
		/**
		 * Запускает процесс триангуляции Делоне. Итеративный алгоритм с динамическим
		 * кэшированием поиска
		 * @param vertices
		 * @param ok
		 * @return
		 */
		std::list<std::shared_ptr<Triangle> > generalIterativeDynamicProcess(
				const std::function<std::list<std::shared_ptr<Vertex2D> >(
						TriangulationCache*,
						DelaunayFactory*,
						const std::list<std::shared_ptr<Vertex2D> >&,
						bool*) >& step1,
				const std::list<std::shared_ptr<Vertex2D> >& vertices,
				bool* ok);

	};
}

#endif //DELAUNAY_DELAUNAY_SERVICE_H
