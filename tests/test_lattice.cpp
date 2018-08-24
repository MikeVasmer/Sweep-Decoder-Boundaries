#include "lattice.h"
#include "gtest/gtest.h"
#include <string>

TEST(Lattice, ExceptsInvalidLatticeSizes)
{
    int l = 2;
    std::string type = "cubic";
    ASSERT_ANY_THROW(Lattice lat(l, type));
    l = 0;
    ASSERT_ANY_THROW(Lattice lat(l, type));
    l = -1;
    ASSERT_ANY_THROW(Lattice lat(l, type));
}

TEST(Lattice, ExceptsInvalidType)
{
    int l = 3;
    std::string type = "rectified";
    ASSERT_ANY_THROW(Lattice lat(l, type));
}

TEST(indexToCoordinateTest, HandlesPositiveInput)
{
    int l = 4;
    std::string type = "bcc";
    Lattice lattice = Lattice(l, type);
    cartesian4 coordinate = {0, 0, 0, 0};
    int index = 0;
    ASSERT_EQ(lattice.indexToCoordinate(index), coordinate);
    coordinate = {1, 2, 3, 0};
    index = 57;
    ASSERT_EQ(lattice.indexToCoordinate(index), coordinate);
    coordinate = {0, 1, 0, 1};
    index = 68;
    ASSERT_EQ(lattice.indexToCoordinate(index), coordinate);
}

TEST(indexToCoordinateTest, ExceptsNegativeInput)
{
    int l = 4;
    std::string type = "rhombic";
    Lattice lattice = Lattice(l, type);
    int index = -1;
    ASSERT_ANY_THROW(lattice.indexToCoordinate(index));
}

TEST(coordinateToIndex, ExceptsInvalidCoordinates)
{
    int l = 4;
    std::string type = "cubic";
    Lattice lattice = Lattice(l, type);
    ASSERT_ANY_THROW(lattice.coordinateToIndex({0, 0, 0, 2}));
    ASSERT_ANY_THROW(lattice.coordinateToIndex({0, 0, 0, -1}));
    ASSERT_ANY_THROW(lattice.coordinateToIndex({-1, 0, 0, 1}));
    ASSERT_ANY_THROW(lattice.coordinateToIndex({2, -1, 0, 0}));
    ASSERT_ANY_THROW(lattice.coordinateToIndex({3, 0, -1, 1}));
    ASSERT_ANY_THROW(lattice.coordinateToIndex({-1, -1, -1, -1}));
}

TEST(coordinateToIndex, HandlesPositiveInput)
{
    int l = 4;
    std::string type = "rhombic";
    Lattice lattice = Lattice(l, type);
    int index = 64;
    cartesian4 coordinate = {0, 0, 0, 1};
    ASSERT_EQ(lattice.indexToCoordinate(index), coordinate);
    index = 42;
    coordinate = {2, 2, 2, 0};
    ASSERT_EQ(lattice.indexToCoordinate(index), coordinate);
    index = 105;
    coordinate = {1, 2, 2, 1};
    ASSERT_EQ(lattice.indexToCoordinate(index), coordinate);
}

TEST(neighbour, ExceptsInvalidLatticeDirectionCombinations)
{
    int l = 4;
    std::string type = "cubic";
    Lattice latticeCubic = Lattice(l, type);
    int index = 0;
    int sign = 1;
    ASSERT_ANY_THROW(latticeCubic.neighbour(index, "xy", sign));
    ASSERT_ANY_THROW(latticeCubic.neighbour(index, "xz", sign));
    ASSERT_ANY_THROW(latticeCubic.neighbour(index, "yz", sign));
    ASSERT_ANY_THROW(latticeCubic.neighbour(index, "xyz", sign));

    type = "rhombic";
    Lattice latticeRhombic = Lattice(l, type);
    ASSERT_ANY_THROW(latticeRhombic.neighbour(index, "x", sign));
    ASSERT_ANY_THROW(latticeRhombic.neighbour(index, "y", sign));
    ASSERT_ANY_THROW(latticeRhombic.neighbour(index, "z", sign));
}

TEST(neighbour, HandlesValidInput)
{
    int l = 4;
    std::string type = "bcc";
    Lattice latticeBCC = Lattice(l, type);
    int index = 0;
    int sign = 1;
    ASSERT_EQ(latticeBCC.neighbour(index, "x", sign), 1);
    ASSERT_EQ(latticeBCC.neighbour(index, "y", sign), 4);
    ASSERT_EQ(latticeBCC.neighbour(index, "z", sign), 16);
    ASSERT_EQ(latticeBCC.neighbour(index, "x", -sign), 3);
    ASSERT_EQ(latticeBCC.neighbour(index, "y", -sign), 12);
    ASSERT_EQ(latticeBCC.neighbour(index, "z", -sign), 48);
    ASSERT_EQ(latticeBCC.neighbour(index, "xy", sign), 112);
    ASSERT_EQ(latticeBCC.neighbour(index, "yz", sign), 67);
    ASSERT_EQ(latticeBCC.neighbour(index, "xz", sign), 76);
    ASSERT_EQ(latticeBCC.neighbour(index, "xy", -sign), 79);
    ASSERT_EQ(latticeBCC.neighbour(index, "yz", -sign), 124);
    ASSERT_EQ(latticeBCC.neighbour(index, "xz", -sign), 115);
    ASSERT_EQ(latticeBCC.neighbour(index, "xyz", sign), 64);
    ASSERT_EQ(latticeBCC.neighbour(index, "xyz", -sign), 127);

    index = 64;
    ASSERT_EQ(latticeBCC.neighbour(index, "x", sign), 65);
    ASSERT_EQ(latticeBCC.neighbour(index, "y", sign), 68);
    ASSERT_EQ(latticeBCC.neighbour(index, "z", sign), 80);
    ASSERT_EQ(latticeBCC.neighbour(index, "x", -sign), 67);
    ASSERT_EQ(latticeBCC.neighbour(index, "y", -sign), 76);
    ASSERT_EQ(latticeBCC.neighbour(index, "z", -sign), 112);
    ASSERT_EQ(latticeBCC.neighbour(index, "xy", sign), 5);
    ASSERT_EQ(latticeBCC.neighbour(index, "yz", sign), 20);
    ASSERT_EQ(latticeBCC.neighbour(index, "xz", sign), 17);
    ASSERT_EQ(latticeBCC.neighbour(index, "xy", -sign), 16);
    ASSERT_EQ(latticeBCC.neighbour(index, "yz", -sign), 1);
    ASSERT_EQ(latticeBCC.neighbour(index, "xz", -sign), 4);
    ASSERT_EQ(latticeBCC.neighbour(index, "xyz", sign), 21);
    ASSERT_EQ(latticeBCC.neighbour(index, "xyz", -sign), 0);
}
