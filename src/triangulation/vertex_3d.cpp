#include "vertex_3d.h"

#include <delaunay/vertex_2d.h>


namespace triangulation
{
	std::list<std::shared_ptr<delaunay::Vertex2D> > projectVertex3D(
			const std::list<Vertex3D>& vertices)
	{
		std::list<std::shared_ptr<delaunay::Vertex2D> > result;
		for (const auto& vertex : vertices) {
			std::shared_ptr<delaunay::Vertex2D> vertex2D = std::make_shared<delaunay::Vertex2D>();
			vertex2D->id = vertex.id;
			vertex2D->x = vertex.x * vertex.z;
			vertex2D->y = vertex.y * vertex.z;
			result.push_back(vertex2D);
		}
		return result;
	}
}
