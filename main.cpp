#include <iostream>
#include "lattice.h"
#include "code.h"
#include "decoder.h"
#include <chrono>

int main()
{
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

    // int l = 6;
    // std::string type = "rhombic";
    // double p = 0.1;
    // Code code = Code(l, type, p, p);

    // code.setError({259, 350, 478});
    // code.calculateSyndrome();
    // auto &syndrome = code.getSyndrome();
    // std::cout << "Syndrome = 1" << std::endl;
    // for (int i = 0; i < syndrome.size(); ++i)
    // {
    //     if (syndrome[i])
    //     {
    //         std::cout << i << std::endl;
    //     }
    // }
    // auto &flipBits = code.getFlipBits();
    // auto &error = code.getError();
    // for (int i = 0; i < 1; ++i)
    // {
    //     code.sweep("yz", true);
    //     code.calculateSyndrome();
    // }
    // std::cout << "Error" << std::endl;
    // for (auto e : error)
    // {
    //     std::cout << e << std::endl;
    // }
    // std::cout << "FlipBits = 1" << std::endl;
    // for (int i = 0; i < flipBits.size(); ++i)
    // {
    //     if (flipBits[i])
    //     {
    //         std::cout << i << std::endl;
    //     }
    // }
    // std::cout << "Syndrome = 1" << std::endl;
    // for (int i = 0; i < syndrome.size(); ++i)
    // {
    //     if (syndrome[i])
    //     {
    //         std::cout << i << std::endl;
    //     }
    // }

    // double p = 0;
    // Code code = Code(4, "rhombic", p, p);
    // code.generateDataError();
    // code.calculateSyndrome();
    // auto &syndrome = code.getSyndrome();
    // code.printUnsatisfiedStabilisers();
    // auto success = std::all_of(syndrome.begin(), syndrome.end(), [](int i) { return i == 0; });
    // std::cout << success << std::endl;
    
    int l = 8;
    int rounds = 16;
    double p = 0.05;
    double q = 0;
    std::string dir = "xyz";
    auto start = std::chrono::high_resolution_clock::now();
    auto succ = runToric(l, rounds, p, q, dir);
    auto finish = std::chrono::high_resolution_clock::now();
    std::cout << "Success: " << succ.at(0) << std::endl;
    std::cout << "Clean syndrome: " << succ.at(1) << std::endl;
    std::chrono::duration<double> elapsed = finish - start;
    std::cout << "Elapsed time: " << elapsed.count() << " s\n";

    // // Profiling
    // double p = 0.05;
    // double q = 0.05;
    // int l = 8;

    // // Build Code
    // auto start = std::chrono::high_resolution_clock::now();
    // Code code = Code(l, "rhombic", p, p);
    // auto finish = std::chrono::high_resolution_clock::now();
    // std::chrono::duration<double> elapsed = finish - start;
    // std::cout << "Code constructor elapsed time: " << elapsed.count() << " s\n";

    // // Generate data error
    // start = std::chrono::high_resolution_clock::now();
    // code.generateDataError();
    // finish = std::chrono::high_resolution_clock::now();
    // elapsed = finish - start;
    // std::cout << "Generate data error elapsed time: " << elapsed.count() << " s\n";

    // // Calculate syndrome
    // start = std::chrono::high_resolution_clock::now();
    // code.calculateSyndrome();
    // finish = std::chrono::high_resolution_clock::now();
    // elapsed = finish - start;
    // std::cout << "Calculate syndrome elapsed time: " << elapsed.count() << " s\n";

    // // Generate measurement error
    // start = std::chrono::high_resolution_clock::now();
    // code.generateMeasError();
    // finish = std::chrono::high_resolution_clock::now();
    // elapsed = finish - start;
    // std::cout << "Generate measurement error elapsed time: " << elapsed.count() << " s\n";

    // // Sweep
    // start = std::chrono::high_resolution_clock::now();
    // code.sweep("xyz", true);
    // finish = std::chrono::high_resolution_clock::now();
    // elapsed = finish - start;
    // std::cout << "Sweep elapsed time: " << elapsed.count() << " s\n";

    // // Check syndrome zero
    // auto &syndrome = code.getSyndrome();
    // start = std::chrono::high_resolution_clock::now();
    // bool zeros = std::all_of(syndrome.begin(), syndrome.end(), [](int i) { return i == 0; });
    // finish = std::chrono::high_resolution_clock::now();
    // elapsed = finish - start;
    // std::cout << "Check syndrome zero elapsed time: " << elapsed.count() << " s\n";

    // // Check correction
    // start = std::chrono::high_resolution_clock::now();
    // bool success = code.checkCorrection();
    // finish = std::chrono::high_resolution_clock::now();
    // elapsed = finish - start;
    // std::cout << "Check correction elapsed time: " << elapsed.count() << " s\n";

    return 0;
}