//
// Created by Vishnu Rajendran on 2025-02-23.
//

#include "bsplinemesh.h"

#include <iostream>

vec3d BSplineMesh::evaluate(std::vector<vec3d> &cpGrid, double u, double v) {
    vec3d point(0,0,0);
    double Bu[4] = { B0(u), B1(u), B2(u), B3(u) };
    double Bv[4] = { B0(v), B1(v), B2(v), B3(v) };

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            point += Bu[i] * Bv[j] * cpGrid[i * 4 + j];
        }
    }
    return point;
}

void BSplineMesh::generate(ControlMesh& controlMesh) {
    int resolution = getResolution();
    int patchCount = controlMesh.getPatches();
    vertices.resize(patchCount * resolution * resolution, 3);
    faces.resize(patchCount * (resolution - 1) * (resolution - 1) * 2, 3);

    int vertexOffset = 0;
    int faceOffset = 0;

    int cols = controlMesh.getColumns();
    for (int p=0; p < patchCount; p++) {
        std::vector<vec3d> patchControlPoints(
            controlMesh.getControlPoints()->begin() + (p*cols), //shift our vertex row.
            controlMesh.getControlPoints()->begin() + (p*cols) + cols*cols
        );

        double minU = 0.0, maxU = 1.0;
        double minV = 0.0, maxV = 1.0;

        // Calculate vertices
        for (int u = 0; u < resolution; u++) {
            double uu = minU + (maxU - minU) * (double)u / (resolution - 1);
            for (int v = 0; v < resolution; v++) {
                double vv = minV + (maxV - minV) * (double)v / (resolution - 1);
                vertices.row(vertexOffset++) = evaluate(patchControlPoints, uu, vv);
            }
        }

        // Calculate faces
        int baseIndex = p * resolution * resolution;
        for (int i = 0; i < resolution - 1; i++) {
            for (int j = 0; j < resolution - 1; j++) {
                int idx = baseIndex + i * resolution + j;
                faces.row(faceOffset++) << idx, idx + resolution, idx + 1;
                faces.row(faceOffset++) << idx + 1, idx + resolution, idx + resolution + 1;
            }
        }
    }
}


MatXd& BSplineMesh::getVertices() {
    return vertices;
}

MatXi& BSplineMesh::getFaces() {
    return faces;
}