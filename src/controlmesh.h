//
// Created by Vishnu Rajendran on 2025-02-23.
//

#ifndef SPLINE_EDITOR_CONTROLMESH_H
#define SPLINE_EDITOR_CONTROLMESH_H

#include "typedefines.h"

class ControlMesh {
private:
    std::vector<vec3> controlPoints;
    MatXd vMat;
    MatXi eMat;
    void generateMesh();

public:
    ControlMesh();
    MatXd getVertices();
    MatXi getEdges();
    inline void setControlPoint(int selected, vec3 value) { controlPoints[selected] = value; }
    inline void update() { generateMesh(); }
    inline std::vector<vec3>* getControlPoints() { return &controlPoints; }
};


#endif //SPLINE_EDITOR_CONTROLMESH_H
