#include "cubicLattice.h"
#include "gtest/gtest.h"
#include <string>

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

    vvint expectedEdges = {{1, 3, 10, 29}, {10, 12, 40, 122}, {8, 10, 17, 36}, {17, 19, 47, 129}, {15, 17, 24, 43}, {29, 33, 40, 141}, {29, 31, 38, 57}, {38, 40,68, 150}, {36, 40, 47, 148}, {36, 38, 45, 64}}; // Reached faceIndex = 9
    for (int i = 0; i < expectedEdges.size(); ++i)
    {
        EXPECT_EQ(faceToEdges[i], expectedEdges[i]);
    } 

}