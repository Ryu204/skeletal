This repository demonstrates how to generate mesh from a transparent image.

For guidance on how to get mesh data *and* weight data (using Blender Scripting API), see [Integrate with Blender](#integrate-with-blender)

To build and run the example:

```sh
git submodule init
git submodule update
mkdir build
cmake -S . -B build
cmake --build build
build/skeletal --debug "img/circle.png"
```

The results are saved in `output` directory.

# Method

## Detect contour

The image is resized to a smaller version. A shader is used so that pixels with non zero alpha remain opaque. The contours are detected using marching squares algorithm.

However, to detect both interiors and exteriors, the resized image is divided into connected parts. Within each part, the algorithm returns a list of edges. The list is transformed into a list of edge loops. Finally, the loop with largest width will be exterior bound, others are interior bounds.

## Generate mesh

This repository makes use of project [TQMesh](https://github.com/FloSewn/TQMesh) for mesh generation.

# Integrate with Blender