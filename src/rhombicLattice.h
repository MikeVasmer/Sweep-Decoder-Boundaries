#ifndef RHOMBIC_LATTICE_H
#define RHOMBIC_LATTICE_H

#include <vector>
#include <string>
#include <map>
#include "lattice.h"

class RhombicLattice : public Lattice
{
  private:

  public:
    RhombicLattice(const int l);
    int neighbour(const int vertexIndex, const std::string &direction, const int sign);
    void createFaces();
    void createVertexToEdges();
    void createUpEdgesMap();
};

#endif