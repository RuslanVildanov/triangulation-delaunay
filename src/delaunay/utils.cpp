#include "utils.h"

#include "constants.h"
#include "delaunay_factory.h"
#include "triangle.h"
#include "vertex_2d.h"
#include <algorithm>
#include <cassert>
#include <cmath>


namespace delaunay
{
	float Utils::sinAlpha(
			const std::shared_ptr<Vertex2D>& vertex1,
			const std::shared_ptr<Vertex2D>& vertex2,
			const std::shared_ptr<Vertex2D>& vertex3,
			bool* ok) noexcept
	{
		if (!vertex1 || !vertex2 || !vertex3) {
			if (ok) {
				*ok = false;
			}
			return 0.0f;
		}
		// Координаты вершины vertex1 должны отличаться от других, чтобы избежать деления на ноль
		if (vertex1->equals(*vertex2) || vertex1->equals(*vertex3)) {
			if (ok) {
				*ok = false;
			}
			return 0.0f;
		}
		if (ok) {
			*ok = true;
		}

		const float x2Difference = vertex1->x - vertex2->x;
		const float y2Difference = vertex1->y - vertex2->y;
		const float x3Difference = vertex1->x - vertex3->x;
		const float y3Difference = vertex1->y - vertex3->y;
		const float sqrt1 = std::sqrtf(x2Difference * x2Difference + y2Difference * y2Difference);
		const float sqrt2 = std::sqrtf(x3Difference * x3Difference + y3Difference * y3Difference);
		const float top = (x2Difference * y3Difference) - (x3Difference * y2Difference);
		// пытаемся вычислить углы внутри треугольника, поэтому принудительно возвращаем
		// значения только положительные, где-то выше делаем проверку корректных значений
		// углов треугольника
		return std::abs(top) / (sqrt1 * sqrt2);
	}

	float Utils::cosAlpha(
			const std::shared_ptr<Vertex2D>& vertex1,
			const std::shared_ptr<Vertex2D>& vertex2,
			const std::shared_ptr<Vertex2D>& vertex3,
			bool *ok) noexcept
	{
		if (!vertex1 || !vertex2 || !vertex3) {
			if (ok) {
				*ok = false;
			}
			return 0.0f;
		}
		if (ok) {
			*ok = true;
		}

		const float x3x1Difference = vertex3->x - vertex1->x;
		const float x2x1Difference = vertex2->x - vertex1->x;
		const float y3y1Difference = vertex3->y - vertex1->y;
		const float y2y1Difference = vertex2->y - vertex1->y;
		const float sqrt1 = std::sqrtf(x3x1Difference * x3x1Difference + y3y1Difference * y3y1Difference);
		const float sqrt2 = std::sqrtf(x2x1Difference * x2x1Difference + y2y1Difference * y2y1Difference);
		const float top = (y3y1Difference * y2y1Difference) + (x3x1Difference * x2x1Difference);
		// пытаемся вычислить углы внутри треугольника, поэтому принудительно возвращаем
		// значения только положительные, где-то выше делаем проверку корректных значений
		// углов треугольника
		return std::abs(top) / (sqrt1 * sqrt2);
	}

	std::shared_ptr<Triangle> Utils::createFirstTriangle(
			const std::list<std::shared_ptr<Vertex2D> >& vertices,
			DelaunayFactory* delaunayFactory)
	{
		if (vertices.size() < 3) {
			return {};
		}
		bool ok;
		int i = 0;
		std::shared_ptr<Vertex2D> vertex1;
		std::shared_ptr<Vertex2D> vertex2;
		std::shared_ptr<Vertex2D> emptyVertex;
		std::shared_ptr<Triangle> result = delaunayFactory->createTriangle(
				vertex1, vertex2, emptyVertex);
		for(const auto& vertex : vertices) {
			if (!vertex) {
				continue;
			}
			if (i == 0) {
				vertex1 = vertex;
			}
			else if (i == 1) {
				vertex2 = vertex;
			}
			i++;
			Direction d = direction(vertex1, vertex2, vertex, &ok);
			if (!ok) {
				continue;
			}
			if (d == Direction::Right) {
				result->vertex1 = vertex1;
				result->vertex2 = vertex2;
				result->vertex3 = vertex;
			}
			else if (d == Direction::Left) {
				result->vertex1 = vertex1;
				result->vertex2 = vertex;
				result->vertex3 = vertex2;
			}
			else {
				continue;
			}
			return result;
		}
		return {};
	}

	bool Utils::checkSumOppositeAngles(
			const std::shared_ptr<Triangle>& triangle,
			int vertexNumber) noexcept
	{
		int commonRib;
		float x0, x1, x2, x3, y0, y1, y2, y3;
		if (vertexNumber == 1) {
			commonRib = triangle->commonRib(triangle->triangle1);
			if (commonRib == 1) {
				x2 = triangle->triangle1->vertex1->x;
				y2 = triangle->triangle1->vertex1->y;
			}
			else if (commonRib == 2) {
				x2 = triangle->triangle1->vertex2->x;
				y2 = triangle->triangle1->vertex2->y;
			}
			else if (commonRib == 3) {
				x2 = triangle->triangle1->vertex3->x;
				y2 = triangle->triangle1->vertex3->y;
			}
			else {
				return true;
			}
			x0 = triangle->vertex1->x;
			y0 = triangle->vertex1->y;
			x1 = triangle->vertex2->x;
			y1 = triangle->vertex2->y;
			x3 = triangle->vertex3->x;
			y3 = triangle->vertex3->y;
		}
		else if (vertexNumber == 2) {
			commonRib = triangle->commonRib(triangle->triangle2);
			if (commonRib == 1) {
				x2 = triangle->triangle2->vertex1->x;
				y2 = triangle->triangle2->vertex1->y;
			}
			else if (commonRib == 2) {
				x2 = triangle->triangle2->vertex2->x;
				y2 = triangle->triangle2->vertex2->y;
			}
			else if (commonRib == 3) {
				x2 = triangle->triangle2->vertex3->x;
				y2 = triangle->triangle2->vertex3->y;
			}
			else {
				return true;
			}
			x0 = triangle->vertex2->x;
			y0 = triangle->vertex2->y;
			x1 = triangle->vertex3->x;
			y1 = triangle->vertex3->y;
			x3 = triangle->vertex1->x;
			y3 = triangle->vertex1->y;
		}
		else if (vertexNumber == 3) {
			commonRib = triangle->commonRib(triangle->triangle3);
			if (commonRib == 1) {
				x2 = triangle->triangle3->vertex1->x;
				y2 = triangle->triangle3->vertex1->y;
			}
			else if (commonRib == 2) {
				x2 = triangle->triangle3->vertex2->x;
				y2 = triangle->triangle3->vertex2->y;
			}
			else if (commonRib == 3) {
				x2 = triangle->triangle3->vertex3->x;
				y2 = triangle->triangle3->vertex3->y;
			}
			else {
				return true;
			}
			x0 = triangle->vertex3->x;
			y0 = triangle->vertex3->y;
			x1 = triangle->vertex1->x;
			y1 = triangle->vertex1->y;
			x3 = triangle->vertex2->x;
			y3 = triangle->vertex2->y;
		}
		else {
			return true;
		}

		// Модифицированная проверка суммы противолежащих углов
		const float sAlfa = ((x0 - x1) * (x0 - x3)) + ((y0 - y1) * (y0 - y3));
		const float sBetta = ((x2 - x1) * (x2 - x3)) + ((y2 - y1) * (y2 - y3));

		// Если sAlfa < 0 и sBetta < 0, то условие Делоне точно не выполняется
		if (sAlfa < 0 && sBetta < 0) {
			return false;
		}
		else if (sAlfa >= 0 && sBetta >= 0) {
			return true;
		}

		// Проверка суммы противолежащих углов
		const float cAlfa = ((x0 - x1) * (y0 - y3)) - ((x0 - x3) * (y0 - y1));
		const float cBetta = ((x2 - x3) * (y2 - y1)) - ((x2 - x1) * (y2 - y3));

		return sAlfa * cBetta + cAlfa * sBetta >= 0;
	}

	bool Utils::checkCircleEquation(
			const std::shared_ptr<Triangle>& triangle1,
			const std::shared_ptr<Triangle>& triangle2) noexcept
	{
		const float x1Difference = triangle1->vertex1->x - triangle2->vertex1->x;
		const float y1Difference = triangle1->vertex1->y - triangle2->vertex1->y;
		const float x2Difference = triangle1->vertex2->x - triangle2->vertex1->x;
		const float y2Difference = triangle1->vertex2->y - triangle2->vertex1->y;
		const float x3Difference = triangle1->vertex3->x - triangle2->vertex1->x;
		const float y3Difference = triangle1->vertex3->y - triangle2->vertex1->y;

		const float a11 = (x1Difference * x1Difference) + (y1Difference * y1Difference);
		const float a12 = x1Difference;
		const float a13 = y1Difference;
		const float a21 = (x2Difference * x2Difference) + (y2Difference * y2Difference);
		const float a22 = x2Difference;
		const float a23 = y2Difference;
		const float a31 = (x3Difference * x3Difference) + (y3Difference * y3Difference);
		const float a32 = x3Difference;
		const float a33 = y3Difference;
		return a11 >= 0 && a12 >= 0 && a13 >= 0
			   && a21 >= 0 && a22 >= 0 && a23 >= 0
			   && a31 >= 0 && a32 >= 0 && a33 >= 0;
	}

	std::shared_ptr<Vertex2D> Utils::centerOfMass(
			const std::shared_ptr<Triangle>& triangle)
	{
		if (!triangle->isValid()) {
			return {};
		}
		std::shared_ptr<Vertex2D> result = std::make_shared<Vertex2D>();
		result->id = FAKE_ID;
		result->x = (triangle->vertex1->x + triangle->vertex2->x + triangle->vertex3->x) / 3.0f;
		result->y = (triangle->vertex1->y + triangle->vertex2->y + triangle->vertex3->y) / 3.0f;
		return result;
	}

	float Utils::distance(
			const std::shared_ptr<Vertex2D>& vertex1,
			const std::shared_ptr<Vertex2D>& vertex2,
			bool *ok) noexcept
	{
		if (!vertex1 || !vertex2) {
			if (ok) {
				*ok = false;
			}
			return 0.0f;
		}
		if (ok) {
			*ok = true;
		}

		const float xDifference = vertex2->x - vertex1->x;
		const float yDifference = vertex2->y - vertex1->y;
		return std::sqrt((xDifference * xDifference) + (yDifference * yDifference));
	}

	bool Utils::bindTriangles(
			std::shared_ptr<Triangle>& innerTriangle,
			std::shared_ptr<Triangle>& outerTriangle1,
			std::shared_ptr<Triangle>& outerTriangle2,
			std::shared_ptr<Triangle>& outerTriangle3) noexcept
	{
		if (!innerTriangle || !innerTriangle->isValid()) {
			return false;
		}
		// Номер общего ребра треугольника outerTriangle1 с одним из рёбер innerTriangle
		int commonRibNumber1 = innerTriangle->commonRib(outerTriangle1);
		if (outerTriangle1 && (commonRibNumber1 <= 0 || commonRibNumber1 > 3)) {
			return false;
		}
		// Номер общего ребра треугольника outerTriangle2 с одним из рёбер innerTriangle
		int commonRibNumber2 = innerTriangle->commonRib(outerTriangle2);
		if (outerTriangle2 && (commonRibNumber2 <= 0 || commonRibNumber2 > 3)) {
			return false;
		}
		// Номер общего ребра треугольника outerTriangle3 с одним из рёбер innerTriangle
		int commonRibNumber3 = innerTriangle->commonRib(outerTriangle3);
		if (outerTriangle3 && (commonRibNumber3 <= 0 || commonRibNumber3 > 3)) {
			return false;
		}

		innerTriangle->triangle1 = outerTriangle1;
		innerTriangle->triangle2 = outerTriangle2;
		innerTriangle->triangle3 = outerTriangle3;
		if (commonRibNumber1 == 1) {
			outerTriangle1->triangle1 = innerTriangle;
		}
		else if (commonRibNumber1 == 2) {
			outerTriangle1->triangle2 = innerTriangle;
		}
		else if (commonRibNumber1 == 3) {
			outerTriangle1->triangle3 = innerTriangle;
		}
		if (commonRibNumber2 == 1) {
			outerTriangle2->triangle1 = innerTriangle;
		}
		else if (commonRibNumber2 == 2) {
			outerTriangle2->triangle2 = innerTriangle;
		}
		else if (commonRibNumber2 == 3) {
			outerTriangle2->triangle3 = innerTriangle;
		}
		if (commonRibNumber3 == 1) {
			outerTriangle3->triangle1 = innerTriangle;
		}
		else if (commonRibNumber3 == 2) {
			outerTriangle3->triangle2 = innerTriangle;
		}
		else if (commonRibNumber3 == 3) {
			outerTriangle3->triangle3 = innerTriangle;
		}
		return true;
	}

	bool Utils::triangleRect(
			const std::shared_ptr<Triangle>& triangle,
			float& minX,
			float& minY,
			float& maxX,
			float& maxY) noexcept
	{
		if (!triangle || !triangle->isValid()) {
			return false;
		}
		minX = std::min({
			triangle->vertex1->x, triangle->vertex2->x, triangle->vertex3->x});
		minY = std::min({
			triangle->vertex1->y, triangle->vertex2->y, triangle->vertex3->y});
		maxX = std::max({
			triangle->vertex1->x, triangle->vertex2->x, triangle->vertex3->x});
		maxY = std::max({
			triangle->vertex1->y, triangle->vertex2->y, triangle->vertex3->y});
		return true;
	}

	Direction Utils::direction(
			const std::shared_ptr<Vertex2D>& vertex1,
			const std::shared_ptr<Vertex2D>& vertex2,
			const std::shared_ptr<Vertex2D>& vertex3,
			bool* ok) noexcept
	{
		if (!vertex1 || !vertex2 || !vertex3) {
			if (ok) {
				*ok = false;
			}
			return Direction::Collinear;
		}
		if (ok) {
			*ok = true;
		}
		const float x2x1Difference = vertex2->x - vertex1->x;
		const float x3x1Difference = vertex3->x - vertex1->x;
		const float y3y1Difference = vertex3->y - vertex1->y;
		const float y2y1Difference = vertex2->y - vertex1->y;
		const float result = (x2x1Difference * y3y1Difference)
				- (x3x1Difference * y2y1Difference);
		if (std::abs(result) < EPSILON) {
			return Direction::Collinear;
		}
		else if (result < 0.0) {
			return Direction::Right;
		}
		return Direction::Left;
	}

	bool Utils::isSegmentsIntersect(
			const std::shared_ptr<Vertex2D>& vertex1,
			const std::shared_ptr<Vertex2D>& vertex2,
			const std::shared_ptr<Vertex2D>& vertex3,
			const std::shared_ptr<Vertex2D>& vertex4,
			bool trueWhenVertexOnSegment,
			bool* ok) noexcept
	{
		bool thisOk;
		Direction d1 = direction(vertex3, vertex4, vertex1, &thisOk);
		if (!thisOk) {
			if (ok) {
				*ok = false;
			}
			return false;
		}
		Direction d2 = direction(vertex3, vertex4, vertex2, &thisOk);
		if (!thisOk) {
			if (ok) {
				*ok = false;
			}
			return false;
		}
		Direction d3 = direction(vertex1, vertex2, vertex3, &thisOk);
		if (!thisOk) {
			if (ok) {
				*ok = false;
			}
			return false;
		}
		Direction d4 = direction(vertex1, vertex2, vertex4, &thisOk);
		if (!thisOk) {
			if (ok) {
				*ok = false;
			}
			return false;
		}
		if (((d1 == Direction::Left && d2 == Direction::Right)
				|| (d1 == Direction::Right && d2 == Direction::Left))
				&& ((d3 == Direction::Left && d4 == Direction::Right)
				|| (d3 == Direction::Right && d4 == Direction::Left))) {
			return true;
		}
		if (!trueWhenVertexOnSegment) {
			return false;
		}
		if (d1 == Direction::Collinear
				&& vertex1->x >= std::min(vertex3->x, vertex4->x)
				&& vertex1->x <= std::max(vertex3->x, vertex4->x)
				&& vertex1->y >= std::min(vertex3->y, vertex4->y)
				&& vertex1->y <= std::max(vertex3->y, vertex4->y)) {
			return true;
		}
		else if (d2 == Direction::Collinear
				 && vertex2->x >= std::min(vertex3->x, vertex4->x)
				 && vertex2->x <= std::max(vertex3->x, vertex4->x)
				 && vertex2->y >= std::min(vertex3->y, vertex4->y)
				 && vertex2->y <= std::max(vertex3->y, vertex4->y)) {
			return true;
		}
		else if (d3 == Direction::Collinear
				 && vertex3->x >= std::min(vertex1->x, vertex2->x)
				 && vertex3->x <= std::max(vertex1->x, vertex2->x)
				 && vertex3->y >= std::min(vertex1->y, vertex2->y)
				 && vertex3->y <= std::max(vertex1->y, vertex2->y)) {
			return true;
		}
		else if (d4 == Direction::Collinear
				 && vertex4->x >= std::min(vertex1->x, vertex2->x)
				 && vertex4->x <= std::max(vertex1->x, vertex2->x)
				 && vertex4->y >= std::min(vertex1->y, vertex2->y)
				 && vertex4->y <= std::max(vertex1->y, vertex2->y)) {
			return true;
		}
		return false;
	}

	std::vector<std::shared_ptr<Vertex2D> > Utils::createVerticesAsRectangle(
			const std::list<std::shared_ptr<Vertex2D> >& vertices)
	{
		if (vertices.empty()) {
			return {};
		}

		const auto& firstVertex = vertices.front();
		float minX = firstVertex ? firstVertex->x : 0.0f;
		float minY = firstVertex ? firstVertex->y : 0.0f;
		float maxX = minX;
		float maxY = minY;
		for (const auto& vertex : vertices) {
			if (!vertex) {
				continue;
			}
			if (vertex->x < minX) {
				minX = vertex->x;
			}
			if (vertex->y < minY) {
				minY = vertex->y;
			}
			if (vertex->x > maxX) {
				maxX = vertex->x;
			}
			if (vertex->y > maxY) {
				maxY = vertex->y;
			}
		}
		minX -= FAKE_RECTANGLE_OFFSET;
		minY -= FAKE_RECTANGLE_OFFSET;
		maxX += FAKE_RECTANGLE_OFFSET;
		maxY += FAKE_RECTANGLE_OFFSET;
		std::vector<std::shared_ptr<Vertex2D> > result;
		result.resize(4);
		auto v1 = std::make_shared<Vertex2D>();
		v1->id = FAKE_ID;
		v1->x = minX;
		v1->y = minY;
		result[0] = v1;
		auto v2 = std::make_shared<Vertex2D>();
		v2->id = FAKE_ID;
		v2->x = minX;
		v2->y = maxY;
		result[1] = v2;
		auto v3 = std::make_shared<Vertex2D>();
		v3->id = FAKE_ID;
		v3->x = maxX;
		v3->y = minY;
		result[2] = v3;
		auto v4 = std::make_shared<Vertex2D>();
		v4->id = FAKE_ID;
		v4->x = maxX;
		v4->y = maxY;
		result[3] = v4;
		return result;
	}

}
