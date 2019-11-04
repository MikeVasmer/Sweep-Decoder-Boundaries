#ifndef CODE_H
#define CODE_H

#include "lattice.h"
#include <string>
#include <set>
#include <memory>
#include "pcg_random.hpp"
#include <random>
// #include "gtest/gtest_prod.h"

class Code
{
protected:
  const int l;
  int numberOfFaces;
  int numberOfEdges;
  std::vector<int8_t> syndrome;
  std::vector<int8_t> flipBits;
  std::set<int> syndromeIndices;
  std::unique_ptr<Lattice> lattice;
  std::vector<int> sweepIndices;
  std::map<std::string, vvint> upEdgesMap;
  vvint faceToEdges;
  vvint vertexToEdges;
  std::set<int> error;
  const double p; // data error probability
  const double q; // measurement error probability
  bool boundaries;
  const int sweepRate; // number of sweeps per stabilizer measurement 
  vint logicalZ1;
  vint logicalZ2;
  vint logicalZ3;
  vvint correlatedIndices;

  // pcg-random
  pcg32 rnEngine;

  // Use standard mersenne twister (Valgrind doesn't like pcg)
  // std::mt19937 rnEngine;

  std::uniform_real_distribution<double> distDouble0To1;
  std::uniform_int_distribution<int> distInt0To2;
  std::uniform_int_distribution<int> distInt0To1;

public:
  Code(const int latticeLength, const double dataErrorProbability, const double measErrorProbability, bool boundaries, const int sweepRate);

  void generateDataError(bool correlated);
  bool checkExtremalVertex(const int vertexIndex, const std::string &direction);
  void localFlip(vint &vertices);
  vint faceVertices(const int vertexIndex, vstr directions);
  void clearSyndrome();
  void clearFlipBits();
  bool checkCorrection();
  void calculateSyndrome();
  void generateMeasError();
  void buildCorrelatedIndices();

  // Test methods
  void setSyndrome(std::vector<int8_t> &syndrome);
  void setError(const std::set<int> &error);

  // Debug methods
  void printUnsatisfiedStabilisers();
  void printError();

  // Getter methods
  std::vector<int8_t> &getFlipBits();
  std::vector<int8_t> &getSyndrome();
  Lattice &getLattice();
  std::set<int> &getError();
  std::set<int> &getSyndromeIndices();
  vint &getSweepIndices();
  vvint getLogicals();
  
  // Virtual methods
  virtual void buildSyndromeIndices() = 0;
  virtual void buildSweepIndices() = 0;
  virtual void sweep(const std::string &direction, bool greedy) = 0;
  virtual vstr findSweepEdges(const int vertexIndex, const std::string &direction) = 0;
  virtual void buildLogicals() = 0;

};

#endif