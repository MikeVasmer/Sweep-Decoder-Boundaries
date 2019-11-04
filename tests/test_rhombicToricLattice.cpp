#include "rhombicToricLattice.h"
#include "gtest/gtest.h"
#include <string>
#include <algorithm>

TEST(RhombicToricLattice, excepts_odd_L)
{
    int l = 3;
    EXPECT_THROW(RhombicToricLattice lattice = RhombicToricLattice(l), std::invalid_argument);
}

TEST(neighbour, excepts_invalid_directions)
{
    int l = 4;
    RhombicToricLattice lattice = RhombicToricLattice(l);
    int vertexIndex = 0;
    int sign = 1;
    std::string direction = "x";
    EXPECT_THROW(lattice.neighbour(vertexIndex, direction, sign), std::invalid_argument);
}

// TEST(neighbour, excepts_InvalidLatticeDirectionCombinations)
// {
//     int l = 4;
//     RhombicToricLattice lattice = RhombicToricLattice(l);
//     int vertexIndex = 0;
//     int sign = 1;

//     EXPECT_THROW(lattice.neighbour(vertexIndex, "x", sign), std::invalid_argument);
//     EXPECT_THROW(lattice.neighbour(vertexIndex, "y", sign), std::invalid_argument);
//     EXPECT_THROW(lattice.neighbour(vertexIndex, "z", sign), std::invalid_argument);
// }

TEST(neighbour, handles_valid_input)
{
    int l = 4;
    RhombicToricLattice lattice = RhombicToricLattice(l);
    int vertexIndex = 0;
    int sign = 1;
    EXPECT_EQ(lattice.neighbour(vertexIndex, "xy", sign), 112);
    EXPECT_EQ(lattice.neighbour(vertexIndex, "yz", sign), 67);
    EXPECT_EQ(lattice.neighbour(vertexIndex, "xz", sign), 76);
    EXPECT_EQ(lattice.neighbour(vertexIndex, "xy", -sign), 79);
    EXPECT_EQ(lattice.neighbour(vertexIndex, "yz", -sign), 124);
    EXPECT_EQ(lattice.neighbour(vertexIndex, "xz", -sign), 115);
    EXPECT_EQ(lattice.neighbour(vertexIndex, "xyz", sign), 64);
    EXPECT_EQ(lattice.neighbour(vertexIndex, "xyz", -sign), 127);
    // EXPECT_EQ(latticeBCC.neighbour(vertexIndex, "x", sign), 1);
    // EXPECT_EQ(latticeBCC.neighbour(vertexIndex, "y", sign), 4);
    // EXPECT_EQ(latticeBCC.neighbour(vertexIndex, "z", sign), 16);
    // EXPECT_EQ(latticeBCC.neighbour(vertexIndex, "x", -sign), 3);
    // EXPECT_EQ(latticeBCC.neighbour(vertexIndex, "y", -sign), 12);
    // EXPECT_EQ(latticeBCC.neighbour(vertexIndex, "z", -sign), 48);

    vertexIndex = 64;
    EXPECT_EQ(lattice.neighbour(vertexIndex, "xy", sign), 5);
    EXPECT_EQ(lattice.neighbour(vertexIndex, "yz", sign), 20);
    EXPECT_EQ(lattice.neighbour(vertexIndex, "xz", sign), 17);
    EXPECT_EQ(lattice.neighbour(vertexIndex, "xy", -sign), 16);
    EXPECT_EQ(lattice.neighbour(vertexIndex, "yz", -sign), 1);
    EXPECT_EQ(lattice.neighbour(vertexIndex, "xz", -sign), 4);
    EXPECT_EQ(lattice.neighbour(vertexIndex, "xyz", sign), 21);
    EXPECT_EQ(lattice.neighbour(vertexIndex, "xyz", -sign), 0);
    // EXPECT_EQ(latticeBCC.neighbour(vertexIndex, "x", sign), 65);
    // EXPECT_EQ(latticeBCC.neighbour(vertexIndex, "y", sign), 68);
    // EXPECT_EQ(latticeBCC.neighbour(vertexIndex, "z", sign), 80);
    // EXPECT_EQ(latticeBCC.neighbour(vertexIndex, "x", -sign), 67);
    // EXPECT_EQ(latticeBCC.neighbour(vertexIndex, "y", -sign), 76);
    // EXPECT_EQ(latticeBCC.neighbour(vertexIndex, "z", -sign), 112);
}

TEST(edgeIndex, excepts_invalid_directions)
{
    int l = 6;
    RhombicToricLattice lattice = RhombicToricLattice(l);
    int vertexIndex = 0;
    int sign = 1;
    std::string direction = "north";
    EXPECT_THROW(lattice.edgeIndex(vertexIndex, direction, sign), std::invalid_argument);
}

// TEST(edgeIndex, excepts_InvalidLatticeDirectionCombinations)
// {
//     int l = 6;
//     int vertexIndex = 5;
//     int sign = 1;

//     RhombicToricLattice lattice = RhombicToricLattice(l);
//     EXPECT_THROW(lattice.edgeIndex(vertexIndex, "x", sign), std::invalid_argument);
//     EXPECT_THROW(lattice.edgeIndex(vertexIndex, "y", sign), std::invalid_argument);
//     EXPECT_THROW(lattice.edgeIndex(vertexIndex, "z", sign), std::invalid_argument);

//     // CubicLattice latticeCubic = RhombicToricLattice(l);
//     // EXPECT_THROW(latticeCubic.edgeIndex(vertexIndex, "xy", sign), std::invalid_argument);
//     // EXPECT_THROW(latticeCubic.edgeIndex(vertexIndex, "xz", sign), std::invalid_argument);
//     // EXPECT_THROW(latticeCubic.edgeIndex(vertexIndex, "yz", sign), std::invalid_argument);
//     // EXPECT_THROW(latticeCubic.edgeIndex(vertexIndex, "xyz", sign), std::invalid_argument);
// }

TEST(edgeIndex, excepts_invalid_signs)
{
    int l = 4;
    RhombicToricLattice lattice = RhombicToricLattice(l);
    int vertexIndex = 56;
    std::string direction = "yz";
    EXPECT_THROW(lattice.edgeIndex(vertexIndex, direction, 0), std::invalid_argument);
    EXPECT_THROW(lattice.edgeIndex(vertexIndex, direction, -0), std::invalid_argument);
    EXPECT_THROW(lattice.edgeIndex(vertexIndex, direction, 7), std::invalid_argument);
    EXPECT_THROW(lattice.edgeIndex(vertexIndex, direction, -6), std::invalid_argument);
}

TEST(edgeIndex, handles_valid_input)
{
    int l = 6;
    RhombicToricLattice lattice = RhombicToricLattice(l);
    int vertexIndex = 3;
    int sign = 1;
    EXPECT_EQ(lattice.edgeIndex(vertexIndex, "xy", sign), 23);
    EXPECT_EQ(lattice.edgeIndex(vertexIndex, "xz", sign), 27);
    EXPECT_EQ(lattice.edgeIndex(vertexIndex, "yz", sign), 25);
    EXPECT_EQ(lattice.edgeIndex(vertexIndex, "xyz", sign), 21);
    // EXPECT_EQ(latticeBCC.edgeIndex(vertexIndex, "x", sign), 22);
    // EXPECT_EQ(latticeBCC.edgeIndex(vertexIndex, "y", sign), 24);
    // EXPECT_EQ(latticeBCC.edgeIndex(vertexIndex, "z", sign), 26);

    vertexIndex = 7;
    EXPECT_EQ(lattice.edgeIndex(vertexIndex, "xy", -sign), 1514);
    EXPECT_EQ(lattice.edgeIndex(vertexIndex, "xz", -sign), 2820);
    EXPECT_EQ(lattice.edgeIndex(vertexIndex, "yz", -sign), 2783);
    EXPECT_EQ(lattice.edgeIndex(vertexIndex, "xyz", -sign), 2772);
    // EXPECT_EQ(latticeBCC.edgeIndex(vertexIndex, "x", -sign), 43);
    // EXPECT_EQ(latticeBCC.edgeIndex(vertexIndex, "y", -sign), 10);
    // EXPECT_EQ(latticeBCC.edgeIndex(vertexIndex, "z", -sign), 1314);
}

TEST(createFaces, handles_valid_input)
{
    int l = 6;
    RhombicToricLattice lattice = RhombicToricLattice(l);
    lattice.createFaces();
    vvint faceToVertices = lattice.getFaceToVertices();
    vvint faceToEdges = lattice.getFaceToEdges();

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
    auto vertexToFaces = lattice.getVertexToFaces();
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

TEST(createFaces, correct_max_edge_index)
{
    vint ls = {4, 6, 8, 10};
    for (auto l : ls)
    {
        RhombicToricLattice lattice = RhombicToricLattice(l);
        lattice.createFaces();
        vvint faceToEdges = lattice.getFaceToEdges();
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

TEST(createFaces, correct_max_vertex_index)
{
    vint ls = {4, 6, 8, 10};
    for (auto l : ls)
    {
        RhombicToricLattice lattice = RhombicToricLattice(l);
        lattice.createFaces();
        vvint faceToVertices = lattice.getFaceToVertices();
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

TEST(createFaces, correct_number_of_faces)
{
    // Checked this on 29/08/18 with the preallocation turned off
    // Preallocation is turned on by default for speed purposes
    vint ls = {4, 6, 8, 10};
    for (auto l : ls)
    {
        RhombicToricLattice lattice = RhombicToricLattice(l);
        lattice.createFaces();
        vvint faceToVertices = lattice.getFaceToVertices();
        vvint faceToEdges = lattice.getFaceToEdges();
        faceToVertices = lattice.getFaceToVertices();
        faceToEdges = lattice.getFaceToEdges();
        EXPECT_EQ(faceToVertices.size(), 3 * l * l * l);
        EXPECT_EQ(faceToEdges.size(), 3 * l * l * l);

        // BCCLattice latticeBCC = RhombicToricLattice(l);
        // latticeBCC.createFaces();
        // vvint faceToVertices = latticeBCC.getFaceToVertices();
        // vvint faceToEdges = latticeBCC.getFaceToEdges();
        // faceToVertices = lattice.getFaceToVertices();
        // faceToEdges = lattice.getFaceToEdges();
        // EXPECT_EQ(faceToVertices.size(), 24 * l * l * l);
        // EXPECT_EQ(faceToEdges.size(), 24 * l * l * l);
    }
}

TEST(createFaces, vertices_in_correct_number_of_faces)
{
    vint ls = {4, 6, 8, 10};
    for (auto l : ls)
    {
        RhombicToricLattice lattice = RhombicToricLattice(l);
        lattice.createFaces();
        auto vertexToFaces = lattice.getVertexToFaces();
        for (int i = 0; i < 2 * l * l * l; ++i)
        {
            auto faces = vertexToFaces[i];
            cartesian4 coordinate = lattice.indexToCoordinate(i);
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

TEST(createUpEdgesMap, vertices_have_correct_number_of_up_edges)
{
    vint ls = {4, 6, 8, 10};
    for (auto l : ls)
    {
        RhombicToricLattice lattice = RhombicToricLattice(l);
        lattice.createUpEdgesMap();
        auto upEdgesMap = lattice.getUpEdgesMap();
        std::vector<std::string> directionList = {"xyz", "-xy", "-xz", "-yz"};
        for (const auto &direction : directionList)
        {
            vvint vertexToUpEdges = upEdgesMap[direction];
            for (int i = 0; i < 2 * l * l * l; ++i)
            {
                cartesian4 coordinate = lattice.indexToCoordinate(i);
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
                cartesian4 coordinate = lattice.indexToCoordinate(i);
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

TEST(createUpEdgesMap, correct_edges_created)
{
    int l = 4;
    RhombicToricLattice lattice = RhombicToricLattice(l);
    lattice.createUpEdgesMap();
    auto upEdgesMap = lattice.getUpEdgesMap();

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

TEST(findFace, excepts_invalid_vertex_list_size)
{
    int l = 8;
    RhombicToricLattice lattice = RhombicToricLattice(l);
    lattice.createFaces();
    vint badVertexList = {1, 2, 3};
    EXPECT_THROW(lattice.findFace(badVertexList), std::invalid_argument);
    badVertexList = {1, 2, 3, 4, 5};
    EXPECT_THROW(lattice.findFace(badVertexList), std::invalid_argument);
}

TEST(findFace, correct_faces_found)
{
    int l = 8;
    RhombicToricLattice lattice = RhombicToricLattice(l);
    lattice.createFaces();
    vint expectedVertices = {0, 504, 1016, 1023};
    EXPECT_EQ(lattice.findFace(expectedVertices), 1512);

    expectedVertices = {0, 9, 512, 960};
    EXPECT_EQ(lattice.findFace(expectedVertices), 2);
}

TEST(createVertexToEdges, correct_edges_created)
{
    int l = 6;
    RhombicToricLattice lattice = RhombicToricLattice(l);
    lattice.createVertexToEdges();
    vint expectedEdges = {14, 16, 18, 20, 1731, 2785, 2989, 3000};
    vvint vertexToEdges = lattice.getVertexToEdges();
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

TEST(createVertexToEdges, correct_number_of_edges_each_vertex)
{
    int l = 6;
    RhombicToricLattice lattice = RhombicToricLattice(l);
    lattice.createVertexToEdges();
    vvint vertexToEdges = lattice.getVertexToEdges();
    for (int vertexIndex = 0; vertexIndex < 2 * l * l * l; ++vertexIndex)
    {
        cartesian4 coordinate = lattice.indexToCoordinate(vertexIndex);
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