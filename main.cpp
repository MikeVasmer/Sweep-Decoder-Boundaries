#include <iostream>
#include "rhombicToricLattice.h"
#include "code.h"
#include "decoder.h"
#include <chrono>
#include <string>

int main(int argc, char *argv[])
{
    if (argc < 9)
    {
        std::cout << "Fewer than six arguments" << std::endl;
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

    std::vector<bool> succ;

    // std::cout << "l = " << l << std::endl
    //           << "rounds = " << rounds << std::endl
    //           << "p = " << p << std::endl
    //           << "q = " << q << std::endl
    //           << "sweep dir = " << sweepDir << std::endl;

    auto start = std::chrono::high_resolution_clock::now();
    if (latticeType == "rhombic_toric")
    {
        succ = runToric(l, rounds, p, q, sweepDir, timeout);
    }
    else if (latticeType == "rhombic_boundaries")
    {
        succ = runBoundaries(l, rounds, p, q, sweepLimit, sweepSchedule, timeout);
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

    // Profiling

    // int l = 28;
    // int rounds = 64;
    // double p = 0.1;
    // double q = 0.1;
    // std::string dir = "xyz";
    // auto start = std::chrono::high_resolution_clock::now();
    // auto succ = runToric(l, rounds, p, q, dir);
    // auto finish = std::chrono::high_resolution_clock::now();
    // std::cout << "Success: " << succ.at(0) << std::endl;
    // std::cout << "Clean syndrome: " << succ.at(1) << std::endl;
    // std::chrono::duration<double> elapsed = finish - start;
    // std::cout << "Elapsed time: " << elapsed.count() << " s\n";

    return 0;
}