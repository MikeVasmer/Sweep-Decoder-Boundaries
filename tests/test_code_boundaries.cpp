#include "code.h"
#include "rhombicLattice.h"
#include "gtest/gtest.h"
#include <string>
#include <iostream>
#include <chrono>

TEST(buildSyndromeIndices, syndromeCorrectSize)
{
    std::vector<int> lList = {4, 6, 8, 10};
    double p = 0.1;
    for (int l : lList)
    {
        Code code = Code(l, "rhombic boundaries", p, p);
        auto &syndromeIndices = code.getSyndromeIndices();
        int expectedNumberOfEdges = 4 * (l - 2) * (l - 2) * (l - 1);
        EXPECT_EQ(syndromeIndices.size(), expectedNumberOfEdges);
    }
}

TEST(buildSyndromeIndices, syndromeCorrectEdges)
{
    std::vector<int> lList = {4, 6, 8, 10};
    double p = 0.1;
    for (int l : lList)
    {
        Code code = Code(l, "rhombic boundaries", p, p);
        auto &syndromeIndices = code.getSyndromeIndices();
        std::vector<cartesian4> coordinateList = {{0, 2, 1, 0}, {1, 1, 1, 0}, {l - 1, 1, 1, 0}, {0, 1, l - 2, 0}, {2, 1, l - 2, 0}, {l - 1, 2, l - 2, 0}, {0, 2, l - 1}, {2, 2, l - 1}, {l - 1, 1, l - 1}};
        std::vector<vstr> expectedEdgeDirections = {{"xyz", "xz"}, {"xyz", "xz", "yz", "xy"}, {"xy", "yz"}, {"xyz", "xz", "xy", "yz"}, {"xyz", "xz", "xy", "yz", "xyz", "xz", "xy", "yz"}, {"xyz", "xz", "xy", "yz"}, {"xy", "yz"}, {"xyz", "xz", "xy", "yz"}, {"xyz", "xz"}};
        std::vector<vint> expectedEdgeSigns = {{1, 1}, {1, 1, 1, -1}, {-1, 1}, {1, 1, 1, -1}, {1, 1, 1, 1, -1, -1, -1, -1}, {-1, -1, -1, 1}, {1, -1}, {-1, -1, 1, -1}, {-1, -1}};
        auto &lattice = code.getLattice();
        for (int i = 0; i < coordinateList.size(); ++i)
        {
            // std::cout << coordinateList[i] << std::endl;
            int vertexIndex = lattice.coordinateToIndex(coordinateList[i]);
            for (int j = 0; j < expectedEdgeDirections[i].size(); ++j)
            {
                auto it = syndromeIndices.find(lattice.edgeIndex(vertexIndex, expectedEdgeDirections[i][j], expectedEdgeSigns[i][j]));
                EXPECT_FALSE(it == syndromeIndices.end());
            }
        }
    }
}

TEST(checkCorrection, handlesStabiliserErrorsBoundaries)
{
    int l = 4;
    double p = 0.1;
    Code code = Code(l, "rhombic boundaries", p, p);
    std::set<int> error = {5, 8, 9, 11, 13, 14, 26, 28};
    code.setError(error);
    code.calculateSyndrome();
    auto &syndrome = code.getSyndrome();
    for (int i = 0; i < syndrome.size(); ++i)
    {
        EXPECT_EQ(syndrome[i], 0);
    }
    // code.printUnsatisfiedStabilisers();
    EXPECT_TRUE(code.checkCorrection());
}

TEST(checkCorrection, handlesLogicalXErrorsBoundaries)
{
    int l = 4;
    double p = 0.1;
    Code code = Code(l, "rhombic boundaries", p, p);
    std::set<int> error = {0, 3, 5, 8, 9, 18, 30, 32, 36, 39, 41, 44, 45};
    code.setError(error);
    code.calculateSyndrome();
    auto &syndrome = code.getSyndrome();
    for (int i = 0; i < syndrome.size(); ++i)
    {
        EXPECT_EQ(syndrome[i], 0);
    }
    // code.printUnsatisfiedStabilisers();
    EXPECT_FALSE(code.checkCorrection());
}

TEST(buildLogical, logicalCorrectWeight)
{
    vint ls = {4, 6, 8, 10};
    double p = 0.1;
    for (auto l : ls)
    {
        Code code = Code(l, "rhombic boundaries", p, p);
        auto logicals = code.getLogicals();
        int expectedWeight = l - 1;
        EXPECT_EQ(logicals[0].size(), expectedWeight);
    }
}

TEST(calculateSyndrome, noInvalidSyndromeIndicesDataErrorsRhombicBoundaries)
{
    vint ls = {4, 6, 8};
    double p = 0.1;
    for (auto l : ls)
    {
        Code code = Code(l, "rhombic boundaries", p, p);
        auto &syndromeIncdices = code.getSyndromeIndices();
        auto &syndrome = code.getSyndrome();
        for (int j = 0; j < 5; ++j)
        {
            code.generateDataError();
            code.calculateSyndrome();
            for (int i = 0; i < syndrome.size(); ++i)
            {
                if (syndrome[i] == 1)
                {
                    auto it = syndromeIncdices.find(i);
                    EXPECT_FALSE(it == syndromeIncdices.end());
                }
            }
        }
    }
}

TEST(calculateSyndrome, noInvalidSyndromeIndicesMeasErrorsRhombicBoundaries)
{
    vint ls = {4, 6, 8};
    double p = 0.1;
    for (auto l : ls)
    {
        Code code = Code(l, "rhombic boundaries", p, p);
        auto &syndromeIncdices = code.getSyndromeIndices();
        auto &syndrome = code.getSyndrome();
        for (int j = 0; j < 5; ++j)
        {
            code.generateMeasError();
            for (int i = 0; i < syndrome.size(); ++i)
            {
                if (syndrome[i] == 1)
                {
                    auto it = syndromeIncdices.find(i);
                    EXPECT_FALSE(it == syndromeIncdices.end());
                }
            }
        }
    }
}

TEST(calculateSyndrome, noInvalidSyndromeIndicesBothErrorsRhombicBoundaries)
{
    vint ls = {4, 6, 8};
    double p = 0.1;
    for (auto l : ls)
    {
        Code code = Code(l, "rhombic boundaries", p, p);
        auto &syndromeIndices = code.getSyndromeIndices();
        auto &syndrome = code.getSyndrome();
        // auto &boundarySyndromeIndices = code.getBoundarySyndromeIndices();
        for (int j = 0; j < 5; ++j)
        {
            code.generateDataError();
            code.calculateSyndrome();
            code.generateMeasError();
            for (int i = 0; i < syndrome.size(); ++i)
            {
                if (syndrome[i] == 1)
                {
                    // if (std::find(boundarySyndromeIndices.begin(), boundarySyndromeIndices.end(), i) == boundarySyndromeIndices.end())
                    // {
                        auto it = syndromeIndices.find(i);
                        EXPECT_FALSE(it == syndromeIndices.end());
                    // }
                }
            }
        }
    }
}


TEST(sweep, runsForSingleQubitErrorsRhombicBoundariesL4L6)
{
    vint ls = {4, 6};
    for (auto l : ls)
    {
        double p = 0.1;
        vstr sweepDirections = {"xyz", "xz", "-xy", "yz", "xy", "-yz", "-xyz", "-xz"};
        int numberOfFaces = 3 * pow(l - 1, 3) - 4 * pow(l - 1, 2) + 2 * (l - 1);
        for (auto &sweepDirection
            : sweepDirections) {
            Code code = Code(l, "rhombic boundaries", p, p);
            for (int i = 0; i < numberOfFaces; ++i)
            {
                code.setError({i});
                code.calculateSyndrome();
                EXPECT_NO_THROW(code.sweep(sweepDirection, true));
            }
        }
    }
}


TEST(sweep, runsWithoutErrorsNoBoundaries)
{
    vint ls = {4, 8};
    for (auto l : ls)
    {
        double p = 0.1;
                vstr sweepDirections = {"xyz", "xz", "-xy", "yz", "xy", "-yz", "-xyz", "-xz"};
        for (auto &sweepDirection
            : sweepDirections) {
            Code code = Code(l, "rhombic boundaries", p, p);
            for (int i = 0; i < 5; ++i)
            {
                code.generateDataError();
                code.calculateSyndrome();
                code.generateMeasError();
                EXPECT_NO_THROW(code.sweep(sweepDirection, true));
            }
        }
    }
}

TEST(sweep, correctsSingleQubitErrors)
{
    vint ls = {4, 6, 8, 10};
    for (auto l : ls)
    {
        double p = 0.1;
        vstr sweepDirections = {"xyz", "xy", "yz", "xz", "-xyz", "-xy", "-yz", "-xz"};
        // vstr sweepDirections = {"-xyz"};
        int numberOfFaces = 3 * pow(l - 1, 3) - 4 * pow(l - 1, 2) + 2 * (l - 1);
        Code code = Code(l, "rhombic boundaries", p, p);
        auto &syndrome = code.getSyndrome();
        auto &lattice = code.getLattice();
        auto &faceToVertices = lattice.getFaceToVertices();
        // auto &faceToEdges = lattice.getFaceToEdges();
        for (int i = 0; i < numberOfFaces; ++i)
        // for (int i = 33; i < 34; ++i)
        {
            auto &f2v = faceToVertices[i];
            // std::cout << "i = " << i << ", " << "error vertices = " << lattice.indexToCoordinate(f2v[0]) << " " << lattice.indexToCoordinate(f2v[1]) << " " << lattice.indexToCoordinate(f2v[2]) << " " << lattice.indexToCoordinate(f2v[3]) << std::endl;
            code.setError({i});
            code.calculateSyndrome();
            // std::cout << "Initial syndrome: " << std::endl;
            // code.printUnsatisfiedStabilisers();
            for (auto &sweepDirection : sweepDirections) 
            {
                for (int j = 0; j < 1; ++j)
                {
                    code.sweep(sweepDirection, true);
                    code.calculateSyndrome();
                    // std::cout << "Final syndrome: " << std::endl;
                    // code.printUnsatisfiedStabilisers();
                }
            }
            // std::cout << "Edges of face: " << i << std::endl;
            // for (const auto edge : faceToEdges[i])
            // {
            //     std::cout << edge << "," << std::endl;
            // }
            for (int k = 0; k < syndrome.size(); ++k)
            {
                EXPECT_EQ(syndrome[k], 0);
            }
        }
    }
}

TEST(sweep, correctsTwoQubitErrors)
{
    vint ls = {4};
    double p = 0.1;
    // vstr sweepDirections = {"xyz", "xy", "yz", "xz", "-xyz", "-xy", "-yz", "-xz"};
    vstr sweepDirections = {"xyz", "xy", "yz", "xz", "-xyz", "-xy", "-yz", "-xz", "xyz", "xy", "yz", "xz", "-xyz", "-xy", "-yz", "-xz"};
    int sweepCycle = 2;
    for (auto const l : ls)
    {
        Code code = Code(l, "rhombic boundaries", p, p);
        auto &syndrome = code.getSyndrome();
        int numberOfFaces = 3 * pow(l - 1, 3) - 4 * pow(l - 1, 2) + 2 * (l - 1);
        auto &lattice = code.getLattice();
        auto &faceToVertices = lattice.getFaceToVertices();
        for (int i = 0; i < numberOfFaces; ++i)
        {
            auto &f2vi = faceToVertices[i];
            for (int j = i + 1; j < numberOfFaces; ++j)
            { 
                code.setError({i, j});
                code.calculateSyndrome();
                for (auto &sweepDirection : sweepDirections) 
                {
                    for (int i = 0; i < sweepCycle; ++i)
                    {
                        code.sweep(sweepDirection, true);
                        code.calculateSyndrome();
                    }
                }
                for (int k = 0; k < syndrome.size(); ++k)
                {
                    EXPECT_EQ(syndrome[k], 0);
                    if (syndrome[k] == 1)
                    {
                        std::cout << "Syndrome index = " << k << std::endl;
                        std::cout << "i = " << i << ", " << "error vertices = " << lattice.indexToCoordinate(f2vi[0]) << " " << lattice.indexToCoordinate(f2vi[1]) << " " << lattice.indexToCoordinate(f2vi[2]) << " " << lattice.indexToCoordinate(f2vi[3]) << std::endl;
                        auto &f2vj = faceToVertices[j];
                        std::cout << "j = " << j << ", " << "error vertices = " << lattice.indexToCoordinate(f2vj[0]) << " " << lattice.indexToCoordinate(f2vj[1]) << " " << lattice.indexToCoordinate(f2vj[2]) << " " << lattice.indexToCoordinate(f2vj[3]) << std::endl;
                    }
                }
            }
        }
    }
}

// TEST(comparison, initTime)
// {
//     vint ls = {4, 6, 8};
//     double p = 0.1;
//     for (auto const l : ls)
//     {
//         auto start = std::chrono::high_resolution_clock::now();
//         Code codeToric = Code(l, "rhombic toric", p, p);
//         auto finish = std::chrono::high_resolution_clock::now();
//         std::chrono::duration<double> elapsed = finish - start;
//         std::cout << "Init time, toric, l=" << l << ": " << elapsed.count() << std::endl;
//         start = std::chrono::high_resolution_clock::now();
//         Code codeBoundaries = Code(l, "rhombic boundaries", p, p);
//         finish = std::chrono::high_resolution_clock::now();
//         elapsed = finish - start;
//         std::cout << "Init time, boundaries, l=" << l << ": " << elapsed.count() << std::endl;
//     }
// }

// TEST(comparison, sweep)
// {
//     vint ls = {4, 6, 8};
//     double p = 0.1;
//     for (auto const l : ls)
//     {
//         Code codeToric = Code(l, "rhombic toric", p, p);
//         auto start = std::chrono::high_resolution_clock::now();
//         for (int i = 0; i < 100; ++i)
//         {
//             codeToric.generateDataError();
//             codeToric.sweep("xyz", true);
//         }
//         auto finish = std::chrono::high_resolution_clock::now();
//         std::chrono::duration<double> elapsed = finish - start;
//         std::cout << "Sweep time, toric, l=" << l << ": " << elapsed.count() << std::endl;
//         Code codeBoundaries = Code(l, "rhombic boundaries", p, p);
//         start = std::chrono::high_resolution_clock::now();
//         for (int i = 0; i < 100; ++i)
//         {
//             codeBoundaries.generateDataError();
//             codeBoundaries.sweep("xyz", true);
//         }
//         finish = std::chrono::high_resolution_clock::now();
//         elapsed = finish - start;
//         std::cout << "Sweep time, boundaries, l=" << l << ": " << elapsed.count() << std::endl;
//     }
// }

TEST(buildSweepIndices, correctIndicesL4)
{
    int l = 4;
    double p = 0.1;
    Code code = Code(l, "rhombic boundaries", p, p);
    vint &sweepIndices = code.getSweepIndices();
    auto &lattice = code.getLattice();
    vint expectedIndices = {21, 23, 24, 26, 36, 38, 41, 43, 53, 55, 56, 58, 80, 81, 82, 84, 85, 86, 88, 89, 90, 96, 97, 98, 100, 101, 102, 104, 105, 106};
    for (int i = 0; i < sweepIndices.size(); ++i)
    {
        EXPECT_EQ(sweepIndices[i], expectedIndices[i]);
    }
}

TEST(buildSweepIndices, correctNumberOfIndices)
{
    vint ls = {4, 6, 8, 10};
    double p = 0.1;
    for (auto const l : ls)
    {
        Code code = Code(l, "rhombic boundaries", p, p);
        vint &sweepIndices = code.getSweepIndices();
        int expectedSize = ((l - 1) * (l - 1) * (l - 2) + ((l * (l - 2) * (l - 1)) / 2));
        EXPECT_EQ(sweepIndices.size(), expectedSize);
    }
}