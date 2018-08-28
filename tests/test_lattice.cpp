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

TEST(Lattice, ExceptsInvalidLengthAndTypeCombination)
{
    int l = 3;
    std::string type = "rhombic";
    ASSERT_ANY_THROW(Lattice lat(l, type));
}

TEST(indexToCoordinateTest, HandlesPositiveInput)
{
    int l = 4;
    std::string type = "bcc";
    Lattice lattice = Lattice(l, type);
    cartesian4 coordinate = {0, 0, 0, 0};
    int vertexIndex = 0;
    ASSERT_EQ(lattice.indexToCoordinate(vertexIndex), coordinate);
    coordinate = {1, 2, 3, 0};
    vertexIndex = 57;
    ASSERT_EQ(lattice.indexToCoordinate(vertexIndex), coordinate);
    coordinate = {0, 1, 0, 1};
    vertexIndex = 68;
    ASSERT_EQ(lattice.indexToCoordinate(vertexIndex), coordinate);
}

TEST(indexToCoordinateTest, ExceptsNegativeInput)
{
    int l = 4;
    std::string type = "rhombic";
    Lattice lattice = Lattice(l, type);
    int vertexIndex = -1;
    ASSERT_ANY_THROW(lattice.indexToCoordinate(vertexIndex));
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
    int vertexIndex = 64;
    cartesian4 coordinate = {0, 0, 0, 1};
    ASSERT_EQ(lattice.indexToCoordinate(vertexIndex), coordinate);
    vertexIndex = 42;
    coordinate = {2, 2, 2, 0};
    ASSERT_EQ(lattice.indexToCoordinate(vertexIndex), coordinate);
    vertexIndex = 105;
    coordinate = {1, 2, 2, 1};
    ASSERT_EQ(lattice.indexToCoordinate(vertexIndex), coordinate);
}

TEST(neighbour, ExceptsInvalidSigns)
{
    int l = 3;
    std::string type = "cubic";
    Lattice latticeCubic = Lattice(l, type);
    int vertexIndex = 0;
    std::string direction = "x";
    ASSERT_ANY_THROW(latticeCubic.neighbour(vertexIndex, direction, 0));
    ASSERT_ANY_THROW(latticeCubic.neighbour(vertexIndex, direction, -0));
    ASSERT_ANY_THROW(latticeCubic.neighbour(vertexIndex, direction, 7));
    ASSERT_ANY_THROW(latticeCubic.neighbour(vertexIndex, direction, -6));
}

TEST(neighbour, ExceptsInvalidDirections)
{
    int l = 3;
    std::string type = "bcc";
    Lattice latticeBCC = Lattice(l, type);
    int vertexIndex = 0;
    int sign = 1;
    std::string direction = "north";
    ASSERT_ANY_THROW(latticeBCC.neighbour(vertexIndex, direction, sign));
}

TEST(neighbour, ExceptsInvalidLatticeDirectionCombinations)
{
    int l = 4;
    std::string type = "cubic";
    Lattice latticeCubic = Lattice(l, type);
    int vertexIndex = 0;
    int sign = 1;
    ASSERT_ANY_THROW(latticeCubic.neighbour(vertexIndex, "xy", sign));
    ASSERT_ANY_THROW(latticeCubic.neighbour(vertexIndex, "xz", sign));
    ASSERT_ANY_THROW(latticeCubic.neighbour(vertexIndex, "yz", sign));
    ASSERT_ANY_THROW(latticeCubic.neighbour(vertexIndex, "xyz", sign));

    type = "rhombic";
    Lattice latticeRhombic = Lattice(l, type);
    ASSERT_ANY_THROW(latticeRhombic.neighbour(vertexIndex, "x", sign));
    ASSERT_ANY_THROW(latticeRhombic.neighbour(vertexIndex, "y", sign));
    ASSERT_ANY_THROW(latticeRhombic.neighbour(vertexIndex, "z", sign));
}

TEST(neighbour, HandlesValidInput)
{
    int l = 4;
    std::string type = "bcc";
    Lattice latticeBCC = Lattice(l, type);
    int vertexIndex = 0;
    int sign = 1;
    ASSERT_EQ(latticeBCC.neighbour(vertexIndex, "x", sign), 1);
    ASSERT_EQ(latticeBCC.neighbour(vertexIndex, "y", sign), 4);
    ASSERT_EQ(latticeBCC.neighbour(vertexIndex, "z", sign), 16);
    ASSERT_EQ(latticeBCC.neighbour(vertexIndex, "x", -sign), 3);
    ASSERT_EQ(latticeBCC.neighbour(vertexIndex, "y", -sign), 12);
    ASSERT_EQ(latticeBCC.neighbour(vertexIndex, "z", -sign), 48);
    ASSERT_EQ(latticeBCC.neighbour(vertexIndex, "xy", sign), 112);
    ASSERT_EQ(latticeBCC.neighbour(vertexIndex, "yz", sign), 67);
    ASSERT_EQ(latticeBCC.neighbour(vertexIndex, "xz", sign), 76);
    ASSERT_EQ(latticeBCC.neighbour(vertexIndex, "xy", -sign), 79);
    ASSERT_EQ(latticeBCC.neighbour(vertexIndex, "yz", -sign), 124);
    ASSERT_EQ(latticeBCC.neighbour(vertexIndex, "xz", -sign), 115);
    ASSERT_EQ(latticeBCC.neighbour(vertexIndex, "xyz", sign), 64);
    ASSERT_EQ(latticeBCC.neighbour(vertexIndex, "xyz", -sign), 127);

    vertexIndex = 64;
    ASSERT_EQ(latticeBCC.neighbour(vertexIndex, "x", sign), 65);
    ASSERT_EQ(latticeBCC.neighbour(vertexIndex, "y", sign), 68);
    ASSERT_EQ(latticeBCC.neighbour(vertexIndex, "z", sign), 80);
    ASSERT_EQ(latticeBCC.neighbour(vertexIndex, "x", -sign), 67);
    ASSERT_EQ(latticeBCC.neighbour(vertexIndex, "y", -sign), 76);
    ASSERT_EQ(latticeBCC.neighbour(vertexIndex, "z", -sign), 112);
    ASSERT_EQ(latticeBCC.neighbour(vertexIndex, "xy", sign), 5);
    ASSERT_EQ(latticeBCC.neighbour(vertexIndex, "yz", sign), 20);
    ASSERT_EQ(latticeBCC.neighbour(vertexIndex, "xz", sign), 17);
    ASSERT_EQ(latticeBCC.neighbour(vertexIndex, "xy", -sign), 16);
    ASSERT_EQ(latticeBCC.neighbour(vertexIndex, "yz", -sign), 1);
    ASSERT_EQ(latticeBCC.neighbour(vertexIndex, "xz", -sign), 4);
    ASSERT_EQ(latticeBCC.neighbour(vertexIndex, "xyz", sign), 21);
    ASSERT_EQ(latticeBCC.neighbour(vertexIndex, "xyz", -sign), 0);
}

TEST(getEdgeIndex, ExceptsInvalidDirections)
{
    int l = 5;
    std::string type = "bcc";
    Lattice latticeBCC = Lattice(l, type);
    int vertexIndex = 0;
    int sign = 1;
    std::string direction = "north";
    ASSERT_ANY_THROW(latticeBCC.getEdgeIndex(vertexIndex, direction, sign));
}

TEST(getEdgeIndex, ExceptsInvalidLatticeDirectionCombinations)
{
    int l = 6;
    std::string type = "cubic";
    Lattice latticeCubic = Lattice(l, type);
    int vertexIndex = 5;
    int sign = 1;
    ASSERT_ANY_THROW(latticeCubic.getEdgeIndex(vertexIndex, "xy", sign));
    ASSERT_ANY_THROW(latticeCubic.getEdgeIndex(vertexIndex, "xz", sign));
    ASSERT_ANY_THROW(latticeCubic.getEdgeIndex(vertexIndex, "yz", sign));
    ASSERT_ANY_THROW(latticeCubic.getEdgeIndex(vertexIndex, "xyz", sign));

    type = "rhombic";
    Lattice latticeRhombic = Lattice(l, type);
    ASSERT_ANY_THROW(latticeRhombic.getEdgeIndex(vertexIndex, "x", sign));
    ASSERT_ANY_THROW(latticeRhombic.getEdgeIndex(vertexIndex, "y", sign));
    ASSERT_ANY_THROW(latticeRhombic.getEdgeIndex(vertexIndex, "z", sign));
}

TEST(getEdgeIndex, ExceptsInvalidSigns)
{
    int l = 4;
    std::string type = "rhombic";
    Lattice latticeCubic = Lattice(l, type);
    int vertexIndex = 56;
    std::string direction = "yz";
    ASSERT_ANY_THROW(latticeCubic.getEdgeIndex(vertexIndex, direction, 0));
    ASSERT_ANY_THROW(latticeCubic.getEdgeIndex(vertexIndex, direction, -0));
    ASSERT_ANY_THROW(latticeCubic.getEdgeIndex(vertexIndex, direction, 7));
    ASSERT_ANY_THROW(latticeCubic.getEdgeIndex(vertexIndex, direction, -6));
}

TEST(getEdgeIndex, HandlesValidInput)
{
    int l = 6;
    std::string type = "bcc";
    Lattice latticeBCC = Lattice(l, type);
    int vertexIndex = 3;
    int sign = 1;
    ASSERT_EQ(latticeBCC.getEdgeIndex(vertexIndex, "x", sign), 22);
    ASSERT_EQ(latticeBCC.getEdgeIndex(vertexIndex, "y", sign), 24);
    ASSERT_EQ(latticeBCC.getEdgeIndex(vertexIndex, "z", sign), 26);
    ASSERT_EQ(latticeBCC.getEdgeIndex(vertexIndex, "xy", sign), 23);
    ASSERT_EQ(latticeBCC.getEdgeIndex(vertexIndex, "xz", sign), 27);
    ASSERT_EQ(latticeBCC.getEdgeIndex(vertexIndex, "yz", sign), 25);
    ASSERT_EQ(latticeBCC.getEdgeIndex(vertexIndex, "xyz", sign), 21);

    vertexIndex = 7;
    ASSERT_EQ(latticeBCC.getEdgeIndex(vertexIndex, "x", -sign), 43);
    ASSERT_EQ(latticeBCC.getEdgeIndex(vertexIndex, "y", -sign), 10);
    ASSERT_EQ(latticeBCC.getEdgeIndex(vertexIndex, "z", -sign), 1314);
    ASSERT_EQ(latticeBCC.getEdgeIndex(vertexIndex, "xy", -sign), 1514);
    ASSERT_EQ(latticeBCC.getEdgeIndex(vertexIndex, "xz", -sign), 2820);
    ASSERT_EQ(latticeBCC.getEdgeIndex(vertexIndex, "yz", -sign), 2783);
    ASSERT_EQ(latticeBCC.getEdgeIndex(vertexIndex, "xyz", -sign), 2772);
}

TEST(createFaces, HandlesValidInputBCC)
{
    int l = 4;
    std::string type = "bcc";
    Lattice latticeBCC = Lattice(l, type);
    latticeBCC.createFaces();
    vvint faceToVertices = latticeBCC.getFaceToVertices();
    vvint faceToEdges = latticeBCC.getFaceToEdges();

    vint vertices = {0, 1, 64};
    ASSERT_EQ(faceToVertices[0], vertices);
    vint edges = {0, 1, 11};
    ASSERT_EQ(faceToEdges[0], edges);

    vertices = {0, 64, 112};
    ASSERT_EQ(faceToVertices[1], vertices);
    edges = {0, 2, 789};
    ASSERT_EQ(faceToEdges[1], edges);

    vertices = {0, 4, 64};
    ASSERT_EQ(faceToVertices[2], vertices);
    edges = {0, 3, 34};
    ASSERT_EQ(faceToEdges[2], edges);

    vertices = {0, 64, 67};
    ASSERT_EQ(faceToVertices[3], vertices);
    edges = {0, 4, 470};
    ASSERT_EQ(faceToEdges[3], edges);

    vertices = {0, 16, 64};
    ASSERT_EQ(faceToVertices[4], vertices);
    edges = {0, 5, 114};
    ASSERT_EQ(faceToEdges[4], edges);

    vertices = {0, 64, 76};
    ASSERT_EQ(faceToVertices[5], vertices);
    edges = {0, 6, 535};
    ASSERT_EQ(faceToEdges[5], edges);

    vertices = {0, 1, 112};
    ASSERT_EQ(faceToVertices[6], vertices);
    edges = {1, 2, 790};
    ASSERT_EQ(faceToEdges[6], edges);

    vertices = {0, 1, 76};
    ASSERT_EQ(faceToVertices[7], vertices);
    edges = {1, 6, 534};
    ASSERT_EQ(faceToEdges[7], edges);

    vertices = {0, 4, 112};
    ASSERT_EQ(faceToVertices[8], vertices);
    edges = {2, 3, 788};
    ASSERT_EQ(faceToEdges[8], edges);

    vertices = {0, 4, 67};
    ASSERT_EQ(faceToVertices[9], vertices);
    edges = {3, 4, 471};
    ASSERT_EQ(faceToEdges[9], edges);

    vertices = {0, 16, 76};
    ASSERT_EQ(faceToVertices[10], vertices);
    edges = {5, 6, 536};
    ASSERT_EQ(faceToEdges[10], edges);
    
    vertices = {0, 16, 67};
    ASSERT_EQ(faceToVertices[11], vertices);
    edges = {4, 5, 475};
    ASSERT_EQ(faceToEdges[11], edges);

    vertices = {9, 72, 73};
    ASSERT_EQ(faceToVertices[111], vertices);
    edges = {63, 67, 505};
    ASSERT_EQ(faceToEdges[111], edges);

    vertices = {20, 21, 64};
    ASSERT_EQ(faceToVertices[771], vertices);
    edges = {141, 448, 452};
    ASSERT_EQ(faceToEdges[771], edges);
}

TEST(createFaces, HandlesValidInputRhombic)
{
    int l = 6;
    std::string type = "rhombic";
    Lattice latticeRhombic = Lattice(l, type);
    latticeRhombic.createFaces();
    vvint faceToVertices = latticeRhombic.getFaceToVertices();
    vvint faceToEdges = latticeRhombic.getFaceToEdges();

    vint vertices = {0, 42, 216, 221};
    ASSERT_EQ(faceToVertices[0], vertices);
    vint edges = {0, 4, 1516, 1547};
    ASSERT_EQ(faceToEdges[0], edges);

    vertices = {0, 37, 216, 246};
    ASSERT_EQ(faceToVertices[1], vertices);
    edges = {0, 6, 1518, 1722};
    ASSERT_EQ(faceToEdges[1], edges);
}

TEST(createFaces, correctNumberOfFaces)
{
    // Checked this on 28/08/18 with the preallocation turned off
    // Preallocation is turned on by default for speed purposes
    int l = 6;
    std::string type = "bcc";
    Lattice latticeBCC = Lattice(l, type);
    latticeBCC.createFaces();
    vvint faceToVertices = latticeBCC.getFaceToVertices();
    vvint faceToEdges = latticeBCC.getFaceToEdges();
    ASSERT_EQ(faceToVertices.size(), 24 * l * l * l);
    ASSERT_EQ(faceToEdges.size(), 24 * l * l * l);

    l = 4;
    type = "rhombic";
    Lattice latticeRhombic = Lattice(l, type);
    latticeRhombic.createFaces();
    faceToVertices = latticeRhombic.getFaceToVertices();
    faceToEdges = latticeRhombic.getFaceToEdges();
    ASSERT_EQ(faceToVertices.size(), 3 * l * l * l);
    ASSERT_EQ(faceToEdges.size(), 3 * l * l * l);
    
}