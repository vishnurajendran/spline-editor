//
// Created by Vishnu Rajendran on 2025-02-23.
//

#include "igl/unproject.h"
#include "igl/unproject_ray.h"
#include "editor.h"

Editor::Editor(ControlMesh &cMesh, BSplineMesh &bMesh) {
    this->bMesh = bMesh;
    this->cMesh = cMesh;
    this->bMesh.generate(this->cMesh);

    viewer.core().background_color = BACKGROUND_COLOR;
    viewer.plugins.push_back(&imGuiPlugin);
    imGuiPlugin.widgets.push_back(&guizmoWidget);
    guizmoWidget.operation = ImGuizmo::OPERATION::TRANSLATE;

    guizmoWidget.callback = [this](const Eigen::Matrix4f T){
        guizmoCallback(T);
    };
    viewer.callback_mouse_down = [this](const igl::opengl::glfw::Viewer& viewer, int bttn, int modif){
        std::cout<<x<<","<<y<<std::endl;
        return mouseDownCallback(viewer, bttn, modif);
    };
    updateViewer();
    viewer.launch();
}

void Editor::updateViewer() {
    viewer.data().clear();
    viewer.data().set_points(cMesh.getVertices(), CONTROLPOINT_COLOR);
    viewer.data().set_edges(cMesh.getVertices(),cMesh.getEdges(), CONTROLEDGE_COLOR);
    viewer.data().set_mesh(bMesh.getSurface(), bMesh.getFaces());
}

void Editor::guizmoCallback(const Eigen::Matrix4f &T) {
    if (selectedPoint == -1) return;

    //set control point from guizmo
    cMesh.setControlPoint(selectedPoint, T.block<3, 1>(0, 3).cast<double>());

    //update control mesh
    cMesh.update();

    // Update bezier mesh
    //Todo:: we are copying the data struct, move this to reference type instead of value
    bMesh.generate(cMesh);
    updateViewer();
}

bool Editor::mouseDownCallback(const igl::opengl::glfw::Viewer& viewer, const int& button, const int& modifier) {
    

    return false;
}