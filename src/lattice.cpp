#include "lattice.h"
#include <string>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <map>

int sgn(int x) { return (x > 0) - (x < 0); }

Lattice::Lattice(int length, std::string latticeType) : l(length), type(latticeType)
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
    }
}

std::string Lattice::getType() { return type; }

cartesian4 Lattice::indexToCoordinate(int vertexIndex)
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

int Lattice::coordinateToIndex(cartesian4 coordinate)
{
    if (coordinate.x < 0 || coordinate.y < 0 || coordinate.z < 0 || coordinate.w < 0 || coordinate.w > 1)
    {
        throw std::invalid_argument("Lattice coordinates must be positive and w coordinate must be either zero or one.");
    }
    return coordinate.w * l * l * l + coordinate.z * l * l + coordinate.y * l + coordinate.x;
}

int Lattice::neighbour(int vertexIndex, std::string direction, int sign)
{
    if (!(sign == 1 || sign == -1))
    {
        throw std::invalid_argument("Sign must be either 1 or -1.");
    }
    if (!(direction == "x" || direction == "y" || direction == "z" ||
          direction == "xy" || direction == "xz" || direction == "yz" ||
          direction == "xyz"))
    {
        throw std::invalid_argument("Direction must be one of 'x', 'y', 'z', 'xy', 'xz', 'yz' or 'xyz'.");
    }
    if (type == "cubic")
    {
        if (!(direction == "x" || direction == "y" || direction == "z"))
        {
            throw std::invalid_argument("Cubic lattice vertices only have neighbours in x, y, and z directions.");
        }
    }
    else if (type == "rhombic")
    {
        if (direction == "x" || direction == "y" || direction == "z")
        {
            throw std::invalid_argument("Rhombic lattice vertices only have neighbours in xy, xz, yz, xyz directions.");
        }
    }
    cartesian4 coordinate;

    coordinate = indexToCoordinate(vertexIndex);
    if (direction == "x")
        coordinate.x = (coordinate.x + sign + l) % l;
    if (direction == "y")
        coordinate.y = (coordinate.y + sign + l) % l;
    if (direction == "z")
        coordinate.z = (coordinate.z + sign + l) % l;
    if (coordinate.w == 1)
    {
        if (direction == "xy")
        {
            coordinate.x = (coordinate.x + (sign > 0)) % l;
            coordinate.y = (coordinate.y + (sign > 0)) % l;
            coordinate.z = (coordinate.z + (sign < 0)) % l;
            coordinate.w = 0;
        }
        if (direction == "xz")
        {
            coordinate.x = (coordinate.x + (sign > 0)) % l;
            coordinate.z = (coordinate.z + (sign > 0)) % l;
            coordinate.y = (coordinate.y + (sign < 0)) % l;
            coordinate.w = 0;
        }
        if (direction == "yz")
        {
            coordinate.y = (coordinate.y + (sign > 0)) % l;
            coordinate.z = (coordinate.z + (sign > 0)) % l;
            coordinate.x = (coordinate.x + (sign < 0)) % l;
            coordinate.w = 0;
        }
        if (direction == "xyz")
        {
            coordinate.x = (coordinate.x + (sign > 0)) % l;
            coordinate.y = (coordinate.y + (sign > 0)) % l;
            coordinate.z = (coordinate.z + (sign > 0)) % l;
            coordinate.w = 0;
        }
    }
    else
    {
        if (direction == "xy")
        {
            coordinate.x = (coordinate.x - (sign < 0) + l) % l;
            coordinate.y = (coordinate.y - (sign < 0) + l) % l;
            coordinate.z = (coordinate.z - (sign > 0) + l) % l;
            coordinate.w = 1;
        }
        if (direction == "xz")
        {
            coordinate.x = (coordinate.x - (sign < 0) + l) % l;
            coordinate.z = (coordinate.z - (sign < 0) + l) % l;
            coordinate.y = (coordinate.y - (sign > 0) + l) % l;
            coordinate.w = 1;
        }
        if (direction == "yz")
        {
            coordinate.y = (coordinate.y - (sign < 0) + l) % l;
            coordinate.z = (coordinate.z - (sign < 0) + l) % l;
            coordinate.x = (coordinate.x - (sign > 0) + l) % l;
            coordinate.w = 1;
        }
        if (direction == "xyz")
        {
            coordinate.x = (coordinate.x - (sign < 0) + l) % l;
            coordinate.y = (coordinate.y - (sign < 0) + l) % l;
            coordinate.z = (coordinate.z - (sign < 0) + l) % l;
            coordinate.w = 1;
        }
    }
    return coordinateToIndex(coordinate);
}

int Lattice::getEdgeIndex(int vertexIndex, std::string direction, int sign)
{
    if (!(sign == 1 || sign == -1))
    {
        throw std::invalid_argument("Sign must be either 1 or -1.");
    }
    if (!(direction == "x" || direction == "y" || direction == "z" ||
          direction == "xy" || direction == "xz" || direction == "yz" ||
          direction == "xyz"))
    {
        throw std::invalid_argument("Direction must be one of 'x', 'y', 'z', 'xy', 'xz', 'yz' or 'xyz'.");
    }
    if (type == "cubic")
    {
        if (!(direction == "x" || direction == "y" || direction == "z"))
        {
            throw std::invalid_argument("Cubic lattice vertices only have neighbours in x, y, and z directions.");
        }
    }
    else if (type == "rhombic")
    {
        if (direction == "x" || direction == "y" || direction == "z")
        {
            throw std::invalid_argument("Rhombic lattice vertices only have neighbours in xy, xz, yz, xyz directions.");
        }
    }
    int edgeIndex;
    if (sign < 0)
    {
        edgeIndex = neighbour(vertexIndex, direction, sign);
    }
    else
    {
        edgeIndex = vertexIndex;
    }
    // Numbering is an arbitrary convention
    if (direction == "xyz")
        edgeIndex = 7 * edgeIndex;
    else if (direction == "x")
        edgeIndex = 7 * edgeIndex + 1;
    else if (direction == "xy")
        edgeIndex = 7 * edgeIndex + 2;
    else if (direction == "y")
        edgeIndex = 7 * edgeIndex + 3;
    else if (direction == "yz")
        edgeIndex = 7 * edgeIndex + 4;
    else if (direction == "z")
        edgeIndex = 7 * edgeIndex + 5;
    else if (direction == "xz")
        edgeIndex = 7 * edgeIndex + 6;
    return edgeIndex;
}

void Lattice::addFace(int vertexIndex, int faceIndex, vstr directions, vint signs)
{
    vint vertices;
    vint edges;
    if (type == "rhombic")
    {
        int neighbourVertex = neighbour(vertexIndex, directions[0], signs[0]);
        vertices = {vertexIndex, neighbourVertex,
                    neighbour(vertexIndex, directions[1], signs[1]),
                    neighbour(neighbourVertex, directions[2], signs[2])};
        edges = {getEdgeIndex(vertexIndex, directions[0], signs[0]),
                 getEdgeIndex(vertexIndex, directions[1], signs[1]),
                 getEdgeIndex(neighbourVertex, directions[2], signs[2]),
                 getEdgeIndex(vertices[2], directions[3], signs[3])};
    }
    else if (type == "bcc")
    {
        vertices = {vertexIndex, neighbour(vertexIndex, directions[0], signs[0]),
                    neighbour(vertexIndex, directions[1], signs[1])};
        edges = {getEdgeIndex(vertexIndex, directions[0], signs[0]),
                 getEdgeIndex(vertexIndex, directions[1], signs[1]),
                 getEdgeIndex(vertices[1], directions[2], signs[2])};
    }
    faceS face;
    std::sort(vertices.begin(), vertices.end());
    std::sort(edges.begin(), edges.end());
    face.vertices = vertices;
    face.faceIndex = faceIndex;

    faceToVertices.push_back(vertices);
    faceToEdges.push_back(edges);
    for (auto const &vertex : vertices)
    {
        vertexToFaces[vertex].push_back(face);
    }
}

void Lattice::createFaces()
{
    if (type == "rhombic")
    {
        int faceIndex = 0;
        for (int vertexIndex = 0; vertexIndex < l * l * l; ++vertexIndex)
        {
            cartesian4 coordinate = indexToCoordinate(vertexIndex);
            if ((coordinate.x + coordinate.y + coordinate.z) % 2 == 0)
            {
                vint signs = {1, 1, 1, 1};
                addFace(vertexIndex, faceIndex, {"xyz", "yz", "yz", "xyz"},
                        signs);
                faceIndex++;
                addFace(vertexIndex, faceIndex, {"xyz", "xz", "xz", "xyz"},
                        signs);
                faceIndex++;
                addFace(vertexIndex, faceIndex, {"xyz", "xy", "xy", "xyz"},
                        signs);
                faceIndex++;
                signs = {1, -1, -1, 1};
                addFace(vertexIndex, faceIndex, {"xy", "xz", "xz", "xy"},
                        signs);
                faceIndex++;
                addFace(vertexIndex, faceIndex, {"xy", "yz", "yz", "xy"},
                        signs);
                faceIndex++;
                addFace(vertexIndex, faceIndex, {"xz", "yz", "yz", "xz"},
                        signs);
                faceIndex++;
            }
        }
    }
    else if (type == "bcc")
    {
        int faceIndex = 0;
        for (int vertexIndex = 0; vertexIndex < 2 * l * l * l; ++vertexIndex)
        {
            vint signs = {1, 1, 1};
            addFace(vertexIndex, faceIndex, {"x", "xyz", "yz"}, signs);
            faceIndex++;
            addFace(vertexIndex, faceIndex, {"xy", "xyz", "z"}, signs);
            faceIndex++;
            addFace(vertexIndex, faceIndex, {"y", "xyz", "xz"}, signs);
            faceIndex++;
            addFace(vertexIndex, faceIndex, {"yz", "xyz", "x"}, signs);
            faceIndex++;
            addFace(vertexIndex, faceIndex, {"z", "xyz", "xy"}, signs);
            faceIndex++;
            addFace(vertexIndex, faceIndex, {"xz", "xyz", "y"}, signs);
            faceIndex++;
            addFace(vertexIndex, faceIndex, {"xy", "x", "xz"}, signs);
            faceIndex++;
            addFace(vertexIndex, faceIndex, {"xz", "x", "xy"}, signs);
            faceIndex++;
            addFace(vertexIndex, faceIndex, {"xy", "y", "yz"}, signs);
            faceIndex++;
            addFace(vertexIndex, faceIndex, {"yz", "y", "xy"}, signs);
            faceIndex++;
            addFace(vertexIndex, faceIndex, {"xz", "z", "yz"}, signs);
            faceIndex++;
            addFace(vertexIndex, faceIndex, {"yz", "z", "xz"}, signs);
            faceIndex++;
        }
    }
}

vvint Lattice::getFaceToVertices()
{
    return faceToVertices;
}

vvint Lattice::getFaceToEdges()
{
    return faceToEdges;
}

std::vector<std::vector<faceS>> Lattice::getVertexToFaces()
{
    return vertexToFaces;
}

void Lattice::createUpEdgesMap()
{
    std::vector<std::string> directionList = {"xyz", "xy", "xz", "yz",
                                              "-xyz", "-xy", "-xz", "-yz"};
    for (auto direction : directionList)
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
                        vertexToUpEdges[vertexIndex].push_back(getEdgeIndex(vertexIndex, "xyz", 1));
                        vertexToUpEdges[vertexIndex].push_back(getEdgeIndex(vertexIndex, "xy", 1));
                        vertexToUpEdges[vertexIndex].push_back(getEdgeIndex(vertexIndex, "xz", 1));
                        vertexToUpEdges[vertexIndex].push_back(getEdgeIndex(vertexIndex, "yz", 1));
                    }
                    else if (direction == "yz")
                    {
                        vertexToUpEdges[vertexIndex].push_back(getEdgeIndex(vertexIndex, "yz", 1));
                        vertexToUpEdges[vertexIndex].push_back(getEdgeIndex(vertexIndex, "xyz", 1));
                        vertexToUpEdges[vertexIndex].push_back(getEdgeIndex(vertexIndex, "xy", -1));
                        vertexToUpEdges[vertexIndex].push_back(getEdgeIndex(vertexIndex, "xz", -1));
                    }
                    else if (direction == "xz")
                    {
                        vertexToUpEdges[vertexIndex].push_back(getEdgeIndex(vertexIndex, "xyz", 1));
                        vertexToUpEdges[vertexIndex].push_back(getEdgeIndex(vertexIndex, "xz", 1));
                        vertexToUpEdges[vertexIndex].push_back(getEdgeIndex(vertexIndex, "xy", -1));
                        vertexToUpEdges[vertexIndex].push_back(getEdgeIndex(vertexIndex, "yz", -1));
                    }
                    else if (direction == "xy")
                    {
                        vertexToUpEdges[vertexIndex].push_back(getEdgeIndex(vertexIndex, "xyz", 1));
                        vertexToUpEdges[vertexIndex].push_back(getEdgeIndex(vertexIndex, "xy", 1));
                        vertexToUpEdges[vertexIndex].push_back(getEdgeIndex(vertexIndex, "xz", -1));
                        vertexToUpEdges[vertexIndex].push_back(getEdgeIndex(vertexIndex, "yz", -1));
                    }
                    else if (direction == "-xyz")
                    {
                        vertexToUpEdges[vertexIndex].push_back(getEdgeIndex(vertexIndex, "xyz", -1));
                        vertexToUpEdges[vertexIndex].push_back(getEdgeIndex(vertexIndex, "xz", -1));
                        vertexToUpEdges[vertexIndex].push_back(getEdgeIndex(vertexIndex, "xy", -1));
                        vertexToUpEdges[vertexIndex].push_back(getEdgeIndex(vertexIndex, "yz", -1));
                    }
                    else if (direction == "-yz")
                    {
                        vertexToUpEdges[vertexIndex].push_back(getEdgeIndex(vertexIndex, "xy", 1));
                        vertexToUpEdges[vertexIndex].push_back(getEdgeIndex(vertexIndex, "xz", 1));
                        vertexToUpEdges[vertexIndex].push_back(getEdgeIndex(vertexIndex, "xyz", -1));
                        vertexToUpEdges[vertexIndex].push_back(getEdgeIndex(vertexIndex, "yz", -1));
                    }
                    else if (direction == "-xz")
                    {
                        vertexToUpEdges[vertexIndex].push_back(getEdgeIndex(vertexIndex, "xy", 1));
                        vertexToUpEdges[vertexIndex].push_back(getEdgeIndex(vertexIndex, "yz", 1));
                        vertexToUpEdges[vertexIndex].push_back(getEdgeIndex(vertexIndex, "xyz", -1));
                        vertexToUpEdges[vertexIndex].push_back(getEdgeIndex(vertexIndex, "xz", -1));
                    }
                    else if (direction == "-xy")
                    {
                        vertexToUpEdges[vertexIndex].push_back(getEdgeIndex(vertexIndex, "xz", 1));
                        vertexToUpEdges[vertexIndex].push_back(getEdgeIndex(vertexIndex, "yz", 1));
                        vertexToUpEdges[vertexIndex].push_back(getEdgeIndex(vertexIndex, "xyz", -1));
                        vertexToUpEdges[vertexIndex].push_back(getEdgeIndex(vertexIndex, "xy", -1));
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
                            vertexToUpEdges[vertexIndex].push_back(getEdgeIndex(vertexIndex, "xy", 1));
                            vertexToUpEdges[vertexIndex].push_back(getEdgeIndex(vertexIndex, "xz", 1));
                            vertexToUpEdges[vertexIndex].push_back(getEdgeIndex(vertexIndex, "yz", 1));
                        }
                        else if (direction == "-xy")
                        {
                            vertexToUpEdges[vertexIndex].push_back(getEdgeIndex(vertexIndex, "xyz", -1));
                            vertexToUpEdges[vertexIndex].push_back(getEdgeIndex(vertexIndex, "xz", 1));
                            vertexToUpEdges[vertexIndex].push_back(getEdgeIndex(vertexIndex, "yz", 1));
                        }
                        else if (direction == "-xz")
                        {
                            vertexToUpEdges[vertexIndex].push_back(getEdgeIndex(vertexIndex, "xyz", -1));
                            vertexToUpEdges[vertexIndex].push_back(getEdgeIndex(vertexIndex, "xy", 1));
                            vertexToUpEdges[vertexIndex].push_back(getEdgeIndex(vertexIndex, "yz", 1));
                        }
                        else if (direction == "-yz")
                        {
                            vertexToUpEdges[vertexIndex].push_back(getEdgeIndex(vertexIndex, "xyz", -1));
                            vertexToUpEdges[vertexIndex].push_back(getEdgeIndex(vertexIndex, "xz", 1));
                            vertexToUpEdges[vertexIndex].push_back(getEdgeIndex(vertexIndex, "xy", 1));
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
                            vertexToUpEdges[vertexIndex].push_back(getEdgeIndex(vertexIndex, "xy", -1));
                            vertexToUpEdges[vertexIndex].push_back(getEdgeIndex(vertexIndex, "xz", -1));
                            vertexToUpEdges[vertexIndex].push_back(getEdgeIndex(vertexIndex, "yz", -1));
                        }
                        else if (direction == "xy")
                        {
                            vertexToUpEdges[vertexIndex].push_back(getEdgeIndex(vertexIndex, "xyz", 1));
                            vertexToUpEdges[vertexIndex].push_back(getEdgeIndex(vertexIndex, "xz", -1));
                            vertexToUpEdges[vertexIndex].push_back(getEdgeIndex(vertexIndex, "yz", -1));
                        }
                        else if (direction == "xz")
                        {
                            vertexToUpEdges[vertexIndex].push_back(getEdgeIndex(vertexIndex, "xyz", 1));
                            vertexToUpEdges[vertexIndex].push_back(getEdgeIndex(vertexIndex, "xy", -1));
                            vertexToUpEdges[vertexIndex].push_back(getEdgeIndex(vertexIndex, "yz", -1));
                        }
                        else if (direction == "yz")
                        {
                            vertexToUpEdges[vertexIndex].push_back(getEdgeIndex(vertexIndex, "xyz", 1));
                            vertexToUpEdges[vertexIndex].push_back(getEdgeIndex(vertexIndex, "xz", -1));
                            vertexToUpEdges[vertexIndex].push_back(getEdgeIndex(vertexIndex, "xy", -1));
                        }
                    }
                }
            }
        }
        upEdgesMap.insert(std::pair<std::string, vvint>(direction, vertexToUpEdges));
    }
}

std::map<std::string, vvint> Lattice::getUpEdgesMap()
{
    return upEdgesMap;
}

int Lattice::findFace(vint vertices)
{
    if (vertices.size() != 4)
    {
        throw std::invalid_argument("Vertex list must contain exactly four vertices.");
    }
    std::sort(vertices.begin(), vertices.end());
    auto v0Faces = vertexToFaces[vertices[0]];
    // auto v1Faces = vertexToFaces[vertices[1]];
    // auto v2Faces = vertexToFaces[vertices[2]];
    // auto v3Faces = vertexToFaces[vertices[3]];
    for (auto face : v0Faces)
    {
        // if (std::find(v1Faces.begin(), v1Faces.end(), face) != v1Faces.end())
        // {
        //     return face.faceIndex;
        // }
        if (face.vertices == vertices)
        {
            return face.faceIndex;
        }
    }
    throw std::invalid_argument("Input vertices do not correspond to a face.");
}