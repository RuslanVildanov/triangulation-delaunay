#ifndef _COORDINATES_FILE_PARSER_H
#define _COORDINATES_FILE_PARSER_H

#include <list>
#include <memory>
#include <string>
#include <set>


struct Vertex3D;

namespace delaunay
{
	struct Triangle;
}

class CoordinatesFileParser
{
private:
	std::list<Vertex3D> _coordinates;

public:
	CoordinatesFileParser() = default;
	~CoordinatesFileParser() = default;
	CoordinatesFileParser(const CoordinatesFileParser&) = delete;
	CoordinatesFileParser(CoordinatesFileParser&&) = delete;
	CoordinatesFileParser & operator=(const CoordinatesFileParser&) = delete;
	CoordinatesFileParser & operator=(CoordinatesFileParser&&) = delete;

	bool loadFile(const std::string& fileName);
	[[nodiscard]] const std::list<Vertex3D>& coordinates() const;
	bool writeFile(
			const std::string& outputFileName,
			const std::list<Vertex3D>& coordinates,
			const std::list<std::shared_ptr<delaunay::Triangle> >& triangles,
			const std::set<int>& usedVertexIds);

};

#endif //_COORDINATES_FILE_PARSER_H
