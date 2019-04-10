#include "rhombicLattice.h"
#include "lattice.h"
#include <string>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <map>
#include <sstream>

RhombicLattice::RhombicLattice(const int l) : Lattice(l)
{
    if (l < 3)
    {
        throw std::invalid_argument("Lattice dimension l must be greater than three.");
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
        stream << "RhombicLattice::neighbour, " << errorDir << " neighbour of " << errorCoord << " is outside the lattice.";
        std::string errorMessage = stream.str();
        throw std::invalid_argument(errorMessage);
    }
    else
    {
        return coordinateToIndex(coordinate);
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

void RhombicLattice::createUpEdgesMap()
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
                if ((coordinate.x + coordinate.y + coordinate.z) % 2 == 1)
                {
                    if (direction == "xyz")
                    {
                        // Third argument is sign
                        try
                        {
                            vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "xyz", 1));
                        }
                        catch (const std::invalid_argument &e)
                        {
                            // Edge includes vertex outside lattice
                        }
                        try
                        {
                            vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "xy", 1));
                        }
                        catch (const std::invalid_argument &e)
                        {
                        }
                        try
                        {
                            vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "xz", 1));
                        }
                        catch (const std::invalid_argument &e)
                        {
                        }
                        try
                        {
                            vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "yz", 1));
                        }
                        catch (const std::invalid_argument &e)
                        {
                        }
                    }
                    else if (direction == "yz")
                    {
                        try
                        {
                            vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "yz", 1));
                        }
                        catch (const std::invalid_argument &e)
                        {
                        }
                        try
                        {
                            vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "xyz", 1));
                        }
                        catch (const std::invalid_argument &e)
                        {
                        }
                        try
                        {
                            vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "xy", -1));
                        }
                        catch (const std::invalid_argument &e)
                        {
                        }
                        try
                        {
                            vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "xz", -1));
                        }
                        catch (const std::invalid_argument &e)
                        {
                        }
                    }
                    else if (direction == "xz")
                    {
                        try
                        {
                            vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "xyz", 1));
                        }
                        catch (const std::invalid_argument &e)
                        {
                        }
                        try
                        {
                            vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "xz", 1));
                        }
                        catch (const std::invalid_argument &e)
                        {
                        }
                        try
                        {
                            vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "xy", -1));
                        }
                        catch (const std::invalid_argument &e)
                        {
                        }
                        try
                        {
                            vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "yz", -1));
                        }
                        catch (const std::invalid_argument &e)
                        {
                        }
                    }
                    else if (direction == "xy")
                    {
                        try
                        {
                            vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "xyz", 1));
                        }
                        catch (const std::invalid_argument &e)
                        {
                        }
                        try
                        {
                            vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "xy", 1));
                        }
                        catch (const std::invalid_argument &e)
                        {
                        }
                        try
                        {
                            vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "xz", -1));
                        }
                        catch (const std::invalid_argument &e)
                        {
                        }
                        try
                        {
                            vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "yz", -1));
                        }
                        catch (const std::invalid_argument &e)
                        {
                        }
                    }
                    else if (direction == "-xyz")
                    {
                        try
                        {
                            vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "xyz", -1));
                        }
                        catch (const std::invalid_argument &e)
                        {
                        }
                        try
                        {
                            vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "xz", -1));
                        }
                        catch (const std::invalid_argument &e)
                        {
                        }
                        try
                        {
                            vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "xy", -1));
                        }
                        catch (const std::invalid_argument &e)
                        {
                        }
                        try
                        {
                            vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "yz", -1));
                        }
                        catch (const std::invalid_argument &e)
                        {
                        }
                    }
                    else if (direction == "-yz")
                    {
                        try
                        {
                            vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "xy", 1));
                        }
                        catch (const std::invalid_argument &e)
                        {
                        }
                        try
                        {
                            vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "xz", 1));
                        }
                        catch (const std::invalid_argument &e)
                        {
                        }
                        try
                        {
                            vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "xyz", -1));
                        }
                        catch (const std::invalid_argument &e)
                        {
                        }
                        try
                        {
                            vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "yz", -1));
                        }
                        catch (std::invalid_argument &e)
                        {
                        }
                    }
                    else if (direction == "-xz")
                    {
                        try
                        {
                            vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "xy", 1));
                        }
                        catch (std::invalid_argument &e)
                        {
                        }
                        try
                        {
                            vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "yz", 1));
                        }
                        catch (std::invalid_argument &e)
                        {
                        }
                        try
                        {
                            vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "xyz", -1));
                        }
                        catch (std::invalid_argument &e)
                        {
                        }
                        try
                        {
                            vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "xz", -1));
                        }
                        catch (std::invalid_argument &e)
                        {
                        }
                    }
                    else if (direction == "-xy")
                    {
                        try
                        {
                            vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "xz", 1));
                        }
                        catch (std::invalid_argument &e)
                        {
                        }
                        try
                        {
                            vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "yz", 1));
                        }
                        catch (std::invalid_argument &e)
                        {
                        }
                        try
                        {
                            vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "xyz", -1));
                        }
                        catch (std::invalid_argument &e)
                        {
                        }
                        try
                        {
                            vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "xy", -1));
                        }
                        catch (std::invalid_argument &e)
                        {
                        }
                    }
                }
            }
            else
            {
                if ((coordinate.x + coordinate.y + coordinate.z) % 2 == 1)
                {
                    if (direction == "xy" || direction == "xz" || direction == "yz" || direction == "-xyz")
                    {
                        // Only one up edge, so return an empty vector as no sweep will happen here.
                    }
                    else
                    {
                        if (direction == "xyz")
                        {
                            try
                            {
                                vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "xy", 1));
                            }
                            catch (std::invalid_argument &e)
                            {
                            }
                            try
                            {
                                vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "xz", 1));
                            }
                            catch (std::invalid_argument &e)
                            {
                            }
                            try
                            {
                                vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "yz", 1));
                            }
                            catch (std::invalid_argument &e)
                            {
                            }
                        }
                        else if (direction == "-xy")
                        {
                            try
                            {
                                vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "xyz", -1));
                            }
                            catch (std::invalid_argument &e)
                            {
                            }
                            try
                            {
                                vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "xz", 1));
                            }
                            catch (std::invalid_argument &e)
                            {
                            }
                            try
                            {
                                vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "yz", 1));
                            }
                            catch (std::invalid_argument &e)
                            {
                            }
                        }
                        else if (direction == "-xz")
                        {
                            try
                            {
                                vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "xyz", -1));
                            }
                            catch (std::invalid_argument &e)
                            {
                            }
                            try
                            {
                                vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "xy", 1));
                            }
                            catch (std::invalid_argument &e)
                            {
                            }
                            try
                            {
                                vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "yz", 1));
                            }
                            catch (std::invalid_argument &e)
                            {
                            }
                        }
                        else if (direction == "-yz")
                        {
                            try
                            {
                                vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "xyz", -1));
                            }
                            catch (std::invalid_argument &e)
                            {
                            }
                            try
                            {
                                vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "xz", 1));
                            }
                            catch (std::invalid_argument &e)
                            {
                            }
                            try
                            {
                                vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "xy", 1));
                            }
                            catch (std::invalid_argument &e)
                            {
                            }
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
                            try
                            {
                                vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "xy", -1));
                            }
                            catch (std::invalid_argument &e)
                            {
                            }
                            try
                            {
                                vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "xz", -1));
                            }
                            catch (std::invalid_argument &e)
                            {
                            }
                            try
                            {
                                vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "yz", -1));
                            }
                            catch (std::invalid_argument &e)
                            {
                            }
                        }
                        else if (direction == "xy")
                        {
                            try
                            {
                                vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "xyz", 1));
                            }
                            catch (std::invalid_argument &e)
                            {
                            }
                            try
                            {
                                vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "xz", -1));
                            }
                            catch (std::invalid_argument &e)
                            {
                            }
                            try
                            {
                                vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "yz", -1));
                            }
                            catch (std::invalid_argument &e)
                            {
                            }
                        }
                        else if (direction == "xz")
                        {
                            try
                            {
                                vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "xyz", 1));
                            }
                            catch (std::invalid_argument &e)
                            {
                            }
                            try
                            {
                                vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "xy", -1));
                            }
                            catch (std::invalid_argument &e)
                            {
                            }
                            try
                            {
                                vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "yz", -1));
                            }
                            catch (std::invalid_argument &e)
                            {
                            }
                        }
                        else if (direction == "yz")
                        {
                            try
                            {
                                vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "xyz", 1));
                            }
                            catch (std::invalid_argument &e)
                            {
                            }
                            try
                            {
                                vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "xz", -1));
                            }
                            catch (std::invalid_argument &e)
                            {
                            }
                            try
                            {
                                vertexToUpEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "xy", -1));
                            }
                            catch (std::invalid_argument &e)
                            {
                            }
                        }
                    }
                }
            }
        }
        upEdgesMap.insert(std::pair<std::string, vvint>(direction, vertexToUpEdges));
    }
}

void RhombicLattice::createVertexToEdges()
{
    for (int vertexIndex = 0; vertexIndex < 2 * l * l * l; ++vertexIndex)
    {
        cartesian4 coordinate = indexToCoordinate(vertexIndex);
        if (coordinate.w == 0)
        {
            if ((coordinate.x + coordinate.y + coordinate.z) % 2 == 1)
            {
                int sign = 1;
                try
                {
                    vertexToEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "xyz", sign));
                }
                catch (std::invalid_argument &e)
                {
                }
                try
                {
                    vertexToEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "xy", sign));
                }
                catch (std::invalid_argument &e)
                {
                }
                try
                {
                    vertexToEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "xz", sign));
                }
                catch (std::invalid_argument &e)
                {
                }
                try
                {
                    vertexToEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "yz", sign));
                }
                catch (std::invalid_argument &e)
                {
                }
                sign = -1;
                try
                {
                    vertexToEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "xyz", sign));
                }
                catch (std::invalid_argument &e)
                {
                }
                try
                {
                    vertexToEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "xy", sign));
                }
                catch (std::invalid_argument &e)
                {
                }
                try
                {
                    vertexToEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "xz", sign));
                }
                catch (std::invalid_argument &e)
                {
                }
                try
                {
                    vertexToEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "yz", sign));
                }
                catch (std::invalid_argument &e)
                {
                }
            }
        }
        else
        {
            if ((coordinate.x + coordinate.y + coordinate.z) % 2 == 1)
            {
                int sign = 1;
                try
                {
                    vertexToEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "xy", sign));
                }
                catch (std::invalid_argument &e)
                {
                }
                try
                {
                    vertexToEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "xz", sign));
                }
                catch (std::invalid_argument &e)
                {
                }
                try
                {
                    vertexToEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "yz", sign));
                }
                catch (std::invalid_argument &e)
                {
                }
                sign = -1;
                try
                {
                    vertexToEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "xyz", sign));
                }
                catch (std::invalid_argument &e)
                {
                }
            }
            else
            {
                int sign = -1;
                try
                {
                    vertexToEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "xy", sign));
                }
                catch (std::invalid_argument &e)
                {
                }
                try
                {
                    vertexToEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "xz", sign));
                }
                catch (std::invalid_argument &e)
                {
                }
                try
                {
                    vertexToEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "yz", sign));
                }
                catch (std::invalid_argument &e)
                {
                }
                sign = 1;
                try
                {
                    vertexToEdges[vertexIndex].push_back(edgeIndex(vertexIndex, "xyz", sign));
                }
                catch (std::invalid_argument &e)
                {
                }
            }
        }
    }
}