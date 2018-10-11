#include "lattice.h"
#include <string>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <map>

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

const vvint& Lattice::getFaceToVertices() const
{
    return faceToVertices;
}

const vvint& Lattice::getFaceToEdges() const
{
    return faceToEdges;
}

const std::vector<std::vector<faceS>>& Lattice::getVertexToFaces() const
{
    return vertexToFaces;
}

std::map<std::string, vvint>& Lattice::getUpEdgesMap()
{
    return upEdgesMap;
}

const vvint& Lattice::getVertexToEdges() const
{
    return vertexToEdges;
}