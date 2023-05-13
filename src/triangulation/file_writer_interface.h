#ifndef TRIANGULATION_FILE_WRITER_INTERFACE_H
#define TRIANGULATION_FILE_WRITER_INTERFACE_H

#include "vertex_3d.h"

#include <list>
#include <memory>
#include <string>
#include <set>


namespace delaunay
{
	struct Triangle;
}

namespace triangulation
{
	class IFileWriter
	{
	public:
		IFileWriter(const IFileWriter&) = delete;
		IFileWriter(IFileWriter&&) = delete;
		IFileWriter& operator=(const IFileWriter&) = delete;
		IFileWriter& operator=(IFileWriter&&) = delete;
		virtual ~IFileWriter() = default;

		virtual bool writeFile(
				const std::string& outputFileName,
				const std::list<Vertex3D>& coordinates,
				const std::list<std::shared_ptr<delaunay::Triangle> >& triangles,
				const std::set<int>& usedVertexIds) = 0;

	protected:
		IFileWriter() = default;

	};
}

#endif //TRIANGULATION_FILE_WRITER_INTERFACE_H
