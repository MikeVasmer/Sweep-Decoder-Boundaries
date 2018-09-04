#include "lattice.h"
#include "gtest/gtest.h"
#include <string>

TEST(Lattice, ExceptsInvalidLatticeSizes)
{
    int l = 2;
    std::string type = "cubic";
    EXPECT_THROW(Lattice lat(l, type), std::invalid_argument);
    l = 0;
    EXPECT_THROW(Lattice lat(l, type), std::invalid_argument);
    l = -1;
    EXPECT_THROW(Lattice lat(l, type), std::invalid_argument);
}

TEST(Lattice, ExceptsInvalidType)
{
    int l = 3;
    std::string type = "rectified";
    EXPECT_THROW(Lattice lat(l, type), std::invalid_argument);
}

TEST(Lattice, ExceptsInvalidLengthAndTypeCombination)
{
    int l = 3;
    std::string type = "rhombic";
    EXPECT_THROW(Lattice lat(l, type), std::invalid_argument);
}

TEST(indexToCoordinateTest, HandlesPositiveInput)
{
    int l = 4;
    std::string type = "bcc";
    Lattice lattice = Lattice(l, type);
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

TEST(indexToCoordinateTest, ExceptsNegativeInput)
{
    int l = 4;
    std::string type = "rhombic";
    Lattice lattice = Lattice(l, type);
    int vertexIndex = -1;
    EXPECT_THROW(lattice.indexToCoordinate(vertexIndex), std::invalid_argument);
}

TEST(coordinateToIndex, ExceptsInvalidCoordinates)
{
    int l = 4;
    std::string type = "cubic";
    Lattice lattice = Lattice(l, type);
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
    std::string type = "rhombic";
    Lattice lattice = Lattice(l, type);
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

TEST(neighbour, ExceptsInvalidSigns)
{
    int l = 3;
    std::string type = "cubic";
    Lattice latticeCubic = Lattice(l, type);
    int vertexIndex = 0;
    std::string direction = "x";
    EXPECT_THROW(latticeCubic.neighbour(vertexIndex, direction, 0), std::invalid_argument);
    EXPECT_THROW(latticeCubic.neighbour(vertexIndex, direction, -0), std::invalid_argument);
    EXPECT_THROW(latticeCubic.neighbour(vertexIndex, direction, 7), std::invalid_argument);
    EXPECT_THROW(latticeCubic.neighbour(vertexIndex, direction, -6), std::invalid_argument);
}

TEST(neighbour, ExceptsInvalidDirections)
{
    int l = 3;
    std::string type = "bcc";
    Lattice latticeBCC = Lattice(l, type);
    int vertexIndex = 0;
    int sign = 1;
    std::string direction = "north";
    EXPECT_THROW(latticeBCC.neighbour(vertexIndex, direction, sign), std::invalid_argument);
}

TEST(neighbour, ExceptsInvalidLatticeDirectionCombinations)
{
    int l = 4;
    std::string type = "cubic";
    Lattice latticeCubic = Lattice(l, type);
    int vertexIndex = 0;
    int sign = 1;
    EXPECT_THROW(latticeCubic.neighbour(vertexIndex, "xy", sign), std::invalid_argument);
    EXPECT_THROW(latticeCubic.neighbour(vertexIndex, "xz", sign), std::invalid_argument);
    EXPECT_THROW(latticeCubic.neighbour(vertexIndex, "yz", sign), std::invalid_argument);
    EXPECT_THROW(latticeCubic.neighbour(vertexIndex, "xyz", sign), std::invalid_argument);

    type = "rhombic";
    Lattice latticeRhombic = Lattice(l, type);
    EXPECT_THROW(latticeRhombic.neighbour(vertexIndex, "x", sign), std::invalid_argument);
    EXPECT_THROW(latticeRhombic.neighbour(vertexIndex, "y", sign), std::invalid_argument);
    EXPECT_THROW(latticeRhombic.neighbour(vertexIndex, "z", sign), std::invalid_argument);
}

TEST(neighbour, HandlesValidInput)
{
    int l = 4;
    std::string type = "bcc";
    Lattice latticeBCC = Lattice(l, type);
    int vertexIndex = 0;
    int sign = 1;
    EXPECT_EQ(latticeBCC.neighbour(vertexIndex, "x", sign), 1);
    EXPECT_EQ(latticeBCC.neighbour(vertexIndex, "y", sign), 4);
    EXPECT_EQ(latticeBCC.neighbour(vertexIndex, "z", sign), 16);
    EXPECT_EQ(latticeBCC.neighbour(vertexIndex, "x", -sign), 3);
    EXPECT_EQ(latticeBCC.neighbour(vertexIndex, "y", -sign), 12);
    EXPECT_EQ(latticeBCC.neighbour(vertexIndex, "z", -sign), 48);
    EXPECT_EQ(latticeBCC.neighbour(vertexIndex, "xy", sign), 112);
    EXPECT_EQ(latticeBCC.neighbour(vertexIndex, "yz", sign), 67);
    EXPECT_EQ(latticeBCC.neighbour(vertexIndex, "xz", sign), 76);
    EXPECT_EQ(latticeBCC.neighbour(vertexIndex, "xy", -sign), 79);
    EXPECT_EQ(latticeBCC.neighbour(vertexIndex, "yz", -sign), 124);
    EXPECT_EQ(latticeBCC.neighbour(vertexIndex, "xz", -sign), 115);
    EXPECT_EQ(latticeBCC.neighbour(vertexIndex, "xyz", sign), 64);
    EXPECT_EQ(latticeBCC.neighbour(vertexIndex, "xyz", -sign), 127);

    vertexIndex = 64;
    EXPECT_EQ(latticeBCC.neighbour(vertexIndex, "x", sign), 65);
    EXPECT_EQ(latticeBCC.neighbour(vertexIndex, "y", sign), 68);
    EXPECT_EQ(latticeBCC.neighbour(vertexIndex, "z", sign), 80);
    EXPECT_EQ(latticeBCC.neighbour(vertexIndex, "x", -sign), 67);
    EXPECT_EQ(latticeBCC.neighbour(vertexIndex, "y", -sign), 76);
    EXPECT_EQ(latticeBCC.neighbour(vertexIndex, "z", -sign), 112);
    EXPECT_EQ(latticeBCC.neighbour(vertexIndex, "xy", sign), 5);
    EXPECT_EQ(latticeBCC.neighbour(vertexIndex, "yz", sign), 20);
    EXPECT_EQ(latticeBCC.neighbour(vertexIndex, "xz", sign), 17);
    EXPECT_EQ(latticeBCC.neighbour(vertexIndex, "xy", -sign), 16);
    EXPECT_EQ(latticeBCC.neighbour(vertexIndex, "yz", -sign), 1);
    EXPECT_EQ(latticeBCC.neighbour(vertexIndex, "xz", -sign), 4);
    EXPECT_EQ(latticeBCC.neighbour(vertexIndex, "xyz", sign), 21);
    EXPECT_EQ(latticeBCC.neighbour(vertexIndex, "xyz", -sign), 0);
}

TEST(getEdgeIndex, ExceptsInvalidDirections)
{
    int l = 5;
    std::string type = "bcc";
    Lattice latticeBCC = Lattice(l, type);
    int vertexIndex = 0;
    int sign = 1;
    std::string direction = "north";
    EXPECT_THROW(latticeBCC.getEdgeIndex(vertexIndex, direction, sign), std::invalid_argument);
}

TEST(getEdgeIndex, ExceptsInvalidLatticeDirectionCombinations)
{
    int l = 6;
    std::string type = "cubic";
    Lattice latticeCubic = Lattice(l, type);
    int vertexIndex = 5;
    int sign = 1;
    EXPECT_THROW(latticeCubic.getEdgeIndex(vertexIndex, "xy", sign), std::invalid_argument);
    EXPECT_THROW(latticeCubic.getEdgeIndex(vertexIndex, "xz", sign), std::invalid_argument);
    EXPECT_THROW(latticeCubic.getEdgeIndex(vertexIndex, "yz", sign), std::invalid_argument);
    EXPECT_THROW(latticeCubic.getEdgeIndex(vertexIndex, "xyz", sign), std::invalid_argument);

    type = "rhombic";
    Lattice latticeRhombic = Lattice(l, type);
    EXPECT_THROW(latticeRhombic.getEdgeIndex(vertexIndex, "x", sign), std::invalid_argument);
    EXPECT_THROW(latticeRhombic.getEdgeIndex(vertexIndex, "y", sign), std::invalid_argument);
    EXPECT_THROW(latticeRhombic.getEdgeIndex(vertexIndex, "z", sign), std::invalid_argument);
}

TEST(getEdgeIndex, ExceptsInvalidSigns)
{
    int l = 4;
    std::string type = "rhombic";
    Lattice latticeCubic = Lattice(l, type);
    int vertexIndex = 56;
    std::string direction = "yz";
    EXPECT_THROW(latticeCubic.getEdgeIndex(vertexIndex, direction, 0), std::invalid_argument);
    EXPECT_THROW(latticeCubic.getEdgeIndex(vertexIndex, direction, -0), std::invalid_argument);
    EXPECT_THROW(latticeCubic.getEdgeIndex(vertexIndex, direction, 7), std::invalid_argument);
    EXPECT_THROW(latticeCubic.getEdgeIndex(vertexIndex, direction, -6), std::invalid_argument);
}

TEST(getEdgeIndex, HandlesValidInput)
{
    int l = 6;
    std::string type = "bcc";
    Lattice latticeBCC = Lattice(l, type);
    int vertexIndex = 3;
    int sign = 1;
    EXPECT_EQ(latticeBCC.getEdgeIndex(vertexIndex, "x", sign), 22);
    EXPECT_EQ(latticeBCC.getEdgeIndex(vertexIndex, "y", sign), 24);
    EXPECT_EQ(latticeBCC.getEdgeIndex(vertexIndex, "z", sign), 26);
    EXPECT_EQ(latticeBCC.getEdgeIndex(vertexIndex, "xy", sign), 23);
    EXPECT_EQ(latticeBCC.getEdgeIndex(vertexIndex, "xz", sign), 27);
    EXPECT_EQ(latticeBCC.getEdgeIndex(vertexIndex, "yz", sign), 25);
    EXPECT_EQ(latticeBCC.getEdgeIndex(vertexIndex, "xyz", sign), 21);

    vertexIndex = 7;
    EXPECT_EQ(latticeBCC.getEdgeIndex(vertexIndex, "x", -sign), 43);
    EXPECT_EQ(latticeBCC.getEdgeIndex(vertexIndex, "y", -sign), 10);
    EXPECT_EQ(latticeBCC.getEdgeIndex(vertexIndex, "z", -sign), 1314);
    EXPECT_EQ(latticeBCC.getEdgeIndex(vertexIndex, "xy", -sign), 1514);
    EXPECT_EQ(latticeBCC.getEdgeIndex(vertexIndex, "xz", -sign), 2820);
    EXPECT_EQ(latticeBCC.getEdgeIndex(vertexIndex, "yz", -sign), 2783);
    EXPECT_EQ(latticeBCC.getEdgeIndex(vertexIndex, "xyz", -sign), 2772);
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
    EXPECT_EQ(faceToVertices[0], vertices);
    vint edges = {0, 1, 11};
    EXPECT_EQ(faceToEdges[0], edges);

    vertices = {0, 64, 112};
    EXPECT_EQ(faceToVertices[1], vertices);
    edges = {0, 2, 789};
    EXPECT_EQ(faceToEdges[1], edges);

    vertices = {0, 4, 64};
    EXPECT_EQ(faceToVertices[2], vertices);
    edges = {0, 3, 34};
    EXPECT_EQ(faceToEdges[2], edges);

    vertices = {0, 64, 67};
    EXPECT_EQ(faceToVertices[3], vertices);
    edges = {0, 4, 470};
    EXPECT_EQ(faceToEdges[3], edges);

    vertices = {0, 16, 64};
    EXPECT_EQ(faceToVertices[4], vertices);
    edges = {0, 5, 114};
    EXPECT_EQ(faceToEdges[4], edges);

    vertices = {0, 64, 76};
    EXPECT_EQ(faceToVertices[5], vertices);
    edges = {0, 6, 535};
    EXPECT_EQ(faceToEdges[5], edges);

    vertices = {0, 1, 112};
    EXPECT_EQ(faceToVertices[6], vertices);
    edges = {1, 2, 790};
    EXPECT_EQ(faceToEdges[6], edges);

    vertices = {0, 1, 76};
    EXPECT_EQ(faceToVertices[7], vertices);
    edges = {1, 6, 534};
    EXPECT_EQ(faceToEdges[7], edges);

    vertices = {0, 4, 112};
    EXPECT_EQ(faceToVertices[8], vertices);
    edges = {2, 3, 788};
    EXPECT_EQ(faceToEdges[8], edges);

    vertices = {0, 4, 67};
    EXPECT_EQ(faceToVertices[9], vertices);
    edges = {3, 4, 471};
    EXPECT_EQ(faceToEdges[9], edges);

    vertices = {0, 16, 76};
    EXPECT_EQ(faceToVertices[10], vertices);
    edges = {5, 6, 536};
    EXPECT_EQ(faceToEdges[10], edges);

    vertices = {0, 16, 67};
    EXPECT_EQ(faceToVertices[11], vertices);
    edges = {4, 5, 475};
    EXPECT_EQ(faceToEdges[11], edges);

    vertices = {9, 72, 73};
    EXPECT_EQ(faceToVertices[111], vertices);
    edges = {63, 67, 505};
    EXPECT_EQ(faceToEdges[111], edges);

    vertices = {20, 21, 64};
    EXPECT_EQ(faceToVertices[771], vertices);
    edges = {141, 448, 452};
    EXPECT_EQ(faceToEdges[771], edges);
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
    EXPECT_EQ(faceToVertices[0], vertices);
    vint edges = {0, 4, 1516, 1547};
    EXPECT_EQ(faceToEdges[0], edges);

    vertices = {0, 37, 216, 246};
    EXPECT_EQ(faceToVertices[1], vertices);
    edges = {0, 6, 1518, 1722};
    EXPECT_EQ(faceToEdges[1], edges);

    vertices = {0, 7, 216, 396};
    EXPECT_EQ(faceToVertices[2], vertices);
    edges = {0, 2, 1514, 2772};
    EXPECT_EQ(faceToEdges[2], edges);

    vertices = {0, 186, 396, 401};
    EXPECT_EQ(faceToVertices[3], vertices);
    edges = {2, 1308, 2809, 2813};
    EXPECT_EQ(faceToEdges[3], edges);

    vertices = {0, 181, 396, 426};
    EXPECT_EQ(faceToVertices[4], vertices);
    edges = {2, 1271, 2984, 2986};
    EXPECT_EQ(faceToEdges[4], edges);

    vertices = {0, 31, 246, 426};
    EXPECT_EQ(faceToVertices[5], vertices);
    edges = {6, 221, 2986, 2988};
    EXPECT_EQ(faceToEdges[5], edges);

    vertices = {167, 168, 347, 383};
    EXPECT_EQ(faceToVertices[500], vertices);
    edges = {1169, 1171, 2429, 2683};
    EXPECT_EQ(faceToEdges[500], edges);

    vertices = {137, 167, 346, 347};
    EXPECT_EQ(faceToVertices[501], vertices);
    edges = {965, 1171, 2424, 2428};
    EXPECT_EQ(faceToEdges[501], edges);

    vint faceIndices = {0, 1, 2, 3, 4, 5, 35, 104, 124, 201, 553, 630};
    int i = 0;
    auto vertexToFaces = latticeRhombic.getVertexToFaces();
    for (const auto &face : vertexToFaces[0])
    {
        EXPECT_EQ(face.faceIndex, faceIndices[i]);
        ++i;
    }

    faceIndices = {240, 259, 350, 366, 367, 368, 369, 370, 371, 389, 459, 478};
    i = 0;
    for (const auto &face : vertexToFaces[123])
    {
        EXPECT_EQ(face.faceIndex, faceIndices[i]);
        ++i;
    }

    faceIndices = {0, 1, 2, 111, 130, 131};
    i = 0;
    for (const auto &face : vertexToFaces[216])
    {
        EXPECT_EQ(face.faceIndex, faceIndices[i]);
        ++i;
    }

    faceIndices = {132, 145, 149, 236, 237, 238};
    i = 0;
    for (const auto &face : vertexToFaces[259])
    {
        EXPECT_EQ(face.faceIndex, faceIndices[i]);
        ++i;
    }
}

TEST(createFaces, correctMaxEdgeIndex)
{
    vint ls = {4, 6, 8, 10};
    for (auto l : ls)
    {
        std::string type = "rhombic";
        Lattice latticeRhombic = Lattice(l, type);
        latticeRhombic.createFaces();
        vvint faceToEdges = latticeRhombic.getFaceToEdges();
        int maxEdgeIndex = 0;
        for (const auto &edges : faceToEdges)
        {
            for (auto edgeIndex : edges)
            {
                if (edgeIndex > maxEdgeIndex)
                    maxEdgeIndex = edgeIndex;
            }
        }
        EXPECT_EQ(maxEdgeIndex, 7 * (2 * l * l * l - 1));
    }
}

TEST(createFaces, correctMaxVertexIndex)
{
    vint ls = {4, 6, 8, 10};
    for (auto l : ls)
    {
        std::string type = "rhombic";
        Lattice latticeRhombic = Lattice(l, type);
        latticeRhombic.createFaces();
        vvint faceToVertices = latticeRhombic.getFaceToVertices();
        int maxVertexIndex = 0;
        for (const auto &vertices : faceToVertices)
        {
            for (auto vertexIndex : vertices)
            {
                if (vertexIndex > maxVertexIndex)
                    maxVertexIndex = vertexIndex;
            }
        }
        EXPECT_EQ(maxVertexIndex, (2 * l * l * l - 1));
    }
}

TEST(createFaces, correctNumberOfFaces)
{
    // Checked this on 29/08/18 with the preallocation turned off
    // Preallocation is turned on by default for speed purposes
    vint ls = {4, 6, 8, 10};
    for (auto l : ls)
    {
        std::string type = "bcc";
        Lattice latticeBCC = Lattice(l, type);
        latticeBCC.createFaces();
        vvint faceToVertices = latticeBCC.getFaceToVertices();
        vvint faceToEdges = latticeBCC.getFaceToEdges();
        EXPECT_EQ(faceToVertices.size(), 24 * l * l * l);
        EXPECT_EQ(faceToEdges.size(), 24 * l * l * l);

        type = "rhombic";
        Lattice latticeRhombic = Lattice(l, type);
        latticeRhombic.createFaces();
        faceToVertices = latticeRhombic.getFaceToVertices();
        faceToEdges = latticeRhombic.getFaceToEdges();
        EXPECT_EQ(faceToVertices.size(), 3 * l * l * l);
        EXPECT_EQ(faceToEdges.size(), 3 * l * l * l);
    }
}

TEST(createFaces, eachVertexinCorrectNumberOfFacesRhombicLattice)
{
    vint ls = {4, 6, 8, 10};
    for (auto l : ls)
    {
        std::string type = "rhombic";
        Lattice latticeRhombic = Lattice(l, type);
        latticeRhombic.createFaces();
        auto vertexToFaces = latticeRhombic.getVertexToFaces();
        for (int i = 0; i < 2 * l * l * l; ++i)
        {
            auto faces = vertexToFaces[i];
            cartesian4 coordinate = latticeRhombic.indexToCoordinate(i);
            if (coordinate.w == 1)
            {
                EXPECT_EQ(faces.size(), 6);
            }
            else if ((coordinate.x + coordinate.y + coordinate.z) % 2 == 0)
            {
                EXPECT_EQ(faces.size(), 12);
            }
            else
            {
                EXPECT_EQ(faces.size(), 0);
            }
        }
    }
}

TEST(createUpEdgesMap, verticesHaveCorrectNumberOfUpEdges)
{
    vint ls = {4, 6, 8, 10};
    for (auto l : ls)
    {
        std::string type = "rhombic";
        Lattice latticeRhombic = Lattice(l, type);
        latticeRhombic.createUpEdgesMap();
        auto upEdgesMap = latticeRhombic.getUpEdgesMap();
        std::vector<std::string> directionList = {"xyz", "-xy", "-xz", "-yz"};
        for (const auto &direction : directionList)
        {
            vvint vertexToUpEdges = upEdgesMap[direction];
            for (int i = 0; i < 2 * l * l * l; ++i)
            {
                cartesian4 coordinate = latticeRhombic.indexToCoordinate(i);
                if (coordinate.w == 0)
                {
                    if ((coordinate.x + coordinate.y + coordinate.z) % 2 == 0)
                    {
                        EXPECT_EQ(vertexToUpEdges[i].size(), 4);
                    }
                    else
                    {
                        EXPECT_EQ(vertexToUpEdges[i].size(), 0);
                    }
                }
                else
                {
                    if ((coordinate.x + coordinate.y + coordinate.z) % 2 == 0)
                    {
                        EXPECT_EQ(vertexToUpEdges[i].size(), 3);
                    }
                    else
                    {
                        EXPECT_EQ(vertexToUpEdges[i].size(), 0);
                    }
                }
            }
        }
        directionList = {"-xyz", "xy", "xz", "yz"};
        for (const auto &direction : directionList)
        {
            vvint vertexToUpEdges = upEdgesMap["xy"];
            for (int i = 0; i < 2 * l * l * l; ++i)
            {
                cartesian4 coordinate = latticeRhombic.indexToCoordinate(i);
                if (coordinate.w == 0)
                {
                    if ((coordinate.x + coordinate.y + coordinate.z) % 2 == 0)
                    {
                        EXPECT_EQ(vertexToUpEdges[i].size(), 4);
                    }
                    else
                    {
                        EXPECT_EQ(vertexToUpEdges[i].size(), 0);
                    }
                }
                else
                {
                    if ((coordinate.x + coordinate.y + coordinate.z) % 2 == 0)
                    {
                        EXPECT_EQ(vertexToUpEdges[i].size(), 0);
                    }
                    else
                    {
                        EXPECT_EQ(vertexToUpEdges[i].size(), 3);
                    }
                }
            }
        }
    }
}

TEST(creatUpEdgesMap, correctEdgesCreated)
{
    int l = 4;
    std::string type = "rhombic";
    Lattice latticeRhombic = Lattice(l, type);
    latticeRhombic.createUpEdgesMap();
    auto upEdgesMap = latticeRhombic.getUpEdgesMap();

    vvint vertexToUpEdges = upEdgesMap["-xy"];
    vint upEdges = vertexToUpEdges[0];
    std::sort(upEdges.begin(), upEdges.end());
    vint expectedEdges = {4, 6, 555, 889};
    for (int i = 0; i < 4; ++i)
    {
        EXPECT_EQ(upEdges[i], expectedEdges[i]);
    }
    upEdges = vertexToUpEdges[64];
    std::sort(upEdges.begin(), upEdges.end());
    expectedEdges = {0, 452, 454};
    for (int i = 0; i < 4; ++i)
    {
        EXPECT_EQ(upEdges[i], expectedEdges[i]);
    }

    vertexToUpEdges = upEdgesMap["yz"];
    upEdges = vertexToUpEdges[0];
    std::sort(upEdges.begin(), upEdges.end());
    expectedEdges = {0, 4, 555, 811};
    for (int i = 0; i < 4; ++i)
    {
        EXPECT_EQ(upEdges[i], expectedEdges[i]);
    }
    upEdges = vertexToUpEdges[65];
    std::sort(upEdges.begin(), upEdges.end());
    expectedEdges = {41, 121, 455};
    for (int i = 0; i < 4; ++i)
    {
        EXPECT_EQ(upEdges[i], expectedEdges[i]);
    }
}

TEST(findFace, ExceptsInvalidVertexListSize)
{
    int l = 8;
    std::string type = "rhombic";
    Lattice latticeRhombic = Lattice(l, type);
    latticeRhombic.createFaces();
    vint badVertexList = {1, 2, 3};
    EXPECT_THROW(latticeRhombic.findFace(badVertexList), std::invalid_argument);
    badVertexList = {1, 2, 3, 4, 5};
    EXPECT_THROW(latticeRhombic.findFace(badVertexList), std::invalid_argument);
}

TEST(findFace, correctFacesFound)
{
    int l = 8;
    std::string type = "rhombic";
    Lattice latticeRhombic = Lattice(l, type);
    latticeRhombic.createFaces();
    vint expectedVertices = {0, 504, 1016, 1023};
    EXPECT_EQ(latticeRhombic.findFace(expectedVertices), 1512);

    expectedVertices = {0, 9, 512, 960};
    EXPECT_EQ(latticeRhombic.findFace(expectedVertices), 2);
}

TEST(createVertexToEdges, correctEdgesCreated)
{
    int l = 6;
    std::string type = "rhombic";
    Lattice latticeRhombic = Lattice(l, type);
    latticeRhombic.createVertexToEdges();
    vint expectedEdges = {14, 16, 18, 20, 1731, 2785, 2989, 3000};
    vvint vertexToEdges = latticeRhombic.getVertexToEdges();
    vint edges = vertexToEdges[2];
    EXPECT_EQ(edges.size(), expectedEdges.size());
    std::sort(edges.begin(), edges.end());
    for (int i = 0; i < edges.size(); ++i)
    {
        EXPECT_EQ(edges[i], expectedEdges[i]);
    }

    expectedEdges = {0, 1514, 1516, 1518};
    edges = vertexToEdges[216];
    EXPECT_EQ(edges.size(), expectedEdges.size());
    std::sort(edges.begin(), edges.end());
    for (int i = 0; i < edges.size(); ++i)
    {
        EXPECT_EQ(edges[i], expectedEdges[i]);
    }

    expectedEdges = {18, 55, 261, 1519};
    edges = vertexToEdges[217];
    EXPECT_EQ(edges.size(), expectedEdges.size());
    std::sort(edges.begin(), edges.end());
    for (int i = 0; i < edges.size(); ++i)
    {
        EXPECT_EQ(edges[i], expectedEdges[i]);
    }
}

TEST(createVertexToEdges, correctNumberOfEdgesForEachVertex)
{
    int l = 6;
    std::string type = "rhombic";
    Lattice latticeRhombic = Lattice(l, type);
    latticeRhombic.createVertexToEdges();
    vvint vertexToEdges = latticeRhombic.getVertexToEdges();
    for (int vertexIndex = 0; vertexIndex < 2 * l * l * l; ++vertexIndex)
    {
        cartesian4 coordinate = latticeRhombic.indexToCoordinate(vertexIndex);
        if (coordinate.w == 0)
        {
            if ((coordinate.x + coordinate.y + coordinate.z) % 2 == 0)
            {
                EXPECT_EQ(vertexToEdges[vertexIndex].size(), 8);
            }
        }
        else
        {
            EXPECT_EQ(vertexToEdges[vertexIndex].size(), 4);
        }
    }
}