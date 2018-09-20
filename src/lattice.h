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

struct cartesian4 {
    int x;
    int y;
    int z;
    // w is either 0 or 1 and fixes the sub-lattice 
    int w;
};

struct faceS {
    vint vertices;
    int faceIndex;
};

inline bool operator==(const cartesian4& lhs, const cartesian4& rhs)
{
    return lhs.x == rhs.x &&
           lhs.y == rhs.y &&
           lhs.z == rhs.z &&
           lhs.w == rhs.w;
}

// Sign of a number, +1, 0 or -1
int sgn(int x);

class Lattice {
    private:
        const std::string type;
        const int l;
        vvint faceToVertices;
        vvint faceToEdges;
        std::vector<std::vector<faceS>> vertexToFaces;
        std::map<std::string, vvint> upEdgesMap;
        void addFace(const int vertexIndex, const int faceIndex, const vstr &directions, const vint &signs);
        vvint vertexToEdges;

    public:
        Lattice(const int l, const std::string &type);
        cartesian4 indexToCoordinate(const int vertexIndex);
        int coordinateToIndex(const cartesian4 &coordinate);
        // Find neighbour of a vertex (index) in the sign direction  
        int neighbour(const int vertexIndex, const std::string &direction, const int sign);
        void createFaces();
        void createUpEdgesMap();
        void createVertexToEdges();
        int findFace(vint &vertices);
        // Find the edge pointing in the sign direction which 
        // contains a vertex (index) 
        int edgeIndex(const int vertexIndex, const std::string &direction, const int sign);
        // Getter methods
        std::map<std::string, vvint>& getUpEdgesMap();
        const vvint& getFaceToVertices() const;
        const vvint& getFaceToEdges() const;
        const std::vector<std::vector<faceS>>& getVertexToFaces() const;
        const vvint& getVertexToEdges() const;
        const std::string& getType() const;
};

#endif