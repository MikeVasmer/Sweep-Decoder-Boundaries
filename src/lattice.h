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
        void addFace(int vertexIndex, int faceIndex, const vstr &directions, const vint &signs);
        vvint vertexToEdges;

    public:
        Lattice(int l, const std::string &type);
        std::string getType();
        // Convert an index into lattice coordinates xyzw
        cartesian4 indexToCoordinate(int vertexIndex);
        // Convert lattice coordinates xyzw into an index
        int coordinateToIndex(const cartesian4 &coordinate);
        // Find neighbour of a vertex (index) in the sign direction  
        int neighbour(int vertexIndex, const std::string &direction, int sign);
        // Find the edge pointing in the sign direction which 
        // contains a vertex (index) 
        int getEdgeIndex(int vertexIndex, const std::string &direction, int sign);
        void createFaces();
        vvint getFaceToVertices();
        vvint getFaceToEdges();
        std::vector<std::vector<faceS>> getVertexToFaces();
        void createUpEdgesMap();
        std::map<std::string, vvint> getUpEdgesMap();
        int findFace(vint &vertices);
        void createVertexToEdges();
        vvint getVertexToEdges();
};

#endif