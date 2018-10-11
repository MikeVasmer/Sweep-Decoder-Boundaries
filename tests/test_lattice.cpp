#include "rhombicToricLattice.h"
#include "gtest/gtest.h"
#include <string>

TEST(Lattice, ExceptsInvalidLatticeSizes)
{
    int l = 2;
    EXPECT_THROW(RhombicToricLattice lattice = RhombicToricLattice(l), std::invalid_argument);
    l = 0;
    EXPECT_THROW(RhombicToricLattice lattice = RhombicToricLattice(l), std::invalid_argument);
    l = -1;
    EXPECT_THROW(RhombicToricLattice lattice = RhombicToricLattice(l), std::invalid_argument);
}

TEST(indexToCoordinate, HandlesPositiveInput)
{
    int l = 4;
    RhombicToricLattice lattice = RhombicToricLattice(l);
    cartesian4 coordinate = {0, 0, 0, 0};
    int vertexIndex = 0;
    EXPECT_EQ(lattice.indexToCoordinate(vertexIndex), coordinate);
    coordinate = {1, 2, 3, 0};
    vertexIndex = 57;
    EXPECT_EQ(lattice.indexToCoordinate(vertexIndex), coordinate);
    coordinate = {0, 1, 0, 1};
    vertexIndex = 68;
    EXPECT_EQ(lattice.indexToCoordinate(vertexIndex), coordinate);
}

TEST(indexToCoordinate, ExceptsNegativeInput)
{
    int l = 4;
    RhombicToricLattice lattice = RhombicToricLattice(l);
    int vertexIndex = -1;
    EXPECT_THROW(lattice.indexToCoordinate(vertexIndex), std::invalid_argument);
}

TEST(coordinateToIndex, ExceptsInvalidCoordinates)
{
    int l = 4;
    RhombicToricLattice lattice = RhombicToricLattice(l);
    EXPECT_THROW(lattice.coordinateToIndex({0, 0, 0, 2}), std::invalid_argument);
    EXPECT_THROW(lattice.coordinateToIndex({0, 0, 0, -1}), std::invalid_argument);
    EXPECT_THROW(lattice.coordinateToIndex({-1, 0, 0, 1}), std::invalid_argument);
    EXPECT_THROW(lattice.coordinateToIndex({2, -1, 0, 0}), std::invalid_argument);
    EXPECT_THROW(lattice.coordinateToIndex({3, 0, -1, 1}), std::invalid_argument);
    EXPECT_THROW(lattice.coordinateToIndex({-1, -1, -1, -1}), std::invalid_argument);
}

TEST(coordinateToIndex, HandlesPositiveInput)
{
    int l = 4;
    RhombicToricLattice lattice = RhombicToricLattice(l);
    int vertexIndex = 64;
    cartesian4 coordinate = {0, 0, 0, 1};
    EXPECT_EQ(lattice.indexToCoordinate(vertexIndex), coordinate);
    vertexIndex = 42;
    coordinate = {2, 2, 2, 0};
    EXPECT_EQ(lattice.indexToCoordinate(vertexIndex), coordinate);
    vertexIndex = 105;
    coordinate = {1, 2, 2, 1};
    EXPECT_EQ(lattice.indexToCoordinate(vertexIndex), coordinate);
}

TEST(neighbourRhombicToric, ExceptsInvalidSigns)
{
    int l = 4;
    RhombicToricLattice lattice = RhombicToricLattice(l);
    int vertexIndex = 0;
    std::string direction = "xy";
    EXPECT_THROW(lattice.neighbour(vertexIndex, direction, 0), std::invalid_argument);
    EXPECT_THROW(lattice.neighbour(vertexIndex, direction, -0), std::invalid_argument);
    EXPECT_THROW(lattice.neighbour(vertexIndex, direction, 7), std::invalid_argument);
    EXPECT_THROW(lattice.neighbour(vertexIndex, direction, -6), std::invalid_argument);
}

// TEST(createFacesBCC, HandlesValidInput)
// {
//     int l = 4;
//     BCCLattice latticeBCC = RhombicToricLattice(l);
//     latticeBCC.createFaces();
//     vvint faceToVertices = latticeBCC.getFaceToVertices();
//     vvint faceToEdges = latticeBCC.getFaceToEdges();

//     vint vertices = {0, 1, 64};
//     EXPECT_EQ(faceToVertices[0], vertices);
//     vint edges = {0, 1, 11};
//     EXPECT_EQ(faceToEdges[0], edges);

//     vertices = {0, 64, 112};
//     EXPECT_EQ(faceToVertices[1], vertices);
//     edges = {0, 2, 789};
//     EXPECT_EQ(faceToEdges[1], edges);

//     vertices = {0, 4, 64};
//     EXPECT_EQ(faceToVertices[2], vertices);
//     edges = {0, 3, 34};
//     EXPECT_EQ(faceToEdges[2], edges);

//     vertices = {0, 64, 67};
//     EXPECT_EQ(faceToVertices[3], vertices);
//     edges = {0, 4, 470};
//     EXPECT_EQ(faceToEdges[3], edges);

//     vertices = {0, 16, 64};
//     EXPECT_EQ(faceToVertices[4], vertices);
//     edges = {0, 5, 114};
//     EXPECT_EQ(faceToEdges[4], edges);

//     vertices = {0, 64, 76};
//     EXPECT_EQ(faceToVertices[5], vertices);
//     edges = {0, 6, 535};
//     EXPECT_EQ(faceToEdges[5], edges);

//     vertices = {0, 1, 112};
//     EXPECT_EQ(faceToVertices[6], vertices);
//     edges = {1, 2, 790};
//     EXPECT_EQ(faceToEdges[6], edges);

//     vertices = {0, 1, 76};
//     EXPECT_EQ(faceToVertices[7], vertices);
//     edges = {1, 6, 534};
//     EXPECT_EQ(faceToEdges[7], edges);

//     vertices = {0, 4, 112};
//     EXPECT_EQ(faceToVertices[8], vertices);
//     edges = {2, 3, 788};
//     EXPECT_EQ(faceToEdges[8], edges);

//     vertices = {0, 4, 67};
//     EXPECT_EQ(faceToVertices[9], vertices);
//     edges = {3, 4, 471};
//     EXPECT_EQ(faceToEdges[9], edges);

//     vertices = {0, 16, 76};
//     EXPECT_EQ(faceToVertices[10], vertices);
//     edges = {5, 6, 536};
//     EXPECT_EQ(faceToEdges[10], edges);

//     vertices = {0, 16, 67};
//     EXPECT_EQ(faceToVertices[11], vertices);
//     edges = {4, 5, 475};
//     EXPECT_EQ(faceToEdges[11], edges);

//     vertices = {9, 72, 73};
//     EXPECT_EQ(faceToVertices[111], vertices);
//     edges = {63, 67, 505};
//     EXPECT_EQ(faceToEdges[111], edges);

//     vertices = {20, 21, 64};
//     EXPECT_EQ(faceToVertices[771], vertices);
//     edges = {141, 448, 452};
//     EXPECT_EQ(faceToEdges[771], edges);
// }