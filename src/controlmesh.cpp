//
// Created by Vishnu Rajendran on 2025-02-23.
//

#include "controlmesh.h"

#include <fstream>
#include <iostream>

const int ControlMesh::COLS = 4;

ControlMesh::ControlMesh() {
    patches = 1;
    float minRange = -1;
    float maxRange = 1;
    gridStep = (maxRange - minRange) / (COLS - 1);
    int rows = COLS; //we start with a square lattice
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < COLS; ++j) {
            float x = minRange + j * gridStep;
            float z = minRange + i * gridStep;
            controlPoints.push_back({x, 0.0f, z});  // Keeping Y = 0
        }
    }

    generateMesh();
}

void ControlMesh::generateMesh() {
    vMat.resize(controlPoints.size(), 3);
    for (int i = 0; i < controlPoints.size(); i++) {
        vMat.row(i) = controlPoints[i];
    }

    std::vector<std::pair<int, int>> edges;
    int rows =  controlPoints.size() / COLS;
    for (int i = 0; i < controlPoints.size() / COLS; i++) {
        for (int j = 0; j < COLS; j++) {
            int idx = i * COLS + j;
            if (j < COLS - 1) edges.emplace_back(idx, idx + 1);
            if (i < rows - 1) edges.emplace_back(idx, idx + COLS);
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

void ControlMesh::addPatch()
{
    // Generate a new row by extending the last row in Z-direction
    std::vector<vec3d> rowAdditions;
    for (int i = 0; i < COLS; i++) {
        int indx = controlPoints.size() - COLS + i;
        vec3d lastPoint = controlPoints[indx];
        rowAdditions.push_back(vec3d(lastPoint.x(), lastPoint.y(), lastPoint.z() + gridStep));
    }

    for (auto row : rowAdditions)
    {
        controlPoints.push_back(row);
    }
    patches+=1;
    generateMesh();
}

void ControlMesh::removeLastPatch()
{
    if (patches <= 1)
        return;

    controlPoints.resize(controlPoints.size()-4);
    patches-=1;
    generateMesh();
}

void ControlMesh::saveToFile(const std::string& path)
{
    std::ofstream file(path, std::ios::binary);
    if (!file)
    {
        std::cerr << "Could not open file for write" << path << std::endl;
        return;
    }

    int totalControlPoints = controlPoints.size();
    file.write(reinterpret_cast<char*>(&patches), sizeof(int));
    file.write(reinterpret_cast<char*>(&gridStep), sizeof(int));
    file.write(reinterpret_cast<char*>(&totalControlPoints), sizeof(int));
    file.write(reinterpret_cast<char*>(controlPoints.data()), controlPoints.size() * sizeof(vec3d));
    file.close();
}

void ControlMesh::loadFromFile(const std::string& path)
{
    std::ifstream file(path, std::ios::binary);
    if (!file)
    {
        std::cerr << "Could not open file for read" << path << std::endl;
        return;
    }

    int totalControlPoints = 0;
    file.read(reinterpret_cast<char*>(&patches), sizeof(int));
    file.read(reinterpret_cast<char*>(&gridStep), sizeof(int));
    file.read(reinterpret_cast<char*>(&totalControlPoints), sizeof(int));

    controlPoints.resize(totalControlPoints);
    file.read(reinterpret_cast<char*>(controlPoints.data()), totalControlPoints * sizeof(vec3d));

    file.close();
    generateMesh();
}
