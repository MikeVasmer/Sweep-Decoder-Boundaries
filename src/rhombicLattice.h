#ifndef RHOMBIC_LATTICE_H
#define RHOMBIC_LATTICE_H

#include <vector>
#include <string>
#include <map>
#include "lattice.h"

class RhombicLattice : public Lattice
{
  private:
    void addFace(const int vertexIndex, const int faceIndex, const vstr &directions, const vint &signs);

  public:
    RhombicLattice(const int l);
    int neighbour(const int vertexIndex, const std::string &direction, const int sign);
    void createFaces();
    int findFace(vint &vertices);
    int edgeIndex(const int vertexIndex, const std::string &direction, const int sign);
    void createUpEdgesMap();
    void createVertexToEdges();
};

#endif