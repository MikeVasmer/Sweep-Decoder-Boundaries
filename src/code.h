#ifndef CODE_H
#define CODE_H

#include "lattice.h"
#include <string>
// #include "gtest/gtest_prod.h"

class Code
{
  private:
    const int l;
    vint error;
    vint syndrome;
    vint flipBit;
    Lattice lattice;
    const double p; // data error probability
    const double q; // measurement error probability
    void sweepFullVertex(const int vertexIndex, vstr &upEdgesInSyndrome, const std::string &sweepDirection, const vstr &edges, const vvint &signs);
    void sweepHalfVertex(const int vertexIndex, vstr &upEdgesInSyndrome, const std::string &sweepDirection, const vstr &edges, const vvint &signs);

  public:
    Code(const int latticeLength, const std::string &latticeType, const double dataErrorProbability, const double measErrorProbability);
    void generateDataError();
    void calculateSyndrome();
    void generateMeasError();
    bool checkExtremalVertex(const int vertexIndex, const std::string &direction);
    void localFlip(vint &vertices);
    void sweep(const std::string &direction, bool greedy);
    vstr findUpEdgesInSynd(const int vertexIndex, const std::string &direction);
    vint faceVertices(const int vertexIndex, const vstr &directions, const vint& signs);
    void setError(const vint &error); // for testing
    // Getter methods
    vint& getFlipBit();
    const vint& getSyndrome() const;
    Lattice& getLattice();
    const vint& getError() const;
};

#endif