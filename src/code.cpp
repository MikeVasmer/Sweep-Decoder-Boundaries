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
std::uniform_real_distribution<double> distDouble0To1(0, nextafter(1, 2));
std::uniform_int_distribution<int> distInt0To2(0, 2);
std::uniform_int_distribution<int> distInt0To1(0, 1);

// STL random mersenne twister
std::random_device rd;
std::mt19937 mt(rd());

Code::Code(const int ll, const std::string &lType, const double dataP, const double measP) : l(ll),
                                                                                             p(dataP),
                                                                                             q(measP),
                                                                                             lattice(ll, lType)
{
    if (dataP < 0 || dataP > 1)
    {
        throw std::invalid_argument("Data error probability must be between zero and one (inclusive).");
    }
    if (measP < 0 || measP > 1)
    {
        throw std::invalid_argument("Measurement error probability must be between zero and one (inclusive).");
    }
    if (lType == "rhombic")
    {
        syndrome.assign(2 * 7 * l * l * l, 0);
        flipBits.assign(3 * l * l * l, 0);
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
        if (distDouble0To1(pcg) <= p)
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

void Code::setSyndrome(vint synd)
{
    syndrome = synd;
}

const vint &Code::getSyndrome() const
{
    return syndrome;
}

Lattice &Code::getLattice()
{
    return lattice;
}

const vint &Code::getError() const
{
    return error;
}

void Code::generateMeasError()
{
    for (int i = 0; i < syndrome.size(); ++i)
    {
        if (distDouble0To1(pcg) <= q)
        {
            syndrome[i] = (syndrome[i] + 1) % 2;
        }
    }
}

bool Code::checkExtremalVertex(const int vertexIndex, const std::string &direction)
{
    auto upEdgesMap = lattice.getUpEdgesMap();
    vint upEdges = upEdgesMap[direction][vertexIndex];
    vvint vertexToEdges = lattice.getVertexToEdges();
    vint edges = vertexToEdges[vertexIndex];
    bool edgeInSyndrome = false;
    for (const int edgeIndex : edges)
    {
        if (syndrome[edgeIndex] == 1)
        {
            edgeInSyndrome = true;
            if (std::find(upEdges.begin(), upEdges.end(), edgeIndex) == upEdges.end())
            {
                // Edge in syndrome is not an up-edge => vertex is extremal
                return false;
            }
        }
    }
    return edgeInSyndrome;
}

void Code::sweep(const std::string &direction, bool greedy)
{
    vstr edgeDirections;
    vvint signsFullVertex, signsHalfVertex;
    if (direction == "xyz")
    {
        edgeDirections = {"xy", "yz", "xz"};
        // signsFullVertex = {{1, 1, 1}, {1, 1, 1}, {1, 1, 1}};
        // signsHalfVertex = {{1, 1, 1}, {1, 1, 1}, {1, 1, 1}};
    }
    else if (direction == "xy")
    {
        edgeDirections = {"xyz", "-xz", "-yz"};
        // signsFullVertex = {{1, 1, 1}, {1, -1, -1}, {1, -1, -1}};
        // signsHalfVertex = {{1, -1, -1}, {1, -1, 1}, {-1, -1, -1}};
    }
    else if (direction == "xz")
    {
        edgeDirections = {"xyz", "-xy", "-yz"};
        // signsFullVertex = {{1, 1, 1}, {1, -1, -1}, {1, -1, -1}};
        // signsHalfVertex = {{1, -1, -1}, {1, -1, -1}, {-1, -1, -1}};
    }
    else if (direction == "yz")
    {
        edgeDirections = {"xyz", "-xy", "-xz"};
        // signsFullVertex = {{1, 1, 1}, {1, -1, -1}, {1, -1, -1}};
        // signsHalfVertex = {{1, -1, -1}, {1, -1, -1}, {-1, -1, -1}};
    }
    else if (direction == "-xyz")
    {
        edgeDirections = {"-xy", "-yz", "-xz"};
        // signsFullVertex = {{-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}};
        // signsHalfVertex = {{-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}};
    }
    else if (direction == "-xy")
    {
        edgeDirections = {"-xyz", "xz", "yz"};
        // signsFullVertex = {{-1, -1, -1}, {-1, 1, 1}, {-1, 1, 1}};
        // signsHalfVertex = {{-1, 1, 1}, {-1, 1, 1}, {1, 1, 1}};
    }
    else if (direction == "-xz")
    {
        edgeDirections = {"-xyz", "xy", "yz"};
        // signsFullVertex = {{-1, -1, -1}, {-1, 1, 1}, {-1, 1, 1}};
        // signsHalfVertex = {{-1, 1, 1}, {-1, 1, 1}, {1, 1, 1}};
    }
    else if (direction == "-yz")
    {
        edgeDirections = {"-xyz", "xy", "xz"};
        // signsFullVertex = {{-1, -1, -1}, {-1, 1, 1}, {-1, 1, 1}};
        // signsHalfVertex = {{-1, 1, 1}, {-1, 1, 1}, {1, 1, 1}};
    }
    else
    {
        throw std::invalid_argument("Invalid direction.");
    }
    for (int vertexIndex = 0; vertexIndex < 2 * l * l * l; ++vertexIndex)
    {
        if (!greedy)
        {
            if (!checkExtremalVertex(vertexIndex, direction))
            {
                continue;
            }
        }
        vstr sweepEdges = findSweepEdges(vertexIndex, direction);
        if (sweepEdges.size() > 4)
        {
            throw std::length_error("More than four up-edges found for a vertex.");
        }
        if (sweepEdges.size() == 0 || sweepEdges.size() == 1)
        {
            continue;
        }
        cartesian4 coordinate = lattice.indexToCoordinate(vertexIndex);
        if (coordinate.w == 0)
        {
            if ((coordinate.x + coordinate.y + coordinate.z) % 2 == 0)
            {
                sweepFullVertex(vertexIndex, sweepEdges, direction, edgeDirections);
            }
            else
            {
                throw std::invalid_argument("Odd w=0 vertex (ie not present in rhombic lattice) has up-edges.");
            }
        }
        else
        {
            sweepHalfVertex(vertexIndex, sweepEdges, direction, edgeDirections);
        }
    }
    // Update the error here using the flipBits
}

void Code::localFlip(vint &vertices)
{
    int faceIndex = lattice.findFace(vertices);
    flipBits[faceIndex] = (flipBits[faceIndex] + 1) % 2;
}

vstr Code::findSweepEdges(const int vertexIndex, const std::string &direction)
{
    vstr sweepEdges;
    auto upEdgesMap = lattice.getUpEdgesMap();
    vint upEdges = upEdgesMap[direction][vertexIndex];
    for (const int edge : upEdges)
    {
        if (syndrome[edge] == 1)
        {
            if (lattice.edgeIndex(vertexIndex, "xyz", 1) == edge)
            {
                sweepEdges.push_back("xyz");
            }
            else if (lattice.edgeIndex(vertexIndex, "xyz", -1) == edge)
            {
                sweepEdges.push_back("-xyz");
            }
            else if (lattice.edgeIndex(vertexIndex, "xy", 1) == edge)
            {
                sweepEdges.push_back("xy");
            }
            else if (lattice.edgeIndex(vertexIndex, "xy", -1) == edge)
            {
                sweepEdges.push_back("-xy");
            }
            else if (lattice.edgeIndex(vertexIndex, "yz", 1) == edge)
            {
                sweepEdges.push_back("yz");
            }
            else if (lattice.edgeIndex(vertexIndex, "yz", -1) == edge)
            {
                sweepEdges.push_back("-yz");
            }
            else if (lattice.edgeIndex(vertexIndex, "xz", 1) == edge)
            {
                sweepEdges.push_back("xz");
            }
            else if (lattice.edgeIndex(vertexIndex, "xz", -1) == edge)
            {
                sweepEdges.push_back("-xz");
            }
            else
            {
                throw std::invalid_argument("Edge index does not correspond to a valid edge.");
            }
        }
    }
    return sweepEdges;
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
    int neighbourVertex = lattice.neighbour(vertexIndex, directions[0], signs[0]);
    vint vertices = {vertexIndex, neighbourVertex,
                     lattice.neighbour(vertexIndex, directions[1], signs[1]),
                     lattice.neighbour(neighbourVertex, directions[2], signs[2])};
    std::sort(vertices.begin(), vertices.end());
    return vertices;
}

vint &Code::getFlipBits()
{
    return flipBits;
}

void Code::sweepFullVertex(const int vertexIndex, vstr &sweepEdges, const std::string &sweepDirection, const vstr &upEdgeDirections)
{
    std::string edge0 = upEdgeDirections[0];
    std::string edge1 = upEdgeDirections[1];
    std::string edge2 = upEdgeDirections[2];
    cartesian4 coordinate = lattice.indexToCoordinate(vertexIndex);
    auto sweepDirectionIndex = std::distance(sweepEdges.begin(), std::find(sweepEdges.begin(), sweepEdges.end(), sweepDirection));
    if (sweepEdges.size() == 4)
    {
        vint vertices = faceVertices(vertexIndex, {sweepDirection, edge0, edge0});
        localFlip(vertices);
        vertices = faceVertices(vertexIndex, {sweepDirection, edge1, edge1});
        localFlip(vertices);
        vertices = faceVertices(vertexIndex, {sweepDirection, edge2, edge2});
        localFlip(vertices);
    }
    else if (sweepDirectionIndex < sweepEdges.size())
    {
        sweepEdges.erase(sweepEdges.begin() + sweepDirectionIndex);
        if (sweepEdges.size() == 2)
        {
            int delIndex = distInt0To1(pcg);
            sweepEdges.erase(sweepEdges.begin() + delIndex);
        }
        if (sweepEdges[0] == edge0)
        {
            vint vertices = faceVertices(vertexIndex, {sweepDirection, edge0, edge0});
            localFlip(vertices);
        }
        else if (sweepEdges[0] == edge2)
        {
            vint vertices = faceVertices(vertexIndex, {sweepDirection, edge2, edge2});
            localFlip(vertices);
        }
        else if (sweepEdges[0] == edge1)
        {
            vint vertices = faceVertices(vertexIndex, {sweepDirection, edge1, edge1});
            localFlip(vertices);
        }
        else
        {
            throw std::invalid_argument("Invalid up-edges for even w=0 vertex.");
        }
    }
    else
    {
        if (sweepEdges.size() == 3)
        {
            int delIndex = distInt0To2(pcg);
            sweepEdges.erase(sweepEdges.begin() + delIndex);
        }
        if ((sweepEdges[0] == edge0 && sweepEdges[1] == edge2) ||
            (sweepEdges[0] == edge2 && sweepEdges[1] == edge0))
        {
            vint vertices = faceVertices(vertexIndex, {sweepDirection, edge0, edge0});
            localFlip(vertices);
            vertices = faceVertices(vertexIndex, {sweepDirection, edge2, edge2});
            localFlip(vertices);
        }
        else if ((sweepEdges[0] == edge0 && sweepEdges[1] == edge1) ||
                 (sweepEdges[0] == edge1 && sweepEdges[1] == edge0))
        {
            vint vertices = faceVertices(vertexIndex, {sweepDirection, edge0, edge0});
            localFlip(vertices);
            vertices = faceVertices(vertexIndex, {sweepDirection, edge1, edge1});
            localFlip(vertices);
        }
        else if ((sweepEdges[0] == edge1 && sweepEdges[1] == edge2) ||
                 (sweepEdges[0] == edge2 && sweepEdges[1] == edge1))
        {
            vint vertices = faceVertices(vertexIndex, {sweepDirection, edge1, edge1});
            localFlip(vertices);
            vertices = faceVertices(vertexIndex, {sweepDirection, edge2, edge2});
            localFlip(vertices);
        }
        else
        {
            throw std::invalid_argument("Invalid up-edges for even w=0 vertex.");
        }
    }
}

void Code::sweepHalfVertex(const int vertexIndex, vstr &sweepEdges, const std::string &sweepDirection, const vstr &upEdgeDirections)
{
    std::string edge0 = upEdgeDirections[0];
    std::string edge1 = upEdgeDirections[1];
    std::string edge2 = upEdgeDirections[2];
    if (sweepEdges.size() == 3)
    {
        int delIndex = distInt0To2(pcg);
        sweepEdges.erase(sweepEdges.begin() + delIndex);
    }
    if ((sweepEdges[0] == edge0 && sweepEdges[1] == edge2) ||
        (sweepEdges[0] == edge2 && sweepEdges[1] == edge0))
    {
        vint vertices = faceVertices(vertexIndex, {edge0, edge2, edge2});
        localFlip(vertices);
    }
    else if ((sweepEdges[0] == edge0 && sweepEdges[1] == edge1) ||
             (sweepEdges[0] == edge1 && sweepEdges[1] == edge0))
    {
        vint vertices = faceVertices(vertexIndex, {edge0, edge1, edge1});
        localFlip(vertices);
    }
    else if ((sweepEdges[0] == edge1 && sweepEdges[1] == edge2) ||
             (sweepEdges[0] == edge2 && sweepEdges[1] == edge1))
    {
        vint vertices = faceVertices(vertexIndex, {edge2, edge1, edge1});
        localFlip(vertices);
    }
    else
    {
        throw std::invalid_argument("Invalid up-edges for even w=1 vertex.");
    }
}

void Code::clearSyndrome()
{
    syndrome.assign(2 * 7 * l * l * l, 0);
}

void Code::clearFlipBits()
{
    flipBits.assign(3 * l * l * l, 0);
}