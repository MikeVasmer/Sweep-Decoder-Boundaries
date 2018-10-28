#ifndef DECODER_H
#define DECODER_H

#include <string>
#include "rhombicToricLattice.h"
#include "code.h"
#include <algorithm>
#include <cmath>

std::vector<bool> runToric(const int l, const int rounds,
                           const double p, const double q,
                           const std::string &sweepDirection)
{
    std::vector<bool> success = {false, false};
    Code code = Code(l, "rhombic toric", p, p);
    vint &syndrome = code.getSyndrome();
    for (int r = 0; r < rounds; ++r)
    {
        code.generateDataError();
        code.calculateSyndrome();
        if (q > 0)
        {
            code.generateMeasError();
        }
        code.sweep(sweepDirection, true);
    }
    code.generateDataError(); // Data errors = measurement errors at readout
    code.calculateSyndrome();
    // code.printUnsatisfiedStabilisers();
    for (int r = 0; r < l * l * l; ++r)
    // for (int r = 0; r < l * l; ++r)
    // for (int r = 0; r < 12 * l; ++r)
    {
        code.sweep(sweepDirection, true);
        code.calculateSyndrome();
        if (std::all_of(syndrome.begin(), syndrome.end(), [](int i) { return i == 0; }))
        {
            // std::cout << "Clean Syndrome" << std::endl;
            success = {code.checkCorrection(), true};
            break;
        }
    }
    return success;
}

std::vector<bool> runBoundaries(const int l, const int rounds,
                                const double p, const double q)
{
    std::vector<bool> success = {false, false};
    Code code = Code(l, "rhombic boundaries", p, p);
    vint &syndrome = code.getSyndrome();
    vstr sweepDirections = {"xyz", "xz", "-xy", "yz", "xy", "-yz", "-xyz", "-xz"};
    std::random_shuffle(sweepDirections.begin(), sweepDirections.end());
    int sweepCount = 0;
    int sweepLimit = (int)ceil(log(l));
    // int sweepLimit = l;
    int sweepIndex = 0;
    for (int r = 0; r < rounds; ++r)
    {
        if (sweepCount == sweepLimit)
        {
            sweepIndex = (sweepIndex + 1) % 8;
        }
        code.generateDataError();
        code.calculateSyndrome();
        if (q > 0)
        {
            code.generateMeasError();
        }
        code.sweep(sweepDirections[sweepIndex], true);
        ++sweepCount;
    }
    code.generateDataError(); // Data errors = measurement errors at readout
    code.calculateSyndrome();
    // code.printUnsatisfiedStabilisers();
    for (int r = 0; r < l * l * l; ++r)
    // for (int r = 0; r < l * l; ++r)
    // for (int r = 0; r < 12 * l; ++r)
    {
        if (sweepCount == sweepLimit)
        {
            sweepIndex = (sweepIndex + 1) % 8;
        }
        code.sweep(sweepDirections[sweepIndex], true);
        code.calculateSyndrome();
        if (std::all_of(syndrome.begin(), syndrome.end(), [](int i) { return i == 0; }))
        {
            // std::cout << "Clean Syndrome" << std::endl;
            success = {code.checkCorrection(), true};
            break;
        }
        ++sweepCount;
    }
    return success;
}

#endif