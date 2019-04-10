#ifndef RHOMBIC_CODE_H
#define RHOMBIC_CODE_H

#include "lattice.h"
#include "code.h"
#include <string>
#include <set>
#include <memory>

class RhombicCode : public Code
{
  private:
    int latticeParity;
    
  public:
    RhombicCode(const int latticeLength, const double dataErrorProbability, const double measErrorProbability, bool boundaries);

    void buildSyndromeIndices();
    void buildSweepIndices();
    void sweep(const std::string &direction, bool greedy);
    vstr findSweepEdges(const int vertexIndex, const std::string &direction);
    void buildLogicals();

    void sweepFullVertex(const int vertexIndex, vstr &sweepEdges, const std::string &sweepDirection, const vstr &edges);
    void sweepHalfVertex(const int vertexIndex, vstr &sweepEdges, const std::string &sweepDirection, const vstr &edges);
    void sweepHalfVertexBoundary(const int vertexIndex, vstr &sweepEdges, const std::string &sweepDirection, const vstr &edges);
};

#endif