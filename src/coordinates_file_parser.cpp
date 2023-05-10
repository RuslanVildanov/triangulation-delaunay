#include "coordinates_file_parser.h"
#include "vertex_3d.h"
#include "delaunay/triangle.h"
#include "delaunay/vertex_2d.h"

#include <iostream>
#include <fstream>
#include <regex>
#include <sstream>


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

std::string intToString(int i, size_t minLength)
{
	std::string result = std::to_string(i);
	if (result.size() < minLength) {
		for(size_t j = result.size(); j <= minLength; j++) {
			result = ' ' + result;
		}
	}
	return result;
}

std::string floatToString(float f, size_t minLength)
{
	std::ostringstream ss;
	ss << f;
	std::string result(ss.str());
	if (result.size() < minLength) {
		for(size_t j = result.size(); j <= minLength; j++) {
			result = ' ' + result;
		}
	}
	return result;
}

bool CoordinatesFileParser::writeFile(
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
		file << intToString(vertex.id, 5) << ','
			<< floatToString(vertex.x, 13) << ','
			<< floatToString(vertex.y, 13) << ','
			<< floatToString(vertex.z, 13) << std::endl;
	}

	file << "*Elements" << std::endl;
	for (const auto& triangle : triangles) {
		file << intToString(triangle->id, 5) << ','
			 << intToString(triangle->vertex1->id, 5) << ','
			 << intToString(triangle->vertex2->id, 5) << ','
			 << intToString(triangle->vertex3->id, 5) << std::endl;
	}
	file.close();
	return true;
}
