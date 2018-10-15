#include "rhombicLattice.h"
#include "lattice.h"
#include <string>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <map>

RhombicLattice::RhombicLattice(const int length) : Lattice(length)
{
    if (length < 3)
    {
        throw std::invalid_argument("Lattice dimension l must be a positive integer greater than two.");
    }
    int numberOfFaces = 3 * pow(l - 1, 3) - 4 * pow(l - 1, 2) + 2 * (l - 1);
    faceToVertices.reserve(numberOfFaces);
    faceToEdges.reserve(numberOfFaces);
    vertexToFaces.assign(2 * l * l * l, {});
    vertexToEdges.assign(2 * l * l * l, {});
}

int RhombicLattice::neighbour(const int vertexIndex, const std::string &direction, const int sign)
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
    if (coordinate.w == 1)
    {
        if (direction == "xy")
        {
            coordinate.x = (coordinate.x + (sign > 0));
            coordinate.y = (coordinate.y + (sign > 0));
            coordinate.z = (coordinate.z + (sign < 0));
            coordinate.w = 0;
        }
        if (direction == "xz")
        {
            coordinate.x = (coordinate.x + (sign > 0));
            coordinate.z = (coordinate.z + (sign > 0));
            coordinate.y = (coordinate.y + (sign < 0));
            coordinate.w = 0;
        }
        if (direction == "yz")
        {
            coordinate.y = (coordinate.y + (sign > 0));
            coordinate.z = (coordinate.z + (sign > 0));
            coordinate.x = (coordinate.x + (sign < 0));
            coordinate.w = 0;
        }
        if (direction == "xyz")
        {
            coordinate.x = (coordinate.x + (sign > 0));
            coordinate.y = (coordinate.y + (sign > 0));
            coordinate.z = (coordinate.z + (sign > 0));
            coordinate.w = 0;
        }
    }
    else
    {
        if (direction == "xy")
        {
            coordinate.x = (coordinate.x - (sign < 0));
            coordinate.y = (coordinate.y - (sign < 0));
            coordinate.z = (coordinate.z - (sign > 0));
            coordinate.w = 1;
        }
        if (direction == "xz")
        {
            coordinate.x = (coordinate.x - (sign < 0));
            coordinate.z = (coordinate.z - (sign < 0));
            coordinate.y = (coordinate.y - (sign > 0));
            coordinate.w = 1;
        }
        if (direction == "yz")
        {
            coordinate.y = (coordinate.y - (sign < 0));
            coordinate.z = (coordinate.z - (sign < 0));
            coordinate.x = (coordinate.x - (sign > 0));
            coordinate.w = 1;
        }
        if (direction == "xyz")
        {
            coordinate.x = (coordinate.x - (sign < 0));
            coordinate.y = (coordinate.y - (sign < 0));
            coordinate.z = (coordinate.z - (sign < 0));
            coordinate.w = 1;
        }
    }
    if (coordinate.x < 0 || coordinate.x >= l || coordinate.y < 0 || coordinate.y >= l || coordinate.z < 0 || coordinate.z >= l)
    {
        throw std::invalid_argument("Vertex is outside the lattice!");
    }
    else
    {
        return coordinateToIndex(coordinate);
    }
}

int RhombicLattice::edgeIndex(const int vertexIndex, const std::string &direction, const int sign)
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
        // Except if an edge to a vertex outside the lattice is requested
        int testIndex = neighbour(vertexIndex, direction, sign);
        // Otherwise do this
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

void RhombicLattice::addFace(const int vertexIndex, const int faceIndex, const vstr &directions, const vint &signs)
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

void RhombicLattice::createFaces()
{
    int faceIndex = 0;
    for (int vertexIndex = 0; vertexIndex < pow(l, 3); ++vertexIndex)
    {
        cartesian4 coordinate = indexToCoordinate(vertexIndex);
        if ((coordinate.x + coordinate.y + coordinate.z) % 2 == 1)
        {
            if (coordinate.z == 0)
            {
                continue;
            }
            else if (coordinate.z % 2 == 1)
            {
                if (coordinate.y == 0)
                {
                    addFace(vertexIndex, faceIndex, {"xyz", "xy", "xy", "xyz"}, {1, 1, 1, 1});
                    ++faceIndex;
                }
                else if (coordinate.x == 0)
                {
                    addFace(vertexIndex, faceIndex, {"xyz", "xy", "xy", "xyz"}, {1, 1, 1, 1});
                    ++faceIndex;
                    if (coordinate.z != l - 1)
                    {
                        addFace(vertexIndex, faceIndex, {"xyz", "xz", "xz", "xyz"}, {1, 1, 1, 1});
                        ++faceIndex;
                    }
                    if (coordinate.z != 1)
                    {
                        addFace(vertexIndex, faceIndex, {"xy", "yz", "yz", "xy"}, {1, -1, -1, 1});
                        ++faceIndex;
                    }
                }
                else if (coordinate.x == l - 1)
                {
                    if (coordinate.y == l - 1)
                    {
                        continue;
                    }
                    addFace(vertexIndex, faceIndex, {"yz", "xz", "xz", "yz"}, {1, -1, -1, 1});
                    ++faceIndex;
                    if (coordinate.z != l - 1)
                    {
                        addFace(vertexIndex, faceIndex, {"xy", "yz", "yz", "xy"}, {-1, 1, 1, -1});
                        ++faceIndex;
                    }
                    if (coordinate.z != 1)
                    {
                        addFace(vertexIndex, faceIndex, {"xyz", "xz", "xz", "xyz"}, {-1, -1, -1, -1});
                        ++faceIndex;
                    }
                }
                else if (coordinate.y == l - 1)
                {
                    addFace(vertexIndex, faceIndex, {"xz", "yz", "yz", "xz"}, {1, -1, -1, 1});
                    ++faceIndex;
                }
                else if (coordinate.x % 2 == 0 && coordinate.y % 2 == 0)
                {
                    if (coordinate.z != l - 1)
                    {
                        addFace(vertexIndex, faceIndex, {"xyz", "xz", "xz", "xyz"}, {1, 1, 1, 1});
                        ++faceIndex;
                        addFace(vertexIndex, faceIndex, {"xy", "yz", "yz", "xy"}, {-1, 1, 1, -1});
                        ++faceIndex;
                    }
                    if (coordinate.z != 1)
                    {
                        addFace(vertexIndex, faceIndex, {"xy", "yz", "yz", "xy"}, {1, -1, -1, 1});
                        ++faceIndex;
                        addFace(vertexIndex, faceIndex, {"xyz", "xz", "xz", "xyz"}, {-1, -1, -1, -1});
                        ++faceIndex;
                    }
                    addFace(vertexIndex, faceIndex, {"xyz", "xy", "xy", "xyz"}, {1, 1, 1, 1});
                    ++faceIndex;
                    addFace(vertexIndex, faceIndex, {"xyz", "xy", "xy", "xyz"}, {-1, -1, -1, -1});
                    ++faceIndex;
                }
                else if (coordinate.x % 2 == 1 && coordinate.y % 2 == 1)
                {
                    if (coordinate.z != l - 1)
                    {
                        addFace(vertexIndex, faceIndex, {"xyz", "xz", "xz", "xyz"}, {1, 1, 1, 1});
                        ++faceIndex;
                        addFace(vertexIndex, faceIndex, {"xy", "yz", "yz", "xy"}, {-1, 1, 1, -1});
                        ++faceIndex;
                    }
                    if (coordinate.z != 1)
                    {
                        addFace(vertexIndex, faceIndex, {"xy", "yz", "yz", "xy"}, {1, -1, -1, 1});
                        ++faceIndex;
                        addFace(vertexIndex, faceIndex, {"xyz", "xz", "xz", "xyz"}, {-1, -1, -1, -1});
                        ++faceIndex;
                    }
                    addFace(vertexIndex, faceIndex, {"xz", "yz", "yz", "xz"}, {1, -1, -1, 1});
                    ++faceIndex;
                    addFace(vertexIndex, faceIndex, {"xz", "yz", "yz", "xz"}, {-1, 1, 1, -1});
                    ++faceIndex;
                }
            }
            else
            {
                if (coordinate.x == 0)
                {
                    addFace(vertexIndex, faceIndex, {"xz", "yz", "yz", "xz"}, {1, -1, -1, 1});
                    ++faceIndex;
                }
                else if (coordinate.y == 0)
                {
                    if (coordinate.x == l - 1)
                    {
                        continue;
                    }
                    addFace(vertexIndex, faceIndex, {"xyz", "xy", "xy", "xyz"}, {1, 1, 1, 1});
                    ++faceIndex;
                    addFace(vertexIndex, faceIndex, {"xyz", "yz", "yz", "xyz"}, {1, 1, 1, 1});
                    ++faceIndex;
                    addFace(vertexIndex, faceIndex, {"xy", "xz", "xz", "xy"}, {1, -1, -1, 1});
                    ++faceIndex;
                }
                else if (coordinate.x == l - 1)
                {
                    addFace(vertexIndex, faceIndex, {"xyz", "xy", "xy", "xyz"}, {-1, -1, -1, -1});
                    ++faceIndex;
                }
                else if (coordinate.y == l - 1)
                {
                    addFace(vertexIndex, faceIndex, {"xz", "yz", "yz", "xz"}, {1, -1, -1, 1});
                    ++faceIndex;
                    addFace(vertexIndex, faceIndex, {"xy", "xz", "xz", "xy"}, {-1, 1, 1, -1});
                    ++faceIndex;
                    addFace(vertexIndex, faceIndex, {"xyz", "yz", "yz", "xyz"}, {-1, -1, -1, -1});
                    ++faceIndex;
                }
                else if (coordinate.x % 2 == 0 && coordinate.y % 2 == 1)
                {
                    addFace(vertexIndex, faceIndex, {"xz", "xy", "xy", "xz"}, {1, -1, -1, 1});
                    ++faceIndex;
                    addFace(vertexIndex, faceIndex, {"xyz", "yz", "yz", "xyz"}, {-1, -1, -1, -1});
                    ++faceIndex;
                    addFace(vertexIndex, faceIndex, {"xyz", "yz", "yz", "xyz"}, {1, 1, 1, 1});
                    ++faceIndex;
                    addFace(vertexIndex, faceIndex, {"xz", "xy", "xy", "xz"}, {-1, 1, 1, -1});
                    ++faceIndex;
                    addFace(vertexIndex, faceIndex, {"xz", "yz", "yz", "xz"}, {1, -1, -1, 1});
                    ++faceIndex;
                    addFace(vertexIndex, faceIndex, {"xz", "yz", "yz", "xz"}, {-1, 1, 1, -1});
                    ++faceIndex;
                }
                else if (coordinate.x % 2 == 1 && coordinate.y % 2 == 0)
                {
                    addFace(vertexIndex, faceIndex, {"xyz", "yz", "yz", "xyz"}, {1, 1, 1, 1});
                    ++faceIndex;
                    addFace(vertexIndex, faceIndex, {"xz", "xy", "xy", "xz"}, {-1, 1, 1, -1});
                    ++faceIndex;
                    addFace(vertexIndex, faceIndex, {"xz", "xy", "xy", "xz"}, {1, -1, -1, 1});
                    ++faceIndex;
                    addFace(vertexIndex, faceIndex, {"xyz", "yz", "yz", "xyz"}, {-1, -1, -1, -1});
                    ++faceIndex;
                    addFace(vertexIndex, faceIndex, {"xyz", "xy", "xy", "xyz"}, {1, 1, 1, 1});
                    ++faceIndex;
                    addFace(vertexIndex, faceIndex, {"xyz", "xy", "xy", "xyz"}, {-1, -1, -1, -1});
                    ++faceIndex;
                }
            }
        }
    }
}

int RhombicLattice::findFace(vint &vertices)
{
}

void RhombicLattice::createUpEdgesMap()
{
}

void RhombicLattice::createVertexToEdges()
{
}