#include "point.h"
#include "edge.h"
#include "box.h"
#include "element.h"

#include <netcdf>
#include <string>
#include <algorithm>
#include <memory>

#include <iostream>

#ifndef GRID_H
#define GRID_H

using namespace netCDF;

struct grid
{
	grid(const std::string&);
	std::vector<std::unique_ptr<elem>> elements;
	std::vector<elem*> indexed_elements;
	box bounding;

	elem* LatLon(size_t);
	elem* XY(const point&);
};

struct conf
{
	// cell centers
	std::string cell_dim_name = "cell";
	std::string center_x_name = "clon";
	std::string center_y_name = "clat";
	// vertices
	std::string vertex_dim_name = "vertex";
	std::string vertex_x_name = "vlon";
	std::string vertex_y_name = "vlat";
	// edge centers
	std::string edge_dim_name = "edge";
	std::string edge_x_name = "elon";
	std::string edge_y_name = "elat";
};

class grid_parser
{
	conf cnf;
	NcFile data;

	size_t getDimSize(const std::string& dim);
	std::vector<point> getPoints(const std::string& x, const std::string& y, const std::string& dim);

	public:
	grid_parser(const conf& cnf, const std::string& file) : cnf(cnf), data(file,NcFile::read){}

	size_t getNumCells();
	size_t getNumEdges();
	size_t getNumVertices();

	std::vector<point> getCellCenters();
	std::vector<point> getEdgeCenters();
	std::vector<point> getVertices();

	std::vector<edge> getEdges();
	std::vector<std::unique_ptr<elem>> getElements();

	std::vector<int> getVertIndexes();
};

#endif /* GRID_H */
