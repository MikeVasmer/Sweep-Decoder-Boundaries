#include "cubicCode.h"
#include "cubicLattice.h"
#include "gtest/gtest.h"
#include <string>
#include <iostream>
#include <cmath>

TEST(buildSyndromeIndices, correct_number_of_indices)
{
    std::vector<int> ls = {4, 6, 8, 10};
    double p = 0.1;
    for (int l : ls)
    {
        CubicCode code = CubicCode(l, p, p, true);
        auto &syndromeIndices = code.getSyndromeIndices();
        int expectedNumber = (2 * pow(l - 1, 2) * (l - 2)) + pow(l - 2, 3); 
        EXPECT_EQ(syndromeIndices.size(), expectedNumber);
    }
}

TEST(buildSyndromeIndices, syndrome_correct_edges)
{
    std::vector<int> ls = {4, 6, 8, 10};
    double p = 0.1;
    for (int l : ls)
    {
        CubicCode code = CubicCode(l, p, p, true);
        auto &syndromeIndices = code.getSyndromeIndices();
        // Check all six boundaries, four edges where rough boundaries meet, one bulk and one z = l - 1
        std::vector<cartesian4> coordinateList = {{0, 0, 0, 0}, {0, l - 1, l - 2, 0}, {l - 1, 0, l - 2}, {l - 1, l - 1, 0, 0}, {1, 0, 0, 0}, {0, 1, 0, 0}, {2, l - 1, 2, 0}, {l - 1, 2, 2, 0}, {1, 1, 0, 0}, {1, 1, l - 2, 0}, {1, 1, 1, 0}, {1, 1, l - 1, 0}};
        std::vector<vstr> expectedEdgeDirections = {{}, {}, {}, {}, {"y"}, {"x"}, {"y"}, {"x"}, {"x", "y", "x", "y", "z"}, {"x", "y", "x", "y", "z"}, {"x", "y", "z", "x", "y", "z"}, {}};
        std::vector<vint> expectedEdgeSigns = {{}, {}, {}, {}, {1}, {1}, {-1}, {-1}, {1, 1, 1, 1, 1}, {1, 1, 1, 1, -1}, {1, 1, 1, 1, 1, 1}, {}};
        auto &lattice = code.getLattice();
        for (int i = 0; i < coordinateList.size(); ++i)
        {
            int vertexIndex = lattice.coordinateToIndex(coordinateList[i]);
            for (int j = 0; j < expectedEdgeDirections[i].size(); ++j)
            {
                auto it = syndromeIndices.find(lattice.edgeIndex(vertexIndex, expectedEdgeDirections[i][j], expectedEdgeSigns[i][j]));
                EXPECT_FALSE(it == syndromeIndices.end());
            }
        }
    }
}

TEST(buildSweepIndices, correct_indices_l4){
    int l = 4;
    double p = 0.1;
    CubicCode code = CubicCode(l, p, p, true);
    vint &sweepIndices = code.getSweepIndices();
    auto &lattice = code.getLattice();
    vint expectedIndices = {5, 6, 9, 10, 21, 22, 25, 26, 37, 38, 41, 42};
    for (int i = 0; i < sweepIndices.size(); ++i)
    {
        EXPECT_EQ(sweepIndices[i], expectedIndices[i]);
    }
}

TEST(buildSweepIndices, correct_number_of_indices)
{
    vint ls = {4, 6, 8, 10};
    double p = 0.1;
    for (const int l : ls)
    {
        CubicCode code = CubicCode(l, p, p, true);
        vint &sweepIndices = code.getSweepIndices();
        int expectedSize = pow(l - 2, 2) * (l - 1);
        EXPECT_EQ(sweepIndices.size(), expectedSize);
    }
}

TEST(calculateSyndrome, no_invalid_syndromes_data_errors)
{
    vint ls = {4, 6, 8};
    double p = 0.1;
    for (auto l : ls)
    {
        CubicCode code = CubicCode(l, p, p, true);
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
        CubicCode code = CubicCode(l, p, p, true);
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
        CubicCode code = CubicCode(l, p, p, true);
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
    CubicCode code = CubicCode(4, 0.1, 0.1, true);
    auto &syndrome = code.getSyndrome();
    std::vector<std::set<int>> stabErrors = {
        {0, 1, 5, 21}, 
        {7, 8, 9, 10, 16, 30}, 
        {15, 16, 17, 18, 38}
        };
    for (auto const &error : stabErrors)
    {
        code.setError(error);
        code.calculateSyndrome();
        for (auto value : syndrome)
        {
            EXPECT_EQ(value, 0);
        }
    }
}

TEST(calculateSyndrome, correct_syndrome_one_error)
{
    CubicCode code = CubicCode(4, 0.1, 0.1, true);
    auto &syndrome = code.getSyndrome();
    std::set<int> error = {0};
    code.setError(error);
    code.calculateSyndrome();
    for (int i = 0; i < syndrome.size(); ++i)
    {
        if (i == 10 || i == 29)
        {
            EXPECT_EQ(syndrome[i], 1);
        }
        else 
        {
            EXPECT_EQ(syndrome[i], 0);
        }
    }
}

TEST(calculateSyndrome, correct_syndrome_two_errors)
{
    CubicCode code = CubicCode(4, 0.1, 0.1, true);
    auto &syndrome = code.getSyndrome();
    std::set<int> error = {0, 1};
    code.setError(error);
    code.calculateSyndrome();
    for (int i = 0; i < syndrome.size(); ++i)
    {
        if (i == 29 || i == 40 || i == 122)
        {
            EXPECT_EQ(syndrome[i], 1);
        }
        else 
        {
            EXPECT_EQ(syndrome[i], 0);
        }
    }
}

TEST(calculateSyndrome, correct_syndrome_three_errors)
{
    CubicCode code = CubicCode(4, 0.1, 0.1, true);
    auto &syndrome = code.getSyndrome();
    std::set<int> error = {0, 1, 5};
    code.setError(error);
    code.calculateSyndrome();
    for (int i = 0; i < syndrome.size(); ++i)
    {
        if (i == 122 || i == 141)
        {
            EXPECT_EQ(syndrome[i], 1);
        }
        else 
        {
            EXPECT_EQ(syndrome[i], 0);
        }
    }
}

TEST(buildLogical, logical_correct_weight)
{
    vint ls = {4, 6, 8, 10};
    double p = 0.1;
    for (auto l : ls)
    {
        CubicCode code = CubicCode(l, p, p, true);
        auto logicals = code.getLogicals();
        int expectedWeight = l - 1;
        EXPECT_EQ(logicals[0].size(), expectedWeight);
    }
}

TEST(checkCorrection, handles_logical_X_errors)
{
    CubicCode code = CubicCode(4, 0.1, 0.1, true);
    std::vector<std::set<int>> errors = {{0, 2, 4, 6, 9, 12, 14, 17, 20}, {21, 23, 25, 27, 30, 33, 35, 38, 41}, {42, 43, 44, 45, 46, 47, 48, 49, 50}};
    auto &syndrome = code.getSyndrome();
    for (auto &error : errors)
    {
        code.setError(error);
        code.calculateSyndrome();
        for (int i = 0; i < syndrome.size(); ++i)
        {
            EXPECT_EQ(syndrome[i], 0);
        }
        EXPECT_FALSE(code.checkCorrection());
    }
}

TEST(checkCorrection, handles_stabilizer_errors)
{
    CubicCode code = CubicCode(4, 0.1, 0.1, true);
    std::vector<std::set<int>> errors = {{0, 1, 5, 21}, {28, 29, 30, 31, 37, 46}, {36, 37, 38, 39, 49}};
    auto &syndrome = code.getSyndrome();
    for (auto &error : errors)
    {
        code.setError(error);
        code.calculateSyndrome();
        for (int i = 0; i < syndrome.size(); ++i)
        {
            EXPECT_EQ(syndrome[i], 0);
        }
        EXPECT_TRUE(code.checkCorrection());
    }
}

