#include "rhombicToricLattice.h"
#include "lattice.h"
#include <string>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <map>

RhombicToricLattice::RhombicToricLattice(const int length) : Lattice(length)
{
    if (length % 2 != 0)
    {
        throw std::invalid_argument("Rhombic toric lattices can only have even L.");
    }
    faceToVertices.reserve(3 * l * l * l);
    faceToEdges.reserve(3 * l * l * l);
    // Not all vertices present in this lattice, but all w=1 faces
    // are present, so the possible vertex indices go from
    // 0 to l^3 -1
    vertexToFaces.assign(2 * l * l * l, {});
    vertexToEdges.assign(2 * l * l * l, {});
}

int RhombicToricLattice::neighbour(const int vertexIndex, const std::string &direction, const int sign)
{
    if (!(sign == 1 || sign == -1))
    {
        throw std::invalid_argument("Sign must be either 1 or -1.");
    }
    if (!(direction == "xy" || direction == "xz" || direction == "yz" ||
          direction == "xyz"))
    {
        throw std::invalid_argument("Direction must be one of 'xy', 'xz', 'yz' or 'xyz'.");
    }
    cartesian4 coordinate;
    coordinate = indexToCoordinate(vertexIndex);
    // if (direction == "x")
    //     coordinate.x = (coordinate.x + sign + l) % l;
    // if (direction == "y")
    //     coordinate.y = (coordinate.y + sign + l) % l;
    // if (direction == "z")
    //     coordinate.z = (coordinate.z + sign + l) % l;
    if (coordinate.w == 1)
    {
        if (direction == "xy")
        {
            coordinate.x = (coordinate.x + (sign > 0)) % l;
            coordinate.y = (coordinate.y + (sign > 0)) % l;
            coordinate.z = (coordinate.z + (sign < 0)) % l;
            coordinate.w = 0;
        }
        if (direction == "xz")
        {
            coordinate.x = (coordinate.x + (sign > 0)) % l;
            coordinate.z = (coordinate.z + (sign > 0)) % l;
            coordinate.y = (coordinate.y + (sign < 0)) % l;
            coordinate.w = 0;
        }
        if (direction == "yz")
        {
            coordinate.y = (coordinate.y + (sign > 0)) % l;
            coordinate.z = (coordinate.z + (sign > 0)) % l;
            coordinate.x = (coordinate.x + (sign < 0)) % l;
            coordinate.w = 0;
        }
        if (direction == "xyz")
        {
            coordinate.x = (coordinate.x + (sign > 0)) % l;
            coordinate.y = (coordinate.y + (sign > 0)) % l;
            coordinate.z = (coordinate.z + (sign > 0)) % l;
            coordinate.w = 0;
        }
    }
    else
    {
        if (direction == "xy")
        {
            coordinate.x = (coordinate.x - (sign < 0) + l) % l;
            coordinate.y = (coordinate.y - (sign < 0) + l) % l;
            coordinate.z = (coordinate.z - (sign > 0) + l) % l;
            coordinate.w = 1;
        }
        if (direction == "xz")
        {
            coordinate.x = (coordinate.x - (sign < 0) + l) % l;
            coordinate.z = (coordinate.z - (sign < 0) + l) % l;
            coordinate.y = (coordinate.y - (sign > 0) + l) % l;
            coordinate.w = 1;
        }
        if (direction == "yz")
        {
            coordinate.y = (coordinate.y - (sign < 0) + l) % l;
            coordinate.z = (coordinate.z - (sign < 0) + l) % l;
            coordinate.x = (coordinate.x - (sign > 0) + l) % l;
            coordinate.w = 1;
        }
        if (direction == "xyz")
        {
            coordinate.x = (coordinate.x - (sign < 0) + l) % l;
            coordinate.y = (coordinate.y - (sign < 0) + l) % l;
            coordinate.z = (coordinate.z - (sign < 0) + l) % l;
            coordinate.w = 1;
        }
    }
    return coordinateToIndex(coordinate);
}

int RhombicToricLattice::edgeIndex(const int vertexIndex, const std::string &direction, const int sign)
{
    if (!(sign == 1 || sign == -1))
    {
        throw std::invalid_argument("Sign must be either 1 or -1.");
    }
    if (!(direction == "xy" || direction == "xz" || direction == "yz" ||
          direction == "xyz"))
    {
        throw std::invalid_argument("Direction must be one of 'xy', 'xz', 'yz' or 'xyz'.");
    }

    int edgeIndex;
    if (sign < 0)
    {
        edgeIndex = neighbour(vertexIndex, direction, sign);
    }
    else
    {
        edgeIndex = vertexIndex;
    }
    // Numbering is an arbitrary convention
    if (direction == "xyz")
        edgeIndex = 7 * edgeIndex;
    // else if (direction == "x")
    //     edgeIndex = 7 * edgeIndex + 1;
    else if (direction == "xy")
        edgeIndex = 7 * edgeIndex + 2;
    // else if (direction == "y")
    //     edgeIndex = 7 * edgeIndex + 3;
    else if (direction == "yz")
        edgeIndex = 7 * edgeIndex + 4;
    // else if (direction == "z")
    //     edgeIndex = 7 * edgeIndex + 5;
    else if (direction == "xz")
        edgeIndex = 7 * edgeIndex + 6;
    return edgeIndex;
}

void RhombicToricLattice::addFace(const int vertexIndex, const int faceIndex, const vstr &directions, const vint &signs)
{
    vint vertices;
    vint edges;
    int neighbourVertex = neighbour(vertexIndex, directions[0], signs[0]);
    vertices = {vertexIndex, neighbourVertex,
                neighbour(vertexIndex, directions[1], signs[1]),
                neighbour(neighbourVertex, directions[2], signs[2])};
    edges = {edgeIndex(vertexIndex, directions[0], signs[0]),
                edgeIndex(vertexIndex, directions[1], signs[1]),
                edgeIndex(neighbourVertex, directions[2], signs[2]),
                edgeIndex(vertices[2], directions[3], signs[3])};    
    faceS face;
    std::sort(vertices.begin(), vertices.end());
    std::sort(edges.begin(), edges.end());
    face.vertices = vertices;
    face.faceIndex = faceIndex;

    faceToVertices.push_back(vertices);
    faceToEdges.push_back(edges);
    for (const auto &vertex : vertices)
    {
        vertexToFaces[vertex].push_back(face);
    }
}

void RhombicToricLattice::createFaces()
{
    int faceIndex = 0;
    for (int vertexIndex = 0; vertexIndex < l * l * l; ++vertexIndex)
    {
        cartesian4 coordinate = indexToCoordinate(vertexIndex);
        if ((coordinate.x + coordinate.y + coordinate.z) % 2 == 0)
        {
            vint signs = {1, 1, 1, 1};
            addFace(vertexIndex, faceIndex, {"xyz", "yz", "yz", "xyz"},
                    signs);
            faceIndex++;
            addFace(vertexIndex, faceIndex, {"xyz", "xz", "xz", "xyz"},
                    signs);
            faceIndex++;
            addFace(vertexIndex, faceIndex, {"xyz", "xy", "xy", "xyz"},
                    signs);
            faceIndex++;
            signs = {1, -1, -1, 1};
            addFace(vertexIndex, faceIndex, {"xy", "xz", "xz", "xy"},
                    signs);
            faceIndex++;
            addFace(vertexIndex, faceIndex, {"xy", "yz", "yz", "xy"},
                    signs);
            faceIndex++;
            addFace(vertexIndex, faceIndex, {"xz", "yz", "yz", "xz"},
                    signs);
            faceIndex++;
        }
    }
}

int RhombicToricLattice::findFace(vint &vertices)
{
    if (vertices.size() != 4)
    {
        throw std::invalid_argument("Vertex list must contain exactly four vertices.");
    }
    std::sort(vertices.begin(), vertices.end());
    auto v0Faces = vertexToFaces[vertices[0]];
    for (const auto &face : v0Faces)
    {
        if (face.vertices == vertices)
        {
            return face.faceIndex;
        }
    }
    throw std::invalid_argument("Input vertices do not correspond to a face.");
}

void RhombicToricLattice::createUpEdgesMap()
{
    std::vector<std::string> directionList = {"xyz", "xy", "xz", "yz",
                                              "-xyz", "-xy", "-xz", "-yz"};
    for (const auto &direction : directionList)
    {
        vvint vertexToUpEdges;
        vertexToUpEdges.assign(2 * l * l * l, {});
        for (int vertexIndex = 0; vertexIndex < 2 * l * l * l; ++vertexIndex)
        {
            cartesian4 coordinate = indexToCoordinate(vertexIndex);
            if (coordinate.w == 0)
            {
                if ((coordinate.x + coordinate.y + coordinate.z) % 2 == 0)
                {
                    if (direction == "xyz")
                    {
                        // Third argument is sign
                        vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "xyz", 1));
                        vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "xy", 1));
                        vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "xz", 1));
                        vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "yz", 1));
                    }
                    else if (direction == "yz")
                    {
                        vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "yz", 1));
                        vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "xyz", 1));
                        vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "xy", -1));
                        vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "xz", -1));
                    }
                    else if (direction == "xz")
                    {
                        vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "xyz", 1));
                        vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "xz", 1));
                        vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "xy", -1));
                        vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "yz", -1));
                    }
                    else if (direction == "xy")
                    {
                        vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "xyz", 1));
                        vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "xy", 1));
                        vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "xz", -1));
                        vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "yz", -1));
                    }
                    else if (direction == "-xyz")
                    {
                        vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "xyz", -1));
                        vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "xz", -1));
                        vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "xy", -1));
                        vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "yz", -1));
                    }
                    else if (direction == "-yz")
                    {
                        vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "xy", 1));
                        vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "xz", 1));
                        vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "xyz", -1));
                        vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "yz", -1));
                    }
                    else if (direction == "-xz")
                    {
                        vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "xy", 1));
                        vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "yz", 1));
                        vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "xyz", -1));
                        vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "xz", -1));
                    }
                    else if (direction == "-xy")
                    {
                        vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "xz", 1));
                        vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "yz", 1));
                        vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "xyz", -1));
                        vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "xy", -1));
                    }
                }
            }
            else
            {
                if ((coordinate.x + coordinate.y + coordinate.z) % 2 == 0)
                {
                    if (direction == "xy" || direction == "xz" || direction == "yz" || direction == "-xyz")
                    {
                        // Only one up edge, so return an empty vector as no sweep will happen here.
                    }
                    else
                    {
                        if (direction == "xyz")
                        {
                            vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "xy", 1));
                            vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "xz", 1));
                            vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "yz", 1));
                        }
                        else if (direction == "-xy")
                        {
                            vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "xyz", -1));
                            vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "xz", 1));
                            vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "yz", 1));
                        }
                        else if (direction == "-xz")
                        {
                            vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "xyz", -1));
                            vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "xy", 1));
                            vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "yz", 1));
                        }
                        else if (direction == "-yz")
                        {
                            vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "xyz", -1));
                            vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "xz", 1));
                            vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "xy", 1));
                        }
                    }
                }
                else
                {
                    if (direction == "-xy" || direction == "-xz" || direction == "-yz" || direction == "xyz")
                    {
                        // Only one up edge, so return an empty vector as no sweep will happen here.
                    }
                    else
                    {
                        if (direction == "-xyz")
                        {
                            vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "xy", -1));
                            vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "xz", -1));
                            vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "yz", -1));
                        }
                        else if (direction == "xy")
                        {
                            vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "xyz", 1));
                            vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "xz", -1));
                            vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "yz", -1));
                        }
                        else if (direction == "xz")
                        {
                            vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "xyz", 1));
                            vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "xy", -1));
                            vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "yz", -1));
                        }
                        else if (direction == "yz")
                        {
                            vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "xyz", 1));
                            vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "xz", -1));
                            vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "xy", -1));
                        }
                    }
                }
            }
        }
        upEdgesMap.insert(std::pair<std::string, vvint>(direction, vertexToUpEdges));
    }
}

void RhombicToricLattice::createVertexToEdges()
{
    for (int vertexIndex = 0; vertexIndex < 2 * l * l * l; ++vertexIndex)
    {
        cartesian4 coordinate = indexToCoordinate(vertexIndex);
        if (coordinate.w == 0)
        {
            if ((coordinate.x + coordinate.y + coordinate.z) % 2 == 0)
            {
                int sign = 1;
                vertexToEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "xyz", sign));
                vertexToEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "xy", sign));
                vertexToEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "xz", sign));
                vertexToEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "yz", sign));
                sign = -1;
                vertexToEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "xyz", sign));
                vertexToEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "xy", sign));
                vertexToEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "xz", sign));
                vertexToEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "yz", sign));
            }
        }
        else
        {
            if ((coordinate.x + coordinate.y + coordinate.z) % 2 == 0)
            {
                int sign = 1;
                vertexToEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "xy", sign));
                vertexToEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "xz", sign));
                vertexToEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "yz", sign));
                sign = -1;
                vertexToEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "xyz", sign));
            }
            else
            {
                int sign = -1;
                vertexToEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "xy", sign));
                vertexToEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "xz", sign));
                vertexToEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "yz", sign));
                sign = 1;
                vertexToEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "xyz", sign));
            }
        }
    }
}