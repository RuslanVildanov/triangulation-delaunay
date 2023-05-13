#ifndef _FILE_WRITER_INTERFACE_H
#define _FILE_WRITER_INTERFACE_H

#include "vertex_3d.h"

#include <list>
#include <memory>
#include <string>
#include <set>


namespace delaunay
{
	struct Triangle;
}

class IFileWriter
{
public:
	virtual bool writeFile(
			const std::string& outputFileName,
			const std::list<Vertex3D>& coordinates,
			const std::list<std::shared_ptr<delaunay::Triangle> >& triangles,
			const std::set<int>& usedVertexIds) = 0;
};

#endif //_FILE_WRITER_INTERFACE_H
