#include "code.h"
#include "rhombicToricLattice.h"
#include "rhombicLattice.h"
#include <string>
#include "pcg_random.hpp"
#include <random>
#include <algorithm>
#include <set>

Code::Code(const int ll, const double dataP, const double measP, bool boundaries) : l(ll),
                                                                   p(dataP),
                                                                   q(measP),
                                                                   boundaries(boundaries)
{
    if (dataP < 0 || dataP > 1)
    {
        throw std::invalid_argument("Data error probability must be between zero and one (inclusive).");
    }
    if (measP < 0 || measP > 1)
    {
        throw std::invalid_argument("Measurement error probability must be between zero and one (inclusive).");
    }
    pcg_extras::seed_seq_from<std::random_device> seedSource;
    pcg = pcg32(seedSource);
    distDouble0To1 = std::uniform_real_distribution<double>(0, nextafter(1, 2));
    distInt0To2 = std::uniform_int_distribution<int>(0, 2);
    distInt0To1 = std::uniform_int_distribution<int>(0, 1);
}

void Code::generateDataError()
{
    // error.clear();
    for (int i = 0; i < numberOfFaces; ++i)
    {
        // if (distDouble0To1(mt) <= p)
        if (distDouble0To1(pcg) <= p)
        {
            auto it = error.find(i);
            if (it == error.end())
            {
                error.insert(i);
            }
            else
            {
                error.erase(it);
            }
        }
    }
}

void Code::setError(const std::set<int> &err)
{
    error.clear();
    for (const int i : err)
    {
        error.insert(i);
    }
}

void Code::setSyndrome(std::vector<int8_t> &synd)
{
    syndrome = synd;
}

std::vector<int8_t> &Code::getSyndrome()
{
    return syndrome;
}

Lattice &Code::getLattice()
{
    return *lattice;
}

std::set<int> &Code::getError()
{
    return error;
}

bool Code::checkExtremalVertex(const int vertexIndex, const std::string &direction)
{
    auto &upEdgesMap = lattice->getUpEdgesMap();
    auto &upEdges = upEdgesMap[direction][vertexIndex];
    auto &vertexToEdges = lattice->getVertexToEdges();
    auto &edges = vertexToEdges[vertexIndex];
    bool edgeInSyndrome = false;
    for (const int edgeIndex : edges)
    {
        if (syndrome[edgeIndex] == 1)
        {
            edgeInSyndrome = true;
            if (std::find(upEdges.begin(), upEdges.end(), edgeIndex) == upEdges.end())
            {
                // Edge in syndrome is not an up-edge => vertex is not extremal
                return false;
            }
        }
    }
    return edgeInSyndrome;
}

void Code::localFlip(vint &vertices)
{
    // std::cout << "Attempting local flip ... ";
    int faceIndex = lattice->findFace(vertices);
    flipBits[faceIndex] = (flipBits[faceIndex] + 1) % 2;
    // std::cout << "flipped." << std::endl;
}

vint Code::faceVertices(const int vertexIndex, vstr directions)
{
    if (directions.size() != 3)
    {
        throw std::invalid_argument("Number of directions not equal to three.");
    }
    vint signs;
    signs.reserve(3);
    for (int i = 0; i < 3; ++i)
    {
        if (directions[i].at(0) == '-')
        {
            signs.push_back(-1);
            directions[i] = directions[i].substr(1, directions[i].size() - 1);
        }
        else
        {
            signs.push_back(1);
        }
    }
    if (signs[1] != signs[2] || directions[1] != directions[2])
    {
        throw std::invalid_argument("Second and third directions (& signs) must be the same otherwise the vertices do not form a face.");
    }
    int neighbourVertex = lattice->neighbour(vertexIndex, directions[0], signs[0]);
    vint vertices = {vertexIndex, neighbourVertex,
                     lattice->neighbour(vertexIndex, directions[1], signs[1]),
                     lattice->neighbour(neighbourVertex, directions[2], signs[2])};
    std::sort(vertices.begin(), vertices.end());
    return vertices;
}

std::vector<int8_t> &Code::getFlipBits()
{
    return flipBits;
}

void Code::clearSyndrome()
{
    syndrome.assign(numberOfEdges, 0);
}

void Code::clearFlipBits()
{
    flipBits.assign(numberOfFaces, 0);
}

void Code::printUnsatisfiedStabilisers()
{
    for (int i = 0; i < syndrome.size(); ++i)
    {
        if (syndrome[i] == 1)
        {
            std::cerr << i << std::endl;
        }
    }
}

void Code::printError()
{
    auto &faceToVertices = lattice->getFaceToVertices();
    for (auto &face : error)
    {
        vint vertices = faceToVertices[face];
        std::cerr << face << std::endl;
        std::cerr << lattice->indexToCoordinate(vertices[0]);
        std::cerr << lattice->indexToCoordinate(vertices[1]);
        std::cerr << lattice->indexToCoordinate(vertices[2]);
        std::cerr << lattice->indexToCoordinate(vertices[3]) << std::endl;
    }
}

std::set<int> &Code::getSyndromeIndices()
{
    return syndromeIndices;
}

vint &Code::getSweepIndices()
{
    return sweepIndices;
}

vvint Code::getLogicals()
{
    vvint logicals;
    if (boundaries)
    {
        logicals.push_back(logicalZ1);
    }
    else
    {
        logicals.push_back(logicalZ1);
        logicals.push_back(logicalZ2);
        logicals.push_back(logicalZ3);
    }
    return logicals;
}

bool Code::checkCorrection()
{
    int parityZ1 = 0, parityZ2 = 0, parityZ3 = 0;
    for (int faceIndex : logicalZ1)
    {
        if (error.find(faceIndex) != error.end())
        {
            parityZ1 = (parityZ1 + 1) % 2;
            // std::cout << faceIndex << std::endl;
        }
    }
    if (parityZ1 % 2 == 1)
    {
        return false;
    }
    if (!boundaries)
    {
        for (int faceIndex : logicalZ2)
        {
            if (error.find(faceIndex) != error.end())
            {
                parityZ2 = (parityZ2 + 1) % 2;
                // std::cout << faceIndex << std::endl;
            }
        }
        if (parityZ2 % 2 == 1)
        {
            return false;
        }
        for (int faceIndex : logicalZ3)
        {
            if (error.find(faceIndex) != error.end())
            {
                parityZ3 = (parityZ3 + 1) % 2;
                // std::cout << faceIndex << std::endl;
            }
        }
        if (parityZ3 % 2 == 1)
        {
            return false;
        }
    }
    return true;
}

void Code::calculateSyndrome()
{
    clearSyndrome();
    auto &faceToEdges = lattice->getFaceToEdges();
    for (const int errorIndex : error)
    {
        auto &edges = faceToEdges[errorIndex];
        for (const int edgeIndex : edges)
        {
            if (boundaries)
            {
                auto it = syndromeIndices.find(edgeIndex);
                if (it == syndromeIndices.end())
                {
                    continue;
                }
            }
            syndrome[edgeIndex] = (syndrome[edgeIndex] + 1) % 2;
        }
    }
}

void Code::generateMeasError()
{
    for (int i = 0; i < syndrome.size(); ++i)
    {
        if (boundaries)
        {
            auto it = syndromeIndices.find(i);
            if (it == syndromeIndices.end())
            {
                continue;
            }
        }
        // if (distDouble0To1(mt) <= q)
        if (distDouble0To1(pcg) <= q)
        {
            syndrome[i] = (syndrome[i] + 1) % 2;
        }
    }
}