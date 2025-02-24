#include "splinecore.h"

// Define control lattice (4x4 grid for now)
std::vector<vec3> controlPoints = {
        { -1, 0,  -1 }, { -0.5, 0, -1 }, {  0.5, 0,  -1 }, {  1, 0 , -1 },
        { -1, 0,  -0.5 }, { -0.5, 0, -0.5 }, {  0.5, 0, -0.5 }, {  1, 0, -0.5 },
        { -1,  0, 0.5 }, { -0.5,  0, 0.5 }, {  0.5,  0, 0.5 }, {  1,  0, 0.5 },
        { -1,  0,  1 }, { -0.5,  0,  1 }, {  0.5,  0,  1 }, {  1,  0,  1 }
};

void getControlMesh(Eigen::MatrixXd &V, Eigen::MatrixXi &E) {
    V.resize(controlPoints.size(), 3);
    for (int i = 0; i < controlPoints.size(); i++) {
        V.row(i) = controlPoints[i];
    }

    std::vector<std::pair<int, int>> edges;
    int rows = 4, cols = 4;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            int idx = i * cols + j;
            if (j < cols - 1) edges.emplace_back(idx, idx + 1);
            if (i < rows - 1) edges.emplace_back(idx, idx + cols);
        }
    }

    E.resize(edges.size(), 2);
    for (int i = 0; i < edges.size(); i++) {
        E.row(i) << edges[i].first, edges[i].second;
    }
}

Eigen::Vector3d deCasteljau(std::vector<Eigen::Vector3d> points, double t) {
    int n = points.size();
    while (n > 1) {
        for (int i = 0; i < n - 1; i++) {
            points[i] = (1 - t) * points[i] + t * points[i + 1];
        }
        n--;
    }
    return points[0];
}

void generateBezierSurface(Eigen::MatrixXd &V, Eigen::MatrixXi &F) {
    int gridSize = 20;
    V.resize(gridSize * gridSize, 3);
    F.resize((gridSize - 1) * (gridSize - 1) * 2, 3);

    int index = 0;
    for (int u = 0; u < gridSize; u++) {
        double uu = static_cast<double>(u) / (gridSize - 1);
        for (int v = 0; v < gridSize; v++) {
            double vv = static_cast<double>(v) / (gridSize - 1);

            std::vector<Eigen::Vector3d> tempCols;
            for (int i = 0; i < 4; i++) {
                std::vector<Eigen::Vector3d> column = {
                        controlPoints[i], controlPoints[i + 4], controlPoints[i + 8], controlPoints[i + 12]
                };
                tempCols.push_back(deCasteljau(column, vv));
            }

            V.row(index++) = deCasteljau(tempCols, uu);
        }
    }

    index = 0;
    for (int i = 0; i < gridSize - 1; i++) {
        for (int j = 0; j < gridSize - 1; j++) {
            int idx = i * gridSize + j;
            F.row(index++) << idx, idx + 1, idx + gridSize;
            F.row(index++) << idx + 1, idx + gridSize + 1, idx + gridSize;
        }
    }
}


int main() {

    ControlMesh cMesh;
    BezierMesh bMesh;

    Eigen::MatrixXd V_control, V_surface;
    Eigen::MatrixXi E_control, F_surface;


    igl::opengl::glfw::imgui::ImGuiPlugin imgui_plugin;

    igl::opengl::glfw::Viewer viewer;
    viewer.plugins.push_back(&imgui_plugin);

    igl::opengl::glfw::imgui::ImGuizmoWidget gizmo;
    imgui_plugin.widgets.push_back(&gizmo);
    gizmo.operation = ImGuizmo::TRANSLATE;

    int selectedPoint = 0;
    gizmo.callback = [&](const Eigen::Matrix4f & T)
    {
        if (selectedPoint == -1) return;

        controlPoints[selectedPoint] = T.block<3, 1>(0, 3).cast<double>();

        // Update meshes
        getControlMesh(V_control, E_control);
        generateBezierSurface(V_surface, F_surface);

        viewer.data().clear();
        viewer.data().set_points(V_control, Eigen::RowVector3d(1, 0, 0));
        viewer.data().set_edges(V_control, E_control, Eigen::RowVector3d(0, 0, 1));
        viewer.data().set_mesh(V_surface, F_surface);
    };


    getControlMesh(V_control, E_control);
    generateBezierSurface(V_surface, F_surface);

    viewer.data().clear();
    viewer.data().set_mesh(V_surface, F_surface);
    viewer.data().set_points(V_control, Eigen::RowVector3d(1, 0, 0));
    viewer.data().set_edges(V_control, E_control, Eigen::RowVector3d(0, 0, 1));

    gizmo.visible = true;
    viewer.launch();
    return 0;
}

