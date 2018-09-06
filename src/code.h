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
    vint flipBits;
    Lattice lattice;
    const double p; // data error probability
    const double q; // measurement error probability

  public:
    Code(const int latticeLength, const std::string &latticeType, const double dataErrorProbability, const double measErrorProbability);
    void generateDataError();
    void calculateSyndrome();
    void generateMeasError();
    bool checkExtremalVertex(const int vertexIndex, const std::string &direction);
    void localFlip(vint &vertices);
    void sweepFullVertex(const int vertexIndex, vstr &sweepEdges, const std::string &sweepDirection, const vstr &edges);
    void sweepHalfVertex(const int vertexIndex, vstr &sweepEdges, const std::string &sweepDirection, const vstr &edges);
    void sweep(const std::string &direction, bool greedy);
    vstr findSweepEdges(const int vertexIndex, const std::string &direction);
    vint faceVertices(const int vertexIndex, vstr directions);
    void setError(const vint &error); // for testing
    void clearSyndrome();
    void clearFlipBits();
    void setSyndrome(vint syndrome);
    // Getter methods
    vint& getFlipBits();
    const vint& getSyndrome() const;
    Lattice& getLattice();
    const vint& getError() const;
};

#endif