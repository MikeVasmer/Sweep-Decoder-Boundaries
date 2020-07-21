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
        RhombicCode code(l, p, p, true, 1);
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
        RhombicCode code(l, p, p, true, 1);
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
    RhombicCode code(l, p, p, true, 1);
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
    RhombicCode code(l, p, p, true, 1);
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
        RhombicCode code(l, p, p, true, 1);
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
        RhombicCode code(l, p, p, true, 1);
        auto &syndromeIndices = code.getSyndromeIndices();
        auto &syndrome = code.getSyndrome();
        for (int j = 0; j < 5; ++j)
        {
            code.generateDataError(false);
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
        RhombicCode code(l, p, p, true, 1);
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
        RhombicCode code(l, p, p, true, 1);
        auto &syndromeIndices = code.getSyndromeIndices();
        auto &syndrome = code.getSyndrome();
        for (int j = 0; j < 5; ++j)
        {
            code.generateDataError(false);
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
        RhombicCode code(l, p, p, true, 1);
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
        for (auto &sweepDirection : sweepDirections)
        {
            RhombicCode code(l, p, p, true, 1);
            for (int i = 0; i < l; ++i)
            {
                code.generateDataError(false);
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
        // vstr sweepDirections = {"xyz", "-xyz", "xy", "-xy", "yz", "-yz", "xz", "-xz"};
        int numberOfFaces = 3 * pow(l - 1, 3) - 4 * pow(l - 1, 2) + 2 * (l - 1);
        RhombicCode code(l, p, p, true, 1);
        auto &syndrome = code.getSyndrome();
        auto &lattice = code.getLattice();
        auto &faceToVertices = lattice.getFaceToVertices();
        // auto &faceToEdges = lattice.getFaceToEdges();
        int repeats = 1;
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
                for (int r = 0; r < repeats; ++r)
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
    // vstr sweepDirections = {"xyz", "xy", "-xz", "yz", "xz", "-yz", "-xyz", "-xy"};
    for (auto const l : ls)
    {
        RhombicCode code(l, p, p, true, 1);
        auto &syndrome = code.getSyndrome();
        int numberOfFaces = 3 * pow(l - 1, 3) - 4 * pow(l - 1, 2) + 2 * (l - 1);
        auto &lattice = code.getLattice();
        auto &faceToVertices = lattice.getFaceToVertices();
        int repeats = 1;
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
                    if (syndrome[k] == 1)
                    {
                        std::cout << "Syndrome index = " << k << std::endl;
                        std::cout << "i = " << i << ", "
                                  << "error vertices = " << lattice.indexToCoordinate(f2vi[0]) << " " << lattice.indexToCoordinate(f2vi[1]) << " " << lattice.indexToCoordinate(f2vi[2]) << " " << lattice.indexToCoordinate(f2vi[3]) << std::endl;
                        auto &f2vj = faceToVertices[j];
                        std::cout << "j = " << j << ", "
                                  << "error vertices = " << lattice.indexToCoordinate(f2vj[0]) << " " << lattice.indexToCoordinate(f2vj[1]) << " " << lattice.indexToCoordinate(f2vj[2]) << " " << lattice.indexToCoordinate(f2vj[3]) << std::endl;
                        std::cerr << "Error:" << std::endl;
                        code.printError();
                        std::cerr << "Unsatisfied stabilizers:" << std::endl;
                        code.printUnsatisfiedStabilisers();
                    }
                }
            }
        }
    }
}

TEST(sweepBoundary, one_edge_faces_swept_correctly)
{
    RhombicCode code(4, 0.1, 0.1, true, 1);
    auto &syndrome = code.getSyndrome();
    code.printError();
    vstr allDirections = {"xyz", "xy", "yz", "xz", "-xyz", "-xy", "-yz", "-xz"};

    vstr directions = {"-xy", "-xyz", "-xz", "-yz"};
    for (auto &dir : allDirections)
    {
        code.setError({1});
        code.calculateSyndrome();
        code.sweep(dir, true);
        code.calculateSyndrome();
        int sum = 0;
        for (int j = 0; j < syndrome.size(); ++j)
        {
            sum = (sum + syndrome[j]);
        }
        auto it = std::find(directions.begin(), directions.end(), dir);
        if (it == directions.end())
        {
            EXPECT_TRUE(sum > 0);
        }
        else
        {
            EXPECT_EQ(sum, 0);
        }
    }

    directions = {"xy", "xyz", "-xz", "-yz"};
    for (auto &dir : allDirections)
    {
        code.setError({8});
        code.calculateSyndrome();
        code.sweep(dir, true);
        code.calculateSyndrome();
        int sum = 0;
        for (int j = 0; j < syndrome.size(); ++j)
        {
            sum = (sum + syndrome[j]);
        }
        auto it = std::find(directions.begin(), directions.end(), dir);
        if (it == directions.end())
        {
            EXPECT_TRUE(sum > 0);
        }
        else
        {
            EXPECT_EQ(sum, 0);
        }
    }

    directions = {"xyz", "xy", "yz", "xz"};
    for (auto &dir : allDirections)
    {
        code.setError({44});
        code.calculateSyndrome();
        code.sweep(dir, true);
        code.calculateSyndrome();
        int sum = 0;
        for (int j = 0; j < syndrome.size(); ++j)
        {
            sum = (sum + syndrome[j]);
        }
        auto it = std::find(directions.begin(), directions.end(), dir);
        if (it == directions.end())
        {
            EXPECT_TRUE(sum > 0);
        }
        else
        {
            EXPECT_EQ(sum, 0);
        }
    }

    directions = {"yz", "-xz", "xy", "-xyz", "-xy", "xyz"};
    for (auto &dir : allDirections)
    {
        code.setError({33});
        code.calculateSyndrome();
        code.sweep(dir, true);
        code.calculateSyndrome();
        int sum = 0;
        for (int j = 0; j < syndrome.size(); ++j)
        {
            sum = (sum + syndrome[j]);
        }
        auto it = std::find(directions.begin(), directions.end(), dir);
        if (it == directions.end())
        {
            EXPECT_TRUE(sum > 0);
        }
        else
        {
            EXPECT_EQ(sum, 0);
        }
    }

    // code.setError({30, 31, 32, 33, 34, 35, 36, 37, 38, 39});
    // code.printError();
}

TEST(sweepBoundary, removes_specific_error)
{
    RhombicCode code(6, 0.1, 0.1, true, 1);
    code.setError({155, 169});
    code.calculateSyndrome();

    vstr directions = {"xyz", "xy", "yz", "xz", "-xyz", "-xy", "-yz", "-xz"};
    for (auto &dir : directions)
    {
        for (int i = 0; i < 6; ++i)
        {
            code.sweep(dir, true);
            code.calculateSyndrome();
        }
    }
    auto &syndrome = code.getSyndrome();
    for (auto s : syndrome)
    {
        EXPECT_EQ(s, 0);
    }
}

TEST(buildSweepIndices, correct_indices_L4)
{
    int l = 4;
    double p = 0.1;
    RhombicCode code(l, p, p, true, 1);
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
        RhombicCode code(l, p, p, true, 1);
        vint &sweepIndices = code.getSweepIndices();
        int expectedSize = ((l - 1) * (l - 1) * (l - 2) + ((l * (l - 2) * (l - 1)) / 2));
        EXPECT_EQ(sweepIndices.size(), expectedSize);
    }
}

TEST(generateErrors, correlated_error_model_runs)
{
    RhombicCode code(4, 0.1, 0.1, true, 1);
    code.buildCorrelatedIndices();
    for (int i = 0; i < 100; ++i)
    {
        EXPECT_NO_THROW(code.generateDataError(true));
    }
}

// There is some randomness in the definition of the rule on the boundaries so sometimes the results don't match up
// TEST(sweep, multiple_sweeps_per_syndrome)
// {
    
//     int l = 4;
//     double p = 0.1;
//     int testRate = 2;
//     RhombicCode highRateCode = RhombicCode(l, p, p, true, testRate);
//     RhombicCode lowRateCode = RhombicCode(l, p, p, true, 1);
//     RhombicCode sanityCode = RhombicCode(l, p, p, true, 1);
//     std::vector<std::set<int>> testErrors = {{22}, {1}, {17}, {41, 42, 43}, {50, 27, 47, 11, 1}, {20, 10}, {11, 15, 6, 2, 22, 45, 40, 21, 0, 3, 33, 5}};
//     for (auto &error : testErrors)
//     {
//         highRateCode.setError(error);
//         lowRateCode.setError(error);
//         highRateCode.calculateSyndrome();
//         lowRateCode.calculateSyndrome();

//         sanityCode.setError(error);
//         sanityCode.calculateSyndrome();

//         for (int i = 0; i < testRate; ++i)
//         {
//             highRateCode.sweep("xz", false);
//         }
//         for (int i = 0; i < testRate; ++i)
//         {
//             lowRateCode.sweep("xz", false);
//             lowRateCode.calculateSyndrome();
//             sanityCode.sweep("xz", false);
//             sanityCode.calculateSyndrome();
//         }

//         auto &lowRateSyndrome = lowRateCode.getSyndrome();
//         auto &highRateSyndrome = highRateCode.getSyndrome();
//         auto &finalErrorLowRate = lowRateCode.getError();
//         auto &finalErrorHighRate = highRateCode.getError();
//         EXPECT_EQ(finalErrorLowRate, finalErrorHighRate);
//         EXPECT_EQ(lowRateSyndrome, highRateSyndrome);

//         auto &sanitySyndrome = sanityCode.getSyndrome();
//         auto &finalErrorSanity = sanityCode.getError();
//         EXPECT_EQ(finalErrorLowRate, finalErrorSanity);
//         EXPECT_EQ(lowRateSyndrome, sanitySyndrome);
//     }
// }