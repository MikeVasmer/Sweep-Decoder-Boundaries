#include "cubicLattice.h"
#include "gtest/gtest.h"
#include <string>
#include <cmath>

TEST(CubicLattice, excepts_invalid_lattice_sizes)
{
    int l = 3;
    EXPECT_THROW(CubicLattice lattice = CubicLattice(l), std::invalid_argument);
    l = 0;
    EXPECT_THROW(CubicLattice lattice = CubicLattice(l), std::invalid_argument);
    l = -1;
    EXPECT_THROW(CubicLattice lattice = CubicLattice(l), std::invalid_argument);
}

TEST(neighbour, excepts_invalid_direction)
{
    int l = 4;
    CubicLattice lattice = CubicLattice(l);
    int vertexIndex = 0;
    int sign = 1;
    std::string direction = "xy";
    EXPECT_THROW(lattice.neighbour(vertexIndex, direction, sign), std::invalid_argument);
}

TEST(neighbour, excepts_outside_lattice)
{
    int l = 4;
    CubicLattice lattice = CubicLattice(l);

    int vertexIndex = 0;
    int sign = -1;
    std::vector<std::string> directions = {"x", "y", "z"};
    for (auto const direction : directions)
    {
        EXPECT_THROW(lattice.neighbour(vertexIndex, direction, sign), std::invalid_argument);
    }

    vertexIndex = 63;
    sign = 1;
    for (auto const direction : directions)
    {
        EXPECT_THROW(lattice.neighbour(vertexIndex, direction, sign), std::invalid_argument);
    }
}

TEST(neighbour, output_as_expected)
{
    int l = 4;
    CubicLattice lattice = CubicLattice(l);

    int vertexIndex = 0;
    EXPECT_EQ(lattice.neighbour(vertexIndex, "x", 1), 1);
    EXPECT_EQ(lattice.neighbour(vertexIndex, "y", 1), 4);
    EXPECT_EQ(lattice.neighbour(vertexIndex, "z", 1), 16);

    vertexIndex = 42;
    EXPECT_EQ(lattice.neighbour(vertexIndex, "x", 1), 42 + 1);
    EXPECT_EQ(lattice.neighbour(vertexIndex, "y", 1), 42 + 4);
    EXPECT_EQ(lattice.neighbour(vertexIndex, "z", 1), 42 + 16);
    EXPECT_EQ(lattice.neighbour(vertexIndex, "x", -1), 42 - 1);
    EXPECT_EQ(lattice.neighbour(vertexIndex, "y", -1), 42 - 4);
    EXPECT_EQ(lattice.neighbour(vertexIndex, "z", -1), 42 - 16);

    vertexIndex = 63;
    EXPECT_EQ(lattice.neighbour(vertexIndex, "x", -1), 63 - 1);
    EXPECT_EQ(lattice.neighbour(vertexIndex, "y", -1), 63 - 4);
    EXPECT_EQ(lattice.neighbour(vertexIndex, "z", -1), 63 - 16);
}

TEST(edgeIndex, excepts_outside_lattice)
{
    int l = 4;
    CubicLattice lattice = CubicLattice(l);

    EXPECT_THROW(lattice.edgeIndex(0, "x", -1), std::invalid_argument);
    EXPECT_THROW(lattice.edgeIndex(63, "z", 1), std::invalid_argument);
}

TEST(edgeIndex, correctly_handles_valid_input)
{
    int l = 4;
    CubicLattice lattice = CubicLattice(l);
    int vertexIndex = 26;
    EXPECT_EQ(lattice.edgeIndex(vertexIndex, "x", 1), 183);
    EXPECT_EQ(lattice.edgeIndex(vertexIndex, "y", 1), 185);
    EXPECT_EQ(lattice.edgeIndex(vertexIndex, "z", 1), 187);
    EXPECT_EQ(lattice.edgeIndex(vertexIndex, "x", -1), 176);
    EXPECT_EQ(lattice.edgeIndex(vertexIndex, "y", -1), 157);
    EXPECT_EQ(lattice.edgeIndex(vertexIndex, "z", -1), 75);
}

TEST(createFaces, correct_number_of_faces_created)
{
    vint ls = {4, 6, 8, 10};
    for (int l : ls)
    {
        CubicLattice lattice = CubicLattice(l);
        lattice.createFaces();
        auto &faceToEdges = lattice.getFaceToEdges();
        auto &faceToVertices = lattice.getFaceToVertices();
        int numberOfFaces = 3 * pow(l - 1, 3) - 4 * pow(l - 1, 2) + 2 * (l - 1);
        EXPECT_EQ(faceToEdges.size(), numberOfFaces);
        EXPECT_EQ(faceToVertices.size(), numberOfFaces);
    }
}

TEST(createFaces, correct_faces_created)
{
    int l = 4;
    CubicLattice lattice = CubicLattice(l);
    lattice.createFaces();
    auto &faceToEdges = lattice.getFaceToEdges();
    auto &faceToVertices = lattice.getFaceToVertices();

    vvint expectedVertices = {{0, 1, 4, 5}, {1, 5, 17, 21}, {1, 2, 5, 6}, {2, 6, 18, 22}, {2, 3, 6, 7}, {4, 5, 20, 21}, {4, 5, 8, 9}, {5, 9, 21, 25}, {5, 6, 21, 22}, {5, 6, 9, 10}}; // Reached faceIndex = 9
    for (int i = 0; i < expectedVertices.size(); ++i)
    {
        EXPECT_EQ(faceToVertices[i], expectedVertices[i]);
    }

    vvint expectedEdges = {{1, 3, 10, 29}, {10, 12, 40, 122}, {8, 10, 17, 36}, {17, 19, 47, 129}, {15, 17, 24, 43}, {29, 33, 40, 141}, {29, 31, 38, 57}, {38, 40, 68, 150}, {36, 40, 47, 148}, {36, 38, 45, 64}}; // Reached faceIndex = 9
    for (int i = 0; i < expectedEdges.size(); ++i)
    {
        EXPECT_EQ(faceToEdges[i], expectedEdges[i]);
    }
}

TEST(createFaces, correct_number_of_faces_in_vertex_to_faces)
{
    int l = 4;
    CubicLattice lattice = CubicLattice(l);
    lattice.createFaces();
    auto &vertexToFaces = lattice.getVertexToFaces();

    // Order is ascending x, then y, then z
    vint faceNumberList = {1, 3, 3, 1,
                           3, 8, 8, 3,
                           3, 8, 8, 3,
                           1, 3, 3, 1,
                           1, 4, 4, 1,
                           4, 12, 12, 4,
                           4, 12, 12, 4,
                           1, 4, 4, 1,
                           1, 3, 3, 1,
                           3, 8, 8, 3,
                           3, 8, 8, 3,
                           1, 3, 3, 1,
                           0, 0, 0, 0,
                           0, 0, 0, 0,
                           0, 0, 0, 0,
                           0, 0, 0, 0};

    for (int i = 0; i < pow(l, 3); ++i)
    {
        EXPECT_EQ(vertexToFaces[i].size(), faceNumberList[i]);
    }
}

TEST(findFace, finds_valid_faces)
{
    int l = 4;
    CubicLattice lattice = CubicLattice(l);
    lattice.createFaces();

    vvint testVertices = {{0, 1, 4, 5}, {1, 5, 17, 21}, {1, 2, 5, 6}, {4, 5, 20, 21}, {4, 5, 8, 9}, {5, 9, 21, 25}, {5, 6, 21, 22}, {5, 6, 9, 10}};
    vint expectedFaceIndices = {0, 1, 2, 5, 6, 7, 8, 9};
    for (int i = 0; i < expectedFaceIndices.size(); ++i)
    {
        EXPECT_EQ(lattice.findFace(testVertices[i]), expectedFaceIndices[i]);
    }
}

TEST(findFace, excepts_invalid_faces)
{
    int l = 4;
    CubicLattice lattice = CubicLattice(l);
    lattice.createFaces();

    vvint testVertices = {{0, 22, 36, 50}, {0, 0, 0, 0}, {-1, 555, 6666, 11}};
    for (int i = 0; i < testVertices.size(); ++i)
    {
        EXPECT_THROW(lattice.findFace(testVertices[i]), std::invalid_argument);
        // try
        // {
        //     lattice.findFace(testVertices[i]);
        // }
        // catch (const std::exception& e)
        // {
        //     std::cout << e.what() << std::endl;
        // }
    }
}

TEST(createUpEdgesMap, correct_number_of_up_edges)
{
    int l = 4;
    CubicLattice lattice = CubicLattice(l);
    lattice.createUpEdgesMap();
    auto upEdgesMap = lattice.getUpEdgesMap();
    std::vector<std::string> directionList = {"xyz", "xy", "xz", "yz", "-xyz", "-xy", "-xz", "-yz"};
    vint testVertices = {0, 3, 12, 48, 15, 51, 60, 63,                 // corners 8/8
                         16, 1, 4, 44, 56, 61, 35, 50, 55, 11, 14, 31, // edges 12/12
                         5, 17, 20, 53, 29, 23,                        // facets 6/6
                         21};                                          // bulk 1/1
    vvint expectedNumberUpEdges = {{3, 2, 2, 2, 0, 1, 1, 1},
                                   {2, 1, 1, 3, 1, 2, 2, 0},
                                   {2, 1, 3, 1, 1, 2, 0, 2},
                                   {2, 3, 1, 1, 1, 0, 2, 2},
                                   {1, 0, 2, 2, 2, 3, 1, 1},
                                   {1, 2, 0, 2, 2, 1, 3, 1},
                                   {1, 2, 2, 0, 2, 1, 1, 3},
                                   {0, 1, 1, 1, 3, 2, 2, 2},
                                   {3, 3, 2, 2, 1, 1, 2, 2},
                                   {3, 2, 2, 3, 1, 2, 2, 1},
                                   {3, 2, 3, 2, 1, 2, 1, 2},
                                   {2, 2, 3, 1, 2, 2, 1, 3},
                                   {2, 3, 2, 1, 2, 1, 2, 3},
                                   {1, 2, 2, 1, 3, 2, 2, 3},
                                   {2, 2, 1, 3, 2, 2, 3, 1},
                                   {2, 3, 1, 2, 2, 1, 3, 2},
                                   {1, 2, 1, 2, 3, 2, 3, 2},
                                   {2, 1, 2, 3, 2, 3, 2, 1},
                                   {2, 1, 3, 2, 2, 3, 1, 2},
                                   {1, 1, 2, 2, 3, 3, 2, 2},
                                   {3, 2, 3, 3, 2, 3, 2, 2},
                                   {3, 3, 2, 3, 2, 2, 3, 2},
                                   {3, 3, 3, 2, 2, 2, 2, 3},
                                   {2, 3, 2, 2, 3, 2, 3, 3},
                                   {2, 2, 3, 2, 3, 3, 2, 3},
                                   {2, 2, 2, 3, 3, 3, 3, 2},
                                   {3, 3, 3, 3, 3, 3, 3, 3}};
    int j = 0;
    for (const auto &direction : directionList)
    {
        vvint vertexToUpEdges = upEdgesMap[direction];
        for (int i = 0; i < testVertices.size(); ++i)
        {
            EXPECT_EQ(vertexToUpEdges[testVertices[i]].size(), expectedNumberUpEdges[i][j]);
        }
        ++j;
    }
}

TEST(createVertexToEdges, correct_number_edges)
{
    int l = 8;
    CubicLattice lattice = CubicLattice(l);
    lattice.createVertexToEdges();
    vvint vertexToEdges = lattice.getVertexToEdges();
    for (int vertexIndex = 0; vertexIndex < pow(l, 3); ++vertexIndex)
    {
        cartesian4 coordinate = lattice.indexToCoordinate(vertexIndex);
        if ((coordinate.x == 0 && coordinate.y == 0 && coordinate.z == 0) ||
            (coordinate.x == l - 1 && coordinate.y == 0 && coordinate.z == 0) ||
            (coordinate.x == 0 && coordinate.y == l - 1 && coordinate.z == 0) ||
            (coordinate.x == 0 && coordinate.y == 0 && coordinate.z == l - 1) ||
            (coordinate.x == l - 1 && coordinate.y == l - 1 && coordinate.z == 0) ||
            (coordinate.x == l - 1 && coordinate.y == 0 && coordinate.z == l - 1) ||
            (coordinate.x == 0 && coordinate.y == l - 1 && coordinate.z == l - 1) ||
            (coordinate.x == l - 1 && coordinate.y == l - 1 && coordinate.z == l - 1))
        {
            EXPECT_EQ(vertexToEdges[vertexIndex].size(), 3);
        }
        else if ((coordinate.x == 0 && coordinate.y == 0 && (coordinate.z < l-1 && coordinate.z > 0)) ||
        (coordinate.x == l-1 && coordinate.y ==0 && (coordinate.z < l-1 && coordinate.z > 0)) ||
        (coordinate.x == 0 && coordinate.y == l-1 && (coordinate.z < l-1 && coordinate.z > 0)) ||
        (coordinate.x == l-1 && coordinate.y == l-1 && (coordinate.z < l-1 && coordinate.z > 0)) ||
        (coordinate.x == 0 && (coordinate.y < l-1 && coordinate.y > 0) && coordinate.z == 0) ||
        (coordinate.x == l-1 && (coordinate.y < l-1 && coordinate.y > 0) && coordinate.z == 0) ||
        (coordinate.x == 0 && (coordinate.y < l-1 && coordinate.y > 0) && coordinate.z == l-1) ||
        (coordinate.x == l-1 && (coordinate.y < l-1 && coordinate.y > 0) && coordinate.z == l-1) ||
        ((coordinate.x < l-1 && coordinate.x > 0) && coordinate.y == 0 && coordinate.z == 0) ||
        ((coordinate.x < l-1 && coordinate.x > 0) && coordinate.y == l-1 && coordinate.z == 0) ||
        ((coordinate.x < l-1 && coordinate.x > 0) && coordinate.y == 0 && coordinate.z == l-1) ||
        ((coordinate.x < l-1 && coordinate.x > 0) && coordinate.y == l-1 && coordinate.z == l-1))
        {
            EXPECT_EQ(vertexToEdges[vertexIndex].size(), 4);
        }
        else if (((coordinate.x < l-1 && coordinate.x > 0) &&( coordinate.y < l-1 && coordinate.y > 0) && coordinate.z == 0) ||
        ((coordinate.x < l-1 && coordinate.x > 0) && ( coordinate.y < l-1 && coordinate.y > 0) && coordinate.z == l-1) ||
        ((coordinate.x < l-1 && coordinate.x > 0) && coordinate.y == 0 && (coordinate.z < l-1 && coordinate.z > 0)) ||
        ((coordinate.x < l-1 && coordinate.x > 0) && coordinate.y == l-1 && (coordinate.z < l-1 && coordinate.z > 0)) ||
        (coordinate.x == 0 && (coordinate.y < l-1 && coordinate.y > 0) && ( coordinate.z < l-1 && coordinate.z > 0)) ||
        (coordinate.x == l-1 && (coordinate.y < l-1 && coordinate.y > 0) && ( coordinate.z < l-1 && coordinate.z > 0)))
        {
            EXPECT_EQ(vertexToEdges[vertexIndex].size(), 5);
        }
        else
        {
            EXPECT_EQ(vertexToEdges[vertexIndex].size(), 6);
        }
    }
}
