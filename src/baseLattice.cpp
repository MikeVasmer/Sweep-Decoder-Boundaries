#include "baseLattice.h"
#include <string>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <map>

int sgn(int x) { return (x > 0) - (x < 0); }

BaseLattice::BaseLattice(const int length, const std::string &latticeType) : l(length), type(latticeType)
{
    if (length < 3)
    {
        throw std::invalid_argument("Lattice dimension l must be a positive integer greater than two.");
    }
    if (!(latticeType == "cubic" || latticeType == "rhombic" || latticeType == "bcc"))
    {
        throw std::invalid_argument("Lattice type must be either 'cubic', 'bcc' or 'rhombic'.");
    }
    if (latticeType == "rhombic" && length % 2 != 0)
    {
        throw std::invalid_argument("For lattice type = 'rhombic', only even lattice dimensions are allowed.");
    }
    if (latticeType == "bcc")
    {
        faceToVertices.reserve(24 * l * l * l);
        faceToEdges.reserve(24 * l * l * l);
        vertexToFaces.assign(2 * l * l * l, {});
    }
    else if (latticeType == "rhombic")
    {
        faceToVertices.reserve(3 * l * l * l);
        faceToEdges.reserve(3 * l * l * l);
        // Not all vertices present in this lattice, but all w=1 faces
        // are present, so the possible vertex indices go from
        // 0 to l^3 -1
        vertexToFaces.assign(2 * l * l * l, {});
        vertexToEdges.assign(2 * l * l * l, {});
    }
}

const std::string& BaseLattice::getType() const { return type; }

cartesian4 BaseLattice::indexToCoordinate(const int vertexIndex)
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

int BaseLattice::coordinateToIndex(const cartesian4 &coordinate)
{
    if (coordinate.x < 0 || coordinate.y < 0 || coordinate.z < 0 || coordinate.w < 0 || coordinate.w > 1)
    {
        throw std::invalid_argument("Lattice coordinates must be positive and w coordinate must be either zero or one.");
    }
    return coordinate.w * l * l * l + coordinate.z * l * l + coordinate.y * l + coordinate.x;
}

const vvint& BaseLattice::getFaceToVertices() const
{
    return faceToVertices;
}

const vvint& BaseLattice::getFaceToEdges() const
{
    return faceToEdges;
}

const std::vector<std::vector<faceS>>& BaseLattice::getVertexToFaces() const
{
    return vertexToFaces;
}

void BaseLattice::createUpEdgesMap()
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

std::map<std::string, vvint>& BaseLattice::getUpEdgesMap()
{
    return upEdgesMap;
}

void BaseLattice::createVertexToEdges()
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

const vvint& BaseLattice::getVertexToEdges() const
{
    return vertexToEdges;
}