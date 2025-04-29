#include <iostream>
#include "PolygonalMesh.hpp"
#include "Utils.hpp"
#include "UCDUtilities.hpp"

using namespace std;
using namespace Eigen;
using namespace PolygonalLibrary;

int main() 
{
    PolygonalMesh mesh;

    if (!ImportMesh(mesh)) 
	{
        cerr << "File not found or error reading mesh." << endl;
        return 1;
    }
	
	if (!CheckEdgeLengths(mesh)) 
	{
		cerr << "Error: Found zero-length edges!" << endl;
		return 1;
	}

	if (!CheckPolygonAreas(mesh)) 
	{
		cerr << "Error: Found zero-area polygons!" << endl;
		return 1;
	}


    Gedim::UCDUtilities utilities;
    utilities.ExportPoints("./Cell0ds.inp", mesh.Cell0DsCoordinates);
    utilities.ExportSegments("./Cell1Ds.inp", mesh.Cell0DsCoordinates, mesh.Cell1DsExtrema);

    return 0;
}

