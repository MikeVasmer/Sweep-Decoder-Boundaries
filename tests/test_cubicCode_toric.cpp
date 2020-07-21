#include "cubicToricLattice.h"
#include "gtest/gtest.h"
#include <string>
#include <algorithm>
#include "cubicCode.h"

TEST(neighbour, handles_valid_input)
{
    int l = 4;
    CubicToricLattice lattice = CubicToricLattice(l);
    int vertexIndex = 0;
    int sign = 1;
    EXPECT_EQ(lattice.neighbour(vertexIndex, "x", sign), 1);
    EXPECT_EQ(lattice.neighbour(vertexIndex, "y", sign), 4);
    EXPECT_EQ(lattice.neighbour(vertexIndex, "z", sign), 16);
    EXPECT_EQ(lattice.neighbour(vertexIndex, "x", -sign), 3);
    EXPECT_EQ(lattice.neighbour(vertexIndex, "y", -sign), 12);
    EXPECT_EQ(lattice.neighbour(vertexIndex, "z", -sign), 48);
}

TEST(edgeIndex, handles_valid_input)
{
    int l = 5;
    CubicToricLattice lattice = CubicToricLattice(l);
    int vertexIndex = 0;
    int sign = 1;
    EXPECT_EQ(lattice.edgeIndex(vertexIndex, "x", sign), 1);
    EXPECT_EQ(lattice.edgeIndex(vertexIndex, "y", sign), 3);
    EXPECT_EQ(lattice.edgeIndex(vertexIndex, "z", sign), 5);
    EXPECT_EQ(lattice.edgeIndex(vertexIndex, "x", -sign), 29);
    EXPECT_EQ(lattice.edgeIndex(vertexIndex, "y", -sign), 143);
    EXPECT_EQ(lattice.edgeIndex(vertexIndex, "z", -sign), 705);
}

TEST(generateDataError, statistics_correct)
{
    double p = 0.5;
    int l = 24;
    double tolerance = 0.02;
    CubicCode code(l, p, 0, false, 1);
    code.generateDataError(false);
    int errorCount = code.getError().size();
    EXPECT_NEAR(pow(l, 3) * 3 * p, errorCount, pow(l, 3) * 3 * p * tolerance);
}

TEST(generateMeasError, statistics_correct)
{
    double q = 0.5;
    int l = 24;
    double tolerance = 0.02;
    CubicCode code(l, 0, q, false, 1);
    code.generateMeasError();
    auto &syndrome = code.getSyndrome();
    int errorCount = 0;
    for (int i = 0; i < syndrome.size(); ++i)
    {
        if (syndrome[i] == 1)
        {
            ++errorCount;
        }
    }
    EXPECT_NEAR(pow(l, 3) * 7 * q, errorCount, pow(l, 3) * 7 * q * tolerance);
    // Note generate meas error can flip "phantom" syndrome indices, 
    // but these are always cleared when we call calculateSyndrome()
}