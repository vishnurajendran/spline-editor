//
// Created by Vishnu Rajendran on 2025-02-23.
//

#include "controlmesh.h"

ControlMesh::ControlMesh() {
    controlPoints = {
            { -1, 0,  -1 }, { -0.5, 0, -1 }, {  0.5, 0,  -1 }, {  1, 0 , -1 },
            { -1, 0,  -0.5 }, { -0.5, 0, -0.5 }, {  0.5, 0, -0.5 }, {  1, 0, -0.5 },
            { -1,  0, 0.5 }, { -0.5,  0, 0.5 }, {  0.5,  0, 0.5 }, {  1,  0, 0.5 },
            { -1,  0,  1 }, { -0.5,  0,  1 }, {  0.5,  0,  1 }, {  1,  0,  1 }
    };
    generateMesh();
}

void ControlMesh::generateMesh() {
    vMat.resize(controlPoints.size(), 3);
    for (int i = 0; i < controlPoints.size(); i++) {
        vMat.row(i) = controlPoints[i];
    }

    std::vector<std::pair<int, int>> edges;
    int rows = 4, cols = 4;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            int idx = i * cols + j;
            if (j < cols - 1) edges.emplace_back(idx, idx + 1);
            if (i < rows - 1) edges.emplace_back(idx, idx + cols);
        }
    }

    eMat.resize(edges.size(), 2);
    for (int i = 0; i < edges.size(); i++) {
        eMat.row(i) << edges[i].first, edges[i].second;
    }
}

MatXd ControlMesh::getVertices() {
    return vMat;
}

MatXi ControlMesh::getEdges() {
    return eMat;
}
