//
// Created by Vishnu Rajendran on 2025-02-23.
//

#include "beziermesh.h"

vec3 BezierMesh::deCasteljau(std::vector<vec3> points, double t) {
    auto n = points.size();
    while (n > 1) {
        for (auto i = 0; i < n - 1; i++) {
            points[i] = (1 - t) * points[i] + t * points[i + 1];
        }
        n--;
    }
    return points[0];
}

void BezierMesh::generate(ControlMesh& controlMesh) {
    int resolution = getResolution();
    vertices.resize(resolution * resolution, 3);
    faces.resize((resolution - 1) * (resolution - 1) * 2, 3);

    int index = 0;
    auto controlPoints = controlMesh.getVertices();
    for (auto u = 0; u < resolution; u++) {
        auto uu = (double) u / (resolution - 1);
        for (int v = 0; v < resolution; v++) {
            double vv = (double) v / (resolution - 1);
            std::vector<Eigen::Vector3d> tempCols;
            for (int i = 0; i < 4; i++) {
                std::vector<Eigen::Vector3d> column = {
                        controlPoints[i], controlPoints[i + 4], controlPoints[i + 8], controlPoints[i + 12]
                };
                tempCols.push_back(deCasteljau(column, vv));
            }
            vertices.row(index++) = deCasteljau(tempCols, uu);
        }
    }

    index = 0;
    for (int i = 0; i < resolution - 1; i++) {
        for (int j = 0; j < resolution - 1; j++) {
            int idx = i * resolution + j;
            faces.row(index++) << idx, idx + 1, idx + resolution;
            faces.row(index++) << idx + 1, idx + resolution + 1, idx + resolution;
        }
    }
}