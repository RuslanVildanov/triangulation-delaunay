#include "triangle.h"

#include "constants.h"
#include "utils.h"
#include "vertex_2d.h"


namespace delaunay
{
	bool Triangle::isValid() const noexcept
	{
		if (!vertex1 || !vertex2 || !vertex3) {
			return false;
		}
		bool ok;
		Direction direction = Utils::direction(vertex1, vertex2, vertex3, &ok);
		return direction == Direction::Right;
	}

	VertexRelation Triangle::vertexRelation(
			const std::shared_ptr<Vertex2D>& vertex,
			bool *ok) const noexcept
	{
		if (!isValid() || !vertex) {
			if (ok) {
				*ok = false;
			}
			return VertexRelation::VertexOutOfTriangle;
		}
		if (ok) {
			*ok = true;
		}

		// Математическая часть - векторное и псевдоскалярное произведение.
		// Реализация - считаются произведения (1,2,3 - вершины треугольника, 0 - точка):
		// (x1-x0)*(y2-y1)-(x2-x1)*(y1-y0)
		// (x2-x0)*(y3-y2)-(x3-x2)*(y2-y0)
		// (x3-x0)*(y1-y3)-(x1-x3)*(y3-y0)
		// Если они одинакового знака, то точка внутри треугольника,
		// если что-то из этого - ноль, то точка лежит на стороне,
		// иначе точка вне треугольника.
		float v12 = (vertex1->x - vertex->x) * (vertex2->y - vertex1->y)
					- (vertex2->x - vertex1->x) * (vertex1->y - vertex->y);
		float v23 = (vertex2->x - vertex->x) * (vertex3->y - vertex2->y)
					- (vertex3->x - vertex2->x) * (vertex2->y - vertex->y);
		float v13 = (vertex3->x - vertex->x) * (vertex1->y - vertex3->y)
					- (vertex1->x - vertex3->x) * (vertex3->y - vertex->y);
		if (std::abs(v12) < EPSILON) {
			return VertexRelation::VertexOnRib12;
		}
		else if (std::abs(v23) < EPSILON) {
			return VertexRelation::VertexOnRib23;
		}
		else if (std::abs(v13) < EPSILON) {
			return VertexRelation::VertexOnRib13;
		}
		else if ((v12 > 0.0f && v23 > 0.0f && v13 > 0.0f)
				 || (v12 < 0.0f && v23 < 0.0f && v13 < 0.0f)) {
			return VertexRelation::VertexInTriangle;
		}
		return VertexRelation::VertexOutOfTriangle;
	}

	int Triangle::commonRib(const std::shared_ptr<Triangle>& triangle) const noexcept
	{
		if (!isValid() || !triangle || !triangle->isValid()) {
			return 0;
		}

		bool ok11 = vertex1->equals(*(triangle->vertex1));
		bool ok12 = vertex1->equals(*(triangle->vertex2));
		bool ok13 = vertex1->equals(*(triangle->vertex3));

		bool ok21 = vertex2->equals(*(triangle->vertex1));
		bool ok22 = vertex2->equals(*(triangle->vertex2));
		bool ok23 = vertex2->equals(*(triangle->vertex3));

		bool ok31 = vertex3->equals(*(triangle->vertex1));
		bool ok32 = vertex3->equals(*(triangle->vertex2));
		bool ok33 = vertex3->equals(*(triangle->vertex3));

		if ((ok11 || ok21 || ok31) && (ok12 || ok22 || ok32)) {
			// ребро 3 между вершинами 1 и 2
			return 3;
		}
		else if ((ok11 || ok21 || ok31) && (ok13 || ok23 || ok33)) {
			// ребро 2 между вершинами 1 и 3
			return 2;
		}
		else if ((ok12 || ok22 || ok32) && (ok13 || ok23 || ok33)) {
			// ребро 1 между вершинами 2 и 3
			return 1;
		}
		return 0;
	}

	std::array<int, 3> Triangle::vertexIds() const noexcept
	{
		std::array<int, 3> result = {
				vertex1 ? vertex1->id : 0,
				vertex2 ? vertex2->id : 0,
				vertex3 ? vertex3->id : 0};
		return result;
	}

}
