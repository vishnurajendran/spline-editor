//
// Created by Vishnu Rajendran on 2025-02-23.
//

#ifndef SPLINE_EDITOR_CONTROLMESH_H
#define SPLINE_EDITOR_CONTROLMESH_H

#include "typedefines.h"

class ControlMesh {
private:
    std::vector<vec3d> controlPoints;
    MatXd vMat;
    MatXi eMat;
    void generateMesh();

    static const int COLS;
    float gridStep=0;
    int patches = 0;
public:
    ControlMesh();
    MatXd getVertices();
    MatXi getEdges();
    void addPatch();
    void removeLastPatch();
    inline int getPatches() { return patches; }
    inline void setControlPoint(int selected, vec3d value) { controlPoints[selected] = value; }
    inline void update() { generateMesh(); }
    inline std::vector<vec3d>* getControlPoints() { return &controlPoints; }
    void saveToFile(const std::string& path);
    void loadFromFile(const std::string& path);
    inline static int getColumns() { return COLS; }
};


#endif //SPLINE_EDITOR_CONTROLMESH_H
