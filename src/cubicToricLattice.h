#ifndef CUBIC_TORIC_LATTICE_H
#define CUBIC_TORIC_LATTICE_H

#include "lattice.h"

class CubicToricLattice : public Lattice
{
  public:
    CubicToricLattice(const int l);
    int neighbour(const int vertexIndex, const std::string &direction, const int sign);
    void createFaces();
    void createVertexToEdges();
    void createUpEdgesMap();
};

#endif