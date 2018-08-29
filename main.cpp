#include <iostream>
#include "lattice.h"

int main() 
{
    std::cout << "Hello, World!" << std::endl;

    int l = 6;
    std::string type = "rhombic";
    Lattice latticeRhombic = Lattice(l, type);
    latticeRhombic.createFaces();
    vvint faceToVertices = latticeRhombic.getFaceToVertices();
    vvint faceToEdges = latticeRhombic.getFaceToEdges();
    for (int i : faceToVertices[494])
    {
        std::cout << "Vertex Index: " << i << std::endl;
    }
    auto vertexToFaces = latticeRhombic.getVertexToFaces();
    for (auto const& face : vertexToFaces[259])
    {
        std::cout << "Face Index: " << face.faceIndex << std::endl;
    }

    return 0;
}