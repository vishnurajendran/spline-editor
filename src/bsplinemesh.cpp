//
// Created by Vishnu Rajendran on 2025-02-23.
//

#include "bsplinemesh.h"

vec3 BSplineMesh::evaluate(std::vector<vec3> &cpGrid, double u, double v) {
    vec3 point(0,0,0);
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
    vertices.resize(resolution * resolution, 3);
    faces.resize((resolution - 1) * (resolution - 1) * 2, 3);

    int index = 0;
    auto controlPoints = *controlMesh.getControlPoints();

    // Extended evaluation range
    double minU = -0.5, maxU = 1.5;
    double minV = -0.5, maxV = 1.5;

    for (int u = 0; u < resolution; u++) {
        double uu = minU + (maxU - minU) * (double)u / (resolution - 1);
        for (int v = 0; v < resolution; v++) {
            double vv = minV + (maxV - minV) * (double)v / (resolution - 1);
            vertices.row(index++) = evaluate(controlPoints, uu, vv);
        }
    }

    index = 0;
    for (int i = 0; i < resolution - 1; i++) {
        for (int j = 0; j < resolution - 1; j++) {
            int idx = i * resolution + j;
            faces.row(index++) << idx, idx + resolution, idx + 1;
            faces.row(index++) << idx + 1, idx + resolution, idx + resolution + 1;

        }
    }
}


MatXd& BSplineMesh::getSurface() {
    return vertices;
}

MatXi& BSplineMesh::getFaces() {
    return faces;
}