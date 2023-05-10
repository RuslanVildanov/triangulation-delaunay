#ifndef DELAUNAY_TRIANGLE_H
#define DELAUNAY_TRIANGLE_H

#include "vertex_relation.h"

#include <array>
#include <memory>
#include <list>


namespace delaunay
{
	struct Vertex2D;

	/**
	 * Данные треугольника.
	 * Нумерация точек и соседних треугольников производится в порядке обхода по часовой
	 * стрелке, при этом напротив точки с определённым номером располагается ребро,
	 * соответствующее соседнему треугольнику с таким же номером.
	 *
	 * В структуре данные о рёбрах не предусмотрены, НО предполагается, что
	 * напротив вершины 1 располагается ребро 1 и т.д.
	 */
	struct Triangle
	{
		int id = 0;

		// нумерация вершин по часовой стрелке
		std::shared_ptr<Vertex2D> vertex1; // вершина вверху треугольника
		std::shared_ptr<Vertex2D> vertex2; // вершина справа треугольника
		std::shared_ptr<Vertex2D> vertex3; // вершина слева треугольника

		std::shared_ptr<Triangle> triangle1; // треугольник напротив vertex1
		std::shared_ptr<Triangle> triangle2; // треугольник напротив vertex2
		std::shared_ptr<Triangle> triangle3; // треугольник напротив vertex3

		[[nodiscard]] bool isValid() const noexcept;

		[[nodiscard]] VertexRelation vertexRelation(
				const std::shared_ptr<Vertex2D>& vertex,
				bool *ok) const noexcept;

		/**
		 * Ищет номер ребра именно в заданном треугольнике triangle, а НЕ у текущего,
		 * которое общее с текущим каким-то ребром текущего треугольника.
		 *
		 * @param triangle
		 * @return Возвращает 0 если общее ребро не найдено
		 */
		[[nodiscard]] int commonRib(const std::shared_ptr<Triangle>& triangle) const noexcept;

		[[nodiscard]] std::array<int, 3> vertexIds() const noexcept;

	};
}

#endif //DELAUNAY_TRIANGLE_H
