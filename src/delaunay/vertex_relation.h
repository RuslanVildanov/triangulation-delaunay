#ifndef DELAUNAY_VERTEX_RELATION_H
#define DELAUNAY_VERTEX_RELATION_H


namespace delaunay
{
	enum class VertexRelation
	{
		VertexInTriangle = 0,
		VertexOnRib12,	// Ребро напротив вершины 3
		VertexOnRib13,	// Ребро напротив вершины 2
		VertexOnRib23,	// Ребро напротив вершины 1
		VertexOutOfTriangle
	};
}

#endif //DELAUNAY_VERTEX_RELATION_H
