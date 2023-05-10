#include "delaunay_factory.h"

#include "triangle.h"
#include "vertex_2d.h"


namespace delaunay
{
	std::shared_ptr<Triangle> DelaunayFactory::createTriangle(
			const std::shared_ptr<Vertex2D>& vertex1,
			const std::shared_ptr<Vertex2D>& vertex2,
			const std::shared_ptr<Vertex2D>& vertex3)
	{
		auto result = std::make_shared<Triangle>();
		result->id = _triangleId;
		result->vertex1 = vertex1;
		result->vertex2 = vertex2;
		result->vertex3 = vertex3;
		_triangleId++;
		return result;
	}

}
