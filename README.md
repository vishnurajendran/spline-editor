# spline-editor
This a basic b-spline editor application made for COMP 6381 (Digital Geometric Modelling) course.
The app is developed using C++ and Libigl (with ImGui and ImGuizmo dependencies)

#![spline-editor-icon](https://i.ibb.co/VcQCdq26/Screenshot-2025-03-01-200037.png)

## Features
- Realtime mesh generation using control lattice.
- Multiple Viewing Options (Surface only, Lattice only, Surface and Lattice).
- Variable Mesh Resolution.
- Add/Remove patches (currently on in Z-axis) to make complex shapes.
- Save/Load spline.
- Export to FBX.

## Installation
run the following commands to setup the project using cmake.
```bash
mkdir build-release
cd  build-release
cmake -DCMAKE_BUILD_TYPE=Release ..
```
Once the above cmake command finishes running and no errors show up, run
```bash
cmake --build . --target spline_editor -j 30
```
```-j 30``` lets cmake build in parallel, it is recommended to use this to make your compile times shorter.

Once compiled, the executable will be available in a new directory ```bin``` in the root directory of the project.
