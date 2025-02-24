//
// Created by Vishnu Rajendran on 2025-02-23.
//

#ifndef SPLINE_EDITOR_BEZIERMESH_H
#define SPLINE_EDITOR_BEZIERMESH_H


#include "splinecore.h"

class BezierMesh {
private:
    MatXd vertices;
    MatXi faces;
    int gridResoultion = 20;

    vec3 deCasteljau(std::vector<vec3> points, double t);
public:
    inline const int getResolution() {return gridResoultion;}
    inline void setResolution(const int& resolution) {gridResoultion = resolution;}

    void generate(ControlMesh& controlMesh);
    MatXd getSurface();
    MatXi getFaces();
};


#endif //SPLINE_EDITOR_BEZIERMESH_H
