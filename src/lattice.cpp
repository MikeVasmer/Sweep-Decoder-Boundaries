#include "lattice.h"
#include <string>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <map>
#include <sstream>

int sgn(int x) { return (x > 0) - (x < 0); }

Lattice::Lattice(const int length) : l(length)
{
    if (length < 3)
    {
        throw std::invalid_argument("Lattice dimension l must be a positive integer greater than two.");
    }
}

cartesian4 Lattice::indexToCoordinate(const int vertexIndex)
{
    if (vertexIndex < 0)
    {
        throw std::invalid_argument("Index must not be negative.");
    }
    cartesian4 coordinate;
    coordinate.x = vertexIndex % l;
    coordinate.y = int(floor(vertexIndex / l)) % l;
    coordinate.z = int(floor(vertexIndex / (l * l))) % l;
    // w is either 0 or 1 and fixes the sub-lattice
    coordinate.w = int(floor(vertexIndex / (l * l * l)));
    return coordinate;
}

int Lattice::coordinateToIndex(const cartesian4 &coordinate)
{
    if (coordinate.x < 0 || coordinate.y < 0 || coordinate.z < 0 || coordinate.w < 0 || coordinate.w > 1)
    {
        throw std::invalid_argument("Lattice coordinates must be positive and w coordinate must be either zero or one.");
    }
    return coordinate.w * l * l * l + coordinate.z * l * l + coordinate.y * l + coordinate.x;
}

int Lattice::edgeIndex(const int vertexIndex, const std::string &direction, const int sign)
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
        // Check that 2nd vertex is not outside the lattice, exception will be thrown if this is the case
        int testIndex = neighbour(vertexIndex, direction, sign);
        // Otherwise do this
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
    std::ostringstream stream;
    stream << "Input vertices: " << indexToCoordinate(vertices[0]) << ", " << indexToCoordinate(vertices[1]) << ", " << indexToCoordinate(vertices[2]) << ", " << indexToCoordinate(vertices[3]) << " do not correspond to a face.";
    std::string errorMessage = stream.str();
    throw std::invalid_argument(errorMessage);
}

const vvint &Lattice::getFaceToVertices() const
{
    return faceToVertices;
}

const vvint &Lattice::getFaceToEdges() const
{
    return faceToEdges;
}

const std::vector<std::vector<faceS>> &Lattice::getVertexToFaces() const
{
    return vertexToFaces;
}

std::map<std::string, vvint> &Lattice::getUpEdgesMap()
{
    return upEdgesMap;
}

const vvint &Lattice::getVertexToEdges() const
{
    return vertexToEdges;
}