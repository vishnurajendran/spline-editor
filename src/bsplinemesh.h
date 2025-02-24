//
// Created by Vishnu Rajendran on 2025-02-23.
//

#ifndef SPLINE_EDITOR_BSPLINEMESH_H
#define SPLINE_EDITOR_BSPLINEMESH_H

#include "typedefines.h"
#include "controlmesh.h"

class BSplineMesh {
private:
    MatXd vertices;
    MatXi faces;
    int gridResoultion = 20;

    vec3 evaluate(std::vector<vec3>& cpGrid, double u, double v);

    double B0(double u) { return (1 - u)*(1 - u)*(1 - u) / 6.0; }
    double B1(double u) { return (3*u*u*u - 6*u*u + 4) / 6.0; }
    double B2(double u) { return (-3*u*u*u + 3*u*u + 3*u + 1) / 6.0; }
    double B3(double u) { return u*u*u / 6.0; }

public:
    inline const int getResolution() {return gridResoultion;}
    inline void setResolution(const int& resolution) {gridResoultion = resolution;}

    void generate(ControlMesh& controlMesh);
    MatXd& getSurface();
    MatXi& getFaces();
};


#endif //SPLINE_EDITOR_BSPLINEMESH_H
