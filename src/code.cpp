#include "code.h"
#include "lattice.h"
#include <string>
#include "pcg_random.hpp"
#include <random>
#include <algorithm>

// pcg-random
// Seed with random val, if available
pcg_extras::seed_seq_from<std::random_device> seedSource;
// Make a random number engine
pcg32 pcg(seedSource);
// Create distribution
std::uniform_real_distribution<double> distribution(0, nextafter(1, 2));

std::random_device rd;
std::mt19937 mt(rd());

Code::Code(int latticeLength, const std::string &latticeType, double dataErrorProbability, double measErrorProbability) : l(latticeLength),
                                                                                                                   p(dataErrorProbability),
                                                                                                                   q(measErrorProbability),
                                                                                                                   lattice(latticeLength, latticeType)
{
    if (dataErrorProbability < 0 || dataErrorProbability > 1)
    {
        throw std::invalid_argument("Data error probability must be between zero and one (inclusive).");
    }
    if (measErrorProbability < 0 || measErrorProbability > 1)
    {
        throw std::invalid_argument("Measurement error probability must be between zero and one (inclusive).");
    }
    if (latticeType == "rhombic")
    {
        syndrome.assign(2 * 7 * l * l * l, 0);
    }
    lattice.createFaces();
    lattice.createUpEdgesMap();
    lattice.createVertexToEdges();
}

void Code::generateDataError()
{
    error.clear();
    for (int i = 0; i < 3 * l * l * l; ++i)
    {
        // if (dist(mt) <= p)
        if (distribution(pcg) <= p)
        {
            error.push_back(i);
        }
    }
}

void Code::calculateSyndrome()
{
    vvint faceToEdges = lattice.getFaceToEdges();
    for (const int errorIndex : error)
    {
        vint edges = faceToEdges[errorIndex];
        for (const int edgeIndex : edges)
        {
            syndrome[edgeIndex] = (syndrome[edgeIndex] + 1) % 2;
        }
    }
}

void Code::setError(const vint &err)
{
    error.clear();
    error.reserve(err.size());
    for (const int i : err)
    {
        error.push_back(i);
    }
}

vint Code::getSyndrome()
{
    return syndrome;
}

Lattice Code::getLattice()
{
    return lattice;
}

vint Code::getError()
{
    return error;
}

void Code::generateMeasError()
{
    for (int i = 0; i < syndrome.size(); ++i)
    {
        if (distribution(pcg) <= q)
        {
            syndrome[i] = (syndrome[i] + 1) % 2;
        }
    }
}

bool Code::checkExtremalVertex(int vertexIndex, const std::string &direction)
{
    auto upEdgesMap = lattice.getUpEdgesMap();
    vint upEdges = upEdgesMap[direction][vertexIndex];
    vvint vertexToEdges = lattice.getVertexToEdges();
    vint edges = vertexToEdges[vertexIndex];
    bool edgeInSyndrome = false;
    // std::cout << "Up Edges:" << std::endl;
    // for (int e : upEdges)
    // {
    //     std::cout << e << std::endl;
    // }
    // std::cout << "Edges:" << std::endl;
    // for (int e : edges)
    // {
    //     std::cout << e << std::endl;
    // }
    for (const int edgeIndex : edges)
    {
        if (syndrome[edgeIndex] == 1)
        {
            edgeInSyndrome = true;
            if (std::find(upEdges.begin(), upEdges.end(), edgeIndex) == upEdges.end())
            {
                // Edge in syndrome is not an up edge => vertex is extremal
                return false;
            }
        }
    }
    return edgeInSyndrome;
}
