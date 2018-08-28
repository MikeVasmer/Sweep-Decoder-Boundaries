#include <iostream>
#include "lattice.h"

int main() 
{
    std::cout << "Hello, World!" << std::endl;
    // cout << 4 / 2 << endl;
    int l = 6;
    std::string type = "rhombic";
    Lattice latticeRhombic = Lattice(l, type);
    latticeRhombic.createFaces();
    return 0;
}