#include "coordinates_file_parser.h"
#include "vertex_3d.h"
#include "delaunay/delaunay_service.h"
#include "delaunay/vertex_2d.h"

#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <list>
#include <set>


static const std::string EMPTY_STRING;
static const std::string DEFAULT_OUTPUT_FILENAME = "nodes_triangles.output";

class InputParser
{
private:
	std::vector<std::string> _tokens;

public:
	InputParser(int &argc, char **argv)
	{
		for (int i = 1; i < argc; i++) {
			_tokens.emplace_back(argv[i]);
		}
	}

	const std::string& getCmdOption(const std::string& option) const
	{
		std::vector<std::string>::const_iterator i;
		i = std::find(_tokens.begin(), _tokens.end(), option);
		if (i != _tokens.end() && ++i != _tokens.end()) {
			return *i;
		}
		return EMPTY_STRING;
	}

	bool cmdOptionExists(const std::string& option) const
	{
		return std::find(_tokens.begin(), _tokens.end(), option) != _tokens.end();
	}
};

void usage()
{
	std::cout << "It creates bunch of triangles for input nodes (vertices)." << std::endl;
	std::cout << "DelaunayTriangulation -i <input file name> [-o <output file name>]" << std::endl;
	std::cout << "\t -i or --input \t Full path to input file" << std::endl;
	std::cout << "\t -o or --output\t Full path to output file. Default name: " << DEFAULT_OUTPUT_FILENAME << std::endl;
}

int main(int argc, char* argv[])
{
	InputParser input(argc, argv);
	if (input.cmdOptionExists("-h") || input.cmdOptionExists("--help")) {
		usage();
		return 0;
	}

	std::string inputFileName = input.getCmdOption("-i");
	if (inputFileName.empty()) {
		inputFileName = input.getCmdOption("--input");
	}
	if (inputFileName.empty()) {
		std::cout << "There is no required argument -i / --input ." << std::endl << std::endl;
		usage();
		return -1;
	}
	std::string outputFileName = input.getCmdOption("-o");
	if (outputFileName.empty()) {
		outputFileName = input.getCmdOption("--output");
		if (outputFileName.empty()) {
			outputFileName = DEFAULT_OUTPUT_FILENAME;
		}
	}

	std::cout << "Loading the input file..." << std::endl;
	CoordinatesFileParser coordinatesParser;
	if (!coordinatesParser.loadFile(inputFileName)) {
		std::cout << "There is an error when loading input file. File can not be processed." << std::endl;
		return -2;
	}
	std::list<Vertex3D> coordinates = coordinatesParser.coordinates();
	std::cout << "Loaded number of coordinates: " << coordinates.size() << std::endl << std::flush;

	std::cout << "Building triangulation..." << std::endl;
	bool ok;
	std::list<std::shared_ptr<delaunay::Vertex2D> > vertices = projectVertex3D(coordinates);
	::delaunay::DelaunayService service;
	std::list<std::shared_ptr<delaunay::Triangle> > triangles =
			service.iterativeDynamicProcess(vertices, &ok);
	std::set<int> usedVertexIds = ::delaunay::DelaunayService::vertexIds(triangles);
	std::cout << "Built number of triangles: " << triangles.size() << std::endl << std::flush;

	std::cout << "Writing output file..." << std::endl;
	if (!coordinatesParser.writeFile(outputFileName, coordinates, triangles, usedVertexIds)) {
		std::cout << "There is an error when writing output file." << std::endl;
		return -3;
	}
	std::cout << "Saved the output file." << std::endl;

	return 0;
}
