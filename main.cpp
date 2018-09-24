#include <iostream>
#include "lattice.h"
#include "code.h"
#include "decoder.h"
#include <chrono>

int main(int argc, char *argv[])
{
    if (argc < 5)
    {
        std::cout << "Less than five arguments" << std::endl;
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

    // std::cout << "l = " << l << std::endl
    //           << "rounds = " << rounds << std::endl
    //           << "p = " << p << std::endl
    //           << "q = " << q << std::endl
    //           << "sweep dir = " << sweepDir << std::endl;

    auto start = std::chrono::high_resolution_clock::now();
    auto succ = runToric(l, rounds, p, q, sweepDir);
    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish - start;

    std::cout << succ[0] << "," // Decoding succeeded
              << succ[1] << "," // Clean syndrome
              << elapsed.count()
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