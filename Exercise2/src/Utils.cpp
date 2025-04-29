#include "Utils.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

namespace PolygonalLibrary {

bool ImportMesh(PolygonalMesh& mesh) 
{
    if (!ImportCell0Ds(mesh)) return false;
    if (!ImportCell1Ds(mesh)) return false;
    if (!ImportCell2Ds(mesh)) return false;
    return true;
}

bool ImportCell0Ds(PolygonalMesh& mesh) 
{
    ifstream file("./Cell0Ds.csv");
    if (file.fail()) 
		return false;

    string line;
    getline(file, line);

    vector<string> lines;
    while (getline(file, line)) lines.push_back(line);

    mesh.NumCell0Ds = lines.size();
    mesh.Cell0DsId.reserve(mesh.NumCell0Ds);
    mesh.Cell0DsMarker.reserve(mesh.NumCell0Ds);
    mesh.Cell0DsCoordinates = MatrixXd::Zero(3, mesh.NumCell0Ds);

    for (const string& row : lines) 
	{
        istringstream ss(row);
        string tmp;
        unsigned int id, marker;
        double x, y;
        getline(ss, tmp, ';'); 
		id = stoi(tmp);
        getline(ss, tmp, ';'); 
		marker = stoi(tmp);
        getline(ss, tmp, ';'); 
		x = stod(tmp);
        getline(ss, tmp, ';'); 
		y = stod(tmp);

        mesh.Cell0DsId.push_back(id);
        mesh.Cell0DsMarker.push_back(marker);
        mesh.Cell0DsCoordinates(0, id) = x;
        mesh.Cell0DsCoordinates(1, id) = y;
    }
    return true;
}

bool ImportCell1Ds(PolygonalMesh& mesh) 
{
    ifstream file("./Cell1Ds.csv");
    if (file.fail()) 
		return false;

    string line;
    getline(file, line);

    vector<string> lines;
    while (getline(file, line)) lines.push_back(line);

    mesh.NumCell1Ds = lines.size();
    mesh.Cell1DsId.reserve(mesh.NumCell1Ds);
    mesh.Cell1DsMarker.reserve(mesh.NumCell1Ds);
    mesh.Cell1DsExtrema = MatrixXi(2, mesh.NumCell1Ds);

    for (const string& row : lines) 
	{
        istringstream ss(row);
        string tmp;
        unsigned int id, marker, origin, end;
        getline(ss, tmp, ';'); 
		id = stoi(tmp);
        getline(ss, tmp, ';'); 
		marker = stoi(tmp);
        getline(ss, tmp, ';'); 
		origin = stoi(tmp);
        getline(ss, tmp, ';'); 
		end = stoi(tmp);

        mesh.Cell1DsId.push_back(id);
        mesh.Cell1DsMarker.push_back(marker);
        mesh.Cell1DsExtrema(0, id) = origin;
        mesh.Cell1DsExtrema(1, id) = end;
    }
    return true;
}

bool ImportCell2Ds(PolygonalMesh& mesh) 
{
    ifstream file("./Cell2Ds.csv");
    if (file.fail())
		return false;

    string line;
    getline(file, line); 

    vector<string> lines;
    while (getline(file, line)) lines.push_back(line);

    mesh.NumCell2Ds = lines.size();
    mesh.Cell2DsId.reserve(mesh.NumCell2Ds);
    mesh.Cell2DsMarker.reserve(mesh.NumCell2Ds);
    mesh.Cell2DsVertices.reserve(mesh.NumCell2Ds);
    mesh.Cell2DsEdges.reserve(mesh.NumCell2Ds);

    for (const string& row : lines) 
	{
        istringstream ss(row);
        string tmp;
        unsigned int id, marker, numVertices, numEdges;

        getline(ss, tmp, ';'); 
		id = stoi(tmp);
        getline(ss, tmp, ';'); 
		marker = stoi(tmp);
        getline(ss, tmp, ';'); 
		numVertices = stoi(tmp);

        vector<unsigned int> vertices(numVertices);
        for (unsigned int i = 0; i < numVertices; ++i) 
		{
            getline(ss, tmp, ';'); 
			vertices[i] = stoi(tmp);
        }

        getline(ss, tmp, ';'); numEdges = stoi(tmp);
        vector<unsigned int> edges(numEdges);
        for (unsigned int i = 0; i < numEdges; ++i) 
		{
            getline(ss, tmp, ';'); 
			edges[i] = stoi(tmp);
        }

        mesh.Cell2DsId.push_back(id);
        mesh.Cell2DsMarker.push_back(marker);
        mesh.Cell2DsVertices.push_back(vertices);
        mesh.Cell2DsEdges.push_back(edges);
    }
    return true;
}

bool CheckEdgeLengths(const PolygonalMesh& mesh) 
{
    for (unsigned int i = 0; i < mesh.NumCell1Ds; ++i) 
	{
        unsigned int origin = mesh.Cell1DsExtrema(0, i);
        unsigned int end = mesh.Cell1DsExtrema(1, i);

        Vector2d p1 = mesh.Cell0DsCoordinates.block<2,1>(0, origin);
        Vector2d p2 = mesh.Cell0DsCoordinates.block<2,1>(0, end);

        double length = (p2 - p1).norm();

        if (length == 0.0) 
		{
            cerr << "Edge with zero length found at ID: " << mesh.Cell1DsId[i] << endl;
            return false;
        }
    }
    return true;
}

bool CheckPolygonAreas(const PolygonalMesh& mesh) 
{
    for (unsigned int i = 0; i < mesh.NumCell2Ds; ++i) 
	{
        const vector<unsigned int>& vertices = mesh.Cell2DsVertices[i];
        double area = 0.0;

        for (size_t j = 0; j < vertices.size(); ++j) 
		{
            unsigned int a = vertices[j];
            unsigned int b = vertices[(j + 1) % vertices.size()];
            area += (mesh.Cell0DsCoordinates(0, a) * mesh.Cell0DsCoordinates(1, b)) -
                    (mesh.Cell0DsCoordinates(0, b) * mesh.Cell0DsCoordinates(1, a));
        }

        area = 0.5 * fabs(area);
        if (area == 0.0) 
		{
            cerr << "Polygon with zero area found at ID: " << mesh.Cell2DsId[i] << endl;
            return false;
        }
    }
    return true;
}

}
