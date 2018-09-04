#ifndef CODE_H
#define CODE_H

#include "lattice.h"
#include <string>

class Code {
    private:
        const int l;
        vint error;
        vint syndrome;
        Lattice lattice;
        double p;
        double q;
    public:
        Code(int l, const std::string &latticeType, double p, double q);
        void generateDataError();
        void calculateSyndrome();
        void generateMeasError();
        void setError(const vint &err); // For testing
        vint getSyndrome();
        Lattice getLattice();
        vint getError();
        bool checkExtremalVertex(int vertexIndex, const std::string &direction);
};

#endif