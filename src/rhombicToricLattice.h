#ifndef RHOMBIC_TORIC_LATTICE_H
#define RHOMBIC_TORIC_LATTICE_H

#include <vector>
#include <string>
#include <map>
#include "lattice.h"

class RhombicToricLattice : public Lattice
{
  private:
    void addFace(const int vertexIndex, const int faceIndex, const vstr &directions, const vint &signs);

  public:
    RhombicToricLattice(const int l);
    int neighbour(const int vertexIndex, const std::string &direction, const int sign);
    void createFaces();
    int findFace(vint &vertices);
    int edgeIndex(const int vertexIndex, const std::string &direction, const int sign);
    void createUpEdgesMap();
    void createVertexToEdges();
};

#endif