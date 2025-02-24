//
// Created by Vishnu Rajendran on 2025-02-23.
//

#ifndef SPLINE_EDITOR_EDITOR_H
#define SPLINE_EDITOR_EDITOR_H


//LIBIGL
#include <Eigen/Core>
#include "igl/opengl/glfw/imgui/ImGuiPlugin.h"
#include "igl/opengl/glfw/imgui/ImGuizmoWidget.h"
#include "igl/opengl/glfw/Viewer.h"

#include "controlmesh.h"
#include "bsplinemesh.h"

class Editor {
    ControlMesh cMesh;
    BSplineMesh bMesh;

    igl::opengl::glfw::Viewer viewer;
    igl::opengl::glfw::imgui::ImGuiPlugin imGuiPlugin;
    igl::opengl::glfw::imgui::ImGuizmoWidget guizmoWidget;

    int selectedPoint = 0;

    const Eigen::Vector4f BACKGROUND_COLOR = Eigen::Vector4f (0.18f, 0.18f, 0.18f, 1);
    const Eigen::RowVector3d CONTROLPOINT_COLOR = Eigen::RowVector3d (1, 0, 0);
    const Eigen::RowVector3d CONTROLEDGE_COLOR = Eigen::RowVector3d (0, 1, 0);

private:
    void guizmoCallback(const Eigen::Matrix4f& T);
    bool mouseDownCallback(const igl::opengl::glfw::Viewer& viewer, const int& button, const int& modifier);
    void updateViewer();
public:
    Editor(ControlMesh& cMesh, BSplineMesh& bMesh);
};


#endif //SPLINE_EDITOR_EDITOR_H
