#include "code.h"
#include "cubicCode.h"
#include "cubicLattice.h"
#include "cubicToricLattice.h"
#include <string>
#include <algorithm>

CubicCode::CubicCode(const int l, const double p, const double q, bool boundaries, const int sweepRate) : Code(l, p, q, boundaries, sweepRate)
{
    if (boundaries)
    {
        numberOfFaces = 3 * pow(l - 1, 3) - 4 * pow(l - 1, 2) + 2 * (l - 1);
        lattice = std::make_unique<CubicLattice>(l);
        buildSyndromeIndices();
    }
    else
    {
        numberOfFaces = 3 * pow(l, 3);
        lattice = std::make_unique<CubicToricLattice>(l);
    }
    numberOfEdges = 7 * pow(l, 3);
    buildSweepIndices();
    syndrome.assign(numberOfFaces, 0);
    flipBits.assign(numberOfFaces, 0);
    lattice->createFaces();
    lattice->createUpEdgesMap();
    lattice->createVertexToEdges();
    upEdgesMap = lattice->getUpEdgesMap();
    faceToEdges = lattice->getFaceToEdges(); 
    vertexToEdges = lattice->getVertexToEdges();
    buildLogicals();
}

void CubicCode::buildSyndromeIndices()
{
    for (int i = 0; i < pow(l, 3); ++i)
    {
        const cartesian4 coordinate = lattice->indexToCoordinate(i);
        if (coordinate.z < l - 2 && coordinate.x > 0 && coordinate.x < l - 1 && coordinate.y > 0 && coordinate.y < l - 1)
        {
            syndromeIndices.insert(lattice->edgeIndex(i, "z", 1));
        }
        if (coordinate.z < l - 1 && coordinate.x > 0 && coordinate.x < l - 1 && coordinate.y < l - 1)
        {
            syndromeIndices.insert(lattice->edgeIndex(i, "y", 1));
        }
        if (coordinate.z < l - 1 && coordinate.y > 0 && coordinate.y < l - 1 && coordinate.x < l - 1)
        {
            syndromeIndices.insert(lattice->edgeIndex(i, "x", 1));
        }
    }
}

void CubicCode::buildSweepIndices()
{
    if (boundaries)
    {
        for (int i = 0; i < pow(l, 3); ++i)
        {
            const cartesian4 coordinate = lattice->indexToCoordinate(i);
            if (coordinate.x > 0 && coordinate.x < l - 1 && coordinate.y > 0 && coordinate.y < l - 1 && coordinate.z < l -1)
            {
                sweepIndices.push_back(i);
            }
        }
    }
    else
    {
        sweepIndices.assign(pow(l, 3), 0);
        std::iota(std::begin(sweepIndices), std::end(sweepIndices), 0);
    }
}

void CubicCode::sweep(const std::string &direction, bool greedy, bool virtualSweep)
{
    clearFlipBits();
    vstr edgeDirections;
    if (direction == "xyz")
    {
        edgeDirections = {"x", "y", "z"};
    }
    else if (direction == "xy")
    {
        edgeDirections = {"x", "y", "-z"};
    }
    else if (direction == "xz")
    {
        edgeDirections = {"x", "-y", "z"};
    }
    else if (direction == "yz")
    {
        edgeDirections = {"-x", "y", "z"};
    }
    else if (direction == "-xyz")
    {
        edgeDirections = {"-x", "-y", "-z"};
    }
    else if (direction == "-xy")
    {
        edgeDirections = {"-x", "-y", "z"};
    }
    else if (direction == "-xz")
    {
        edgeDirections = {"-x", "y", "-z"};
    }
    else if (direction == "-yz")
    {
        edgeDirections = {"x", "-y", "-z"};
    }
    else
    {
        throw std::invalid_argument("Invalid sweep direction.");
    }
    for (auto const vertexIndex : sweepIndices)
    {
        if (!greedy)
        {
            if (!checkExtremalVertex(vertexIndex, direction))
            {
                continue;
            }
        }
        vstr sweepEdges = findSweepEdges(vertexIndex, direction);
        if (sweepEdges.size() > 3)
        {
            throw std::length_error("More than three up-edges found for a cubic lattice vertex.");
        }
        if (sweepEdges.size() < 2)
        {
            continue;
        }
        cellularAutomatonStep(vertexIndex, sweepEdges, direction, edgeDirections);
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
            if (sweepRate > 1)
            {
                for (const int edge : faceToEdges[i])
                {
                    // std::cerr << edge << std::endl;
                    if (boundaries)
                    {
                        auto it2 = syndromeIndices.find(edge);
                        if (it2 == syndromeIndices.end())
                        {
                            continue;
                        }
                    }
                    syndrome[edge] = (syndrome[edge] + 1) % 2;
                }
            }
        }
    }
}

void CubicCode::cellularAutomatonStep(const int vertexIndex, vstr &sweepEdges, const std::string &sweepDirections, const vstr &upEdgeDirections)
{
    auto &edge0 = upEdgeDirections[0];
    auto &edge1 = upEdgeDirections[1];
    auto &edge2 = upEdgeDirections[2];
    if (sweepEdges.size() == 3)
    {
        int delIndex = distInt0To2(rnEngine);
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
        }
    }
    else
    {
        throw std::invalid_argument("Invalid up-edges.");
    }
}

vstr CubicCode::findSweepEdges(const int vertexIndex, const std::string &direction)
{
    vstr sweepEdges;
    auto &upEdges = upEdgesMap[direction][vertexIndex];
    for (const int edge : upEdges)
    {
        if (syndrome[edge] == 1)
        {
            int xEdge = -1, yEdge = -1, zEdge = -1;
            int minusXEdge = -1, minusYEdge = -1, minusZEdge = -1;
            try
            {
                xEdge = lattice->edgeIndex(vertexIndex, "x", 1);
            }
            catch (const std::invalid_argument &e)
            {
            }
            try
            {
                yEdge = lattice->edgeIndex(vertexIndex, "y", 1);
            }
            catch (const std::invalid_argument &e)
            {
            }
            try
            {
                zEdge = lattice->edgeIndex(vertexIndex, "z", 1);
            }
            catch (const std::invalid_argument &e)
            {
            }
            try
            {
                minusXEdge = lattice->edgeIndex(vertexIndex, "x", -1);
            }
            catch (const std::invalid_argument &e)
            {
            }
            try
            {
                minusYEdge = lattice->edgeIndex(vertexIndex, "y", -1);
            }
            catch (const std::invalid_argument &e)
            {
            }
            try
            {
                minusZEdge = lattice->edgeIndex(vertexIndex, "z", -1);
            }
            catch (const std::invalid_argument &e)
            {
            }
            if (xEdge == edge)
            {
                sweepEdges.push_back("x");
            }
            else if (yEdge == edge)
            {
                sweepEdges.push_back("y");
            }
            else if (zEdge == edge)
            {
                sweepEdges.push_back("z");
            }
            else if (minusXEdge == edge)
            {
                sweepEdges.push_back("-x");
            }
            else if (minusYEdge == edge)
            {
                sweepEdges.push_back("-y");
            }
            else if (minusZEdge == edge)
            {
                sweepEdges.push_back("-z");
            }
            else
            {
                throw std::invalid_argument("Edge index does not correspond to a valid edge.");
            }
        }
    }
    return sweepEdges;
}

void CubicCode::buildLogicals()
{
    for (int i = 0; i < l - 1; ++i)
    {
        cartesian4 coordinate{0, 0, i, 0};
        int vertexIndex = lattice->coordinateToIndex(coordinate);
        int neighbourVertex = lattice->neighbour(vertexIndex, "x", 1);
        vint faceVertices = {vertexIndex,
                                neighbourVertex,
                                lattice->neighbour(vertexIndex, "y", 1),
                                lattice->neighbour(neighbourVertex, "y", 1)};
        std::sort(faceVertices.begin(), faceVertices.end());
        logicalZ1.push_back(lattice->findFace(faceVertices));
    }
    if (!boundaries)
    {
        for (int i = 0; i < l - 1; ++i)
        {
            cartesian4 coordinate{i, 0, 0, 0};
            int vertexIndex = lattice->coordinateToIndex(coordinate);
            int neighbourVertex = lattice->neighbour(vertexIndex, "y", 1);
            vint faceVertices = {vertexIndex,
                                    neighbourVertex,
                                    lattice->neighbour(vertexIndex, "z", 1),
                                    lattice->neighbour(neighbourVertex, "z", 1)};
            std::sort(faceVertices.begin(), faceVertices.end());
            logicalZ2.push_back(lattice->findFace(faceVertices));
        }
        for (int i = 0; i < l - 1; ++i)
        {
            cartesian4 coordinate{0, i, 0, 0};
            int vertexIndex = lattice->coordinateToIndex(coordinate);
            int neighbourVertex = lattice->neighbour(vertexIndex, "x", 1);
            vint faceVertices = {vertexIndex,
                                    neighbourVertex,
                                    lattice->neighbour(vertexIndex, "z", 1),
                                    lattice->neighbour(neighbourVertex, "z", 1)};
            std::sort(faceVertices.begin(), faceVertices.end());
            logicalZ3.push_back(lattice->findFace(faceVertices));
        }
    }
}