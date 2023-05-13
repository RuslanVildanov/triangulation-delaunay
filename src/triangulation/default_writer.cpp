#include "default_writer.h"

#include "utils.h"
#include <delaunay/triangle.h>
#include <delaunay/vertex_2d.h>
#include <fstream>


namespace triangulation
{
	DefaultWriter::DefaultWriter() : IFileWriter()
	{}

	bool DefaultWriter::writeFile(
			const std::string& outputFileName,
			const std::list<Vertex3D>& coordinates,
			const std::list<std::shared_ptr<delaunay::Triangle> >& triangles,
			const std::set<int>& usedVertexIds)
	{
		std::fstream file;
		file.open(outputFileName, std::ios::out);
		if (!file.is_open()) {
			return false;
		}
		file << "*   N,            X             Y             Z" << std::endl;
		file << "* Nodes" << std::endl;
		for (const auto& vertex : coordinates) {
			auto i = usedVertexIds.find(vertex.id);
			if (i == usedVertexIds.end()) {
				continue;
			}
			file << Utils::intToString(vertex.id, 5) << ','
				 << Utils::floatToString(vertex.x, 13) << ','
				 << Utils::floatToString(vertex.y, 13) << ','
				 << Utils::floatToString(vertex.z, 13) << std::endl;
		}

		file << "*Elements" << std::endl;
		for (const auto& triangle : triangles) {
			file << Utils::intToString(triangle->id, 5) << ','
				 << Utils::intToString(triangle->vertex1->id, 5) << ','
				 << Utils::intToString(triangle->vertex2->id, 5) << ','
				 << Utils::intToString(triangle->vertex3->id, 5) << std::endl;
		}
		file.close();
		return true;
	}
}
