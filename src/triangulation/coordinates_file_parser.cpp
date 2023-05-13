#include "coordinates_file_parser.h"
#include "default_writer.h"
#include "utils.h"
#include "vertex_3d.h"
#include <delaunay/triangle.h>
#include <delaunay/vertex_2d.h>

#include <iostream>
#include <fstream>
#include <regex>
#include <sstream>


namespace triangulation
{
	const std::regex COMMENT_REGEX("^\\s*\\*.*");
// ^\s*([0-9]+),\s*([-+]?[0-9]+\.?[0-9]*(?:[eE][-+]?[0-9]+)?),\s*([-+]?[0-9]+\.?[0-9]*(?:[eE][-+]?[0-9]+)?),\s*([-+]?[0-9]+\.?[0-9]*(?:[eE][-+]?[0-9]+)?).*
	const std::regex NUMBERS_LINE_REGEX(R"(^\s*([0-9]+),\s*([-+]?[0-9]+\.?[0-9]*(?:[eE][-+]?[0-9]+)?),\s*([-+]?[0-9]+\.?[0-9]*(?:[eE][-+]?[0-9]+)?),\s*([-+]?[0-9]+\.?[0-9]*(?:[eE][-+]?[0-9]+)?).*)");

	bool CoordinatesFileParser::loadFile(const std::string& fileName)
	{
		std::fstream file;
		file.open(fileName, std::ios::in);
		if (!file.is_open()) {
			return false;
		}

		std::string line;
		while (std::getline(file, line)) {
			if (std::regex_match(line, COMMENT_REGEX)) {
				continue;
			}
			std::smatch matches;
			if (std::regex_search(line, matches, NUMBERS_LINE_REGEX)) {
				if (matches.size() != 5) {
					continue;
				}
				Vertex3D vertex;
				vertex.id = std::stoi(matches[1].str());
				vertex.x = std::stof(matches[2].str());
				vertex.y = std::stof(matches[3].str());
				vertex.z = std::stof(matches[4].str());
				_coordinates.push_back(vertex);
			}
			else {
				std::cout << "Can not load line: " << line << std::endl;
			}
		}
		file.close();
		return true;
	}

	const std::list<Vertex3D>& CoordinatesFileParser::coordinates() const
	{
		return _coordinates;
	}

	bool CoordinatesFileParser::writeFile(
			const std::string& outputFileName,
			const std::list<Vertex3D>& coordinates,
			const std::list<std::shared_ptr<delaunay::Triangle> >& triangles,
			const std::set<int>& usedVertexIds)
	{
		std::unique_ptr<DefaultWriter> writer = std::make_unique<DefaultWriter>();
		return  writer->writeFile(
				outputFileName,
				coordinates,
				triangles,
				usedVertexIds);
	}
}
