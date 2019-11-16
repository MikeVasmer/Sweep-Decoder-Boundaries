#include "cubicToricLattice.h"
#include "gtest/gtest.h"
#include <string>
#include <algorithm>

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