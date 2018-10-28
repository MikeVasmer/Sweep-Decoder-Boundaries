#ifndef CODE_H
#define CODE_H

#include "lattice.h"
#include <string>
#include <set>
#include <memory>
// #include "gtest/gtest_prod.h"

class Code
{
  private:
    const int l;
    std::set<int> error;
    vint syndrome;
    vint flipBits;
    std::set<int> syndromeIndices;
    std::unique_ptr<Lattice> lattice;
    std::vector<int> sweepIndices;
    // RhombicToricLattice lattice;
    int numberOfFaces;
    int numberOfEdges;
    int latticeParity;
    const double p; // data error probability
    const double q; // measurement error probability
    vint logicalZ1;
    vint logicalZ2;
    vint logicalZ3;
    std::map<std::string, vvint> upEdgesMap;
    std::string latticeType;

  public:
    Code(const int latticeLength, const std::string &latticeType, const double dataErrorProbability, const double measErrorProbability);
    void generateDataError();
    void calculateSyndrome();
    void generateMeasError();
    bool checkExtremalVertex(const int vertexIndex, const std::string &direction);
    void localFlip(vint &vertices);
    void sweepFullVertex(const int vertexIndex, vstr &sweepEdges, const std::string &sweepDirection, const vstr &edges);
    void sweepHalfVertex(const int vertexIndex, vstr &sweepEdges, const std::string &sweepDirection, const vstr &edges);
    void sweepHalfVertexBoundary(const int vertexIndex, vstr &sweepEdges, const std::string &sweepDirection, const vstr &edges);
    void sweep(const std::string &direction, bool greedy);
    vstr findSweepEdges(const int vertexIndex, const std::string &direction);
    vint faceVertices(const int vertexIndex, vstr directions);
    void buildLogicals();
    void buildSyndromeIndices();
    void buildSweepIndices();
    bool checkCorrection();
    void setError(const std::set<int> &error);
    void clearSyndrome();
    void clearFlipBits();
    void setSyndrome(vint syndrome);
    void printUnsatisfiedStabilisers();
    // Getter methods
    vint& getFlipBits();
    vint& getSyndrome();
    Lattice& getLattice();
    std::set<int>& getError();
    vvint getLogicals();
    std::set<int>& getSyndromeIndices();
    vint& getSweepIndices();
};

#endif