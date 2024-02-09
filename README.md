This repository demonstrates how to generate mesh and weight vertices from a transparent image.

To build and run the example:

```sh
mkdir build
cmake -S . -B build
cmake --build build
build/skeletal --debug "img/circle.png"
```

The results are saved in `output` directory.

# Method

## Triangulation

### Detect contour

The image is resized to a smaller version. A shader is used so that pixels with non zero alpha remain opaque. The contours are detected using marching squares algorithm.

However, to detect both interiors and exteriors, the resized image is divided into connected parts. Within each part, the algorithm returns a list of edges. The list is transformed into a list of edge loops. Finally, the loop with largest width will be exterior bound, others are interior bounds.

### Generate mesh

This repository makes use of project [TQMesh](https://github.com/FloSewn/TQMesh) for mesh generation.

## Vertex weighting

To be continued...
