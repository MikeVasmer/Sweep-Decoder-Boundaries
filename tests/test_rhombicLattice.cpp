#include "rhombicLattice.h"
#include "gtest/gtest.h"
#include <string>

TEST(Lattice, ExceptsInvalidLatticeSizes)
{
    int l = 2;
    EXPECT_THROW(RhombicLattice lattice = RhombicLattice(l), std::invalid_argument);
    l = 0;
    EXPECT_THROW(RhombicLattice lattice = RhombicLattice(l), std::invalid_argument);
    l = -1;
    EXPECT_THROW(RhombicLattice lattice = RhombicLattice(l), std::invalid_argument);
}

TEST(neighbour, ExceptsInvalidDirections)
{
    int l = 4;
    RhombicLattice lattice = RhombicLattice(l);
    int vertexIndex = 0;
    int sign = 1;
    std::string direction = "x";
    EXPECT_THROW(lattice.neighbour(vertexIndex, direction, sign), std::invalid_argument);
}

TEST(neighbour, ExceptsVerticesBeyondLatticeBoundaries)
{
    int l = 4;
    RhombicLattice lattice = RhombicLattice(l);

    int vertexIndex = 0;
    int sign = -1;
    std::string direction = "xyz";
    EXPECT_THROW(lattice.neighbour(vertexIndex, direction, sign), std::invalid_argument);
    std::vector<std::string> directions = {"xy", "xz", "yz"};
    std::vector<int> signs = {1, -1};
    for (auto const direction : directions)
    {
        EXPECT_THROW(lattice.neighbour(vertexIndex, direction, signs[0]), std::invalid_argument);
        EXPECT_THROW(lattice.neighbour(vertexIndex, direction, signs[1]), std::invalid_argument);
    }

    vertexIndex = 127;
    sign = 1;
    direction = "xyz";
    EXPECT_THROW(lattice.neighbour(vertexIndex, direction, sign), std::invalid_argument);
    for (auto const direction : directions)
    {
        EXPECT_THROW(lattice.neighbour(vertexIndex, direction, signs[0]), std::invalid_argument);
        EXPECT_THROW(lattice.neighbour(vertexIndex, direction, signs[1]), std::invalid_argument);
    }
}

TEST(neighbour, HandlesValidInput)
{
    int l = 4;
    RhombicLattice lattice = RhombicLattice(l);
    
    // w = 0 vertices
    int vertexIndex = 0;
    int sign = 1;
    std::string direction = "xyz";
    EXPECT_EQ(lattice.neighbour(vertexIndex, direction, sign), 64);
    vertexIndex = 4;
    EXPECT_EQ(lattice.neighbour(vertexIndex, "xyz", 1), 68);
    EXPECT_EQ(lattice.neighbour(vertexIndex, "xz", 1), 64);
    vertexIndex = 21;
    EXPECT_EQ(lattice.neighbour(vertexIndex, "xyz", -1), 64);
    EXPECT_EQ(lattice.neighbour(vertexIndex, "xz", -1), 68);
    EXPECT_EQ(lattice.neighbour(vertexIndex, "yz", -1), 65);
    EXPECT_EQ(lattice.neighbour(vertexIndex, "xy", -1), 80);
    EXPECT_EQ(lattice.neighbour(vertexIndex, "yz", 1), 84);
    EXPECT_EQ(lattice.neighbour(vertexIndex, "xy", 1), 69);

    // w = 1 vertices
    vertexIndex = 127;
    EXPECT_EQ(lattice.neighbour(vertexIndex, "xyz", -1), 63);
    vertexIndex = 95;
    EXPECT_EQ(lattice.neighbour(vertexIndex, "xyz", -1), 31);
    EXPECT_EQ(lattice.neighbour(vertexIndex, "xy", -1), 47);
    vertexIndex = 85;
    EXPECT_EQ(lattice.neighbour(vertexIndex, "xyz", 1), 42);
    EXPECT_EQ(lattice.neighbour(vertexIndex, "xz", 1), 38);
    EXPECT_EQ(lattice.neighbour(vertexIndex, "yz", 1), 41);
    EXPECT_EQ(lattice.neighbour(vertexIndex, "xy", 1), 26);
    EXPECT_EQ(lattice.neighbour(vertexIndex, "yz", -1), 22);
    EXPECT_EQ(lattice.neighbour(vertexIndex, "xz", -1), 25);
}

TEST(edgeIndex, ExceptsEdgesBeyondLatticeBoundaries)
{
    int l = 4;
    RhombicLattice lattice = RhombicLattice(l);
    
    int vertexIndex = 0;
    int sign = 1;
    EXPECT_THROW(lattice.edgeIndex(vertexIndex, "xy", sign), std::invalid_argument);

    vertexIndex = 127;
    EXPECT_THROW(lattice.edgeIndex(vertexIndex, "yz", -1), std::invalid_argument);
}

TEST(edgeIndex, ExceptsInvalidDirections)
{
    int l = 6;
    RhombicLattice lattice = RhombicLattice(l);
    int vertexIndex = 0;
    int sign = 1;
    std::string direction = "north";
    EXPECT_THROW(lattice.edgeIndex(vertexIndex, direction, sign), std::invalid_argument);
}

TEST(edgeIndex, ExceptsInvalidSigns)
{
    int l = 4;
    RhombicLattice lattice = RhombicLattice(l);
    int vertexIndex = 56;
    std::string direction = "yz";
    EXPECT_THROW(lattice.edgeIndex(vertexIndex, direction, 0), std::invalid_argument);
    EXPECT_THROW(lattice.edgeIndex(vertexIndex, direction, -0), std::invalid_argument);
    EXPECT_THROW(lattice.edgeIndex(vertexIndex, direction, 7), std::invalid_argument);
    EXPECT_THROW(lattice.edgeIndex(vertexIndex, direction, -6), std::invalid_argument);
}

TEST(edgeIndex, HandlesValidInput)
{
    int l = 4;
    RhombicLattice lattice = RhombicLattice(l);
    int vertexIndex = 42;
    EXPECT_EQ(lattice.edgeIndex(vertexIndex, "xyz", 1), 294);
    EXPECT_EQ(lattice.edgeIndex(vertexIndex, "xy", 1), 296);
    EXPECT_EQ(lattice.edgeIndex(vertexIndex, "xz", 1), 300);
    EXPECT_EQ(lattice.edgeIndex(vertexIndex, "yz", 1), 298);
    EXPECT_EQ(lattice.edgeIndex(vertexIndex, "xyz", -1), 595);
    EXPECT_EQ(lattice.edgeIndex(vertexIndex, "xy", -1), 709);
    EXPECT_EQ(lattice.edgeIndex(vertexIndex, "xz", -1), 629);
    EXPECT_EQ(lattice.edgeIndex(vertexIndex, "yz", -1), 606);
}