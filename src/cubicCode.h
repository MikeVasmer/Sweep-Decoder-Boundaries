#ifndef CUBIC_CODE_H
#define CUBIC_CODE_H

#include "code.h"

class CubicCode : public Code
{
  public:
    CubicCode(const int latticeLength, const double dataErrorProbability, const double measErrorProbability, bool boundaries);

    void buildSyndromeIndices();
    void buildSweepIndices();
    void sweep(const std::string &direction, bool greedy);
    vstr findSweepEdges(const int vertexIndex, const std::string &direction);
    void buildLogicals();

};

#endif