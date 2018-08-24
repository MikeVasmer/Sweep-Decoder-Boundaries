#include "lattice.h"
#include <string>
#include <iostream>
#include <cmath>

int sgn(int x) { return (x > 0) - (x < 0); }

Lattice::Lattice(int length, std::string latticeType) : l(length), type(latticeType)
{
    if (length < 3)
    {
        throw std::invalid_argument("Lattice dimension l must be a positive integer greater than two.");
    }
    if (!(latticeType == "cubic" || latticeType == "rhombic" || latticeType == "bcc"))
    {
        throw std::invalid_argument("Lattice type must be either 'cubic', 'bcc' or 'rhombic'.");
    }
}

std::string Lattice::getType() { return type; }

cartesian4 Lattice::indexToCoordinate(int index)
{
    if (index < 0)
    {
        throw std::invalid_argument("Index must not be negative.");
    }
    cartesian4 coordinate;
    coordinate.x = index % l;
    coordinate.y = int(floor(index / l)) % l;
    coordinate.z = int(floor(index / (l * l))) % l;
    // w is either 0 or 1 and fixes the sub-lattice
    coordinate.w = int(floor(index / (l * l * l)));
    return coordinate;
}

int Lattice::coordinateToIndex(cartesian4 coordinate)
{
    if (coordinate.x < 0 || coordinate.y < 0 || coordinate.z < 0 || coordinate.w < 0 || coordinate.w > 1)
    {
        throw std::invalid_argument("Lattice coordinates must be positive and w coordinate must be either zero or one.");
    }
    return coordinate.w * l * l * l + coordinate.z * l * l + coordinate.y * l + coordinate.x;
}

int Lattice::neighbour(int index, std::string direction, int sign)
{
    if (type == "cubic") {
        if (!(direction == "x" || direction == "y" || direction == "z")){
            throw std::invalid_argument("Cubic lattice vertices only have neighbours in x, y, and z directions.");
        }
    } 
    else if (type == "rhombic") {
        if (direction == "x" || direction == "y" || direction == "z") {
            throw std::invalid_argument("Rhombic lattice vertices only have neighbours in xy, xz, yz, xyz directions.");
        }
    }
    cartesian4 coordinate;

    coordinate = indexToCoordinate(index);
    if (direction == "x")
        coordinate.x = (coordinate.x + sign + l) % l;
    if (direction == "y")
        coordinate.y = (coordinate.y + sign + l) % l;
    if (direction == "z")
        coordinate.z = (coordinate.z + sign + l) % l;
    if (coordinate.w == 1)
    {
        if (direction == "xy")
        {
            coordinate.x = (coordinate.x + (sgn(sign) > 0)) % l;
            coordinate.y = (coordinate.y + (sgn(sign) > 0)) % l;
            coordinate.z = (coordinate.z + (sgn(sign) < 0)) % l;
            coordinate.w = 0;
        }
        if (direction == "xz")
        {
            coordinate.x = (coordinate.x + (sgn(sign) > 0)) % l;
            coordinate.z = (coordinate.z + (sgn(sign) > 0)) % l;
            coordinate.y = (coordinate.y + (sgn(sign) < 0)) % l;
            coordinate.w = 0;
        }
        if (direction == "yz")
        {
            coordinate.y = (coordinate.y + (sgn(sign) > 0)) % l;
            coordinate.z = (coordinate.z + (sgn(sign) > 0)) % l;
            coordinate.x = (coordinate.x + (sgn(sign) < 0)) % l;
            coordinate.w = 0;
        }
        if (direction == "xyz")
        {
            coordinate.x = (coordinate.x + (sgn(sign) > 0)) % l;
            coordinate.y = (coordinate.y + (sgn(sign) > 0)) % l;
            coordinate.z = (coordinate.z + (sgn(sign) > 0)) % l;
            coordinate.w = 0;
        }
    }
    else
    {
        if (direction == "xy")
        {
            coordinate.x = (coordinate.x - (sgn(sign) < 0) + l) % l;
            coordinate.y = (coordinate.y - (sgn(sign) < 0) + l) % l;
            coordinate.z = (coordinate.z - (sgn(sign) > 0) + l) % l;
            coordinate.w = 1;
        }
        if (direction == "xz")
        {
            coordinate.x = (coordinate.x - (sgn(sign) < 0) + l) % l;
            coordinate.z = (coordinate.z - (sgn(sign) < 0) + l) % l;
            coordinate.y = (coordinate.y - (sgn(sign) > 0) + l) % l;
            coordinate.w = 1;
        }
        if (direction == "yz")
        {
            coordinate.y = (coordinate.y - (sgn(sign) < 0) + l) % l;
            coordinate.z = (coordinate.z - (sgn(sign) < 0) + l) % l;
            coordinate.x = (coordinate.x - (sgn(sign) > 0) + l) % l;
            coordinate.w = 1;
        }
        if (direction == "xyz")
        {
            coordinate.x = (coordinate.x - (sgn(sign) < 0) + l) % l;
            coordinate.y = (coordinate.y - (sgn(sign) < 0) + l) % l;
            coordinate.z = (coordinate.z - (sgn(sign) < 0) + l) % l;
            coordinate.w = 1;
        }
    }
    return coordinateToIndex(coordinate);
}