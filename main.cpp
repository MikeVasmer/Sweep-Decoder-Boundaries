#include <iostream>
#include "lattice.h"
#include "code.h"
#include <chrono>

int main()
{
    std::cout << "Hello, World!" << std::endl;

    // int l = 6;
    // std::string type = "rhombic";
    // Lattice latticeRhombic = Lattice(l, type);
    // latticeRhombic.createFaces();
    // vvint faceToVertices = latticeRhombic.getFaceToVertices();
    // vvint faceToEdges = latticeRhombic.getFaceToEdges();
    // for (int i : faceToVertices[494])
    // {
    //     std::cout << "Vertex Index: " << i << std::endl;
    // }
    // auto vertexToFaces = latticeRhombic.getVertexToFaces();
    // for (auto const& face : vertexToFaces[259])
    // {
    //     std::cout << "Face Index: " << face.faceIndex << std::endl;
    // }

    // Lattice lat = Lattice(4, "rhombic");
    // lat.createFaces();
    // auto vertexToFaces = lat.getVertexToFaces();
    // for (auto face : vertexToFaces[8])
    // {
    //     std::cout << face.faceIndex << std::endl;
    // }

    // int latticeLength = 4;
    // std::string latticeType = "rhombic";
    // double p = 0.1;
    // double q = p;
    // Code code = Code(latticeLength, latticeType, p, q);
    // vint error = {0};
    // code.setError(error);
    // code.calculateSyndrome();
    // vint syndrome = code.getSyndrome();
    // int i = 0;
    // for (int value : syndrome)
    // {
    //     if (value == 1)
    //     {
    //         std::cout << i << std::endl;
    //     }
    //     ++i;
    // }
    // Lattice lat = code.getLattice();
    // vvint faceToVertices = lat.getFaceToVertices();
    // for (int v : faceToVertices[0])
    // {
    //     std::cout << "Vertex: " << v << " is in face." << std::endl;
    // }
    // vint vertices = {0, 5, 64, 67};
    // for (int v : vertices)
    // {
    //     std::cout << "Vertex: " << v << std::endl;
    //     code.checkExtremalVertex(v, "-yz");
    // }

    // auto start = std::chrono::high_resolution_clock::now();
    // Code code = Code(40, "rhombic", 0.1, 0.1);
    // auto finish = std::chrono::high_resolution_clock::now();
    // std::chrono::duration<double> elapsed = finish - start;
    // std::cout << "Elapsed time: " << elapsed.count() << " s\n";

    // std::cout << (1 ^ 1) << std::endl;
    // std::cout << (1 ^ 0) << std::endl;
    // std::cout << (1 ^ 0 ^ 1) << std::endl;
    // std::cout << (1 ^ 0 ^ 0) << std::endl;

    return 0;
}