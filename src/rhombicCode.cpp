#include "code.h"
#include "rhombicCode.h"
#include "rhombicToricLattice.h"
#include "rhombicLattice.h"
#include <string>
#include "pcg_random.hpp"
#include <random>
#include <algorithm>
#include <set>

RhombicCode::RhombicCode(const int l, const double p, const double q, bool boundaries) : Code(l, p, q, boundaries)
{
    if (boundaries)
    {
        numberOfFaces = 3 * pow(l - 1, 3) - 4 * pow(l - 1, 2) + 2 * (l - 1);
        latticeParity = 1;
        lattice = std::make_unique<RhombicLattice>(l);
        buildSyndromeIndices();
    }
    else
    {
        numberOfFaces = 3 * pow(l, 3);
        latticeParity = 0;
        lattice = std::make_unique<RhombicToricLattice>(l);
    }
    numberOfEdges = 2 * 7 * pow(l, 3);
    buildSweepIndices();
    syndrome.assign(numberOfEdges, 0);
    flipBits.assign(numberOfFaces, 0);
    lattice->createFaces();
    lattice->createUpEdgesMap();
    lattice->createVertexToEdges();
    upEdgesMap = lattice->getUpEdgesMap();
    buildLogicals();
}

void RhombicCode::buildSyndromeIndices()
{
    for (int i = 0; i < pow(l, 3); ++i)
    {
        const cartesian4 coordinate = lattice->indexToCoordinate(i);
        if (coordinate.z == 0 || coordinate.y == 0 || coordinate.y == l - 1)
        {
            continue;
        }
        else
        {
            if ((coordinate.x + coordinate.y + coordinate.z) % 2 == latticeParity)
            {
                if (coordinate.z == 1)
                {
                    if (coordinate.x != 0)
                    {
                        syndromeIndices.insert(lattice->edgeIndex(i, "yz", 1));
                        syndromeIndices.insert(lattice->edgeIndex(i, "xy", -1));
                    }
                    if (coordinate.x != l - 1)
                    {
                        syndromeIndices.insert(lattice->edgeIndex(i, "xyz", 1));
                        syndromeIndices.insert(lattice->edgeIndex(i, "xz", 1));
                    }
                }
                else if (coordinate.z == l - 1)
                {
                    if (coordinate.x != 0)
                    {
                        syndromeIndices.insert(lattice->edgeIndex(i, "xyz", -1));
                        syndromeIndices.insert(lattice->edgeIndex(i, "xz", -1));
                    }
                    if (coordinate.x != l - 1)
                    {
                        syndromeIndices.insert(lattice->edgeIndex(i, "yz", -1));
                        syndromeIndices.insert(lattice->edgeIndex(i, "xy", 1));
                    }
                }
                else
                {
                    if (coordinate.x != 0)
                    {
                        syndromeIndices.insert(lattice->edgeIndex(i, "xyz", -1));
                        syndromeIndices.insert(lattice->edgeIndex(i, "xy", -1));
                        syndromeIndices.insert(lattice->edgeIndex(i, "xz", -1));
                        syndromeIndices.insert(lattice->edgeIndex(i, "yz", 1));
                    }
                    if (coordinate.x != l - 1)
                    {
                        syndromeIndices.insert(lattice->edgeIndex(i, "xyz", 1));
                        syndromeIndices.insert(lattice->edgeIndex(i, "xy", 1));
                        syndromeIndices.insert(lattice->edgeIndex(i, "xz", 1));
                        syndromeIndices.insert(lattice->edgeIndex(i, "yz", -1));
                    }
                }
            }
        }
    }
}

void RhombicCode::buildSweepIndices()
{
    if (boundaries)
    {
        for (int i = 0; i < 2 * pow(l, 3); ++i)
        {
            const cartesian4 coordinate = lattice->indexToCoordinate(i);
            if (coordinate.w == 0)
            {
                if ((coordinate.x + coordinate.y + coordinate.z) % 2 == 0)
                {
                    continue;
                }
                if (coordinate.z >= 1 && coordinate.z <= l - 1 && coordinate.x >= 0 && coordinate.x <= l - 1 && coordinate.y >= 1 && coordinate.y <= l - 2)
                {
                    sweepIndices.push_back(i);
                }
            }
            else if (coordinate.w == 1)
            {
                if (coordinate.z >= 1 && coordinate.z <= l - 2 && coordinate.x >= 0 && coordinate.x <= l - 2 && coordinate.y >= 0 && coordinate.y <= l - 2)
                {
                    sweepIndices.push_back(i);
                }
            }
        }
    }
    else
    {
        sweepIndices.assign(2 * pow(l, 3), 0);
        std::iota(std::begin(sweepIndices), std::end(sweepIndices), 0);
    }
}

void RhombicCode::sweep(const std::string &direction, bool greedy)
{
    clearFlipBits();
    vstr edgeDirections;
    if (direction == "xyz")
    {
        edgeDirections = {"xy", "yz", "xz"};
    }
    else if (direction == "xy")
    {
        edgeDirections = {"xyz", "-xz", "-yz"};
    }
    else if (direction == "xz")
    {
        edgeDirections = {"xyz", "-xy", "-yz"};
    }
    else if (direction == "yz")
    {
        edgeDirections = {"xyz", "-xy", "-xz"};
    }
    else if (direction == "-xyz")
    {
        edgeDirections = {"-xy", "-yz", "-xz"};
    }
    else if (direction == "-xy")
    {
        edgeDirections = {"-xyz", "xz", "yz"};
    }
    else if (direction == "-xz")
    {
        edgeDirections = {"-xyz", "xy", "yz"};
    }
    else if (direction == "-yz")
    {
        edgeDirections = {"-xyz", "xy", "xz"};
    }
    else
    {
        throw std::invalid_argument("Invalid sweep direction.");
    }
    // for (int vertexIndex = 0; vertexIndex < 2 * pow(l, 3); ++vertexIndex)
    for (auto const vertexIndex : sweepIndices)
    {
        if (!greedy)
        {
            if (!checkExtremalVertex(vertexIndex, direction))
            {
                continue;
            }
        }
        // std::cout << "Trying to find sweep edges... ";
        vstr sweepEdges = findSweepEdges(vertexIndex, direction);
        // if (sweepEdges.size() > 0)
        // {
        //     std::cerr << "Vertex = " << lattice->indexToCoordinate(vertexIndex) << std::endl;
        //     for (auto &e : sweepEdges)
        //     {
        //         std::cerr << e << std::endl;
        //     }
        // }
        // std::cout << "FOUND." << std::endl;
        if (sweepEdges.size() > 4)
        {
            throw std::length_error("More than four up-edges found for a rhombic lattice vertex.");
        }
        if (sweepEdges.size() == 0)
        {
            continue;
        }
        cartesian4 coordinate = lattice->indexToCoordinate(vertexIndex);
        if (sweepEdges.size() == 1 && (!boundaries || coordinate.w == 0))
        {
            continue;
        }
        if (coordinate.w == 0)
        {
            if ((coordinate.x + coordinate.y + coordinate.z) % 2 == latticeParity)
            {
                sweepFullVertex(vertexIndex, sweepEdges, direction, edgeDirections);
            }
            else
            {
                throw std::invalid_argument("Vertex not present in lattice has up-edges.");
            }
        }
        else
        {
            if (boundaries)
            {
                sweepHalfVertexBoundary(vertexIndex, sweepEdges, direction, edgeDirections);
            }
            else
            {
                sweepHalfVertex(vertexIndex, sweepEdges, direction, edgeDirections);
            }
        }
    }
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (flipBits[i])
        {
            auto it = error.find(i);
            if (it != error.end())
            {
                error.erase(it);
            }
            else
            {
                error.insert(i);
            }
        }
    }
}

vstr RhombicCode::findSweepEdges(const int vertexIndex, const std::string &direction)
{
    vstr sweepEdges;
    auto &upEdges = upEdgesMap[direction][vertexIndex];
    for (const int edge : upEdges)
    {
        if (syndrome[edge] == 1)
        {
            int xyzEdge = -1, xyEdge = -1, xzEdge = -1, yzEdge = -1;
            int minusXYZEdge = -1, minusXYEdge = -1, minusXZEdge = -1, minusYZEdge = -1;
            try
            {
                xyzEdge = lattice->edgeIndex(vertexIndex, "xyz", 1);
            }
            catch (const std::invalid_argument &e)
            {
            }
            try
            {
                xyEdge = lattice->edgeIndex(vertexIndex, "xy", 1);
            }
            catch (const std::invalid_argument &e)
            {
            }
            try
            {
                xzEdge = lattice->edgeIndex(vertexIndex, "xz", 1);
            }
            catch (const std::invalid_argument &e)
            {
            }
            try
            {
                yzEdge = lattice->edgeIndex(vertexIndex, "yz", 1);
            }
            catch (const std::invalid_argument &e)
            {
            }
            try
            {
                minusXYZEdge = lattice->edgeIndex(vertexIndex, "xyz", -1);
            }
            catch (const std::invalid_argument &e)
            {
            }
            try
            {
                minusXYEdge = lattice->edgeIndex(vertexIndex, "xy", -1);
            }
            catch (const std::invalid_argument &e)
            {
            }
            try
            {
                minusXZEdge = lattice->edgeIndex(vertexIndex, "xz", -1);
            }
            catch (const std::invalid_argument &e)
            {
            }
            try
            {
                minusYZEdge = lattice->edgeIndex(vertexIndex, "yz", -1);
            }
            catch (const std::invalid_argument &e)
            {
            }

            if (xyzEdge == edge)
            {
                sweepEdges.push_back("xyz");
            }
            else if (xyEdge == edge)
            {
                sweepEdges.push_back("xy");
            }
            else if (yzEdge == edge)
            {
                sweepEdges.push_back("yz");
            }
            else if (xzEdge == edge)
            {
                sweepEdges.push_back("xz");
            }
            else if (minusYZEdge == edge)
            {
                sweepEdges.push_back("-yz");
            }
            else if (minusXZEdge == edge)
            {
                sweepEdges.push_back("-xz");
            }
            else if (minusXYEdge == edge)
            {
                sweepEdges.push_back("-xy");
            }
            else if (minusXYZEdge == edge)
            {
                sweepEdges.push_back("-xyz");
            }
            else
            {
                throw std::invalid_argument("Edge index does not correspond to a valid edge.");
            }
        }
    }
    return sweepEdges;
}

void RhombicCode::sweepFullVertex(const int vertexIndex, vstr &sweepEdges, const std::string &sweepDirection, const vstr &upEdgeDirections)
{
    // std::cout << "Sweep of coordinate = " << lattice->indexToCoordinate(vertexIndex) << " ... ";
    auto &edge0 = upEdgeDirections[0];
    auto &edge1 = upEdgeDirections[1];
    auto &edge2 = upEdgeDirections[2];
    cartesian4 coordinate = lattice->indexToCoordinate(vertexIndex);
    auto sweepDirectionIndex = std::distance(sweepEdges.begin(), std::find(sweepEdges.begin(), sweepEdges.end(), sweepDirection));
    if (sweepEdges.size() == 4)
    {
        try
        {
            vint vertices = faceVertices(vertexIndex, {sweepDirection, edge0, edge0});
            localFlip(vertices);
        }
        catch (const std::invalid_argument &e)
        {
            // std::cerr << "WARNING: " << e.what() << std::endl;
        }
        try
        {
            vint vertices = faceVertices(vertexIndex, {sweepDirection, edge1, edge1});
            localFlip(vertices);
        }
        catch (const std::invalid_argument &e)
        {
            // std::cerr << "WARNING: " << e.what() << std::endl;
        }
        try
        {
            vint vertices = faceVertices(vertexIndex, {sweepDirection, edge2, edge2});
            localFlip(vertices);
        }
        catch (const std::invalid_argument &e)
        {
            // std::cerr << "WARNING: " << e.what() << std::endl;
        }
    }
    else if (sweepDirectionIndex < sweepEdges.size())
    {
        sweepEdges.erase(sweepEdges.begin() + sweepDirectionIndex);
        if (sweepEdges.size() == 2)
        {
            // int delIndex = distInt0To1(mt);
            int delIndex = distInt0To1(pcg);
            sweepEdges.erase(sweepEdges.begin() + delIndex);
        }
        if (sweepEdges[0] == edge0)
        {
            try
            {
                vint vertices = faceVertices(vertexIndex, {sweepDirection, edge0, edge0});
                localFlip(vertices);
            }
            catch (const std::invalid_argument &e)
            {
                // std::cerr << "WARNING: " << e.what() << std::endl;
            }
        }
        else if (sweepEdges[0] == edge2)
        {
            try
            {
                vint vertices = faceVertices(vertexIndex, {sweepDirection, edge2, edge2});
                localFlip(vertices);
            }
            catch (const std::invalid_argument &e)
            {
                // std::cerr << "WARNING: " << e.what() << std::endl;
            }
        }
        else if (sweepEdges[0] == edge1)
        {
            try
            {
                vint vertices = faceVertices(vertexIndex, {sweepDirection, edge1, edge1});
                localFlip(vertices);
            }
            catch (const std::invalid_argument &e)
            {
                // std::cerr << "WARNING: " << e.what() << std::endl;
            }
        }
        else
        {
            throw std::invalid_argument("Invalid up-edges.");
        }
    }
    else
    {
        if (sweepEdges.size() == 3)
        {
            // int delIndex = distInt0To2(mt);
            int delIndex = distInt0To2(pcg);
            sweepEdges.erase(sweepEdges.begin() + delIndex);
        }
        if ((sweepEdges[0] == edge0 && sweepEdges[1] == edge2) ||
            (sweepEdges[0] == edge2 && sweepEdges[1] == edge0))
        {
            try
            {
                vint vertices = faceVertices(vertexIndex, {sweepDirection, edge0, edge0});
                localFlip(vertices);
            }
            catch (const std::invalid_argument &e)
            {
                // std::cerr << "WARNING: " << e.what() << std::endl;
            }
            try
            {
                vint vertices = faceVertices(vertexIndex, {sweepDirection, edge2, edge2});
                localFlip(vertices);
            }
            catch (const std::invalid_argument &e)
            {
                // std::cerr << "WARNING: " << e.what() << std::endl;
            }
        }
        else if ((sweepEdges[0] == edge0 && sweepEdges[1] == edge1) ||
                 (sweepEdges[0] == edge1 && sweepEdges[1] == edge0))
        {
            try
            {
                vint vertices = faceVertices(vertexIndex, {sweepDirection, edge0, edge0});
                localFlip(vertices);
            }
            catch (const std::invalid_argument &e)
            {
                // std::cerr << "WARNING: " << e.what() << std::endl;
            }
            try
            {
                vint vertices = faceVertices(vertexIndex, {sweepDirection, edge1, edge1});
                localFlip(vertices);
            }
            catch (const std::invalid_argument &e)
            {
                // std::cerr << "WARNING: " << e.what() << std::endl;
            }
        }
        else if ((sweepEdges[0] == edge1 && sweepEdges[1] == edge2) ||
                 (sweepEdges[0] == edge2 && sweepEdges[1] == edge1))
        {
            try
            {
                vint vertices = faceVertices(vertexIndex, {sweepDirection, edge1, edge1});
                localFlip(vertices);
            }
            catch (const std::invalid_argument &e)
            {
                // std::cerr << "WARNING: " << e.what() << std::endl;
            }
            try
            {
                vint vertices = faceVertices(vertexIndex, {sweepDirection, edge2, edge2});
                localFlip(vertices);
            }
            catch (const std::invalid_argument &e)
            {
                // std::cerr << "WARNING: " << e.what() << std::endl;
            }
        }
        else
        {
            throw std::invalid_argument("Invalid up-edges.");
        }
    }
    // std::cout << "Successful." << std::endl;
}

void RhombicCode::sweepHalfVertex(const int vertexIndex, vstr &sweepEdges, const std::string &sweepDirection, const vstr &upEdgeDirections)
{
    // std::cout << "Sweep of coordinate = " << lattice->indexToCoordinate(vertexIndex) << " ... ";
    auto &edge0 = upEdgeDirections[0];
    auto &edge1 = upEdgeDirections[1];
    auto &edge2 = upEdgeDirections[2];
    if (sweepEdges.size() == 3)
    {
        // int delIndex = distInt0To2(mt);
        int delIndex = distInt0To2(pcg);
        sweepEdges.erase(sweepEdges.begin() + delIndex);
    }
    if ((sweepEdges[0] == edge0 && sweepEdges[1] == edge2) ||
        (sweepEdges[0] == edge2 && sweepEdges[1] == edge0))
    {
        vint vertices;
        try
        {
            vertices = faceVertices(vertexIndex, {edge0, edge2, edge2});
            localFlip(vertices);
        }
        catch (const std::invalid_argument &e)
        {
            // std::cerr << "WARNING: " << e.what() << std::endl;
        }
    }
    else if ((sweepEdges[0] == edge0 && sweepEdges[1] == edge1) ||
             (sweepEdges[0] == edge1 && sweepEdges[1] == edge0))
    {
        vint vertices;
        try
        {
            vertices = faceVertices(vertexIndex, {edge0, edge1, edge1});
            localFlip(vertices);
        }
        catch (const std::invalid_argument &e)
        {
            // std::cerr << "WARNING: " << e.what() << std::endl;
        }
    }
    else if ((sweepEdges[0] == edge1 && sweepEdges[1] == edge2) ||
             (sweepEdges[0] == edge2 && sweepEdges[1] == edge1))
    {
        vint vertices;
        try
        {
            vertices = faceVertices(vertexIndex, {edge2, edge1, edge1});
            localFlip(vertices);
        }
        catch (const std::invalid_argument &e)
        {
            // std::cerr << "WARNING: " << e.what() << std::endl;
        }
    }
    else
    {
        throw std::invalid_argument("Invalid up-edges.");
    }
    // std::cout << "Successful." << std::endl;
}

void RhombicCode::sweepHalfVertexBoundary(const int vertexIndex, vstr &sweepEdges, const std::string &sweepDirection, const vstr &upEdgeDirections)
{
    // Minimal option
    cartesian4 coordinate = lattice->indexToCoordinate(vertexIndex);
    if (sweepEdges.size() == 1)
    {
        vint vertices;
        int coordinateSumParity = (coordinate.x + coordinate.y + coordinate.z) % 2;
        if (coordinate.y == 0 && coordinate.z == 1)
        {
            // std::cerr << "y=0, z=1 half vertex" << std::endl;
            // if (sweepEdges[0] == "xy" && coordinateSumParity == latticeParity)
            if (sweepEdges[0] == "xy")
            {
                if (sweepDirection == "-yz" || sweepDirection == "-xz")
                {
                    vertices = faceVertices(vertexIndex, {"xy", "-xyz", "-xyz"});
                    localFlip(vertices);
                }
            }
            else if (sweepEdges[0] == "-xz")
            {
                if (sweepDirection == "xy" || sweepDirection == "-xyz")
                {
                    vertices = faceVertices(vertexIndex, {"-xz", "-yz", "-yz"});
                    localFlip(vertices);
                }
            }
        }
        else if (coordinate.y == 0 && coordinate.z == l - 2)
        {
            // std::cerr << "y=0, z=l-2 half vertex" << std::endl;
            if (sweepEdges[0] == "yz")
            {
                if (sweepDirection == "-xy" || sweepDirection == "xyz")
                {
                    vertices = faceVertices(vertexIndex, {"yz", "xz", "xz"});
                    localFlip(vertices);
                }
            }
            else if (sweepEdges[0] == "xyz")
            {
                if (sweepDirection == "xz" || sweepDirection == "yz")
                {
                    vertices = faceVertices(vertexIndex, {"xyz", "-xy", "-xy"});
                    localFlip(vertices);
                }
            }
        }
        else if (coordinate.y == l - 2 && coordinate.z == 1)
        {
            // std::cerr << "y=l-2, z=1 half vertex" << std::endl;
            if (sweepEdges[0] == "-xyz")
            {
                if (sweepDirection == "-xz" || sweepDirection == "-yz")
                {
                    vertices = faceVertices(vertexIndex, {"-xyz", "xy", "xy"});
                    localFlip(vertices);
                }
            }
            else if (sweepEdges[0] == "-yz")
            {
                if (sweepDirection == "xy" || sweepDirection == "-xyz")
                {
                    vertices = faceVertices(vertexIndex, {"-xz", "-yz", "-yz"});
                    localFlip(vertices);
                }
            }
        }
        else if (coordinate.y == l - 2 && coordinate.z == l - 2)
        {
            // std::cerr << "y=z=l-2 half vertex" << std::endl;
            if (sweepEdges[0] == "xz")
            {
                if (sweepDirection == "-xy" || sweepDirection == "xyz")
                {
                    vertices = faceVertices(vertexIndex, {"xz", "yz", "yz"});
                    localFlip(vertices);
                }
            }
            else if (sweepEdges[0] == "-xy")
            {
                if (sweepDirection == "xz" || sweepDirection == "yz")
                {
                    vertices = faceVertices(vertexIndex, {"xyz", "-xy", "-xy"});
                    localFlip(vertices);
                }
            }
        }
    }
    else
    {
        sweepHalfVertex(vertexIndex, sweepEdges, sweepDirection, upEdgeDirections);
    }
}

void RhombicCode::buildLogicals()
{
    if (boundaries)
    {
        for (int i = 0; i < l; i += 2)
        {
            cartesian4 coordinate = {i, 0, 1, 0};
            int vertexIndex = lattice->coordinateToIndex(coordinate);
            int neighbourVertex = lattice->neighbour(vertexIndex, "xyz", 1);
            vint faceVertices = {vertexIndex,
                                 neighbourVertex,
                                 lattice->neighbour(vertexIndex, "xy", 1),
                                 lattice->neighbour(neighbourVertex, "xy", 1)};
            std::sort(faceVertices.begin(), faceVertices.end());
            logicalZ1.push_back(lattice->findFace(faceVertices));
            if (i != 0)
            {
                neighbourVertex = lattice->neighbour(vertexIndex, "yz", 1);
                faceVertices = {vertexIndex,
                                neighbourVertex,
                                lattice->neighbour(vertexIndex, "xz", -1),
                                lattice->neighbour(neighbourVertex, "xz", -1)};
                std::sort(faceVertices.begin(), faceVertices.end());
                logicalZ1.push_back(lattice->findFace(faceVertices));
            }
        }
    }
    else
    {
        for (int i = 0; i < l; i += 2)
        {
            int vertexIndex = lattice->coordinateToIndex({i, 0, 0, 0});
            int neighbourVertex = lattice->neighbour(vertexIndex, "xz", -1);
            vint faceVertices = {vertexIndex,
                                 neighbourVertex,
                                 lattice->neighbour(vertexIndex, "xyz", -1),
                                 lattice->neighbour(neighbourVertex, "xyz", -1)};
            std::sort(faceVertices.begin(), faceVertices.end());
            logicalZ1.push_back(lattice->findFace(faceVertices));
            neighbourVertex = lattice->neighbour(vertexIndex, "xy", 1);
            faceVertices = {vertexIndex,
                            neighbourVertex,
                            lattice->neighbour(vertexIndex, "yz", -1),
                            lattice->neighbour(neighbourVertex, "yz", -1)};
            std::sort(faceVertices.begin(), faceVertices.end());
            logicalZ1.push_back(lattice->findFace(faceVertices));
        }
        for (int i = 0; i < l; i += 2)
        {
            int vertexIndex = lattice->coordinateToIndex({0, i, 0, 0});
            int neighbourVertex = lattice->neighbour(vertexIndex, "yz", -1);
            vint faceVertices = {vertexIndex,
                                 neighbourVertex,
                                 lattice->neighbour(vertexIndex, "xyz", -1),
                                 lattice->neighbour(neighbourVertex, "xyz", -1)};
            std::sort(faceVertices.begin(), faceVertices.end());
            logicalZ2.push_back(lattice->findFace(faceVertices));
            neighbourVertex = lattice->neighbour(vertexIndex, "xy", 1);
            faceVertices = {vertexIndex,
                            neighbourVertex,
                            lattice->neighbour(vertexIndex, "xz", -1),
                            lattice->neighbour(neighbourVertex, "xz", -1)};
            std::sort(faceVertices.begin(), faceVertices.end());
            logicalZ2.push_back(lattice->findFace(faceVertices));
        }
        for (int i = 0; i < l; i += 2)
        {
            int vertexIndex = lattice->coordinateToIndex({0, 0, i, 0});
            int neighbourVertex = lattice->neighbour(vertexIndex, "xz", -1);
            vint faceVertices = {vertexIndex,
                                 neighbourVertex,
                                 lattice->neighbour(vertexIndex, "xyz", -1),
                                 lattice->neighbour(neighbourVertex, "xyz", -1)};
            std::sort(faceVertices.begin(), faceVertices.end());
            logicalZ3.push_back(lattice->findFace(faceVertices));
            neighbourVertex = lattice->neighbour(vertexIndex, "yz", 1);
            faceVertices = {vertexIndex,
                            neighbourVertex,
                            lattice->neighbour(vertexIndex, "xy", -1),
                            lattice->neighbour(neighbourVertex, "xy", -1)};
            std::sort(faceVertices.begin(), faceVertices.end());
            logicalZ3.push_back(lattice->findFace(faceVertices));
        }
    }
}