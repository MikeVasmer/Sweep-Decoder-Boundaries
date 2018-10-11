#ifndef LATTICE_H
#define LATTICE_H

#include <vector>
#include <string>
#include <map>

typedef std::vector<int> vint;
typedef std::vector<double> vdbl;
typedef std::vector<vint> vvint;
typedef std::vector<std::pair<int, int>> vpint;
typedef std::vector<std::string> vstr;

struct cartesian4
{
    int x;
    int y;
    int z;
    // w is either 0 or 1 and fixes the sub-lattice
    int w;
};

struct faceS
{
    vint vertices;
    int faceIndex;
};

inline bool operator==(const cartesian4 &lhs, const cartesian4 &rhs)
{
    return lhs.x == rhs.x &&
           lhs.y == rhs.y &&
           lhs.z == rhs.z &&
           lhs.w == rhs.w;
}

// Sign of a number, +1, 0 or -1
int sgn(int x);

class Lattice
{
  private:
    virtual void addFace(const int vertexIndex, const int faceIndex, const vstr &directions, const vint &signs) = 0;

  protected:
    const int l;
    vvint faceToVertices;
    vvint faceToEdges;
    std::vector<std::vector<faceS>> vertexToFaces;
    std::map<std::string, vvint> upEdgesMap;
    vvint vertexToEdges;

  public:
    Lattice(const int l);
    cartesian4 indexToCoordinate(const int vertexIndex);
    int coordinateToIndex(const cartesian4 &coordinate);

    // Pure virtual methods
    // Find neighbour of a vertex (index) in the sign direction
    virtual int neighbour(const int vertexIndex, const std::string &direction, const int sign) = 0;
    virtual void createFaces() = 0;
    virtual int findFace(vint &vertices) = 0;
    // Find the edge pointing in the sign direction which
    // contains a vertex (index)
    virtual int edgeIndex(const int vertexIndex, const std::string &direction, const int sign) = 0;
    virtual void createUpEdgesMap() = 0;
    virtual void createVertexToEdges() = 0;

    // Getter methods
    std::map<std::string, vvint> &getUpEdgesMap();
    const vvint &getFaceToVertices() const;
    const vvint &getFaceToEdges() const;
    const std::vector<std::vector<faceS>> &getVertexToFaces() const;
    const vvint &getVertexToEdges() const;
};

#endif