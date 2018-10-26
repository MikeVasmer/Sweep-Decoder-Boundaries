#ifndef RHOMBIC_TORIC_LATTICE_H
#define RHOMBIC_TORIC_LATTICE_H

#include <vector>
#include <string>
#include <map>
#include "lattice.h"

class RhombicToricLattice : public Lattice
{
  private:

  public:
    RhombicToricLattice(const int l);
    RhombicToricLattice();
    int neighbour(const int vertexIndex, const std::string &direction, const int sign);
    void createFaces();
    void createVertexToEdges();
    void createUpEdgesMap();
};

#endif