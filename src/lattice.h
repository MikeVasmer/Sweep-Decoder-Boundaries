#ifndef LATTICE_H
#define LATTICE_H

#include <vector>
#include <string>
#include <map>
#include "baseLattice.h"

class Lattice : public BaseLattice
{
  private:
    virtual void addFace(const int vertexIndex, const int faceIndex, const vstr &directions, const vint &signs);

  public:
    Lattice(const int l, const std::string &type);
    virtual int neighbour(const int vertexIndex, const std::string &direction, const int sign);
    virtual void createFaces();
    virtual int findFace(vint &vertices);
    virtual int edgeIndex(const int vertexIndex, const std::string &direction, const int sign);
};

#endif