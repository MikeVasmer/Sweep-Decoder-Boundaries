#ifndef LATTICE_H
#define LATTICE_H

#include <vector>
#include <string>

typedef std::vector<int> vint;
typedef std::vector<double> vdbl;
typedef std::vector<vint> vvint;
typedef std::vector<std::pair<int, int>> vpint;

struct cartesian4 {
    int x;
    int y;
    int z;
    // w is either 0 or 1 and fixes the sub-lattice 
    int w;
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

    public:
        Lattice(int l, std::string type);
        std::string getType();
        // Convert an index into lattice coordinates xyzw
        cartesian4 indexToCoordinate(int index);
        // Convert lattice coordinates xyzw into an index
        int coordinateToIndex(cartesian4 coordinate);
        // Find neighbour of a vertex (index) in the sign direction  
        int neighbour(int index, std::string direction, int sign);
};

#endif