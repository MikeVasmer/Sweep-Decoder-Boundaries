#include "code.h"
#include "cubicCode.h"
#include "cubicLattice.h"
#include "cubicToricLattice.h"
#include <string>
#include <algorithm>

CubicCode::CubicCode(const int l, const double p, const double q, bool boundaries) : Code(l, p, q, boundaries)
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

void CubicCode::sweep(const std::string &direction, bool greedy)
{
    // ToDo
}

vstr CubicCode::findSweepEdges(const int vertexIndex, const std::string &direction)
{
    // ToDo
    return {"-1"};
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