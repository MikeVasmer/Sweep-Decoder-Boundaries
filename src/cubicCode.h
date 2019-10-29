#ifndef CUBIC_CODE_H
#define CUBIC_CODE_H

#include "code.h"

class CubicCode : public Code
{
  public:
    CubicCode(const int latticeLength, const double dataErrorProbability, const double measErrorProbability, bool boundaries, const int sweepRate);

    void buildSyndromeIndices();
    void buildSweepIndices();
    void sweep(const std::string &direction, bool greedy, bool virtualSweep);
    vstr findSweepEdges(const int vertexIndex, const std::string &direction);
    void buildLogicals();

    void cellularAutomatonStep(const int vertexIndex, vstr &sweepEdges, const std::string &sweepDirection, const vstr &upEdgeDirections);

};

#endif