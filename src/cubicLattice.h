#ifndef CUBIC_LATTICE_H
#define CUBIC_LATTICE_H

#include <vector>
#include <string>
#include <map>
#include "lattice.h"

class CubicLattice : public Lattice
{
    private:

    public:
        CubicLattice(const int l);
        int neighbour(const int vertexIndex, const std::string &direction, const int sign);
        void createFaces();
        void createVertexToEdges();
        void createUpEdgesMap();
};

#endif