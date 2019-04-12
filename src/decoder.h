#ifndef DECODER_H
#define DECODER_H

#include <string>
#include "rhombicToricLattice.h"
#include "rhombicCode.h"
#include <algorithm>
#include <cmath>
#include "pcg_random.hpp"

pcg_extras::seed_seq_from<std::random_device> seed;
pcg32 pcg(seed);
std::uniform_int_distribution<int> distInt0To7(0, 7);

std::vector<bool> runToric(const int l, const int rounds,
                           const double p, const double q,
                           const std::string &sweepDirection, 
                           const int timeout)
{
    std::vector<bool> success = {false, false};
    RhombicCode code = RhombicCode(l, p, q, false);
    std::vector<int8_t> &syndrome = code.getSyndrome();
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
    for (int r = 0; r < timeout; ++r)
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
                                const double p, const double q,
                                const int sweepLimit,
                                const std::string sweepSchedule,
                                const int timeout)
{
    std::vector<bool> success = {false, false};
    RhombicCode code = RhombicCode(l, p, q, true);
    std::vector<int8_t> &syndrome = code.getSyndrome();
    vstr sweepDirections = {"xyz", "xy", "xz", "yz", "-xyz", "-xy", "-xz", "-yz"};
    bool randomSchedule = false;
    int sweepIndex = 0;
    int sweepCount = 0;
    if (sweepSchedule == "rotating_XZ")
    {
        sweepDirections = {"xyz", "xy", "-xz", "yz", "xz", "-yz", "-xyz", "-xy"};
    }
    else if (sweepSchedule == "alternating_XZ")
    {
        sweepDirections = {"xyz", "-xz", "-yz", "-xy", "-xyz", "xz", "yz", "xy"};
    }
    else if (sweepSchedule == "rotating_YZ")
    {
        sweepDirections = {"xyz", "xy", "-yz", "xz", "yz", "-xz", "-xyz", "-xy"};
    }
    else if (sweepSchedule == "alternating_YZ")
    {
        sweepDirections = {"xyz", "-yz", "-xz", "-xy", "-xyz", "yz", "xz", "xy"};
    }
    else if (sweepSchedule == "rotating_XY")
    {
        sweepDirections = {"xyz", "yz", "-xy", "xz", "xy", "-xz", "-xyz", "-yz"};
    }
    else if (sweepSchedule == "alternating_XY")
    {
        sweepDirections = {"xyz", "-xy", "-xz", "-yz", "-xyz", "xy", "xz", "yz"};
    }
    else if (sweepSchedule == "random")
    {
        randomSchedule = true;
        sweepIndex = distInt0To7(pcg);
    }
    else 
    {
        throw std::invalid_argument("Invalid sweep schedule.");
    }
    for (int r = 0; r < rounds; ++r)
    {
        if (sweepCount == sweepLimit)
        {
            if (randomSchedule)
            {
                sweepIndex = distInt0To7(pcg);
            }
            else
            {
                sweepIndex = (sweepIndex + 1) % 8;
            }
            sweepCount = 0;
        }
        code.generateDataError();
        code.calculateSyndrome();
        if (q > 0)
        {
            // std::cerr << "Generating measurement error." << std::endl;
            code.generateMeasError();
        }
        code.sweep(sweepDirections[sweepIndex], true);
        // std::cerr << "direction=" << sweepDirections[sweepIndex] << std::endl;
        // std::cerr << "sweepIndex=" << sweepIndex << std::endl;
        // std::cerr << "sweepCount=" << sweepCount << std::endl;
        ++sweepCount;
    }
    code.generateDataError(); // Data errors = measurement errors at readout
    code.calculateSyndrome();
    // code.printUnsatisfiedStabilisers();
    for (int r = 0; r < timeout; ++r)
    {
        if (sweepCount == l)
        {
            if (randomSchedule)
            {
                sweepIndex = distInt0To7(pcg);
            }
            else
            {
                sweepIndex = (sweepIndex + 1) % 8;
            }
            sweepCount = 0;
        }
        code.sweep(sweepDirections[sweepIndex], true);
        code.calculateSyndrome();
        if (std::all_of(syndrome.begin(), syndrome.end(), [](int i) { return i == 0; }))
        {
            // std::cout << "Clean Syndrome" << std::endl;
            success = {code.checkCorrection(), true};
            break;
        }
        // std::cerr << "r=" << r << std::endl;
        // std::cerr << "direction=" << sweepDirections[sweepIndex] << std::endl;
        ++sweepCount;
    }

    // Testing
    // std::cerr << "Error:" << std::endl;
    // code.printError();
    // std::cerr << "Unsatisfied stabilizers:" << std::endl;
    // code.printUnsatisfiedStabilisers();

    return success;
}

#endif