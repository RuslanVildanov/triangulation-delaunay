#ifndef DELAUNAY_VERTEX_2D_H
#define DELAUNAY_VERTEX_2D_H

#include "constants.h"
#include <cmath>


namespace delaunay
{
	/**
	 * Данные вершины 2D
	 */
	struct Vertex2D
	{
		int id = 0;

		float x = 0.0;
		float y = 0.0;

		bool operator==(const Vertex2D& vertex) const noexcept
		{
			return id == vertex.id
					&& std::abs(x - vertex.x) < EPSILON
					&& std::abs(y - vertex.y) < EPSILON;
		}

		/**
		 * Проверка равенства только координат
		 * @param vertex
		 * @return
		 */
		[[nodiscard]] bool equals(const Vertex2D& vertex) const noexcept
		{
			return std::abs(x - vertex.x) < EPSILON
				   && std::abs(y - vertex.y) < EPSILON;
		}
	};
}

#endif //DELAUNAY_VERTEX_2D_H
