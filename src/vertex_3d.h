#ifndef DELAUNAY_VERTEX_3D_H
#define DELAUNAY_VERTEX_3D_H

#include <memory>
#include <list>

namespace delaunay
{
	struct Vertex2D;
}

// Данные вершины 3D
struct Vertex3D
{
	int id = 0;

	float x = 0.0;
	float y = 0.0;
	float z = 0.0;
};

std::list<std::shared_ptr<delaunay::Vertex2D> > projectVertex3D(
		const std::list<Vertex3D>& vertices);

#endif //DELAUNAY_VERTEX_3D_H
