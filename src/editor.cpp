//
// Created by Vishnu Rajendran on 2025-02-23.
//

#include "editor.h"

#include "igl/unproject_onto_mesh.h"
#include "igl/unproject_ray.h"

Editor::Editor(ControlMesh &cMesh, BSplineMesh &bMesh) {
    this->bMesh = bMesh;
    this->cMesh = cMesh;
    this->bMesh.generate(this->cMesh);

    viewer.core().background_color = BACKGROUND_COLOR;
    viewer.plugins.push_back(&imGuiPlugin);

    imGuiPlugin.widgets.push_back(&menuWidget);
    imGuiPlugin.widgets.push_back(&guizmoWidget);

    guizmoWidget.operation = ImGuizmo::OPERATION::TRANSLATE;

    guizmoWidget.callback = [this](const Eigen::Matrix4f T){
        guizmoCallback(T);
    };
    viewer.callback_mouse_down = [this](const igl::opengl::glfw::Viewer& viewer, int bttn, int modif){
        return mouseDownCallback(viewer, bttn, modif);
    };

    viewer.callback_mouse_move = [&](igl::opengl::glfw::Viewer& viewer, int mouse_x, int mouse_y)
    {
        mousePos = vec2i(mouse_x, mouse_y);
        return false;
    };

    menuWidget.callback_draw_viewer_menu = [this]()
    {
        onMenuGUI();
    };

    updateViewer();
    guizmoWidget.visible = false;
    viewer.launch();
}

void Editor::updateViewer() {
    viewer.data().clear();
    viewer.data().point_size = 15.0f;

    if (visibility != Surface_Only)
    {
        viewer.data().set_points(cMesh.getVertices(), CONTROLPOINT_COLOR);
        viewer.data().set_edges(cMesh.getVertices(),cMesh.getEdges(), CONTROLEDGE_COLOR);
    }

    if (visibility != Lattice_Only)
        viewer.data().set_mesh(bMesh.getSurface(), bMesh.getFaces());
}

void Editor::updateSelection()
{
    if (selectedPoint == -1)
        return;

    vec3d pos = (*cMesh.getControlPoints())[selectedPoint];
    guizmoWidget.T(0,3) = pos.x();
    guizmoWidget.T(1,3) = pos.y();
    guizmoWidget.T(2,3) = pos.z();
    guizmoWidget.T(3,3) = 1.0f;
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

    if (button != GLFW_MOUSE_BUTTON_LEFT) return false;

    auto io = ImGui::GetIO();
    int closest_vertex = -1;
    double closest_distance = 99999;
    double precision_radius_pixels = 10.0; // Adjust in pixels
    auto V = cMesh.getVertices();
    auto core = viewer.core();
    vec3f screen_coords;

    for (int i = 0; i < V.rows(); ++i) {
        // Project vertex to screen-space
        igl::project(V.row(i).cast<float>(), core.view, core.proj, core.viewport, screen_coords);

        // Compute distance from mouse (flip y-coordinate)
        double dx = viewer.current_mouse_x - screen_coords[0];
        double dy = (core.viewport[3] - viewer.current_mouse_y) - screen_coords[1];
        double distance = std::sqrt(dx * dx + dy * dy);

        if (distance < closest_distance && distance <= precision_radius_pixels) {
            closest_distance = distance;
            closest_vertex = i;
        }
    }

    if (closest_vertex != -1)
    {
        selectedPoint = closest_vertex;
        updateSelection();
        guizmoWidget.visible = true;
    }
    else
    {
        guizmoWidget.visible = false;
        selectedPoint = -1;
    }
    return false;
}

void Editor::onMenuGUI()
{
    // Draw parent menu content
    //menuWidget.draw_viewer_menu();
    //Add new group
    if (ImGui::CollapsingHeader("Spline Editor Options", ImGuiTreeNodeFlags_DefaultOpen))
    {

        static const char* items[]{"Lattice Only","Surface Only","Lattice And Surface"};
        static int Selecteditem = 2;
        ImGui::SetNextItemWidth(130);
        if ( ImGui::Combo("Viewing Options", &Selecteditem, items, IM_ARRAYSIZE(items)))
        {
            switch (Selecteditem)
            {
                case 0: visibility = Visibility::Lattice_Only; break;
                case 1: visibility = Visibility::Surface_Only; break;

                case 2:
                default: visibility = Visibility::Lattice_And_Surface; break;
            }
            updateViewer();
        }


        auto resolution = bMesh.getResolution();
        ImGui::SetNextItemWidth(130);
        if (ImGui::InputInt("Mesh Resolution", &resolution, 1))
        {
            if (resolution > 200)
                resolution = 200;

            bMesh.setResolution(resolution);
            bMesh.generate(cMesh);
            updateViewer();

        }
    }
}