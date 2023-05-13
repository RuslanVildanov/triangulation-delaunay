#ifndef TRIANGULATION_DEFAULT_WRITER_H
#define TRIANGULATION_DEFAULT_WRITER_H

#include "file_writer_interface.h"


namespace triangulation
{
	class DefaultWriter : public IFileWriter
	{
	public:
		DefaultWriter(const DefaultWriter&) = delete;
		DefaultWriter(DefaultWriter&&) = delete;
		DefaultWriter& operator=(const DefaultWriter&) = delete;
		DefaultWriter& operator=(DefaultWriter&&) = delete;
		DefaultWriter();

		bool writeFile(
				const std::string& outputFileName,
				const std::list<Vertex3D>& coordinates,
				const std::list<std::shared_ptr<delaunay::Triangle> >& triangles,
				const std::set<int>& usedVertexIds) final;

	};
}

#endif //TRIANGULATION_DEFAULT_WRITER_H
