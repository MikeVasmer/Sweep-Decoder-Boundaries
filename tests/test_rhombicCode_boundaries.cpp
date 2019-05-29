#include "rhombicCode.h"
#include "rhombicLattice.h"
#include "gtest/gtest.h"
#include <string>
#include <iostream>
#include <chrono>
#include <cmath>

TEST(buildSyndromeIndices, syndrome_correct_size)
{
    std::vector<int> lList = {4, 6, 8, 10};
    double p = 0.1;
    for (int l : lList)
    {
        RhombicCode code = RhombicCode(l, p, p, true);
        auto &syndromeIndices = code.getSyndromeIndices();
        int expectedNumberOfEdges = 4 * (l - 2) * (l - 2) * (l - 1);
        EXPECT_EQ(syndromeIndices.size(), expectedNumberOfEdges);
    }
}

TEST(buildSyndromeIndices, syndrome_correct_edges)
{
    std::vector<int> lList = {4, 6, 8, 10};
    double p = 0.1;
    for (int l : lList)
    {
        RhombicCode code = RhombicCode(l, p, p, true);
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

TEST(checkCorrection, handles_stabiliser_errors)
{
    int l = 4;
    double p = 0.1;
    RhombicCode code = RhombicCode(l, p, p, true);
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

TEST(checkCorrection, handles_logical_X_errors)
{
    int l = 4;
    double p = 0.1;
    RhombicCode code = RhombicCode(l, p, p, true);
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

TEST(buildLogical, logical_correct_weight)
{
    vint ls = {4, 6, 8, 10};
    double p = 0.1;
    for (auto l : ls)
    {
        RhombicCode code = RhombicCode(l, p, p, true);
        auto logicals = code.getLogicals();
        int expectedWeight = l - 1;
        EXPECT_EQ(logicals[0].size(), expectedWeight);
    }
}

TEST(calculateSyndrome, no_invalid_syndromes_data_errors)
{
    vint ls = {4, 6, 8};
    double p = 0.1;
    for (auto l : ls)
    {
        RhombicCode code = RhombicCode(l, p, p, true);
        auto &syndromeIndices = code.getSyndromeIndices();
        auto &syndrome = code.getSyndrome();
        for (int j = 0; j < 5; ++j)
        {
            code.generateDataError();
            code.calculateSyndrome();
            for (int i = 0; i < syndrome.size(); ++i)
            {
                if (syndrome[i] == 1)
                {
                    auto it = syndromeIndices.find(i);
                    EXPECT_FALSE(it == syndromeIndices.end());
                }
            }
        }
    }
}

TEST(calculateSyndrome, no_invalid_syndromes_meas_errors)
{
    vint ls = {4, 6, 8};
    double p = 0.1;
    for (auto l : ls)
    {
        RhombicCode code = RhombicCode(l, p, p, true);
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

TEST(calculateSyndrome, no_invalid_syndromes_both_errors)
{
    vint ls = {4, 6, 8};
    double p = 0.1;
    for (auto l : ls)
    {
        RhombicCode code = RhombicCode(l, p, p, true);
        auto &syndromeIndices = code.getSyndromeIndices();
        auto &syndrome = code.getSyndrome();
        for (int j = 0; j < 5; ++j)
        {
            code.generateDataError();
            code.calculateSyndrome();
            code.generateMeasError();
            for (int i = 0; i < syndrome.size(); ++i)
            {
                if (syndrome[i] == 1)
                {
                    auto it = syndromeIndices.find(i);
                    EXPECT_FALSE(it == syndromeIndices.end());
                }
            }
        }
    }
}

TEST(calculateSyndrome, no_syndrome_stabilizer_errors)
{
    vint ls = {4};
    double p = 0.1;
    for (auto l : ls)
    {
        RhombicCode code = RhombicCode(l, p, p, true);
        std::set<int> error = {16, 36, 40};
        // Lattice &lattice = code.getLattice();
        // auto vertexToFaces = lattice.getVertexToFaces();
        // cartesian4 coordinate = {1, 1, 3, 0};
        // int vertex = lattice.coordinateToIndex(coordinate);
        // for (auto face : vertexToFaces[vertex])
        // {
        //     std::cout << face.faceIndex << std::endl;
        // }
        code.setError(error);
        code.calculateSyndrome();
        // code.printUnsatisfiedStabilisers();
        auto &syndrome = code.getSyndrome();
        for (auto value : syndrome)
        {
            EXPECT_EQ(value, 0);
        }
        // EXPECT
    }
}

TEST(sweep, runs_without_errors)
{
    vint ls = {4, 6};
    for (auto l : ls)
    {
        double p = 0.1;
                vstr sweepDirections = {"xyz", "xz", "-xy", "yz", "xy", "-yz", "-xyz", "-xz"};
        for (auto &sweepDirection
            : sweepDirections) {
            RhombicCode code = RhombicCode(l, p, p, true);
            for (int i = 0; i < l; ++i)
            {
                code.generateDataError();
                code.calculateSyndrome();
                code.generateMeasError();
                EXPECT_NO_THROW(code.sweep(sweepDirection, true));
            }
        }
    }
}

TEST(sweep, corrects_single_qubit_errors)
{
    vint ls = {4, 6};
    for (auto l : ls)
    {
        double p = 0.1;
        vstr sweepDirections = {"xyz", "xy", "yz", "xz", "-xyz", "-xy", "-yz", "-xz"};
        // vstr sweepDirections = {"-xyz"};
        int numberOfFaces = 3 * pow(l - 1, 3) - 4 * pow(l - 1, 2) + 2 * (l - 1);
        RhombicCode code = RhombicCode(l, p, p, true);
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

TEST(sweep, corrects_two_qubit_errors)
{
    vint ls = {4};
    double p = 0.1;
    vstr sweepDirections = {"xyz", "xy", "yz", "xz", "-xyz", "-xy", "-yz", "-xz"};
    for (auto const l : ls)
    {
        RhombicCode code = RhombicCode(l, p, p, true);
        auto &syndrome = code.getSyndrome();
        int numberOfFaces = 3 * pow(l - 1, 3) - 4 * pow(l - 1, 2) + 2 * (l - 1);
        auto &lattice = code.getLattice();
        auto &faceToVertices = lattice.getFaceToVertices();
        int repeats = 2;
        int sweepsPerDirection = l;
        for (int i = 0; i < numberOfFaces; ++i)
        {
            auto &f2vi = faceToVertices[i];
            for (int j = i + 1; j < numberOfFaces; ++j)
            {
                code.setError({i, j});
                code.calculateSyndrome();
                // std::cout << "Error = " << i << "," << j << std::endl;
                // std::cout << "Unsatisifed stabilizers = " << std::endl;
                // code.printUnsatisfiedStabilisers();
                for (int r = 0; r < repeats; ++r)
                {
                    for (auto &sweepDirection : sweepDirections)
                    {
                        for (int s = 0; s < sweepsPerDirection; ++s)
                        {
                            code.sweep(sweepDirection, true);
                            code.calculateSyndrome();
                        }
                    }
                }
                for (int k = 0; k < syndrome.size(); ++k)
                {
                    EXPECT_EQ(syndrome[k], 0);
                    // if (syndrome[k] == 1)
                    // {
                    //     std::cout << "Syndrome index = " << k << std::endl;
                    //     std::cout << "i = " << i << ", " << "error vertices = " << lattice.indexToCoordinate(f2vi[0]) << " " << lattice.indexToCoordinate(f2vi[1]) << " " << lattice.indexToCoordinate(f2vi[2]) << " " << lattice.indexToCoordinate(f2vi[3]) << std::endl;
                    //     auto &f2vj = faceToVertices[j];
                    //     std::cout << "j = " << j << ", " << "error vertices = " << lattice.indexToCoordinate(f2vj[0]) << " " << lattice.indexToCoordinate(f2vj[1]) << " " << lattice.indexToCoordinate(f2vj[2]) << " " << lattice.indexToCoordinate(f2vj[3]) << std::endl;
                    //     std::cerr << "Error:" << std::endl;
                    //     code.printError();
                    //     std::cerr << "Unsatisfied stabilizers:" << std::endl;
                    //     code.printUnsatisfiedStabilisers();
                    // }
                }
            }
        }
    }
}

TEST(sweep, all_directions_sweep_correctly)
{
    int l = 4;
    double p = 0.1;
    RhombicCode code = RhombicCode(l, p, p, true);
    std::set<int> error = {13, 18};
    code.setError(error);
    code.calculateSyndrome();
    auto &syndrome = code.getSyndrome();
    vint expectedSyndrome = {147, 258, 564, 597};
    for (int j = 0; j < syndrome.size(); ++j)
    {
        if (std::find(expectedSyndrome.begin(), expectedSyndrome.end(), j) != expectedSyndrome.end())
        {
            EXPECT_EQ(syndrome[j], 1);
        }
        else
        {
            EXPECT_EQ(syndrome[j], 0);
        }
        
    }

    vstr sweepDirections = {"xyz", "xy", "yz", "xz", "-xyz", "-xy", "-yz", "-xz"};
    vvint expectedSyndromes = {expectedSyndrome, expectedSyndrome, expectedSyndrome, {147, 597}, {147, 597}, {147, 597}, {}, {}};

    for (int i = 0; i < 8; ++i)
    {
        code.sweep(sweepDirections[i], true);
        code.calculateSyndrome();
        for (int j = 0; j < syndrome.size(); ++j)
        {
            if (std::find(expectedSyndromes[i].begin(), expectedSyndromes[i].end(), j) != expectedSyndromes[i].end())
            {
                EXPECT_EQ(syndrome[j], 1);
            }
            else
            {
                EXPECT_EQ(syndrome[j], 0);
            }
        }
    }

    error = {12, 36};
    code.setError(error);
    code.calculateSyndrome();
    // code.printUnsatisfiedStabilisers();
    for (int i = 0; i < 8; ++i)
    {
        code.sweep(sweepDirections[i], true);
        code.calculateSyndrome();
    }
    for (auto &s : syndrome)
    {
        EXPECT_EQ(s, 0);
    }
}

TEST(buildSweepIndices, correct_indices_L4)
{
    int l = 4;
    double p = 0.1;
    RhombicCode code = RhombicCode(l, p, p, true);
    vint &sweepIndices = code.getSweepIndices();
    auto &lattice = code.getLattice();
    vint expectedIndices = {21, 23, 24, 26, 36, 38, 41, 43, 53, 55, 56, 58, 80, 81, 82, 84, 85, 86, 88, 89, 90, 96, 97, 98, 100, 101, 102, 104, 105, 106};
    for (int i = 0; i < sweepIndices.size(); ++i)
    {
        EXPECT_EQ(sweepIndices[i], expectedIndices[i]);
    }
}

TEST(buildSweepIndices, correct_number_of_indices)
{
    vint ls = {4, 6, 8, 10};
    double p = 0.1;
    for (auto const l : ls)
    {
        RhombicCode code = RhombicCode(l, p, p, true);
        vint &sweepIndices = code.getSweepIndices();
        int expectedSize = ((l - 1) * (l - 1) * (l - 2) + ((l * (l - 2) * (l - 1)) / 2));
        EXPECT_EQ(sweepIndices.size(), expectedSize);
    }
}