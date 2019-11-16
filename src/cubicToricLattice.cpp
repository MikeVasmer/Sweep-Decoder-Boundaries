#include "cubicToricLattice.h"
#include "lattice.h"
#include <string>
#include <cmath>

CubicToricLattice::CubicToricLattice(const int l) : Lattice(l)
{
    if (l <= 3)
    {
        throw std::invalid_argument("Lattice dimension l must be greater than three.");
    }
    int numberOfFaces = 3 * pow(l, 3);
    faceToVertices.reserve(numberOfFaces);
    faceToEdges.reserve(numberOfFaces);
    vertexToFaces.assign(pow(l, 3), {});
    vertexToEdges.assign(pow(l, 3), {});
}

int CubicToricLattice::neighbour(const int vertexIndex, const std::string &direction, const int sign)
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
            coordinate.x = (l + ((coordinate.x + sign) % l)) % l;
        }
        else if (direction == "y")
        {
            coordinate.y = (l + ((coordinate.y + sign) % l)) % l;
        }
        else if (direction == "z")
        {
            coordinate.z = (l + ((coordinate.z + sign) % l)) % l;
        }
    }
    // std::cerr << coordinate << std::endl;
    return coordinateToIndex(coordinate);
}

void CubicToricLattice::createFaces()
{
    int faceIndex = 0;
    for (int vertexIndex = 0; vertexIndex < pow(l, 3); ++vertexIndex)
    {
        // cartesian4 coordinate = indexToCoordinate(vertexIndex);
        addFace(vertexIndex, faceIndex, {"x", "y", "y", "x"}, {1, 1, 1, 1});
        ++faceIndex;
        addFace(vertexIndex, faceIndex, {"x", "z", "z", "x"}, {1, 1, 1, 1});
        ++faceIndex;
        addFace(vertexIndex, faceIndex, {"y", "z", "z", "y"}, {1, 1, 1, 1});
        ++faceIndex;
    }
}

void CubicToricLattice::createUpEdgesMap()
{
    std::vector<std::string> directionList = {"xyz", "xy", "xz", "yz",
                                              "-xyz", "-xy", "-xz", "-yz"};
    for (const auto &direction : directionList)
    {
        vvint vertexToUpEdges;
        vertexToUpEdges.assign(pow(l, 3), {});
        for (int vertexIndex = 0; vertexIndex < pow(l, 3); ++vertexIndex)
        {
            cartesian4 coordinate = indexToCoordinate(vertexIndex);
            if (direction == "xyz")
            {
                vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "x", 1));
                vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "y", 1));
                vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "z", 1));
            }
            else if (direction == "xy")
            {
                
                vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "x", 1));
                vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "y", 1));
                vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "z", -1));
            }
            else if (direction == "xz")
            {
                vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "x", 1));
                vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "y", -1));
                vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "z", 1));
            }
            else if (direction == "yz")
            {
                vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "x", -1));
                vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "y", 1));
                vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "z", 1));
            }
            else if (direction == "-xyz")
            {
                vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "x", -1));
                vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "y", -1));
                vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "z", -1));
            }
            else if (direction == "-xy")
            {
                vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "x", -1));
                vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "y", -1));
                vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "z", 1));
            }
            else if (direction == "-xz")
            {
                vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "x", -1));
                vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "y", 1));
                vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "z", -1));
            }
            else if (direction == "-yz")
            {
                vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "x", 1));
                vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "y", -1));
                vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "z", -1));
            }
        }
        upEdgesMap.insert(std::pair<std::string, vvint>(direction, vertexToUpEdges));
    }
}

void CubicToricLattice::createVertexToEdges()
{
    for (int vertexIndex = 0; vertexIndex < pow(l, 3); ++vertexIndex)
    {
        vertexToEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "x", 1));
        vertexToEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "y", 1));
        vertexToEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "z", 1));
        vertexToEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "x", -1));
        vertexToEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "y", -1));
        vertexToEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "z", -1));
    }
}