#include "code.h"
#include "rhombicToricLattice.h"
#include "rhombicLattice.h"
#include <string>
#include "pcg_random.hpp"
#include <random>
#include <algorithm>
#include <set>

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
// std::random_device rd;
// std::mt19937 mt(rd());
// std::mt19937 mt;

Code::Code(const int ll, const std::string &lType, const double dataP, const double measP) : l(ll),
                                                                                             p(dataP),
                                                                                             q(measP),
                                                                                             latticeType(lType)
{
    if (dataP < 0 || dataP > 1)
    {
        throw std::invalid_argument("Data error probability must be between zero and one (inclusive).");
    }
    if (measP < 0 || measP > 1)
    {
        throw std::invalid_argument("Measurement error probability must be between zero and one (inclusive).");
    }
    if (lType == "rhombic toric")
    {
        numberOfFaces = (3 * l * l * l);
        latticeParity = 0;
        lattice = std::make_unique<RhombicToricLattice>(l);
    }
    else if (lType == "rhombic boundaries")
    {
        numberOfFaces = 3 * pow(l - 1, 3) - 4 * pow(l - 1, 2) + 2 * (l - 1);
        latticeParity = 1;
        lattice = std::make_unique<RhombicLattice>(l);
        buildSyndromeIndices();
    }
    buildSweepIndices();
    numberOfEdges = 2 * 7 * l * l * l;
    syndrome.assign(numberOfEdges, 0);
    flipBits.assign(numberOfFaces, 0);
    lattice->createFaces();
    lattice->createUpEdgesMap();
    lattice->createVertexToEdges();
    upEdgesMap = lattice->getUpEdgesMap();
    buildLogicals();
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

void Code::calculateSyndrome()
{
    clearSyndrome();
    vvint faceToEdges = lattice->getFaceToEdges();
    for (const int errorIndex : error)
    {
        vint edges = faceToEdges[errorIndex];
        for (const int edgeIndex : edges)
        {
            if (latticeType == "rhombic boundaries")
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

void Code::buildSyndromeIndices()
{
    for (int i = 0; i < l * l * l; ++i)
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

void Code::buildSweepIndices()
{
    if (latticeType == "rhombic toric")
    {
        sweepIndices.assign(2 * l * l * l, 0);
        std::iota (std::begin(sweepIndices), std::end(sweepIndices), 0);
    }
    else if (latticeType == "rhombic boundaries")
    {
        for (int i = 0; i < 2 * l * l * l; ++i)
        {
            const cartesian4 coordinate = lattice->indexToCoordinate(i);
            if (coordinate.w == 0)
            {
                if ((coordinate.x + coordinate.y +coordinate.z) % 2 == 0)
                {
                    continue;
                }
                if (coordinate.z >=1 && coordinate.z <= l - 1 && coordinate.x >= 0 && coordinate.x <= l - 1 && coordinate.y >= 1 && coordinate.y <= l - 2)
                {
                    sweepIndices.push_back(i);
                }
            }
            else if (coordinate.w == 1)
            {
                if (coordinate.z >=1 && coordinate.z <= l - 2 && coordinate.x >= 0 && coordinate.x <= l - 2 && coordinate.y >= 0 && coordinate.y <= l - 2)
                {
                    sweepIndices.push_back(i);
                }
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

void Code::setSyndrome(vint synd)
{
    syndrome = synd;
}

vint &Code::getSyndrome()
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

void Code::generateMeasError()
{
    for (int i = 0; i < syndrome.size(); ++i)
    {
        if (latticeType == "rhombic boundaries")
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

bool Code::checkExtremalVertex(const int vertexIndex, const std::string &direction)
{
    auto upEdgesMap = lattice->getUpEdgesMap();
    vint upEdges = upEdgesMap[direction][vertexIndex];
    vvint vertexToEdges = lattice->getVertexToEdges();
    vint edges = vertexToEdges[vertexIndex];
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

void Code::sweep(const std::string &direction, bool greedy)
{
    clearFlipBits();
    vstr edgeDirections;
    vvint signsFullVertex, signsHalfVertex;
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
        throw std::invalid_argument("Invalid direction.");
    }
    // for (int vertexIndex = 0; vertexIndex < 2 * l * l * l; ++vertexIndex)
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
        // std::cout << "FOUND." << std::endl;
        if (sweepEdges.size() > 4)
        {
            throw std::length_error("More than four up-edges found for a vertex.");
        }
        if (sweepEdges.size() == 0)
        {
            continue;
        }
        cartesian4 coordinate = lattice->indexToCoordinate(vertexIndex);
        if (sweepEdges.size() == 1 && (latticeType == "rhombic toric" || coordinate.w == 0))
        {
            continue;
        }
        if (coordinate.w == 0)
        {
            if ((coordinate.x + coordinate.y + coordinate.z) % 2 == latticeParity)
            {
                if (latticeType == "rhombic boundaries")
                {
                    if (coordinate.z == 1)
                    {
                        if (direction == "-xyz" || direction == "-xz" || direction == "-yz" || direction == "xy")
                        {
                            continue;
                        }
                    }
                    if (coordinate.z == l - 1)
                    {
                        if (direction == "xyz" || direction == "xz" || direction == "yz" || direction == "-xy")
                        {
                            continue;
                        }
                    }
                    if (coordinate.x == 0)
                    {
                        if (direction == "-xyz" || direction == "-xz" || direction == "yz" || direction == "-xy")
                        {
                            continue;
                        }
                    }
                    if (coordinate.x == l - 1)
                    {
                        if (direction == "xyz" || direction == "xz" || direction == "-yz" || direction == "xy")
                        {
                            continue;
                        }
                    }
                }
                sweepFullVertex(vertexIndex, sweepEdges, direction, edgeDirections);
            }
            else
            {
                throw std::invalid_argument("Vertex not present in lattice has up-edges.");
            }
        }
        else
        {
            if (latticeType == "rhombic boundaries")
            {
                // if (coordinate.y == 0)
                // {
                //     if (direction == "-xyz" || direction == "-yz" || direction == "xz" || direction == "-xy")
                //     {
                //         continue;
                //     }
                // }
                // if (coordinate.y == l - 2)
                // {
                //     if (direction == "xyz" || direction == "yz" || direction == "-xz" || direction == "xy")
                //     {
                //         continue;
                //     }
                // }
                sweepHalfVertexBoundary(vertexIndex, sweepEdges, direction, edgeDirections);
            }
            else if (latticeType == "rhombic toric")  
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

void Code::localFlip(vint &vertices)
{
    // std::cout << "Attempting local flip ... ";
    int faceIndex = lattice->findFace(vertices);
    flipBits[faceIndex] = (flipBits[faceIndex] + 1) % 2;
    // std::cout << "flipped." << std::endl;
}

vstr Code::findSweepEdges(const int vertexIndex, const std::string &direction)
{
    vstr sweepEdges;
    vint upEdges = upEdgesMap[direction][vertexIndex];
    for (const int edge : upEdges)
    {
        if (syndrome[edge] == 1)
        {
            int xyzEdge = -1, xyEdge = -1, xzEdge = -1, yzEdge = -1;
            int mxyzEdge = -1, mxyEdge = -1, mxzEdge = -1, myzEdge = -1;
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
                mxyzEdge = lattice->edgeIndex(vertexIndex, "xyz", -1);
            }
            catch (const std::invalid_argument &e)
            {
            }
            try 
            {
                mxyEdge = lattice->edgeIndex(vertexIndex, "xy", -1);
            }
            catch (const std::invalid_argument &e)
            {
            }
            try 
            {
                mxzEdge = lattice->edgeIndex(vertexIndex, "xz", -1);
            }
            catch (const std::invalid_argument &e)
            {
            }
            try 
            {
                myzEdge = lattice->edgeIndex(vertexIndex, "yz", -1);
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
            else if (myzEdge == edge)
            {
                sweepEdges.push_back("-yz");
            }
            else if (mxzEdge == edge)
            {
                sweepEdges.push_back("-xz");
            }
             else if (mxyEdge == edge)
            {
                sweepEdges.push_back("-xy");
            }
            else if (mxyzEdge == edge)
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

vint &Code::getFlipBits()
{
    return flipBits;
}

void Code::sweepFullVertex(const int vertexIndex, vstr &sweepEdges, const std::string &sweepDirection, const vstr &upEdgeDirections)
{
    // std::cout << "Sweep of coordinate = " << lattice->indexToCoordinate(vertexIndex) << " ... ";
    std::string edge0 = upEdgeDirections[0];
    std::string edge1 = upEdgeDirections[1];
    std::string edge2 = upEdgeDirections[2];
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
            std::cout << "WARNING: " << e.what() << std::endl;
        }
        try
        {
            vint vertices = faceVertices(vertexIndex, {sweepDirection, edge1, edge1});
            localFlip(vertices);
        }
        catch (const std::invalid_argument &e)
        {
            std::cout << "WARNING: " << e.what() << std::endl;
        }
        try
        {
            vint vertices = faceVertices(vertexIndex, {sweepDirection, edge2, edge2});
            localFlip(vertices);
        }
        catch (const std::invalid_argument &e)
        {
            std::cout << "WARNING: " << e.what() << std::endl;
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
                std::cout << "WARNING: " << e.what() << std::endl;
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
                std::cout << "WARNING: " << e.what() << std::endl;
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
                std::cout << "WARNING: " << e.what() << std::endl;
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
                std::cout << "WARNING: " << e.what() << std::endl;
            }
            try
            {
                vint vertices = faceVertices(vertexIndex, {sweepDirection, edge2, edge2});
                localFlip(vertices);
            }
            catch (const std::invalid_argument &e)
            {
                std::cout << "WARNING: " << e.what() << std::endl;
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
                std::cout << "WARNING: " << e.what() << std::endl;
            }
            try
            {
                vint vertices = faceVertices(vertexIndex, {sweepDirection, edge1, edge1});
                localFlip(vertices);
            }
            catch (const std::invalid_argument &e)
            {
                std::cout << "WARNING: " << e.what() << std::endl;
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
                std::cout << "WARNING: " << e.what() << std::endl;
            }
            try
            {
                vint vertices = faceVertices(vertexIndex, {sweepDirection, edge2, edge2});
                localFlip(vertices);
            }
            catch (const std::invalid_argument &e)
            {
                std::cout << "WARNING: " << e.what() << std::endl;
            }
        }
        else
        {
            throw std::invalid_argument("Invalid up-edges.");
        }
    }
    // std::cout << "Successful." << std::endl;
}

void Code::sweepHalfVertex(const int vertexIndex, vstr &sweepEdges, const std::string &sweepDirection, const vstr &upEdgeDirections)
{
    // std::cout << "Sweep of coordinate = " << lattice->indexToCoordinate(vertexIndex) << " ... ";
    std::string edge0 = upEdgeDirections[0];
    std::string edge1 = upEdgeDirections[1];
    std::string edge2 = upEdgeDirections[2];
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
            std::cout << "WARNING: " << e.what() << std::endl;
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
            std::cout << "WARNING: " << e.what() << std::endl;
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
            std::cout << "WARNING: " << e.what() << std::endl;
        }
        
    }
    else
    {
        throw std::invalid_argument("Invalid up-edges.");
    }
    // std::cout << "Successful." << std::endl;
}

void Code::sweepHalfVertexBoundary(const int vertexIndex, vstr &sweepEdges, const std::string &sweepDirection, const vstr &upEdgeDirections)
{
    // std::cout << "Sweep of coordinate = " << lattice->indexToCoordinate(vertexIndex) << " ... ";
    // std::cout << "Up Edges = ";
    // for (auto e : upEdgeDirections)
    // {
    //     std::cout << e << ",";
    // }
    // std::cout << std::endl;
    // std::cout << "Sweep Edges = ";
    // for (auto e : sweepEdges)
    // {
    //     std::cout << e << ",";
    // }
    // std::cout << std::endl;
    cartesian4 coordinate = lattice->indexToCoordinate(vertexIndex);
    if (sweepEdges.size() == 1)
    {
        double flipP = 1;
        vint vertices;
        if (coordinate.y == 0)
        {
            if (coordinate.z == 1)
            {
                if (sweepEdges[0] == "-xz")
                {
                    // if (sweepDirection == "xy" || sweepDirection == "yz")
                    if (sweepDirection == "xy")
                    {
                        vertices = faceVertices(vertexIndex, {"-xz", "-yz", "-yz"});
                        localFlip(vertices);
                    }
                    else if (sweepDirection == "-xyz")
                    {
                        if (distDouble0To1(pcg) <= flipP)
                        {
                            vertices = faceVertices(vertexIndex, {"-xz", "-yz", "-yz"});
                            localFlip(vertices);
                        }
                    }
                }
                else if (sweepEdges[0] == "xy")
                {
                    // if (sweepDirection == "-xz" || sweepDirection == "xyz")
                    if (sweepDirection == "-xz")
                    {
                        vertices = faceVertices(vertexIndex, {"xy", "-xyz", "-xyz"});
                        localFlip(vertices);
                    }
                    else if (sweepDirection == "-yz")
                    {
                        if (distDouble0To1(pcg) <= flipP)
                        {
                            vertices = faceVertices(vertexIndex, {"xy", "-xyz", "-xyz"});
                            localFlip(vertices);    
                        }
                    }
                }
            }
            else if (coordinate.z == l - 2)
            {
                if (sweepEdges[0] == "xyz")
                {
                    // if (sweepDirection == "xy" || sweepDirection == "yz")
                    if (sweepDirection == "yz")
                    {
                        vertices = faceVertices(vertexIndex, {"xyz", "-xy", "-xy"});
                        localFlip(vertices);
                    }
                    else if (sweepDirection == "xz")
                    {
                        if (distDouble0To1(pcg) <= flipP)
                        { 
                            vertices = faceVertices(vertexIndex, {"xyz", "-xy", "-xy"});
                            localFlip(vertices);
                        }
                    }
                }
                else if (sweepEdges[0] == "yz")
                {
                    // if (sweepDirection == "-xz" || sweepDirection == "xyz")
                    if (sweepDirection == "xyz")
                    {
                        vertices = faceVertices(vertexIndex, {"yz", "xz", "xz"});
                        localFlip(vertices);
                    }
                    else if (sweepDirection == "-xy")
                    {
                        if (distDouble0To1(pcg) <= flipP)
                        {
                            vertices = faceVertices(vertexIndex, {"yz", "xz", "xz"});
                            localFlip(vertices); 
                        }
                    }
                }
            }
        }
        else if (coordinate.y == l - 2)
        {
            if (coordinate.z == 1)
            {
                if (sweepEdges[0] == "-yz")
                {
                    // if (sweepDirection == "-xyz" || sweepDirection == "xz")
                    if (sweepDirection == "-xyz")
                    {
                        vertices = faceVertices(vertexIndex, {"-xz", "-yz", "-yz"});
                        localFlip(vertices);
                    }
                    else if (sweepDirection == "-xy")
                    {
                        if (distDouble0To1(pcg) <= flipP)
                        {
                            vertices = faceVertices(vertexIndex, {"-xz", "-yz", "-yz"});
                            localFlip(vertices);
                        }
                    }
                }
                else if (sweepEdges[0] == "-xyz")
                {
                    // if (sweepDirection == "-yz" || sweepDirection == "-xy")
                    if (sweepDirection == "-yz")
                    {
                        vertices = faceVertices(vertexIndex, {"xy", "-xyz", "-xyz"});
                        localFlip(vertices);
                    }
                    else if (sweepDirection == "-xz")
                    {
                        if (distDouble0To1(pcg) <= flipP)
                        {
                            vertices = faceVertices(vertexIndex, {"xy", "-xyz", "-xyz"});
                            localFlip(vertices);
                        }
                    }
                }
            }
            else if (coordinate.z == l - 2)
            {
                if (sweepEdges[0] == "-xy")
                {
                    // if (sweepDirection == "xz" || sweepDirection == "-xyz")
                    if (sweepDirection == "xz")
                    {
                        vertices = faceVertices(vertexIndex, {"xyz", "-xy", "-xy"});
                        localFlip(vertices);
                    }
                    else if (sweepDirection == "yz")
                    {
                        if (distDouble0To1(pcg) <= flipP)
                        {
                            vertices = faceVertices(vertexIndex, {"xyz", "-xy", "-xy"});
                            localFlip(vertices);
                        }
                    }
                }
                else if (sweepEdges[0] == "xz")
                {
                    // if (sweepDirection == "-yz" || sweepDirection == "-xy")
                    if (sweepDirection == "-xy")
                    {
                        vertices = faceVertices(vertexIndex, {"yz", "xz", "xz"});
                        localFlip(vertices);
                    }
                    else if (sweepDirection == "xyz")
                    {
                        if (distDouble0To1(pcg) <= flipP)
                        {
                            vertices = faceVertices(vertexIndex, {"yz", "xz", "xz"});
                            localFlip(vertices); 
                        }
                    }
                }
            }
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
        if (coordinate.x == 0)
        {
            vint vertices;
            if ((sweepEdges[0] == "-xyz" && sweepEdges[1] == "yz") || (sweepEdges[1] == "-xyz" && sweepEdges[0] == "yz"))
            {
                if (sweepDirection == "-xy")
                {
                    vertices = faceVertices(vertexIndex, {"xz", sweepEdges[1], sweepEdges[1]});
                    localFlip(vertices);
                    vertices = faceVertices(vertexIndex, {"xz", sweepEdges[0], sweepEdges[0]});
                    localFlip(vertices);
                }
                else if (sweepDirection == "-xz")
                {
                    vertices = faceVertices(vertexIndex, {"xy", sweepEdges[1], sweepEdges[1]});
                    localFlip(vertices);
                    vertices = faceVertices(vertexIndex, {"xy", sweepEdges[0], sweepEdges[0]});
                    localFlip(vertices);
                }
            }
            else if ((sweepEdges[0] == "-xy" && sweepEdges[1] == "-xz") || (sweepEdges[1] == "-xy" && sweepEdges[0] == "-xz"))
            {
                if (sweepDirection == "yz")
                {
                    vertices = faceVertices(vertexIndex, {"xyz", sweepEdges[1], sweepEdges[1]});
                    localFlip(vertices);
                    vertices = faceVertices(vertexIndex, {"xyz", sweepEdges[0], sweepEdges[0]});
                    localFlip(vertices);
                }
                else if (sweepDirection == "-xyz")
                {
                    vertices = faceVertices(vertexIndex, {"-yz", sweepEdges[1], sweepEdges[1]});
                    localFlip(vertices);
                    vertices = faceVertices(vertexIndex, {"-yz", sweepEdges[0], sweepEdges[0]});
                    localFlip(vertices);
                }
            }
            else 
            {
                sweepHalfVertex(vertexIndex, sweepEdges, sweepDirection, upEdgeDirections);
            }
        }
        else if (coordinate.x == l - 2)
        {
            vint vertices;
            if ((sweepEdges[0] == "xyz" && sweepEdges[1] == "-yz") || (sweepEdges[1] == "xyz" && sweepEdges[0] == "-yz"))
            {
                if (sweepDirection == "xy")
                {
                    vertices = faceVertices(vertexIndex, {"-xz", sweepEdges[1], sweepEdges[1]});
                    localFlip(vertices);
                    vertices = faceVertices(vertexIndex, {"-xz", sweepEdges[0], sweepEdges[0]});
                    localFlip(vertices);
                }
                else if (sweepDirection == "xz")
                {
                    vertices = faceVertices(vertexIndex, {"-xy", sweepEdges[1], sweepEdges[1]});
                    localFlip(vertices);
                    vertices = faceVertices(vertexIndex, {"-xy", sweepEdges[0], sweepEdges[0]});
                    localFlip(vertices);
                }
            }
            else if ((sweepEdges[0] == "xy" && sweepEdges[1] == "xz") || (sweepEdges[1] == "xy" && sweepEdges[0] == "xz"))
            {
                if (sweepDirection == "-yz")
                {
                    vertices = faceVertices(vertexIndex, {"-xyz", sweepEdges[1], sweepEdges[1]});
                    localFlip(vertices);
                    vertices = faceVertices(vertexIndex, {"-xyz", sweepEdges[0], sweepEdges[0]});
                    localFlip(vertices);
                }
                else if (sweepDirection == "xyz")
                {
                    vertices = faceVertices(vertexIndex, {"yz", sweepEdges[1], sweepEdges[1]});
                    localFlip(vertices);
                    vertices = faceVertices(vertexIndex, {"yz", sweepEdges[0], sweepEdges[0]});
                    localFlip(vertices);
                }
            }
            else 
            {
                sweepHalfVertex(vertexIndex, sweepEdges, sweepDirection, upEdgeDirections);
            }
        }
        else 
        {
            sweepHalfVertex(vertexIndex, sweepEdges, sweepDirection, upEdgeDirections);
        }
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

void Code::buildLogicals()
{
    if (latticeType == "rhombic toric")
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
    else if (latticeType == "rhombic boundaries")
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
}

vvint Code::getLogicals()
{
    vvint logicals;
    if (latticeType == "rhombic toric")
    {
        logicals.push_back(logicalZ1);
        logicals.push_back(logicalZ2);
        logicals.push_back(logicalZ3);
    }
    else
    {
        logicals.push_back(logicalZ1);
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
    if (latticeType == "rhombic toric")
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

void Code::printUnsatisfiedStabilisers()
{
    for (int i = 0; i < syndrome.size(); ++i)
    {
        if (syndrome[i] == 1)
        {
            std::cout << i << std::endl;
        }
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

// void Code::initBoundarySyndrome()
// {
    // for (int i = 0; i < l; ++i)
    // {
    //     if (i == 0)
    //     {
    //         int index = lattice->coordinateToIndex({0, l - 2, 1, 0});
    //         boundarySyndromeIndices.push_back(lattice->edgeIndex(index, "xy", 1));
    //         index = lattice->coordinateToIndex({0, l - 2, l - 1, 0});
    //         boundarySyndromeIndices.push_back(lattice->edgeIndex(index, "xyz", 1));
    //     }
    //     else if (i % 2 == 0)
    //     {
    //         int index = lattice->coordinateToIndex({i, l - 2, 1, 0});
    //         boundarySyndromeIndices.push_back(lattice->edgeIndex(index, "xy", 1));
    //         boundarySyndromeIndices.push_back(lattice->edgeIndex(index, "xz", -1));
    //         index = lattice->coordinateToIndex({i, l - 2, l - 1, 0});
    //         boundarySyndromeIndices.push_back(lattice->edgeIndex(index, "xyz", 1));
    //         boundarySyndromeIndices.push_back(lattice->edgeIndex(index, "yz", 1));
    //     }
    //     else if (i == l - 1)
    //     {
    //         int index = lattice->coordinateToIndex({l - 1, 1, 1, 0});
    //         boundarySyndromeIndices.push_back(lattice->edgeIndex(index, "xyz", -1));
    //         index = lattice->coordinateToIndex({l - 1, 1, l - 1, 0});
    //         boundarySyndromeIndices.push_back(lattice->edgeIndex(index, "xy", -1));
    //     }
    //     else if (i % 2 == 1)
    //     {
    //         int index = lattice->coordinateToIndex({i, 1, 1, 0});
    //         boundarySyndromeIndices.push_back(lattice->edgeIndex(index, "xyz", -1));
    //         boundarySyndromeIndices.push_back(lattice->edgeIndex(index, "yz", -1));
    //         index = lattice->coordinateToIndex({i, 1, l - 1, 0});
    //         boundarySyndromeIndices.push_back(lattice->edgeIndex(index, "xy", -1));
    //         boundarySyndromeIndices.push_back(lattice->edgeIndex(index, "xz", 1));
    //     }
    // }
    // for (int i = 0; i < l; ++i)
    // {
    //     if (i == 0)
    //     {
    //         int index = lattice->coordinateToIndex({0, 0, 1, 0});
    //         boundarySyndromeIndices.push_back(lattice->edgeIndex(index, "xyz", 1));
    //         index = lattice->coordinateToIndex({0, 0, l - 1, 0});
    //         boundarySyndromeIndices.push_back(lattice->edgeIndex(index, "xy", 1));
    //     }
    //     else if (i % 2 == 0)
    //     {
    //         int index = lattice->coordinateToIndex({i, 0, 1, 0});
    //         boundarySyndromeIndices.push_back(lattice->edgeIndex(index, "xyz", 1));
    //         boundarySyndromeIndices.push_back(lattice->edgeIndex(index, "yz", 1));
    //         index = lattice->coordinateToIndex({i, 0, l - 1, 0});
    //         boundarySyndromeIndices.push_back(lattice->edgeIndex(index, "xy", 1));
    //         boundarySyndromeIndices.push_back(lattice->edgeIndex(index, "xz", -1));
    //     }
    //     else if (i == l - 1)
    //     {
    //         int index = lattice->coordinateToIndex({l - 1, l - 1, 1, 0});
    //         boundarySyndromeIndices.push_back(lattice->edgeIndex(index, "xy", -1));
    //         index = lattice->coordinateToIndex({l - 1, l - 1, l - 1, 0});
    //         boundarySyndromeIndices.push_back(lattice->edgeIndex(index, "xyz", -1));
    //     }
    //     else if (i % 2 == 1)
    //     {
    //         int index = lattice->coordinateToIndex({i, l - 1, 1, 0});
    //         boundarySyndromeIndices.push_back(lattice->edgeIndex(index, "xz", 1));
    //         boundarySyndromeIndices.push_back(lattice->edgeIndex(index, "xy", -1));
    //         index = lattice->coordinateToIndex({i, l - 1, l - 1, 0});
    //         boundarySyndromeIndices.push_back(lattice->edgeIndex(index, "xyz", -1));
    //         boundarySyndromeIndices.push_back(lattice->edgeIndex(index, "yz", -1));
    //     }
    // }
// }

// vint &Code::getBoundarySyndromeIndices()
// {
//     return boundarySyndromeIndices;
// }