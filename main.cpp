#include <iostream>
#include "rhombicToricLattice.h"
#include "code.h"
#include "decoder.h"
#include <chrono>
#include <string>
#include <sstream>

int main(int argc, char *argv[])
{
    if (argc < 10)
    {
        std::cout << "Fewer than ten arguments" << std::endl;
        for (int i = 0; i < argc; ++i)
        {
            std::cout << "Argument " << i << " = " << argv[i] << std::endl;
        }
        return -1;
    }

    int l = std::atoi(argv[1]);
    double p = std::atof(argv[2]);
    double q = std::atof(argv[3]);
    std::string sweepDir(argv[4]);
    int rounds = std::atoi(argv[5]);
    std::string latticeType(argv[6]);
    int sweepLimit = std::atoi(argv[7]);
    std::string sweepSchedule(argv[8]);
    int timeout = std::atoi(argv[9]);
    bool greedy;
    std::stringstream ss(argv[10]);
    if (!(ss >> std::boolalpha >> greedy))
    {
        std::cerr << "Incorrect argument provided (boolean)." << std::endl;
        return 1;
    }
    std::vector<bool> succ;

    auto start = std::chrono::high_resolution_clock::now();
    // if (latticeType == "rhombic_toric")
    // {
    //     succ = runToric(l, rounds, p, q, sweepDir, timeout, greedy);
    // }
    // else if (latticeType == "rhombic_boundaries" || latticeType == "cubic_boundaries")
    if (latticeType == "rhombic_boundaries" || latticeType == "cubic_boundaries" || latticeType == "rhombic_toric")
    {
        succ = runBoundaries(l, rounds, p, q, sweepLimit, sweepSchedule, timeout, latticeType, greedy);
    }
    else
    {
        throw std::invalid_argument("Invalid lattice type.");
    }
    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish - start;

    std::cout << succ[0] << ", " // Decoding succeeded
              << succ[1] << ", " // Clean syndrome
              << elapsed.count() // "s" <<
              << std::endl;

    return 0;
}