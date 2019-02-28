#include "cubicLattice.h"
#include "lattice.h"
#include <string>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <map>
#include <sstream>

CubicLattice::CubicLattice(const int l) : Lattice(l)
{
    if (l <= 3)
    {
        throw std::invalid_argument("Lattice dimension l must be greater than three.");
    }
    int numberOfFaces = 3 * pow(l - 1, 3) - 4 * pow(l - 1, 2) + 2 * (l - 1);
    faceToVertices.reserve(numberOfFaces);
    faceToEdges.reserve(numberOfFaces);
    vertexToFaces.assign(pow(l, 3), {});
    vertexToEdges.assign(pow(l, 3), {});
}

int CubicLattice::neighbour(const int vertexIndex, const std::string &direction, const int sign)
{
    if (!(sign == 1 || sign == -1))
    {
        throw std::invalid_argument("Sign must be either 1 or -1.");
    }
    if (!(direction == "x" || direction == "y" || direction == "z"))
    {
        throw std::invalid_argument("Direction must be one of 'x', 'y' or 'z'.");
    }
    cartesian4 coordinate;
    coordinate = indexToCoordinate(vertexIndex);
    if (coordinate.w == 1)
    {
        throw std::invalid_argument("Cubic lattice doesn't contain w=0 vertices.");
    }
    else
    {
        if (direction == "x")
        {
            coordinate.x = coordinate.x + sign;
        }
        else if (direction == "y")
        {
            coordinate.y = coordinate.y + sign;
        }
        else if (direction == "z")
        {
            coordinate.z = coordinate.z + sign;
        }
    }
    if (coordinate.x < 0 || coordinate.x >= l || coordinate.y < 0 || coordinate.y >= l || coordinate.z < 0 || coordinate.z >= l)
    {
        std::ostringstream stream;
        cartesian4 errorCoord = indexToCoordinate(vertexIndex);
        std::string errorDir;
        if (sign == 1)
        {
            errorDir = "+" + direction;
        }
        else if (sign == -1)
        {
            errorDir = "-" + direction;
        }
        stream << "CubicLattice::neighbour, " << errorDir << " neighbour of " << errorCoord << " is outside the lattice.";
        std::string errorMessage = stream.str();
        throw std::invalid_argument(errorMessage);
    }
    else
    {
        return coordinateToIndex(coordinate);
    }
}

void CubicLattice::createFaces()
{
    int faceIndex = 0;
    for (int vertexIndex = 0; vertexIndex < pow(l, 3); ++vertexIndex)
    {
        cartesian4 coordinate = indexToCoordinate(vertexIndex);
        if (coordinate.z == l - 1 || coordinate.x == l - 1 || coordinate.y == l - 1)
        {
            continue;
        }
        if (coordinate.z < l - 2)
        {
            if (!(coordinate.x == 0))
            {
                // Add yz face
                addFace(vertexIndex, faceIndex, {"y", "z", "z", "y"}, {1, 1, 1, 1});
                ++faceIndex;
            }
            if (!(coordinate.y == 0))
            {
                // Add xz face
                addFace(vertexIndex, faceIndex, {"x", "z", "z", "x"}, {1, 1, 1, 1});
                ++faceIndex;
            }
        }
        // Add xy face
        addFace(vertexIndex, faceIndex, {"x", "y", "y", "x"}, {1, 1, 1, 1});
        ++faceIndex;
    }
}

void CubicLattice::createUpEdgesMap()
{
}

void CubicLattice::createVertexToEdges()
{
}
