//
// Created by Vishnu Rajendran on 2025-02-23.
//

#ifndef SPLINE_EDITOR_EDITOR_H
#define SPLINE_EDITOR_EDITOR_H


//LIBIGL
#include <Eigen/Core>
#include <typedefines.h>
#include "igl/opengl/glfw/imgui/ImGuiPlugin.h"
#include "igl/opengl/glfw/imgui/ImGuizmoWidget.h"
#include "igl/opengl/glfw/Viewer.h"

#include "controlmesh.h"
#include "bsplinemesh.h"

class Editor {

    enum Visibility
    {
        Lattice_Only=0,
        Surface_Only,
        Lattice_And_Surface
    };

    ControlMesh cMesh;
    BSplineMesh bMesh;

    igl::opengl::glfw::Viewer viewer;
    igl::opengl::glfw::imgui::ImGuiPlugin imGuiPlugin;
    igl::opengl::glfw::imgui::ImGuizmoWidget guizmoWidget;
    igl::opengl::glfw::imgui::ImGuiMenu menuWidget;

    int selectedPoint = 0;

    const vec4f BACKGROUND_COLOR = vec4f (0.18f, 0.18f, 0.18f, 1);
    const vecR3d CONTROLPOINT_COLOR = vecR3d (1, 0, 0);
    const vecR3d CONTROLEDGE_COLOR = vecR3d (0, 1, 0);

    vec2i mousePos = vec2i (0, 0);
    Visibility visibility = Visibility::Lattice_And_Surface;

private:
    void guizmoCallback(const Eigen::Matrix4f& T);
    bool mouseDownCallback(const igl::opengl::glfw::Viewer& viewer, const int& button, const int& modifier);
    void updateViewer();
    void updateSelection();
    void onMenuGUI();
    void onAddPatch();
    void onRemoveLastPatch();
    void onLoadSpline();
    void onSaveSpline();
    void onExportSpline();

public:
    Editor(ControlMesh& cMesh, BSplineMesh& bMesh);
};


#endif //SPLINE_EDITOR_EDITOR_H
