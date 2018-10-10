#include "lattice.h"
#include "baseLattice.h"
#include <string>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <map>

Lattice::Lattice(const int length, const std::string &latticeType) : BaseLattice(length, latticeType)
{
}

int Lattice::neighbour(const int vertexIndex, const std::string &direction, const int sign)
{
    if (!(sign == 1 || sign == -1))
    {
        throw std::invalid_argument("Sign must be either 1 or -1.");
    }
    if (!(direction == "x" || direction == "y" || direction == "z" ||
          direction == "xy" || direction == "xz" || direction == "yz" ||
          direction == "xyz"))
    {
        throw std::invalid_argument("Direction must be one of 'x', 'y', 'z', 'xy', 'xz', 'yz' or 'xyz'.");
    }
    if (type == "cubic")
    {
        if (!(direction == "x" || direction == "y" || direction == "z"))
        {
            throw std::invalid_argument("Cubic lattice vertices only have neighbours in x, y, and z directions.");
        }
    }
    else if (type == "rhombic")
    {
        if (direction == "x" || direction == "y" || direction == "z")
        {
            throw std::invalid_argument("Rhombic lattice vertices only have neighbours in xy, xz, yz, xyz directions.");
        }
    }
    cartesian4 coordinate;

    coordinate = indexToCoordinate(vertexIndex);
    if (direction == "x")
        coordinate.x = (coordinate.x + sign + l) % l;
    if (direction == "y")
        coordinate.y = (coordinate.y + sign + l) % l;
    if (direction == "z")
        coordinate.z = (coordinate.z + sign + l) % l;
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

int Lattice::edgeIndex(const int vertexIndex, const std::string &direction, const int sign)
{
    if (!(sign == 1 || sign == -1))
    {
        throw std::invalid_argument("Sign must be either 1 or -1.");
    }
    if (!(direction == "x" || direction == "y" || direction == "z" ||
          direction == "xy" || direction == "xz" || direction == "yz" ||
          direction == "xyz"))
    {
        throw std::invalid_argument("Direction must be one of 'x', 'y', 'z', 'xy', 'xz', 'yz' or 'xyz'.");
    }
    if (type == "cubic")
    {
        if (!(direction == "x" || direction == "y" || direction == "z"))
        {
            throw std::invalid_argument("Cubic lattice vertices only have neighbours in x, y, and z directions.");
        }
    }
    else if (type == "rhombic")
    {
        if (direction == "x" || direction == "y" || direction == "z")
        {
            throw std::invalid_argument("Rhombic lattice vertices only have neighbours in xy, xz, yz, xyz directions.");
        }
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
    else if (direction == "x")
        edgeIndex = 7 * edgeIndex + 1;
    else if (direction == "xy")
        edgeIndex = 7 * edgeIndex + 2;
    else if (direction == "y")
        edgeIndex = 7 * edgeIndex + 3;
    else if (direction == "yz")
        edgeIndex = 7 * edgeIndex + 4;
    else if (direction == "z")
        edgeIndex = 7 * edgeIndex + 5;
    else if (direction == "xz")
        edgeIndex = 7 * edgeIndex + 6;
    return edgeIndex;
}

void Lattice::addFace(const int vertexIndex, const int faceIndex, const vstr &directions, const vint &signs)
{
    vint vertices;
    vint edges;
    if (type == "rhombic")
    {
        int neighbourVertex = neighbour(vertexIndex, directions[0], signs[0]);
        vertices = {vertexIndex, neighbourVertex,
                    neighbour(vertexIndex, directions[1], signs[1]),
                    neighbour(neighbourVertex, directions[2], signs[2])};
        edges = {edgeIndex(vertexIndex, directions[0], signs[0]),
                 edgeIndex(vertexIndex, directions[1], signs[1]),
                 edgeIndex(neighbourVertex, directions[2], signs[2]),
                 edgeIndex(vertices[2], directions[3], signs[3])};
    }
    else if (type == "bcc")
    {
        vertices = {vertexIndex, neighbour(vertexIndex, directions[0], signs[0]),
                    neighbour(vertexIndex, directions[1], signs[1])};
        edges = {edgeIndex(vertexIndex, directions[0], signs[0]),
                 edgeIndex(vertexIndex, directions[1], signs[1]),
                 edgeIndex(vertices[1], directions[2], signs[2])};
    }
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

void Lattice::createFaces()
{
    if (type == "rhombic")
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
    else if (type == "bcc")
    {
        int faceIndex = 0;
        for (int vertexIndex = 0; vertexIndex < 2 * l * l * l; ++vertexIndex)
        {
            vint signs = {1, 1, 1};
            addFace(vertexIndex, faceIndex, {"x", "xyz", "yz"}, signs);
            faceIndex++;
            addFace(vertexIndex, faceIndex, {"xy", "xyz", "z"}, signs);
            faceIndex++;
            addFace(vertexIndex, faceIndex, {"y", "xyz", "xz"}, signs);
            faceIndex++;
            addFace(vertexIndex, faceIndex, {"yz", "xyz", "x"}, signs);
            faceIndex++;
            addFace(vertexIndex, faceIndex, {"z", "xyz", "xy"}, signs);
            faceIndex++;
            addFace(vertexIndex, faceIndex, {"xz", "xyz", "y"}, signs);
            faceIndex++;
            addFace(vertexIndex, faceIndex, {"xy", "x", "xz"}, signs);
            faceIndex++;
            addFace(vertexIndex, faceIndex, {"xz", "x", "xy"}, signs);
            faceIndex++;
            addFace(vertexIndex, faceIndex, {"xy", "y", "yz"}, signs);
            faceIndex++;
            addFace(vertexIndex, faceIndex, {"yz", "y", "xy"}, signs);
            faceIndex++;
            addFace(vertexIndex, faceIndex, {"xz", "z", "yz"}, signs);
            faceIndex++;
            addFace(vertexIndex, faceIndex, {"yz", "z", "xz"}, signs);
            faceIndex++;
        }
    }
}

int Lattice::findFace(vint &vertices)
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