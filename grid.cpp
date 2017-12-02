#include "grid.h"

// grid_parser member functions
size_t grid_parser::getDimSize(const std::string& dim)
{
        return data.getDim(dim).getSize();
}

size_t grid_parser::getNumCells()
{
        return getDimSize(cnf.cell_dim_name);
}

size_t grid_parser::getNumEdges()
{
        return getDimSize(cnf.edge_dim_name);
}

size_t grid_parser::getNumVertices()
{
        return getDimSize(cnf.vertex_dim_name);
}

std::vector<point> grid_parser::getPoints(const std::string& x, const std::string& y, const std::string& dim)
{
        size_t N = getDimSize(dim);
        std::vector<double> lons(N);
        std::vector<double> lats(N);

        // Retrieve the cell center point latitudes
        NcVar clat=data.getVar(y);
        clat.getVar(lats.data());

        // Retrieve the cell center point longitudes
        NcVar clon=data.getVar(x);
        clon.getVar(lons.data());

        std::vector<point> points(N);

        std::transform(lats.begin(),lats.end(),lons.begin(),points.begin(),
                        [](const double& lat, const double& lon)
                        {
                                return point(lon,lat);
                        });
        return points;
}

std::vector<point> grid_parser::getCellCenters()
{
        return getPoints(cnf.center_x_name,cnf.center_y_name,cnf.cell_dim_name);
}

std::vector<point> grid_parser::getVertices()
{
        return getPoints(cnf.vertex_x_name,cnf.vertex_y_name,cnf.vertex_dim_name);        
}

std::vector<point> grid_parser::getEdgeCenters()
{
        return getPoints(cnf.edge_x_name,cnf.edge_y_name,cnf.edge_dim_name);
}

std::vector<edge> grid_parser::getEdges()
{
        size_t N = getNumEdges();
        std::vector<edge> edges;
        edges.reserve(N);

        auto vertices = getVertices();
        auto edge_c = getEdgeCenters();

        std::vector<int> indexes(2*N);

        NcVar edgeIndexes=data.getVar("edge_vertices");
        edgeIndexes.getVar(indexes.data());

        for(size_t i = 0; i<N; ++i)
        {
                edges.emplace_back(vertices[indexes[i]-1],vertices[indexes[i+N]-1],edge_c[i]);
        }
        return edges;
}

std::vector<std::unique_ptr<elem>> grid_parser::getElements()
{
        size_t N = getNumCells();
        // construct list of empty elements
        std::vector<std::unique_ptr<elem>> elems;
        elems.reserve(N);
        for(size_t i = 0; i<N ; ++i)
        {
                elems.push_back(std::make_unique<elem>());
        }

        auto vertices = getVertices();
        auto centers = getCellCenters();
        auto edges = getEdges();

        std::vector<int> vind(3*N);
        std::vector<int> eind(3*N);
        std::vector<int> ncind(3*N);

        NcVar edgeIndexes=data.getVar("edge_of_cell");
        edgeIndexes.getVar(eind.data());

        NcVar vertIndexes=data.getVar("vertex_of_cell");
        vertIndexes.getVar(vind.data());

        NcVar ncIndexes=data.getVar("neighbor_cell_index");
        ncIndexes.getVar(ncind.data());

        for(size_t i = 0; i<N; ++i)
        {
                elems[i]->index = i;
                elems[i]->center_point = centers[i];
                elems[i]->vertices.reserve(3);
                        elems[i]->vertices.push_back(vertices[vind[i]-1]);
                        elems[i]->vertices.push_back(vertices[vind[i+N]-1]);
                        elems[i]->vertices.push_back(vertices[vind[i+2*N]-1]);
                elems[i]->edges.reserve(3);
                        elems[i]->edges.push_back(edges[eind[i]-1]);
                        elems[i]->edges.push_back(edges[eind[i+N]-1]);
                        elems[i]->edges.push_back(edges[eind[i+2*N]-1]);
                elems[i]->neighbours.reserve(3);
                        elems[i]->neighbours.push_back(elems[ncind[i]-1].get());
                        elems[i]->neighbours.push_back(elems[ncind[i+N]-1].get());
                        elems[i]->neighbours.push_back(elems[ncind[i+2*N]-1].get());
	}
        return elems;
}

std::vector<int> grid_parser::getVertIndexes()
{
        size_t N = getNumVertices();
        // construct list of empty elements
        std::vector<int> verts(N);

        NcVar vIndexes=data.getVar("vertex_index");
        vIndexes.getVar(verts.data());
        return verts;
}

// grid member functions
grid::grid(const std::string& file)
{
	auto G = grid_parser(conf(),file);
	elements = G.getElements();
	indexed_elements.resize(elements.size());
	std::transform(elements.begin(),elements.end(),indexed_elements.begin(),[](const std::unique_ptr<elem>& ptr){return ptr.get();});
}

elem* grid::LatLon(size_t index)
{
	return indexed_elements[index];
}

elem* grid::XY(const point& p)
{

        auto n = std::find_if(elements.begin(),elements.end(),
                [&](const std::unique_ptr<elem>& e)
                {
                        return within(*e,p);
                });
        return n->get();
}
