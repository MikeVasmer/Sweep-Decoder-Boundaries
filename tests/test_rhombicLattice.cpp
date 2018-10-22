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

TEST(createFaces, correctNumberOfFacesCreated)
{
    std::vector<int> ls = {4, 6, 8, 10};
    for (int l : ls)
    {
        RhombicLattice lattice = RhombicLattice(l);
        lattice.createFaces();
        auto &faceToEdges = lattice.getFaceToEdges();
        auto &faceToVertices = lattice.getFaceToVertices();
        if (l == 4)
        {
            int i = 0;
            for (auto vs : faceToVertices)
            {
                // if (!(vs[0] == 63 || vs[1] == 63))
                // {
                //     continue;
                // }
                std::cout << "Face Index = " << i << std::endl;
                std::cout << "Vertex Indices = " << vs[0] << ", " << vs[1] << ", "
                          << vs[2] << ", " << vs[3] << std::endl;
                std::cout << "Vertices = " << lattice.indexToCoordinate(vs[0]) << " "
                          << lattice.indexToCoordinate(vs[1]) << " "
                          << lattice.indexToCoordinate(vs[2]) << " "
                          << lattice.indexToCoordinate(vs[3]) << std::endl;
                ++i;
            }
        }
        int numberOfFaces = 3 * pow(l - 1, 3) - 4 * pow(l - 1, 2) + 2 * (l - 1);
        EXPECT_EQ(faceToEdges.size(), numberOfFaces);
        EXPECT_EQ(faceToVertices.size(), numberOfFaces);
    }
}

TEST(createFaces, correctFacesL4)
{
    int l = 4;
    RhombicLattice lattice = RhombicLattice(l);
    lattice.createFaces();
    auto &faceToEdges = lattice.getFaceToEdges();
    auto &faceToVertices = lattice.getFaceToVertices();

    vvint expectedVertices = {{16, 21, 64, 80}, {18, 23, 66, 82}, {21, 38, 81, 85}, {21, 36, 80, 84}, {18, 21, 65, 81}, {21, 24, 68, 84}, {23, 26, 70, 86}, {23, 38, 82, 86}, {24, 29, 72, 88}, {24, 41, 84, 88}, {26, 43, 86, 90}, {26, 41, 85, 89}, {26, 31, 74, 90}, {21, 26, 69, 85}, {26, 29, 73, 89}, {33, 38, 81, 97}, {33, 53, 96, 97}, {21, 33, 80, 81}, {33, 36, 80, 96}, {38, 50, 97, 98}, {18, 38, 81, 82}, {38, 58, 101, 102}, {26, 38, 85, 86}, {35, 38, 82, 98}, {38, 41, 85, 101}, {41, 61, 104, 105}, {29, 41, 88, 89}, {41, 53, 100, 101}, {21, 41, 84, 85}, {41, 46, 89, 105}, {36, 41, 84, 100}, {38, 43, 86, 102}, {41, 44, 88, 104}, {43, 46, 90, 106}, {46, 58, 105, 106}, {26, 46, 89, 90}, {48, 53, 96, 112}, {50, 55, 98, 114}, {38, 53, 97, 101}, {36, 53, 96, 100}, {50, 53, 97, 113}, {53, 56, 100, 116}, {55, 58, 102, 118}, {38, 55, 98, 102}, {56, 61, 104, 120}, {41, 56, 100, 104}, {43, 58, 102, 106}, {41, 58, 101, 105}, {58, 63, 106, 122}, {53, 58, 101, 117}, {58, 61, 105, 121}};
    for (int i = 0; i < expectedVertices.size(); ++i)
    {
        EXPECT_EQ(faceToVertices[i], expectedVertices[i]);
    }
    vvint expectedEdges = {{112, 114, 448, 562}, {126, 128, 462, 576}, {147, 153, 567, 601}, {151, 254, 562, 564}, {130, 153, 459, 461}, {151, 174, 480, 482}, {165, 188, 494, 496}, {165, 268, 576, 578}, {168, 170, 504, 618}, {168, 174, 588, 622}, {182, 188, 602, 636}, {186, 289, 597, 599}, {182, 184, 518, 632}, {147, 149, 483, 597}, {186, 209, 515, 517}, {231, 233, 567, 681}, {231, 235, 672, 683}, {153, 233, 562, 566}}; // Next faceIndex = 18
    for (int i = 0; i < expectedEdges.size(); ++i)
    {
        EXPECT_EQ(faceToEdges[i], expectedEdges[i]);
    }
}

TEST(createFaces, correctNumberOfFacesInVertexToFacesL4)
{
    int l = 4;
    RhombicLattice lattice = RhombicLattice(l);
    lattice.createFaces();
    auto &vertexToFaces = lattice.getVertexToFaces();

    // Order is ascending y, then x, then z
    vint vertexList = {16, 24, 21, 29, 18, 26, 23, 31, 36, 44, 33, 41, 38, 46, 35, 43, 48, 56, 53, 61, 50, 58, 55, 63};
    vint faceNumberList = {1, 3, 8, 3, 3, 8, 3, 1, 4, 1, 4, 12, 12, 4, 1, 4, 1, 3, 8, 3, 3, 8, 3, 1};

    for (int i = 0; i < vertexList.size(); ++i)
    {
        EXPECT_EQ(vertexToFaces[vertexList[i]].size(), faceNumberList[i]);
    }
}

TEST(findFace, handlesValidInput)
{
    int l = 4;
    RhombicLattice lattice = RhombicLattice(l);
    lattice.createFaces();

    vvint testVertices = {{41, 56, 100, 104}, {50, 55, 98, 114}, {46, 58, 105, 106}, {29, 41, 88, 89}, {26, 29, 73, 89}, {21, 36, 80, 84}};
    vint expectedFaceIndices = {45, 37, 34, 26, 14, 3};
    for (int i = 0; i < expectedFaceIndices.size(); ++i)
    {
        EXPECT_EQ(lattice.findFace(testVertices[i]), expectedFaceIndices[i]);
    }
}

TEST(createUpEdgesMap, correctNumberOfUpEdgesAllVertexTypes)
{
    int l = 4;
    RhombicLattice lattice = RhombicLattice(l);
    lattice.createUpEdgesMap();
    auto upEdgesMap = lattice.getUpEdgesMap();
    std::vector<std::string> directionList = {"xyz", "yz", "xz", "xy", "-xyz", "-yz", "-xz", "-xy"};
    vint testVertices = {16, 24, 18, 26, 90, 42, 127, 91, 87, 110, 109, 117, 118};
    vvint expectedUpEdgeNumbers = {{2, 1, 1, 2, 0, 1, 1, 0}, {3, 1, 3, 3, 1, 3, 1, 1}, {3, 3, 1, 3, 1, 1, 3, 1}, {4, 4, 4, 4, 4, 4, 4, 4}, {3, 0, 0, 0, 0, 3, 3, 3}, {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 1, 1, 1}, {0, 2, 1, 1, 2, 0, 0, 0}, {1, 0, 0, 0, 0, 1, 2, 2}, {1, 0, 0, 0, 0, 2, 1, 2}, {0, 1, 2, 1, 2, 0, 0, 0}, {1, 0, 0, 0, 0, 2, 2, 1}, {0, 1, 1, 2, 2, 0, 0, 0}};
    int j = 0;
    for (const auto &direction : directionList)
    {
        vvint vertexToUpEdges = upEdgesMap[direction];
        
        // cartesian4 coordinate = {2, 1, 3, 1};
        // int index = lattice.coordinateToIndex(coordinate);
        // std::cout << index << std::endl;
        // std::cout << coordinate << std::endl;
        // for (const auto edge : vertexToUpEdges[index])
        // {
        //     std::cout << edge << ", "; 
        // }
        // std::cout << std::endl;

        for (int i = 0; i < testVertices.size(); ++i)
        {
            EXPECT_EQ(vertexToUpEdges[testVertices[i]].size(), expectedUpEdgeNumbers[i][j]);
        }
        ++j;
    }
}

TEST(createVertexToEdges, correctNumberOfEdges)
{
    int l = 6;
    RhombicLattice lattice = RhombicLattice(l);
    lattice.createVertexToEdges();
    vvint vertexToEdges = lattice.getVertexToEdges();
    for (int vertexIndex = 0; vertexIndex < 2 * l * l * l; ++vertexIndex)
    {
        cartesian4 coordinate = lattice.indexToCoordinate(vertexIndex);
        if (coordinate.w == 0)
        {
            if ((coordinate.x + coordinate.y + coordinate.z) % 2 == 1)
            {
                if (coordinate.x == 0 && coordinate.y == 0 && coordinate.z == 0)
                {
                    EXPECT_EQ(vertexToEdges[vertexIndex].size(), 1);
                }
                else if (coordinate.x != 0 && coordinate.y == 0 && coordinate.z == 0)
                {
                    EXPECT_EQ(vertexToEdges[vertexIndex].size(), 2);
                }
                else if (coordinate.x == 0 && coordinate.y != 0 && coordinate.z == 0)
                {
                    EXPECT_EQ(vertexToEdges[vertexIndex].size(), 2);
                }
                else if (coordinate.x == 0 && coordinate.y == 0 && coordinate.z != 0)
                {
                    EXPECT_EQ(vertexToEdges[vertexIndex].size(), 2);
                }
                else if (coordinate.x == 0 && coordinate.y != 0 && coordinate.z != 0)
                {
                    EXPECT_EQ(vertexToEdges[vertexIndex].size(), 4);
                }
                else if (coordinate.x != 0 && coordinate.y == 0 && coordinate.z != 0)
                {
                    EXPECT_EQ(vertexToEdges[vertexIndex].size(), 4);
                }
                else if (coordinate.x != 0 && coordinate.y != 0 && coordinate.z == 0)
                {
                    EXPECT_EQ(vertexToEdges[vertexIndex].size(), 4);
                }
                else
                {
                    EXPECT_EQ(vertexToEdges[vertexIndex].size(), 8);
                }
            }
        }
        else
        {
           if ((coordinate.x + coordinate.y + coordinate.z) % 2 == 1)
           {
               if (coordinate.x == l - 1 && coordinate.y == l - 1 && coordinate.z == l - 1)
                {
                    EXPECT_EQ(vertexToEdges[vertexIndex].size(), 1);
                }
                else if (coordinate.x != l - 1 && coordinate.y == l - 1 && coordinate.z == l - 1)
                {
                    EXPECT_EQ(vertexToEdges[vertexIndex].size(), 1);
                }
                else if (coordinate.x == l - 1 && coordinate.y != l - 1 && coordinate.z == l - 1)
                {
                    EXPECT_EQ(vertexToEdges[vertexIndex].size(), 1);
                }
                else if (coordinate.x == l - 1 && coordinate.y == l - 1 && coordinate.z != l - 1)
                {
                    EXPECT_EQ(vertexToEdges[vertexIndex].size(), 1);
                }
                else if (coordinate.x == l - 1 && coordinate.y != l - 1 && coordinate.z != l - 1)
                {
                    EXPECT_EQ(vertexToEdges[vertexIndex].size(), 2);
                }
                else if (coordinate.x != l - 1 && coordinate.y == l - 1 && coordinate.z != l - 1)
                {
                    EXPECT_EQ(vertexToEdges[vertexIndex].size(), 2);
                }
                else if (coordinate.x != l - 1 && coordinate.y != l - 1 && coordinate.z == l - 1)
                {
                    EXPECT_EQ(vertexToEdges[vertexIndex].size(), 2);
                }
                else
                {
                    EXPECT_EQ(vertexToEdges[vertexIndex].size(), 4);
                }
           }
           else
           {
               // l - 1 cubed is an odd coordinate
                if (coordinate.x != l - 1 && coordinate.y == l - 1 && coordinate.z == l - 1)
                {
                    EXPECT_EQ(vertexToEdges[vertexIndex].size(), 1);
                }
                else if (coordinate.x == l - 1 && coordinate.y != l - 1 && coordinate.z == l - 1)
                {
                    EXPECT_EQ(vertexToEdges[vertexIndex].size(), 1);
                }
                else if (coordinate.x == l - 1 && coordinate.y == l - 1 && coordinate.z != l - 1)
                {
                    EXPECT_EQ(vertexToEdges[vertexIndex].size(), 1);
                }
                else if (coordinate.x == l - 1 && coordinate.y != l - 1 && coordinate.z != l - 1)
                {
                    EXPECT_EQ(vertexToEdges[vertexIndex].size(), 2);
                }
                else if (coordinate.x != l - 1 && coordinate.y == l - 1 && coordinate.z != l - 1)
                {
                    EXPECT_EQ(vertexToEdges[vertexIndex].size(), 2);
                }
                else if (coordinate.x != l - 1 && coordinate.y != l - 1 && coordinate.z == l - 1)
                {
                    EXPECT_EQ(vertexToEdges[vertexIndex].size(), 2);
                }
                else
                {
                    EXPECT_EQ(vertexToEdges[vertexIndex].size(), 4);
                }

           }
        }
    }
}
