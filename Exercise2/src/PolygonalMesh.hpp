#pragma once

#include <iostream>
#include "Eigen/Eigen"

using namespace std;
using namespace Eigen;

namespace PolygonalLibrary {

struct PolygonalMesh {
	
    unsigned int NumCell0Ds;
    unsigned int NumCell1Ds;
    unsigned int NumCell2Ds;

    vector<unsigned int> Cell0DsId;
    vector<unsigned int> Cell0DsMarker;
    MatrixXd Cell0DsCoordinates;

    vector<unsigned int> Cell1DsId;
    vector<unsigned int> Cell1DsMarker;
    MatrixXi Cell1DsExtrema;

    vector<unsigned int> Cell2DsId;
    vector<unsigned int> Cell2DsMarker;
    vector<vector<unsigned int>> Cell2DsVertices;
    vector<vector<unsigned int>> Cell2DsEdges;
};

}