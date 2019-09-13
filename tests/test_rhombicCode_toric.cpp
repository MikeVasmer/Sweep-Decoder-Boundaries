#include "rhombicCode.h"
#include "rhombicLattice.h"
#include "gtest/gtest.h"
#include <string>
#include <iostream>

TEST(Code, excepts_invalid_probabilities)
{
    int l = 4;
    std::vector<std::pair<double, double>> errorProbabilities = {{2, 0.1}, {-2, 0.2}, {0.5, 3}, {0.8, -1}};
    for (const auto &errorPair : errorProbabilities)
    {
        EXPECT_THROW(RhombicCode code(l, errorPair.first, errorPair.second, false, 1), std::invalid_argument);
    }
}

TEST(RhombicCode, syndrome_correct_size)
{
    std::vector<int> latticeLengths = {4, 6, 8, 10};
    for (const int l : latticeLengths)
    {
        RhombicCode code(l, 0.1, 0.1, false, 1);
        auto syndrome = code.getSyndrome();
        EXPECT_EQ(syndrome.size(), 2 * 7 * l * l * l);
    }
}

TEST(calculateSyndrome, correctly_calculates_error)
{
    int latticeLength = 6;
    double p = 0.1;
    double q = p;
    RhombicCode code = RhombicCode(latticeLength, p, q, false, 1);
    std::set<int> error = {0, 1};
    code.setError(error);
    code.calculateSyndrome();
    auto syndrome = code.getSyndrome();
    vint expectedUnsatisfied = {4, 6, 1516, 1518, 1547, 1722};
    for (int i = 0; i < syndrome.size(); ++i)
    {
        if (std::find(expectedUnsatisfied.begin(), expectedUnsatisfied.end(), i) != expectedUnsatisfied.end())
        {
            EXPECT_EQ(syndrome[i], 1);
        }
        else
        {
            EXPECT_EQ(syndrome[i], 0);
        }
    }
}

TEST(calculateSyndrome, correctly_calculates_stabilizer_error)
{
    int latticeLength = 4;

    double p = 0.1;
    double q = p;
    RhombicCode code = RhombicCode(latticeLength, p, q, false, 1);
    code.setError({0, 2, 3, 19, 20, 22, 23, 29, 63, 64, 156, 157});
    code.calculateSyndrome();
    auto syndrome = code.getSyndrome();
    for (const int value : syndrome)
    {
        EXPECT_EQ(value, 0);
    }
}

TEST(generateDataError, handles_error_probability_one)
{
    int l = 4;
    double p = 1;
    RhombicCode code = RhombicCode(l, p, p, false, 1);
    code.generateDataError(false);
    EXPECT_EQ(code.getError().size(), 3 * l * l * l);
}

TEST(generateDataError, handles_error_probability_zero)
{
    int l = 6;
    double p = 0;
    RhombicCode code = RhombicCode(l, p, p, false, 1);
    code.generateDataError(false);
    EXPECT_EQ(code.getError().size(), 0);
}

TEST(generateMeasError, handles_error_probability_one)
{
    int l = 6;
    double p = 1;
    RhombicCode code = RhombicCode(l, p, p, false, 1);
    code.generateMeasError();
    auto syndrome = code.getSyndrome();
    for (const int value : syndrome)
    {
        EXPECT_EQ(value, 1);
    }
}

TEST(generateMeasError, handles_error_probability_zero)
{
    int l = 4;
    double p = 0;
    RhombicCode code = RhombicCode(l, p, p, false, 1);
    code.generateMeasError();
    auto syndrome = code.getSyndrome();
    for (const int value : syndrome)
    {
        EXPECT_EQ(value, 0);
    }
}

TEST(checkExtremalVertex, correct_extremal_vertices_one_error)
{
    int l = 4;
    double p = 0.1;
    RhombicCode code = RhombicCode(l, p, p, false, 1);
    code.setError({0});
    code.calculateSyndrome();
    EXPECT_TRUE(code.checkExtremalVertex(0, "xyz"));
    EXPECT_FALSE(code.checkExtremalVertex(20, "xyz"));
    EXPECT_FALSE(code.checkExtremalVertex(64, "xyz"));
    EXPECT_FALSE(code.checkExtremalVertex(67, "xyz"));
    EXPECT_FALSE(code.checkExtremalVertex(1, "xyz"));

    EXPECT_FALSE(code.checkExtremalVertex(0, "xy"));
    EXPECT_FALSE(code.checkExtremalVertex(20, "xy"));
    EXPECT_FALSE(code.checkExtremalVertex(64, "xy"));
    EXPECT_TRUE(code.checkExtremalVertex(67, "xy"));
    EXPECT_FALSE(code.checkExtremalVertex(1, "xy"));

    EXPECT_FALSE(code.checkExtremalVertex(0, "-yz"));
    EXPECT_TRUE(code.checkExtremalVertex(20, "-yz"));
    EXPECT_FALSE(code.checkExtremalVertex(64, "-yz"));
    EXPECT_FALSE(code.checkExtremalVertex(67, "-yz"));
    EXPECT_FALSE(code.checkExtremalVertex(1, "-yz"));

    EXPECT_FALSE(code.checkExtremalVertex(0, "-xz"));
    EXPECT_FALSE(code.checkExtremalVertex(20, "-xz"));
    EXPECT_TRUE(code.checkExtremalVertex(64, "-xz"));
    EXPECT_FALSE(code.checkExtremalVertex(67, "-xz"));
    EXPECT_FALSE(code.checkExtremalVertex(1, "-xz"));

    EXPECT_FALSE(code.checkExtremalVertex(0, "-xyz"));
    EXPECT_TRUE(code.checkExtremalVertex(20, "-xyz"));
    EXPECT_FALSE(code.checkExtremalVertex(64, "-xyz"));
    EXPECT_FALSE(code.checkExtremalVertex(67, "-xyz"));
    EXPECT_FALSE(code.checkExtremalVertex(1, "-xyz"));

    EXPECT_TRUE(code.checkExtremalVertex(0, "yz"));
    EXPECT_FALSE(code.checkExtremalVertex(20, "yz"));
    EXPECT_FALSE(code.checkExtremalVertex(64, "yz"));
    EXPECT_FALSE(code.checkExtremalVertex(67, "yz"));
    EXPECT_FALSE(code.checkExtremalVertex(1, "yz"));

    EXPECT_FALSE(code.checkExtremalVertex(0, "xz"));
    EXPECT_FALSE(code.checkExtremalVertex(20, "xz"));
    EXPECT_FALSE(code.checkExtremalVertex(64, "xz"));
    EXPECT_TRUE(code.checkExtremalVertex(67, "xz"));
    EXPECT_FALSE(code.checkExtremalVertex(1, "xz"));

    EXPECT_FALSE(code.checkExtremalVertex(0, "-xy"));
    EXPECT_FALSE(code.checkExtremalVertex(20, "-xy"));
    EXPECT_TRUE(code.checkExtremalVertex(64, "-xy"));
    EXPECT_FALSE(code.checkExtremalVertex(67, "-xy"));
    EXPECT_FALSE(code.checkExtremalVertex(1, "-xy"));
}

TEST(checkExtremalVertex, correct_extremal_vertices_two_errors)
{
    int l = 6;
    double p = 0.1;
    RhombicCode code = RhombicCode(l, p, p, false, 1);
    code.setError({0, 1});
    code.calculateSyndrome();
    EXPECT_TRUE(code.checkExtremalVertex(0, "xyz"));
    EXPECT_FALSE(code.checkExtremalVertex(37, "xyz"));
    EXPECT_FALSE(code.checkExtremalVertex(42, "xyz"));
    EXPECT_TRUE(code.checkExtremalVertex(216, "xyz"));
    EXPECT_FALSE(code.checkExtremalVertex(221, "xyz"));
    EXPECT_FALSE(code.checkExtremalVertex(246, "xyz"));
    EXPECT_FALSE(code.checkExtremalVertex(1, "xyz"));

    EXPECT_FALSE(code.checkExtremalVertex(0, "xy"));
    EXPECT_FALSE(code.checkExtremalVertex(37, "xy"));
    EXPECT_FALSE(code.checkExtremalVertex(42, "xy"));
    EXPECT_FALSE(code.checkExtremalVertex(216, "xy"));
    EXPECT_TRUE(code.checkExtremalVertex(221, "xy"));
    EXPECT_TRUE(code.checkExtremalVertex(246, "xy"));
    EXPECT_FALSE(code.checkExtremalVertex(1, "xy"));

    EXPECT_FALSE(code.checkExtremalVertex(0, "xz"));
    EXPECT_FALSE(code.checkExtremalVertex(37, "xz"));
    EXPECT_FALSE(code.checkExtremalVertex(42, "xz"));
    EXPECT_FALSE(code.checkExtremalVertex(216, "xz"));
    EXPECT_TRUE(code.checkExtremalVertex(221, "xz"));
    EXPECT_FALSE(code.checkExtremalVertex(246, "xz"));
    EXPECT_FALSE(code.checkExtremalVertex(1, "xz"));

    EXPECT_FALSE(code.checkExtremalVertex(0, "yz"));
    EXPECT_FALSE(code.checkExtremalVertex(37, "yz"));
    EXPECT_FALSE(code.checkExtremalVertex(42, "yz"));
    EXPECT_FALSE(code.checkExtremalVertex(216, "yz"));
    EXPECT_FALSE(code.checkExtremalVertex(221, "yz"));
    EXPECT_TRUE(code.checkExtremalVertex(246, "yz"));
    EXPECT_FALSE(code.checkExtremalVertex(1, "yz"));

    EXPECT_FALSE(code.checkExtremalVertex(0, "-xyz"));
    EXPECT_TRUE(code.checkExtremalVertex(37, "-xyz"));
    EXPECT_TRUE(code.checkExtremalVertex(42, "-xyz"));
    EXPECT_FALSE(code.checkExtremalVertex(216, "-xyz"));
    EXPECT_FALSE(code.checkExtremalVertex(221, "-xyz"));
    EXPECT_FALSE(code.checkExtremalVertex(246, "-xyz"));
    EXPECT_FALSE(code.checkExtremalVertex(1, "-xyz"));

    EXPECT_TRUE(code.checkExtremalVertex(0, "-xy"));
    EXPECT_FALSE(code.checkExtremalVertex(37, "-xy"));
    EXPECT_FALSE(code.checkExtremalVertex(42, "-xy"));
    EXPECT_TRUE(code.checkExtremalVertex(216, "-xy"));
    EXPECT_FALSE(code.checkExtremalVertex(221, "-xy"));
    EXPECT_FALSE(code.checkExtremalVertex(246, "-xy"));
    EXPECT_FALSE(code.checkExtremalVertex(1, "-xy"));

    EXPECT_FALSE(code.checkExtremalVertex(0, "-yz"));
    EXPECT_FALSE(code.checkExtremalVertex(37, "-yz"));
    EXPECT_TRUE(code.checkExtremalVertex(42, "-yz"));
    EXPECT_FALSE(code.checkExtremalVertex(216, "-yz"));
    EXPECT_FALSE(code.checkExtremalVertex(221, "-yz"));
    EXPECT_FALSE(code.checkExtremalVertex(246, "-yz"));
    EXPECT_FALSE(code.checkExtremalVertex(1, "-yz"));

    EXPECT_FALSE(code.checkExtremalVertex(0, "-xz"));
    EXPECT_TRUE(code.checkExtremalVertex(37, "-xz"));
    EXPECT_FALSE(code.checkExtremalVertex(42, "-xz"));
    EXPECT_FALSE(code.checkExtremalVertex(216, "-xz"));
    EXPECT_FALSE(code.checkExtremalVertex(221, "-xz"));
    EXPECT_FALSE(code.checkExtremalVertex(246, "-xz"));
    EXPECT_FALSE(code.checkExtremalVertex(1, "yz"));
}

TEST(localFlip, flips_correctly_one_and_twice)
{
    int l = 8;
    double p = 0.1;
    RhombicCode code = RhombicCode(l, p, p, false, 1);
    vint vs = {0, 72, 512, 519};
    code.localFlip(vs);
    std::vector<int8_t> &flipBits = code.getFlipBits();
    EXPECT_EQ(flipBits[0], 1);
    code.localFlip(vs);
    for (const int value : flipBits)
    {
        EXPECT_EQ(value, 0);
    }
}

TEST(findSweepEdges, correct_edges_one_error)
{
    int l = 4;
    double p = 0.1;
    RhombicCode code = RhombicCode(l, p, p, false, 1);
    code.setError({120});
    code.calculateSyndrome();

    // Full vertex
    vstr upEdgesInSynd = code.findSweepEdges(40, "xyz");
    EXPECT_TRUE(std::find(upEdgesInSynd.begin(), upEdgesInSynd.end(), "xyz") != upEdgesInSynd.end());
    EXPECT_TRUE(std::find(upEdgesInSynd.begin(), upEdgesInSynd.end(), "yz") != upEdgesInSynd.end());
    EXPECT_EQ(upEdgesInSynd.size(), 2);

    upEdgesInSynd = code.findSweepEdges(40, "yz");
    EXPECT_TRUE(std::find(upEdgesInSynd.begin(), upEdgesInSynd.end(), "xyz") != upEdgesInSynd.end());
    EXPECT_TRUE(std::find(upEdgesInSynd.begin(), upEdgesInSynd.end(), "yz") != upEdgesInSynd.end());
    EXPECT_EQ(upEdgesInSynd.size(), 2);

    upEdgesInSynd = code.findSweepEdges(40, "xz");
    EXPECT_TRUE(std::find(upEdgesInSynd.begin(), upEdgesInSynd.end(), "xyz") != upEdgesInSynd.end());
    EXPECT_EQ(upEdgesInSynd.size(), 1);

    upEdgesInSynd = code.findSweepEdges(40, "xy");
    EXPECT_TRUE(std::find(upEdgesInSynd.begin(), upEdgesInSynd.end(), "xyz") != upEdgesInSynd.end());
    EXPECT_EQ(upEdgesInSynd.size(), 1);

    upEdgesInSynd = code.findSweepEdges(40, "-xy");
    EXPECT_TRUE(std::find(upEdgesInSynd.begin(), upEdgesInSynd.end(), "yz") != upEdgesInSynd.end());
    EXPECT_EQ(upEdgesInSynd.size(), 1);

    upEdgesInSynd = code.findSweepEdges(40, "-xz");
    EXPECT_TRUE(std::find(upEdgesInSynd.begin(), upEdgesInSynd.end(), "yz") != upEdgesInSynd.end());
    EXPECT_EQ(upEdgesInSynd.size(), 1);

    upEdgesInSynd = code.findSweepEdges(40, "-xyz");
    EXPECT_EQ(upEdgesInSynd.size(), 0);

    upEdgesInSynd = code.findSweepEdges(40, "-yz");
    EXPECT_EQ(upEdgesInSynd.size(), 0);

    // Type 1 half vertex
    upEdgesInSynd = code.findSweepEdges(104, "xyz");
    EXPECT_TRUE(std::find(upEdgesInSynd.begin(), upEdgesInSynd.end(), "yz") != upEdgesInSynd.end());
    EXPECT_EQ(upEdgesInSynd.size(), 1);

    upEdgesInSynd = code.findSweepEdges(104, "yz");
    EXPECT_EQ(upEdgesInSynd.size(), 0);

    upEdgesInSynd = code.findSweepEdges(104, "xz");
    EXPECT_EQ(upEdgesInSynd.size(), 0);

    upEdgesInSynd = code.findSweepEdges(104, "xy");
    EXPECT_EQ(upEdgesInSynd.size(), 0);

    upEdgesInSynd = code.findSweepEdges(104, "-xy");
    EXPECT_TRUE(std::find(upEdgesInSynd.begin(), upEdgesInSynd.end(), "-xyz") != upEdgesInSynd.end());
    EXPECT_TRUE(std::find(upEdgesInSynd.begin(), upEdgesInSynd.end(), "yz") != upEdgesInSynd.end());
    EXPECT_EQ(upEdgesInSynd.size(), 2);

    upEdgesInSynd = code.findSweepEdges(104, "-xz");
    EXPECT_TRUE(std::find(upEdgesInSynd.begin(), upEdgesInSynd.end(), "-xyz") != upEdgesInSynd.end());
    EXPECT_TRUE(std::find(upEdgesInSynd.begin(), upEdgesInSynd.end(), "yz") != upEdgesInSynd.end());
    EXPECT_EQ(upEdgesInSynd.size(), 2);

    upEdgesInSynd = code.findSweepEdges(104, "-xyz");
    EXPECT_EQ(upEdgesInSynd.size(), 0);

    upEdgesInSynd = code.findSweepEdges(104, "-yz");
    EXPECT_TRUE(std::find(upEdgesInSynd.begin(), upEdgesInSynd.end(), "-xyz") != upEdgesInSynd.end());
    EXPECT_EQ(upEdgesInSynd.size(), 1);

    // Type 2 half vertex
    upEdgesInSynd = code.findSweepEdges(107, "xyz");
    EXPECT_EQ(upEdgesInSynd.size(), 0);

    upEdgesInSynd = code.findSweepEdges(107, "yz");
    EXPECT_TRUE(std::find(upEdgesInSynd.begin(), upEdgesInSynd.end(), "xyz") != upEdgesInSynd.end());
    EXPECT_EQ(upEdgesInSynd.size(), 1);

    upEdgesInSynd = code.findSweepEdges(107, "xz");
    EXPECT_TRUE(std::find(upEdgesInSynd.begin(), upEdgesInSynd.end(), "xyz") != upEdgesInSynd.end());
    EXPECT_TRUE(std::find(upEdgesInSynd.begin(), upEdgesInSynd.end(), "-yz") != upEdgesInSynd.end());
    EXPECT_EQ(upEdgesInSynd.size(), 2);

    upEdgesInSynd = code.findSweepEdges(107, "xy");
    EXPECT_TRUE(std::find(upEdgesInSynd.begin(), upEdgesInSynd.end(), "xyz") != upEdgesInSynd.end());
    EXPECT_TRUE(std::find(upEdgesInSynd.begin(), upEdgesInSynd.end(), "-yz") != upEdgesInSynd.end());
    EXPECT_EQ(upEdgesInSynd.size(), 2);

    upEdgesInSynd = code.findSweepEdges(107, "-xy");
    EXPECT_EQ(upEdgesInSynd.size(), 0);

    upEdgesInSynd = code.findSweepEdges(107, "-xz");
    EXPECT_EQ(upEdgesInSynd.size(), 0);

    upEdgesInSynd = code.findSweepEdges(107, "-xyz");
    EXPECT_TRUE(std::find(upEdgesInSynd.begin(), upEdgesInSynd.end(), "-yz") != upEdgesInSynd.end());
    EXPECT_EQ(upEdgesInSynd.size(), 1);

    upEdgesInSynd = code.findSweepEdges(107, "-yz");
    EXPECT_EQ(upEdgesInSynd.size(), 0);
}

TEST(faceVertices, handles_valid_input)
{
    int l = 6;
    double p = 0.1;
    RhombicCode code = RhombicCode(l, p, p, false, 1);

    // Full vertex
    vint expectedVertices = {86, 93, 266, 302};
    EXPECT_EQ(code.faceVertices(86, {"xyz", "xy", "xy"}), expectedVertices);
    EXPECT_EQ(code.faceVertices(86, {"xy", "xyz", "xyz"}), expectedVertices);

    // Full vertex
    expectedVertices = {86, 91, 265, 301};
    EXPECT_EQ(code.faceVertices(86, {"-xz", "yz", "yz"}), expectedVertices);
    EXPECT_EQ(code.faceVertices(86, {"yz", "-xz", "-xz"}), expectedVertices);

    // Half Vertex Type 1
    expectedVertices = {172, 209, 382, 388};
    EXPECT_EQ(code.faceVertices(388, {"xz", "-xyz", "-xyz"}), expectedVertices);
    EXPECT_EQ(code.faceVertices(388, {"-xyz", "xz", "xz"}), expectedVertices);

    // Half Vertex Type 2
    expectedVertices = {135, 165, 344, 345};
    EXPECT_EQ(code.faceVertices(345, {"-xz", "-xy", "-xy"}), expectedVertices);
    EXPECT_EQ(code.faceVertices(345, {"-xy", "-xz", "-xz"}), expectedVertices);
}

TEST(faceVertices, excepts_too_many_directions)
{
    int l = 6;
    double p = 0.1;
    RhombicCode code = RhombicCode(l, p, p, false, 1);
    EXPECT_THROW(code.faceVertices(0, {"xyz", "-xy", "xy", "xz"}), std::invalid_argument);
}

TEST(faceVertices, excepts_invalid_signs)
{
    int l = 6;
    double p = 0.1;
    RhombicCode code = RhombicCode(l, p, p, false, 1);
    EXPECT_THROW(code.faceVertices(0, {"xyz", "-xy", "xy"}), std::invalid_argument);
    EXPECT_THROW(code.faceVertices(0, {"xyz", "xy", "-xy"}), std::invalid_argument);
}

TEST(faceVertices, excepts_invalid_directions)
{
    int l = 6;
    double p = 0.1;
    RhombicCode code = RhombicCode(l, p, p, false, 1);
    EXPECT_THROW(code.faceVertices(0, {"xyz", "xy", "xz"}), std::invalid_argument);
    EXPECT_THROW(code.faceVertices(0, {"xyz", "-xy", "xy"}), std::invalid_argument);
}

TEST(sweepFullVertex, handles_qubits_errors_xy)
{
    int l = 4;
    double p = 0.1;
    RhombicCode code = RhombicCode(l, p, p, false, 1);

    // One error
    // xy -xz face of vertex 27
    code.setError({81});
    code.calculateSyndrome();
    auto sweepEdges = code.findSweepEdges(27, "xy");
    code.sweepFullVertex(27, sweepEdges, "xy", {"xyz", "-xz", "-yz"});
    auto &flipBits = code.getFlipBits();
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 81)
        {
            EXPECT_EQ(flipBits[i], 1);
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
    // xy xyz face of vertex 27
    code.clearSyndrome();
    code.clearFlipBits();
    code.setError({80});
    code.calculateSyndrome();
    sweepEdges = code.findSweepEdges(27, "xy");
    code.sweepFullVertex(27, sweepEdges, "xy", {"xyz", "-xz", "-yz"});
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 80)
        {
            EXPECT_EQ(flipBits[i], 1);
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
    // xy -yz face of vertex 27
    code.clearSyndrome();
    code.clearFlipBits();
    code.setError({82});
    code.calculateSyndrome();
    sweepEdges = code.findSweepEdges(27, "xy");
    code.sweepFullVertex(27, sweepEdges, "xy", {"xyz", "-xz", "-yz"});
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 82)
        {
            EXPECT_EQ(flipBits[i], 1);
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }

    // Two Errors
    // xy -yz and xy -xz faces of vertex 27
    code.clearSyndrome();
    code.clearFlipBits();
    code.setError({81, 82});
    code.calculateSyndrome();
    sweepEdges = code.findSweepEdges(27, "xy");
    code.sweepFullVertex(27, sweepEdges, "xy", {"xyz", "-xz", "-yz"});
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 81 || i == 82)
        {
            EXPECT_EQ(flipBits[i], 1);
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
    // xy -yz and xy xyz faces of vertex 27
    code.clearSyndrome();
    code.clearFlipBits();
    code.setError({80, 82});
    code.calculateSyndrome();
    sweepEdges = code.findSweepEdges(27, "xy");
    code.sweepFullVertex(27, sweepEdges, "xy", {"xyz", "-xz", "-yz"});
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 80 || i == 82)
        {
            EXPECT_EQ(flipBits[i], 1);
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
    // xy -xz and xy xyz faces of vertex 27
    code.clearSyndrome();
    code.clearFlipBits();
    code.setError({80, 81});
    code.calculateSyndrome();
    sweepEdges = code.findSweepEdges(27, "xy");
    code.sweepFullVertex(27, sweepEdges, "xy", {"xyz", "-xz", "-yz"});
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 80 || i == 81)
        {
            EXPECT_EQ(flipBits[i], 1);
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }

    // Three errors
    // xy -xz, xy xyz and xy -yz faces of vertex 27
    code.clearSyndrome();
    code.clearFlipBits();
    code.setError({80, 81, 82});
    code.calculateSyndrome();
    sweepEdges = code.findSweepEdges(27, "xy");
    code.sweepFullVertex(27, sweepEdges, "xy", {"xyz", "-xz", "-yz"});
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 80 || i == 81 || i == 82)
        {
            EXPECT_EQ(flipBits[i], 1);
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
}

TEST(sweepFullVertex, handles_measurement_errors_xy)
{
    int l = 4;
    double p = 0.1;
    RhombicCode code = RhombicCode(l, p, p, false, 1);

    // 3 sweep edges around vertex
    // xy, xyz, -yz edges of vertex 27
    std::vector<int8_t> syndrome;
    syndrome.assign(14 * l * l * l, 0);
    syndrome[189] = 1;
    syndrome[191] = 1;
    syndrome[501] = 1;
    code.setSyndrome(syndrome);
    vstr sweepEdges = code.findSweepEdges(27, "xy");
    std::vector<int8_t> &flipBits = code.getFlipBits();
    code.sweepFullVertex(27, sweepEdges, "xy", {"xyz", "-xz", "-yz"});
    EXPECT_TRUE(flipBits[80] ^ flipBits[82]);
    // std::cout << "80 = " << flipBits[80] << ", 82 = " << flipBits[82] << std::endl;
    // std::cout << "XOR = " << (flipBits[80] ^ flipBits[82]) << std::endl;
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 80 || i == 82)
        {
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
    // xy, -xz, -yz edges of vertex 27
    code.clearFlipBits();
    syndrome.assign(14 * l * l * l, 0);
    syndrome[191] = 1;
    syndrome[501] = 1;
    syndrome[524] = 1;
    code.setSyndrome(syndrome);
    sweepEdges = code.findSweepEdges(27, "xy");
    code.sweepFullVertex(27, sweepEdges, "xy", {"xyz", "-xz", "-yz"});
    EXPECT_TRUE(flipBits[81] ^ flipBits[82]);
    // std::cout << "80 = " << flipBits[81] << ", 82 = " << flipBits[82] << std::endl;
    // std::cout << "XOR = " << (flipBits[81] ^ flipBits[82]) << std::endl;
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 81 || i == 82)
        {
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
    // xy, xyz, -xz edges of vertex 27
    code.clearFlipBits();
    syndrome.assign(14 * l * l * l, 0);
    syndrome[191] = 1;
    syndrome[189] = 1;
    syndrome[524] = 1;
    code.setSyndrome(syndrome);
    sweepEdges = code.findSweepEdges(27, "xy");
    code.sweepFullVertex(27, sweepEdges, "xy", {"xyz", "-xz", "-yz"});
    EXPECT_TRUE(flipBits[80] ^ flipBits[81]);
    // std::cout << "80 = " << flipBits[80] << ", 81 = " << flipBits[81] << std::endl;
    // std::cout << "XOR = " << (flipBits[80] ^ flipBits[81]) << std::endl;
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 80 || i == 81)
        {
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
    // xyz, -xz, -yz edges of vertex 27
    code.clearFlipBits();
    syndrome.assign(14 * l * l * l, 0);
    syndrome[501] = 1;
    syndrome[189] = 1;
    syndrome[524] = 1;
    code.setSyndrome(syndrome);
    sweepEdges = code.findSweepEdges(27, "xy");
    code.sweepFullVertex(27, sweepEdges, "xy", {"xyz", "-xz", "-yz"});
    EXPECT_TRUE((flipBits[80] && flipBits[81]) ^
                (flipBits[80] && flipBits[82]) ^
                (flipBits[81] && flipBits[82]));
    // std::cout << "80 = " << flipBits[80] << ", 81 = " << flipBits[81] << ", 82 = " << flipBits[82] << std::endl;
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 80 || i == 81 || i == 82)
        {
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
}

TEST(sweepHalfVertex, handles_qubit_errors_xy)
{
    int l = 6;
    double p = 0.1;
    RhombicCode code = RhombicCode(l, p, p, false, 1);

    // One error
    // xyx -xz face of vertex 283
    code.setError({109});
    code.calculateSyndrome();
    auto sweepEdges = code.findSweepEdges(283, "xy");
    code.sweepHalfVertex(283, sweepEdges, "xy", {"xyz", "-xz", "-yz"});
    auto &flipBits = code.getFlipBits();
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 109)
        {
            EXPECT_EQ(flipBits[i], 1);
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
    // xyx -yz face of vertex 283
    code.clearSyndrome();
    code.clearFlipBits();
    code.setError({204});
    code.calculateSyndrome();
    sweepEdges = code.findSweepEdges(283, "xy");
    code.sweepHalfVertex(283, sweepEdges, "xy", {"xyz", "-xz", "-yz"});
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 204)
        {
            EXPECT_EQ(flipBits[i], 1);
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
    // -xz -yz face of vertex 283
    code.clearSyndrome();
    code.clearFlipBits();
    code.setError({113});
    code.calculateSyndrome();
    sweepEdges = code.findSweepEdges(283, "xy");
    code.sweepHalfVertex(283, sweepEdges, "xy", {"xyz", "-xz", "-yz"});
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 113)
        {
            EXPECT_EQ(flipBits[i], 1);
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }

    // Two errors
    // xyz -xz, -xz -yz faces of vertex 283
    code.clearSyndrome();
    code.clearFlipBits();
    code.setError({109, 113});
    code.calculateSyndrome();
    sweepEdges = code.findSweepEdges(283, "xy");
    code.sweepHalfVertex(283, sweepEdges, "xy", {"xyz", "-xz", "-yz"});
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 204)
        {
            EXPECT_EQ(flipBits[i], 1);
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
    // xyz -xz, xyz -yz faces of vertex 283
    code.clearSyndrome();
    code.clearFlipBits();
    code.setError({109, 204});
    code.calculateSyndrome();
    sweepEdges = code.findSweepEdges(283, "xy");
    code.sweepHalfVertex(283, sweepEdges, "xy", {"xyz", "-xz", "-yz"});
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 113)
        {
            EXPECT_EQ(flipBits[i], 1);
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
    // xyz -yz, -xz -yz faces of vertex 283
    code.clearSyndrome();
    code.clearFlipBits();
    code.setError({113, 204});
    code.calculateSyndrome();
    sweepEdges = code.findSweepEdges(283, "xy");
    code.sweepHalfVertex(283, sweepEdges, "xy", {"xyz", "-xz", "-yz"});
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 109)
        {
            EXPECT_EQ(flipBits[i], 1);
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
}

TEST(sweepHalfVertex, handles_measurement_errors_xy)
{
    int l = 6;
    double p = 0.1;
    RhombicCode code = RhombicCode(l, p, p, false, 1);

    // Three sweep edges at vertex 283
    // xyz, -yz and -xz edges
    std::vector<int8_t> syndrome;
    syndrome.assign(14 * l * l * l, 0);
    syndrome[1981] = 1;
    syndrome[480] = 1;
    syndrome[265] = 1;
    code.setSyndrome(syndrome);
    auto sweepEdges = code.findSweepEdges(283, "xy");
    code.sweepHalfVertex(283, sweepEdges, "xy", {"xyz", "-xz", "-yz"});
    auto &flipBits = code.getFlipBits();
    EXPECT_TRUE(flipBits[109] ^ flipBits[113] ^ flipBits[204]);
    // std::cout << "109 = " << flipBits[109] << ", 113 = " << flipBits[113] << ", 204 = " << flipBits[204] << std::endl;
    // std::cout << "XOR = " << (flipBits[109] ^ flipBits[113] ^ flipBits[204]) << std::endl;
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 109 || i == 113 || i == 204)
        {
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
}

TEST(sweepFullVertex, handles_qubit_errors_minus_xy)
{
    int l = 4;
    double p = 0.1;
    RhombicCode code = RhombicCode(l, p, p, false, 1);

    // One error
    // -xyz -xy face of vertex 0
    code.setError({44});
    code.calculateSyndrome();
    auto sweepEdges = code.findSweepEdges(0, "-xy");
    auto &flipBits = code.getFlipBits();
    code.sweepFullVertex(0, sweepEdges, "-xy", {"-xyz", "yz", "xz"});
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 44)
        {
            EXPECT_EQ(flipBits[i], 1);
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
    // -xy xz face of vertex 0
    code.clearSyndrome();
    code.clearFlipBits();
    code.setError({87});
    code.calculateSyndrome();
    sweepEdges = code.findSweepEdges(0, "-xy");
    code.sweepFullVertex(0, sweepEdges, "-xy", {"-xyz", "yz", "xz"});
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 87)
        {
            EXPECT_EQ(flipBits[i], 1);
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
    // -xy yz face of vertex 0
    code.clearSyndrome();
    code.clearFlipBits();
    code.setError({58});
    code.calculateSyndrome();
    sweepEdges = code.findSweepEdges(0, "-xy");
    code.sweepFullVertex(0, sweepEdges, "-xy", {"-xyz", "yz", "xz"});
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 58)
        {
            EXPECT_EQ(flipBits[i], 1);
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }

    // Two errors
    // -xy xz and -xy yz faces of vertex 0
    code.clearSyndrome();
    code.clearFlipBits();
    code.setError({87, 58});
    code.calculateSyndrome();
    sweepEdges = code.findSweepEdges(0, "-xy");
    code.sweepFullVertex(0, sweepEdges, "-xy", {"-xyz", "yz", "xz"});
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 87 || i == 58)
        {
            EXPECT_EQ(flipBits[i], 1);
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
    // -xy xz and -xy -xyz faces of vertex 0
    code.clearSyndrome();
    code.clearFlipBits();
    code.setError({87, 44});
    code.calculateSyndrome();
    sweepEdges = code.findSweepEdges(0, "-xy");
    code.sweepFullVertex(0, sweepEdges, "-xy", {"-xyz", "yz", "xz"});
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 87 || i == 44)
        {
            EXPECT_EQ(flipBits[i], 1);
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
    // -xy yz and -xy -xyz faces of vertex 0
    code.clearSyndrome();
    code.clearFlipBits();
    code.setError({44, 58});
    code.calculateSyndrome();
    sweepEdges = code.findSweepEdges(0, "-xy");
    code.sweepFullVertex(0, sweepEdges, "-xy", {"-xyz", "yz", "xz"});
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 44 || i == 58)
        {
            EXPECT_EQ(flipBits[i], 1);
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }

    // Three errors
    // -xy xz, -xy yz and -xy -xyz faces of vertex 0
    code.clearSyndrome();
    code.clearFlipBits();
    code.setError({87, 58, 44});
    code.calculateSyndrome();
    sweepEdges = code.findSweepEdges(0, "-xy");
    code.sweepFullVertex(0, sweepEdges, "-xy", {"-xyz", "yz", "xz"});
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 87 || i == 58 || i == 44)
        {
            EXPECT_EQ(flipBits[i], 1);
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
}

TEST(sweepFullVertex, handles_measurement_errors_minus_xy)
{
    int l = 4;
    double p = 0.1;
    RhombicCode code = RhombicCode(l, p, p, false, 1);

    // 3 sweep edges around vertex
    // -xy, xz, -xyz edges of vertex 0
    std::vector<int8_t> syndrome;
    syndrome.assign(14 * l * l * l, 0);
    syndrome[555] = 1;
    syndrome[6] = 1;
    syndrome[889] = 1;
    code.setSyndrome(syndrome);
    vstr sweepEdges = code.findSweepEdges(0, "-xy");
    std::vector<int8_t> &flipBits = code.getFlipBits();
    code.sweepFullVertex(0, sweepEdges, "-xy", {"-xyz", "xz", "yz"});
    EXPECT_TRUE(flipBits[44] ^ flipBits[87]);
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 44 || i == 87)
        {
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
    // -xy, -xyz, yz edges of vertex 27
    code.clearFlipBits();
    syndrome.assign(14 * l * l * l, 0);
    syndrome[555] = 1;
    syndrome[889] = 1;
    syndrome[4] = 1;
    code.setSyndrome(syndrome);
    sweepEdges = code.findSweepEdges(0, "-xy");
    code.sweepFullVertex(0, sweepEdges, "-xy", {"-xyz", "xz", "yz"});
    EXPECT_TRUE(flipBits[44] ^ flipBits[58]);
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 44 || i == 58)
        {
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
    // -xy, yz, xz edges of vertex 27
    code.clearFlipBits();
    syndrome.assign(14 * l * l * l, 0);
    syndrome[555] = 1;
    syndrome[4] = 1;
    syndrome[6] = 1;
    code.setSyndrome(syndrome);
    sweepEdges = code.findSweepEdges(0, "-xy");
    code.sweepFullVertex(0, sweepEdges, "-xy", {"-xyz", "xz", "yz"});
    EXPECT_TRUE(flipBits[58] ^ flipBits[87]);
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 58 || i == 87)
        {
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
    // -xyz, xz, yz edges of vertex 27
    code.clearFlipBits();
    syndrome.assign(14 * l * l * l, 0);
    syndrome[889] = 1;
    syndrome[4] = 1;
    syndrome[6] = 1;
    code.setSyndrome(syndrome);
    sweepEdges = code.findSweepEdges(0, "-xy");
    code.sweepFullVertex(0, sweepEdges, "-xy", {"-xyz", "xz", "yz"});
    EXPECT_TRUE((flipBits[44] && flipBits[87]) ^
                (flipBits[44] && flipBits[58]) ^
                (flipBits[87] && flipBits[58]));
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 44 || i == 87 || i == 58)
        {
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
}

TEST(sweepFullVertex, handles_qubit_errors_xz)
{
    int l = 4;
    double p = 0.1;
    RhombicCode code = RhombicCode(l, p, p, false, 1);

    // Two errors
    // xy -yz and xy xyz faces of vertex 27
    code.setError({80, 82});
    code.calculateSyndrome();
    auto sweepEdges = code.findSweepEdges(27, "xz");
    auto &flipBits = code.getFlipBits();
    code.sweepFullVertex(27, sweepEdges, "xz", {"xyz", "-xy", "-yz"});
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 79 || i == 83)
        {
            EXPECT_EQ(flipBits[i], 1);
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
    // xz -yz and xz -xy faces of vertex 27
    code.clearSyndrome();
    code.clearFlipBits();
    code.setError({83, 117});
    code.calculateSyndrome();
    sweepEdges = code.findSweepEdges(27, "xz");
    code.sweepFullVertex(27, sweepEdges, "xz", {"xyz", "-xy", "-yz"});
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 83 || i == 117)
        {
            EXPECT_EQ(flipBits[i], 1);
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
    // xz xyz and xz -xy faces of vertex 27
    code.clearSyndrome();
    code.clearFlipBits();
    code.setError({79, 117});
    code.calculateSyndrome();
    sweepEdges = code.findSweepEdges(27, "xz");
    code.sweepFullVertex(27, sweepEdges, "xz", {"xyz", "-xy", "-yz"});
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 79 || i == 117)
        {
            EXPECT_EQ(flipBits[i], 1);
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }

    // One error
    // xz -yz face of vertex 27
    code.clearSyndrome();
    code.clearFlipBits();
    code.setError({83});
    code.calculateSyndrome();
    sweepEdges = code.findSweepEdges(27, "xz");
    code.sweepFullVertex(27, sweepEdges, "xz", {"xyz", "-xy", "-yz"});
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 83)
        {
            EXPECT_EQ(flipBits[i], 1);
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
    // xz xyz face of vertex 27
    code.clearSyndrome();
    code.clearFlipBits();
    code.setError({79});
    code.calculateSyndrome();
    sweepEdges = code.findSweepEdges(27, "xz");
    code.sweepFullVertex(27, sweepEdges, "xz", {"xyz", "-xy", "-yz"});
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 79)
        {
            EXPECT_EQ(flipBits[i], 1);
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
    // xz -xy face of vertex 27
    code.clearSyndrome();
    code.clearFlipBits();
    code.setError({117});
    code.calculateSyndrome();
    sweepEdges = code.findSweepEdges(27, "xz");
    code.sweepFullVertex(27, sweepEdges, "xz", {"xyz", "-xy", "-yz"});
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 117)
        {
            EXPECT_EQ(flipBits[i], 1);
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }

    // Three errors
    // xz -xy, xz -yz, xz xyz faces of vertex 27
    code.clearSyndrome();
    code.clearFlipBits();
    code.setError({79, 83, 117});
    code.calculateSyndrome();
    sweepEdges = code.findSweepEdges(27, "xz");
    code.sweepFullVertex(27, sweepEdges, "xz", {"xyz", "-xy", "-yz"});
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 79 || i == 83 || i == 117)
        {
            EXPECT_EQ(flipBits[i], 1);
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
}

TEST(sweepFullVertex, handles_measurement_errors_xz)
{
    int l = 4;
    double p = 0.1;
    RhombicCode code = RhombicCode(l, p, p, false, 1);

    // 3 sweep edges around vertex
    // xz, xyz, -yz edges of vertex 27
    std::vector<int8_t> syndrome;
    syndrome.assign(14 * l * l * l, 0);
    syndrome[189] = 1;
    syndrome[195] = 1;
    syndrome[501] = 1;
    code.setSyndrome(syndrome);
    vstr sweepEdges = code.findSweepEdges(27, "xz");
    std::vector<int8_t> &flipBits = code.getFlipBits();
    code.sweepFullVertex(27, sweepEdges, "xz", {"xyz", "-xy", "-yz"});
    EXPECT_TRUE(flipBits[79] ^ flipBits[83]);
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 79 || i == 83)
        {
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
    // // xz, -xy, -yz edges of vertex 27
    code.clearFlipBits();
    syndrome.assign(14 * l * l * l, 0);
    syndrome[195] = 1;
    syndrome[501] = 1;
    syndrome[604] = 1;
    code.setSyndrome(syndrome);
    sweepEdges = code.findSweepEdges(27, "xz");
    code.sweepFullVertex(27, sweepEdges, "xz", {"xyz", "-xy", "-yz"});
    EXPECT_TRUE(flipBits[83] ^ flipBits[117]);
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 83 || i == 117)
        {
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
    // // xz, xyz, -xy edges of vertex 27
    code.clearFlipBits();
    syndrome.assign(14 * l * l * l, 0);
    syndrome[195] = 1;
    syndrome[189] = 1;
    syndrome[604] = 1;
    code.setSyndrome(syndrome);
    sweepEdges = code.findSweepEdges(27, "xz");
    code.sweepFullVertex(27, sweepEdges, "xz", {"xyz", "-xy", "-yz"});
    EXPECT_TRUE(flipBits[79] ^ flipBits[117]);
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 79 || i == 117)
        {
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
    // xyz, -xy, -yz edges of vertex 27
    code.clearFlipBits();
    syndrome.assign(14 * l * l * l, 0);
    syndrome[501] = 1;
    syndrome[189] = 1;
    syndrome[604] = 1;
    code.setSyndrome(syndrome);
    sweepEdges = code.findSweepEdges(27, "xz");
    code.sweepFullVertex(27, sweepEdges, "xz", {"xyz", "-xy", "-yz"});
    EXPECT_TRUE((flipBits[79] && flipBits[83]) ^
                (flipBits[79] && flipBits[117]) ^
                (flipBits[83] && flipBits[117]));
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 79 || i == 83 || i == 117)
        {
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
}

TEST(sweepFullVertex, handles_qubit_errors_minus_xz)
{
    int l = 4;
    double p = 0.1;
    RhombicCode code = RhombicCode(l, p, p, false, 1);

    // One error
    // xy -xz face of vertex 27
    code.setError({81});
    code.calculateSyndrome();
    auto sweepEdges = code.findSweepEdges(27, "-xz");
    auto &flipBits = code.getFlipBits();
    code.sweepFullVertex(27, sweepEdges, "-xz", {"-xyz", "xy", "yz"});
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 81)
        {
            EXPECT_EQ(flipBits[i], 1);
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
    // -xz yz face of vertex 27
    code.clearSyndrome();
    code.clearFlipBits();
    code.setError({95});
    code.calculateSyndrome();
    sweepEdges = code.findSweepEdges(27, "-xz");
    code.sweepFullVertex(27, sweepEdges, "-xz", {"-xyz", "xy", "yz"});
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 95)
        {
            EXPECT_EQ(flipBits[i], 1);
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
    // -xz -xyz face of vertex 27
    code.clearSyndrome();
    code.clearFlipBits();
    code.setError({31});
    code.calculateSyndrome();
    sweepEdges = code.findSweepEdges(27, "-xz");
    code.sweepFullVertex(27, sweepEdges, "-xz", {"-xyz", "xy", "yz"});
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 31)
        {
            EXPECT_EQ(flipBits[i], 1);
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }

    // Two errors
    // -xz xy and -xz yz faces of vertex 27
    code.clearSyndrome();
    code.clearFlipBits();
    code.setError({81, 95});
    code.calculateSyndrome();
    sweepEdges = code.findSweepEdges(27, "-xz");
    code.sweepFullVertex(27, sweepEdges, "-xz", {"-xyz", "xy", "yz"});
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 81 || i == 95)
        {
            EXPECT_EQ(flipBits[i], 1);
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
    // -xz xy and -xz -xyz faces of vertex 27
    code.clearSyndrome();
    code.clearFlipBits();
    code.setError({31, 81});
    code.calculateSyndrome();
    sweepEdges = code.findSweepEdges(27, "-xz");
    code.sweepFullVertex(27, sweepEdges, "-xz", {"-xyz", "xy", "yz"});
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 31 || i == 81)
        {
            EXPECT_EQ(flipBits[i], 1);
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
    // -xz yz and -xz -xyz faces of vertex 27
    code.clearSyndrome();
    code.clearFlipBits();
    code.setError({31, 95});
    code.calculateSyndrome();
    sweepEdges = code.findSweepEdges(27, "-xz");
    code.sweepFullVertex(27, sweepEdges, "-xz", {"-xyz", "xy", "yz"});
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 31 || i == 95)
        {
            EXPECT_EQ(flipBits[i], 1);
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }

    // Three errors
    // -xz yx, -xz xy and -xz -xyz faces of vertex 27
    code.clearSyndrome();
    code.clearFlipBits();
    code.setError({31, 81, 95});
    code.calculateSyndrome();
    sweepEdges = code.findSweepEdges(27, "-xz");
    code.sweepFullVertex(27, sweepEdges, "-xz", {"-xyz", "xy", "yz"});
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 31 || i == 81 || i == 95)
        {
            EXPECT_EQ(flipBits[i], 1);
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
}

TEST(sweepFullVertex, handles_measurement_errors_minus_xz)
{
    int l = 4;
    double p = 0.1;
    RhombicCode code = RhombicCode(l, p, p, false, 1);

    // 3 sweep edges around vertex
    // -xz, -xyz, yz edges of vertex 27
    std::vector<int8_t> syndrome;
    syndrome.assign(14 * l * l * l, 0);
    syndrome[193] = 1;
    syndrome[490] = 1;
    syndrome[524] = 1;
    code.setSyndrome(syndrome);
    vstr sweepEdges = code.findSweepEdges(27, "-xz");
    std::vector<int8_t> &flipBits = code.getFlipBits();
    code.sweepFullVertex(27, sweepEdges, "-xz", {"-xyz", "xy", "yz"});
    EXPECT_TRUE(flipBits[31] ^ flipBits[95]);
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 31 || i == 95)
        {
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
    // // -xz, xy, yz edges of vertex 27
    code.clearFlipBits();
    syndrome.assign(14 * l * l * l, 0);
    syndrome[191] = 1;
    syndrome[193] = 1;
    syndrome[524] = 1;
    code.setSyndrome(syndrome);
    sweepEdges = code.findSweepEdges(27, "-xz");
    code.sweepFullVertex(27, sweepEdges, "-xz", {"-xyz", "xy", "yz"});
    EXPECT_TRUE(flipBits[81] ^ flipBits[95]);
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 81 || i == 95)
        {
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
    // // -xz, -xyz, xy edges of vertex 27
    code.clearFlipBits();
    syndrome.assign(14 * l * l * l, 0);
    syndrome[191] = 1;
    syndrome[490] = 1;
    syndrome[524] = 1;
    code.setSyndrome(syndrome);
    sweepEdges = code.findSweepEdges(27, "-xz");
    code.sweepFullVertex(27, sweepEdges, "-xz", {"-xyz", "xy", "yz"});
    EXPECT_TRUE(flipBits[31] ^ flipBits[81]);
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 31 || i == 81)
        {
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
    // -xyz, xy, yz edges of vertex 27
    code.clearFlipBits();
    syndrome.assign(14 * l * l * l, 0);
    syndrome[191] = 1;
    syndrome[193] = 1;
    syndrome[490] = 1;
    code.setSyndrome(syndrome);
    sweepEdges = code.findSweepEdges(27, "-xz");
    code.sweepFullVertex(27, sweepEdges, "-xz", {"-xyz", "xy", "yz"});
    EXPECT_TRUE((flipBits[81] && flipBits[95]) ^
                (flipBits[81] && flipBits[31]) ^
                (flipBits[95] && flipBits[31]));
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 31 || i == 81 || i == 95)
        {
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
}

TEST(sweepFullVertex, handles_qubit_errors_yz)
{
    int l = 4;
    double p = 0.1;
    RhombicCode code = RhombicCode(l, p, p, false, 1);

    // Two errors
    // xy -xz and xy xyz faces of vertex 27
    code.setError({80, 81});
    code.calculateSyndrome();
    auto sweepEdges = code.findSweepEdges(27, "yz");
    auto &flipBits = code.getFlipBits();
    code.sweepFullVertex(27, sweepEdges, "yz", {"xyz", "-xy", "-xz"});
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 78 || i == 95)
        {
            EXPECT_EQ(flipBits[i], 1);
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
    // yz -xy and yz -xz faces of vertex 27
    code.clearSyndrome();
    code.clearFlipBits();
    code.setError({130, 95});
    code.calculateSyndrome();
    sweepEdges = code.findSweepEdges(27, "yz");
    code.sweepFullVertex(27, sweepEdges, "yz", {"xyz", "-xy", "-xz"});
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 130 || i == 95)
        {
            EXPECT_EQ(flipBits[i], 1);
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
    // yz -xy and yz xyz faces of vertex 27
    code.clearSyndrome();
    code.clearFlipBits();
    code.setError({130, 78});
    code.calculateSyndrome();
    sweepEdges = code.findSweepEdges(27, "yz");
    code.sweepFullVertex(27, sweepEdges, "yz", {"xyz", "-xy", "-xz"});
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 130 || i == 78)
        {
            EXPECT_EQ(flipBits[i], 1);
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }

    // One error
    // yz xyz face of vertex 27
    code.clearSyndrome();
    code.clearFlipBits();
    code.setError({78});
    code.calculateSyndrome();
    sweepEdges = code.findSweepEdges(27, "yz");
    code.sweepFullVertex(27, sweepEdges, "yz", {"xyz", "-xy", "-xz"});
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 78)
        {
            EXPECT_EQ(flipBits[i], 1);
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
    // yz -xy face of vertex 27
    code.clearSyndrome();
    code.clearFlipBits();
    code.setError({130});
    code.calculateSyndrome();
    sweepEdges = code.findSweepEdges(27, "yz");
    code.sweepFullVertex(27, sweepEdges, "yz", {"xyz", "-xy", "-xz"});
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 130)
        {
            EXPECT_EQ(flipBits[i], 1);
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
    // yz -xz face of vertex 27
    code.clearSyndrome();
    code.clearFlipBits();
    code.setError({95});
    code.calculateSyndrome();
    sweepEdges = code.findSweepEdges(27, "yz");
    code.sweepFullVertex(27, sweepEdges, "yz", {"xyz", "-xy", "-xz"});
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 95)
        {
            EXPECT_EQ(flipBits[i], 1);
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }

    // Three errors
    // yz -xy, yz -xz and yz xyz faces of vertex 27
    code.clearSyndrome();
    code.clearFlipBits();
    code.setError({130, 78, 95});
    code.calculateSyndrome();
    sweepEdges = code.findSweepEdges(27, "yz");
    code.sweepFullVertex(27, sweepEdges, "yz", {"xyz", "-xy", "-xz"});
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 130 || i == 78 || i == 95)
        {
            EXPECT_EQ(flipBits[i], 1);
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
}

TEST(sweepFullVertex, handles_measurement_errors_yz)
{
    int l = 4;
    double p = 0.1;
    RhombicCode code = RhombicCode(l, p, p, false, 1);

    // 3 sweep edges around vertex
    // -xz, xyz, yz edges of vertex 27
    std::vector<int8_t> syndrome;
    syndrome.assign(14 * l * l * l, 0);
    syndrome[193] = 1;
    syndrome[189] = 1;
    syndrome[524] = 1;
    code.setSyndrome(syndrome);
    vstr sweepEdges = code.findSweepEdges(27, "yz");
    std::vector<int8_t> &flipBits = code.getFlipBits();
    code.sweepFullVertex(27, sweepEdges, "yz", {"xyz", "-xy", "-xz"});
    EXPECT_TRUE(flipBits[78] ^ flipBits[95]);
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 78 || i == 95)
        {
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
    // -xz, -xy, yz edges of vertex 27
    code.clearFlipBits();
    syndrome.assign(14 * l * l * l, 0);
    syndrome[604] = 1;
    syndrome[193] = 1;
    syndrome[524] = 1;
    code.setSyndrome(syndrome);
    sweepEdges = code.findSweepEdges(27, "yz");
    code.sweepFullVertex(27, sweepEdges, "yz", {"xyz", "-xy", "-xz"});
    EXPECT_TRUE(flipBits[130] ^ flipBits[95]);
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 130 || i == 95)
        {
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
    // yz, xyz, -xy edges of vertex 27
    code.clearFlipBits();
    syndrome.assign(14 * l * l * l, 0);
    syndrome[193] = 1;
    syndrome[189] = 1;
    syndrome[604] = 1;
    code.setSyndrome(syndrome);
    sweepEdges = code.findSweepEdges(27, "yz");
    code.sweepFullVertex(27, sweepEdges, "yz", {"xyz", "-xy", "-xz"});
    EXPECT_TRUE(flipBits[130] ^ flipBits[78]);
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 130 || i == 78)
        {
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
    // xyz, -xy, -xz edges of vertex 27
    code.clearFlipBits();
    syndrome.assign(14 * l * l * l, 0);
    syndrome[189] = 1;
    syndrome[524] = 1;
    syndrome[604] = 1;
    code.setSyndrome(syndrome);
    sweepEdges = code.findSweepEdges(27, "yz");
    code.sweepFullVertex(27, sweepEdges, "yz", {"xyz", "-xy", "-xz"});
    EXPECT_TRUE((flipBits[78] && flipBits[95]) ^
                (flipBits[78] && flipBits[130]) ^
                (flipBits[95] && flipBits[130]));
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 78 || i == 130 || i == 95)
        {
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
}

TEST(sweepFullVertex, handles_qubit_errors_minus_yz)
{
    int l = 4;
    double p = 0.1;
    RhombicCode code = RhombicCode(l, p, p, false, 1);

    // One error
    // xy -yz face of vertex 27
    code.setError({82});
    code.calculateSyndrome();
    auto sweepEdges = code.findSweepEdges(27, "-yz");
    auto &flipBits = code.getFlipBits();
    code.sweepFullVertex(27, sweepEdges, "-yz", {"-xyz", "xy", "xz"});
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 82)
        {
            EXPECT_EQ(flipBits[i], 1);
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
    // -yz xz face of vertex 27
    code.clearSyndrome();
    code.clearFlipBits();
    code.setError({83});
    code.calculateSyndrome();
    sweepEdges = code.findSweepEdges(27, "-yz");
    code.sweepFullVertex(27, sweepEdges, "-yz", {"-xyz", "xy", "xz"});
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 83)
        {
            EXPECT_EQ(flipBits[i], 1);
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
    // -yz -xyz face of vertex 27
    code.clearSyndrome();
    code.clearFlipBits();
    code.setError({18});
    code.calculateSyndrome();
    sweepEdges = code.findSweepEdges(27, "-yz");
    code.sweepFullVertex(27, sweepEdges, "-yz", {"-xyz", "xy", "xz"});
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 18)
        {
            EXPECT_EQ(flipBits[i], 1);
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }

    // Two errors
    // -yz xz and -yz xy face of vertex 27
    code.clearSyndrome();
    code.clearFlipBits();
    code.setError({83, 82});
    code.calculateSyndrome();
    sweepEdges = code.findSweepEdges(27, "-yz");
    code.sweepFullVertex(27, sweepEdges, "-yz", {"-xyz", "xy", "xz"});
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 83 || i == 82)
        {
            EXPECT_EQ(flipBits[i], 1);
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
    // -yz xz and -yz -xyz face of vertex 27
    code.clearSyndrome();
    code.clearFlipBits();
    code.setError({83, 18});
    code.calculateSyndrome();
    sweepEdges = code.findSweepEdges(27, "-yz");
    code.sweepFullVertex(27, sweepEdges, "-yz", {"-xyz", "xy", "xz"});
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 83 || i == 18)
        {
            EXPECT_EQ(flipBits[i], 1);
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
    // -yz -xyz and -yz xy face of vertex 27
    code.clearSyndrome();
    code.clearFlipBits();
    code.setError({82, 18});
    code.calculateSyndrome();
    sweepEdges = code.findSweepEdges(27, "-yz");
    code.sweepFullVertex(27, sweepEdges, "-yz", {"-xyz", "xy", "xz"});
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 18 || i == 82)
        {
            EXPECT_EQ(flipBits[i], 1);
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }

    // Three errors
    // -yz xz, -yz -xyz and -yz xy face of vertex 27
    code.clearSyndrome();
    code.clearFlipBits();
    code.setError({83, 82, 18});
    code.calculateSyndrome();
    sweepEdges = code.findSweepEdges(27, "-yz");
    code.sweepFullVertex(27, sweepEdges, "-yz", {"-xyz", "xy", "xz"});
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 83 || i == 82 || i == 18)
        {
            EXPECT_EQ(flipBits[i], 1);
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
}

TEST(sweepFullVertex, handles_measurement_errors_minus_yz)
{
    int l = 4;
    double p = 0.1;
    RhombicCode code = RhombicCode(l, p, p, false, 1);

    // 3 sweep edges around vertex
    // xz, -xyz, -yz edges of vertex 27
    std::vector<int8_t> syndrome;
    syndrome.assign(14 * l * l * l, 0);
    syndrome[195] = 1;
    syndrome[501] = 1;
    syndrome[490] = 1;
    code.setSyndrome(syndrome);
    vstr sweepEdges = code.findSweepEdges(27, "-yz");
    std::vector<int8_t> &flipBits = code.getFlipBits();
    code.sweepFullVertex(27, sweepEdges, "-yz", {"-xyz", "xy", "xz"});
    EXPECT_TRUE(flipBits[83] ^ flipBits[18]);
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 83 || i == 18)
        {
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
    // xz, xy, -yz edges of vertex 27
    code.clearFlipBits();
    syndrome.assign(14 * l * l * l, 0);
    syndrome[191] = 1;
    syndrome[195] = 1;
    syndrome[501] = 1;
    code.setSyndrome(syndrome);
    sweepEdges = code.findSweepEdges(27, "-yz");
    code.sweepFullVertex(27, sweepEdges, "-yz", {"-xyz", "xy", "xz"});
    EXPECT_TRUE(flipBits[82] ^ flipBits[83]);
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 82 || i == 83)
        {
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
    // -yz, -xyz, xy edges of vertex 27
    code.clearFlipBits();
    syndrome.assign(14 * l * l * l, 0);
    syndrome[191] = 1;
    syndrome[490] = 1;
    syndrome[501] = 1;
    code.setSyndrome(syndrome);
    sweepEdges = code.findSweepEdges(27, "-yz");
    code.sweepFullVertex(27, sweepEdges, "-yz", {"-xyz", "xy", "xz"});
    EXPECT_TRUE(flipBits[82] ^ flipBits[18]);
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 82 || i == 18)
        {
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
    // xyz, -xy, -xz edges of vertex 27
    code.clearFlipBits();
    syndrome.assign(14 * l * l * l, 0);
    syndrome[191] = 1;
    syndrome[195] = 1;
    syndrome[490] = 1;
    code.setSyndrome(syndrome);
    sweepEdges = code.findSweepEdges(27, "-yz");
    code.sweepFullVertex(27, sweepEdges, "-yz", {"-xyz", "xy", "xz"});
    EXPECT_TRUE((flipBits[18] && flipBits[82]) ^
                (flipBits[18] && flipBits[83]) ^
                (flipBits[82] && flipBits[83]));
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 18 || i == 83 || i == 82)
        {
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
}

TEST(sweepFullVertex, handles_qubit_errors_xyz)
{
    int l = 4;
    double p = 0.1;
    RhombicCode code = RhombicCode(l, p, p, false, 1);

    // One Error
    // xy xyz face of vertex 27
    code.setError({80});
    code.calculateSyndrome();
    auto sweepEdges = code.findSweepEdges(27, "xyz");
    auto &flipBits = code.getFlipBits();
    code.sweepFullVertex(27, sweepEdges, "xyz", {"xy", "yz", "xz"});
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 80)
        {
            EXPECT_EQ(flipBits[i], 1);
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
    // xz xyz face of vertex 27
    code.clearSyndrome();
    code.clearFlipBits();
    code.setError({79});
    code.calculateSyndrome();
    sweepEdges = code.findSweepEdges(27, "xyz");
    code.sweepFullVertex(27, sweepEdges, "xyz", {"xy", "yz", "xz"});
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 79)
        {
            EXPECT_EQ(flipBits[i], 1);
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
    // yz xyz face of vertex 27
    code.clearSyndrome();
    code.clearFlipBits();
    code.setError({78});
    code.calculateSyndrome();
    sweepEdges = code.findSweepEdges(27, "xyz");
    code.sweepFullVertex(27, sweepEdges, "xyz", {"xy", "yz", "xz"});
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 78)
        {
            EXPECT_EQ(flipBits[i], 1);
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }

    // Two errors
    // xyz xy and xyz yz faces of vertex 27
    code.clearSyndrome();
    code.clearFlipBits();
    code.setError({78, 80});
    code.calculateSyndrome();
    sweepEdges = code.findSweepEdges(27, "xyz");
    code.sweepFullVertex(27, sweepEdges, "xyz", {"xy", "yz", "xz"});
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 78 || i == 80)
        {
            EXPECT_EQ(flipBits[i], 1);
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
    // xyz xy and xyz xz faces of vertex 27
    code.clearSyndrome();
    code.clearFlipBits();
    code.setError({78, 79});
    code.calculateSyndrome();
    sweepEdges = code.findSweepEdges(27, "xyz");
    code.sweepFullVertex(27, sweepEdges, "xyz", {"xy", "yz", "xz"});
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 78 || i == 79)
        {
            EXPECT_EQ(flipBits[i], 1);
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
    // xyz xz and xyz yz faces of vertex 27
    code.clearSyndrome();
    code.clearFlipBits();
    code.setError({79, 80});
    code.calculateSyndrome();
    sweepEdges = code.findSweepEdges(27, "xyz");
    code.sweepFullVertex(27, sweepEdges, "xyz", {"xy", "yz", "xz"});
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 79 || i == 80)
        {
            EXPECT_EQ(flipBits[i], 1);
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
    // Three errors
    // xyz xy, xyz xz and xyz yz faces of vertex 27
    code.clearSyndrome();
    code.clearFlipBits();
    code.setError({78, 80, 79});
    code.calculateSyndrome();
    sweepEdges = code.findSweepEdges(27, "xyz");
    code.sweepFullVertex(27, sweepEdges, "xyz", {"xy", "yz", "xz"});
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 78 || i == 80 || i == 79)
        {
            EXPECT_EQ(flipBits[i], 1);
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
}

TEST(sweepFullVertex, handles_measurement_errors_xyz)
{
    int l = 4;
    double p = 0.1;
    RhombicCode code = RhombicCode(l, p, p, false, 1);

    // 3 sweep edges around vertex
    // xyz, yz and xz edges of vertex 27
    std::vector<int8_t> syndrome;
    syndrome.assign(14 * l * l * l, 0);
    syndrome[189] = 1;
    syndrome[193] = 1;
    syndrome[195] = 1;
    code.setSyndrome(syndrome);
    vstr sweepEdges = code.findSweepEdges(27, "xyz");
    std::vector<int8_t> &flipBits = code.getFlipBits();
    code.sweepFullVertex(27, sweepEdges, "xyz", {"xy", "xz", "yz"});
    EXPECT_TRUE(flipBits[78] ^ flipBits[79]);
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 78 || i == 79)
        {
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
    // xyz, xy, yz edges of vertex 27
    code.clearFlipBits();
    syndrome.assign(14 * l * l * l, 0);
    syndrome[191] = 1;
    syndrome[189] = 1;
    syndrome[193] = 1;
    code.setSyndrome(syndrome);
    sweepEdges = code.findSweepEdges(27, "xyz");
    code.sweepFullVertex(27, sweepEdges, "xyz", {"xy", "xz", "yz"});
    EXPECT_TRUE(flipBits[80] ^ flipBits[78]);
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 78 || i == 80)
        {
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
    // xyz, xy, xz edges of vertex 27
    code.clearFlipBits();
    syndrome.assign(14 * l * l * l, 0);
    syndrome[189] = 1;
    syndrome[195] = 1;
    syndrome[191] = 1;
    code.setSyndrome(syndrome);
    sweepEdges = code.findSweepEdges(27, "xyz");
    code.sweepFullVertex(27, sweepEdges, "xyz", {"xy", "xz", "yz"});
    EXPECT_TRUE(flipBits[79] ^ flipBits[80]);
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 79 || i == 80)
        {
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
    // yz, xy, xz edges of vertex 27
    code.clearFlipBits();
    syndrome.assign(14 * l * l * l, 0);
    syndrome[191] = 1;
    syndrome[195] = 1;
    syndrome[193] = 1;
    code.setSyndrome(syndrome);
    sweepEdges = code.findSweepEdges(27, "xyz");
    code.sweepFullVertex(27, sweepEdges, "xyz", {"xy", "xz", "yz"});
    EXPECT_TRUE((flipBits[78] && flipBits[79]) ^
                (flipBits[78] && flipBits[80]) ^
                (flipBits[79] && flipBits[80]));
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 78 || i == 79 || i == 80)
        {
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
}

TEST(sweepFullVertex, handles_qubit_errors_minus_xyz)
{
    int l = 4;
    double p = 0.1;
    RhombicCode code = RhombicCode(l, p, p, false, 1);

    // Two Errors
    // xy -yz and xy -xz faces of vertex 27
    code.setError({81, 82});
    code.calculateSyndrome();
    auto sweepEdges = code.findSweepEdges(27, "-xyz");
    auto &flipBits = code.getFlipBits();
    code.sweepFullVertex(27, sweepEdges, "-xyz", {"-xy", "-yz", "-xz"});
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 18 || i == 31)
        {
            EXPECT_EQ(flipBits[i], 1);
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
    // -xyz -xy and -xyz -xz faces of vertex 27
    code.clearSyndrome();
    code.clearFlipBits();
    code.setError({68, 31});
    code.calculateSyndrome();
    sweepEdges = code.findSweepEdges(27, "-xyz");
    code.sweepFullVertex(27, sweepEdges, "-xyz", {"-xy", "-yz", "-xz"});
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 68 || i == 31)
        {
            EXPECT_EQ(flipBits[i], 1);
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
    // -xyz -xy and -xyz -yz faces of vertex 27
    code.clearSyndrome();
    code.clearFlipBits();
    code.setError({68, 18});
    code.calculateSyndrome();
    sweepEdges = code.findSweepEdges(27, "-xyz");
    code.sweepFullVertex(27, sweepEdges, "-xyz", {"-xy", "-yz", "-xz"});
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 68 || i == 18)
        {
            EXPECT_EQ(flipBits[i], 1);
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }

    // Three errors
    // -xyz -xy, -xyz -yz and -xyz -xz faces of vertex 27
    // -xyz -xy and -xyz -xz faces of vertex 27
    code.clearSyndrome();
    code.clearFlipBits();
    code.setError({68, 31, 18});
    code.calculateSyndrome();
    sweepEdges = code.findSweepEdges(27, "-xyz");
    code.sweepFullVertex(27, sweepEdges, "-xyz", {"-xy", "-yz", "-xz"});
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 68 || i == 31 || i == 18)
        {
            EXPECT_EQ(flipBits[i], 1);
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }

    // One error
    // -xyz -xy face of vertex 27
    code.clearSyndrome();
    code.clearFlipBits();
    code.setError({68});
    code.calculateSyndrome();
    sweepEdges = code.findSweepEdges(27, "-xyz");
    code.sweepFullVertex(27, sweepEdges, "-xyz", {"-xy", "-yz", "-xz"});
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 68)
        {
            EXPECT_EQ(flipBits[i], 1);
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
    // -xyz -xz face of vertex 27
    code.clearSyndrome();
    code.clearFlipBits();
    code.setError({31});
    code.calculateSyndrome();
    sweepEdges = code.findSweepEdges(27, "-xyz");
    code.sweepFullVertex(27, sweepEdges, "-xyz", {"-xy", "-yz", "-xz"});
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 31)
        {
            EXPECT_EQ(flipBits[i], 1);
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
    // -xyz -yz face of vertex 27
    code.clearSyndrome();
    code.clearFlipBits();
    code.setError({18});
    code.calculateSyndrome();
    sweepEdges = code.findSweepEdges(27, "-xyz");
    code.sweepFullVertex(27, sweepEdges, "-xyz", {"-xy", "-yz", "-xz"});
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 18)
        {
            EXPECT_EQ(flipBits[i], 1);
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
}

TEST(sweepFullVertex, handles_measurement_errors_minus_xyz)
{
    int l = 4;
    double p = 0.1;
    RhombicCode code = RhombicCode(l, p, p, false, 1);

    // 3 sweep edges around vertex
    // -xyz, -yz and -xz edges of vertex 27
    std::vector<int8_t> syndrome;
    syndrome.assign(14 * l * l * l, 0);
    syndrome[490] = 1;
    syndrome[501] = 1;
    syndrome[524] = 1;
    code.setSyndrome(syndrome);
    vstr sweepEdges = code.findSweepEdges(27, "-xyz");
    std::vector<int8_t> &flipBits = code.getFlipBits();
    code.sweepFullVertex(27, sweepEdges, "-xyz", {"-xy", "-xz", "-yz"});
    EXPECT_TRUE(flipBits[18] ^ flipBits[31]);
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 18 || i == 31)
        {
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
    // -xyz, -xy, -yz edges of vertex 27
    code.clearFlipBits();
    syndrome.assign(14 * l * l * l, 0);
    syndrome[490] = 1;
    syndrome[604] = 1;
    syndrome[501] = 1;
    code.setSyndrome(syndrome);
    sweepEdges = code.findSweepEdges(27, "-xyz");
    code.sweepFullVertex(27, sweepEdges, "-xyz", {"-xy", "-xz", "-yz"});
    EXPECT_TRUE(flipBits[68] ^ flipBits[18]);
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 68 || i == 18)
        {
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
    // -xyz, -xy, -xz edges of vertex 27
    code.clearFlipBits();
    syndrome.assign(14 * l * l * l, 0);
    syndrome[490] = 1;
    syndrome[604] = 1;
    syndrome[524] = 1;
    code.setSyndrome(syndrome);
    sweepEdges = code.findSweepEdges(27, "-xyz");
    code.sweepFullVertex(27, sweepEdges, "-xyz", {"-xy", "-xz", "-yz"});
    EXPECT_TRUE(flipBits[68] ^ flipBits[31]);
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 68 || i == 31)
        {
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
    // -yz, -xy, -xz edges of vertex 27
    code.clearFlipBits();
    syndrome.assign(14 * l * l * l, 0);
    syndrome[501] = 1;
    syndrome[524] = 1;
    syndrome[604] = 1;
    code.setSyndrome(syndrome);
    sweepEdges = code.findSweepEdges(27, "-xyz");
    code.sweepFullVertex(27, sweepEdges, "-xyz", {"-xy", "-xz", "-yz"});
    EXPECT_TRUE((flipBits[31] && flipBits[18]) ^
                (flipBits[31] && flipBits[68]) ^
                (flipBits[18] && flipBits[68]));
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 18 || i == 31 || i == 68)
        {
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
}

TEST(sweepHalfVertex, handles_qubit_errors_minus_xy)
{
    int l = 6;
    double p = 0.1;
    RhombicCode code = RhombicCode(l, p, p, false, 1);

    // One error
    // xz yz face of vertex 309
    code.setError({407});
    code.calculateSyndrome();
    auto sweepEdges = code.findSweepEdges(309, "-xy");
    auto &flipBits = code.getFlipBits();
    code.sweepHalfVertex(309, sweepEdges, "-xy", {"-xyz", "xz", "yz"});
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 407)
        {
            EXPECT_EQ(flipBits[i], 1);
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
    // xz -xyz face of vertex 309
    code.calculateSyndrome();
    code.clearFlipBits();
    code.setError({277});
    code.calculateSyndrome();
    sweepEdges = code.findSweepEdges(309, "-xy");
    code.sweepHalfVertex(309, sweepEdges, "-xy", {"-xyz", "xz", "yz"});
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 277)
        {
            EXPECT_EQ(flipBits[i], 1);
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
    // yz -xyz face of vertex 309
    code.calculateSyndrome();
    code.clearFlipBits();
    code.setError({276});
    code.calculateSyndrome();
    sweepEdges = code.findSweepEdges(309, "-xy");
    code.sweepHalfVertex(309, sweepEdges, "-xy", {"-xyz", "xz", "yz"});
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 276)
        {
            EXPECT_EQ(flipBits[i], 1);
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }

    // Two errors
    // xz yz and xz -xyz faces of vertex 309
    code.calculateSyndrome();
    code.clearFlipBits();
    code.setError({407, 277});
    code.calculateSyndrome();
    sweepEdges = code.findSweepEdges(309, "-xy");
    code.sweepHalfVertex(309, sweepEdges, "-xy", {"-xyz", "xz", "yz"});
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 276)
        {
            EXPECT_EQ(flipBits[i], 1);
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
    // xz yz and yz -xyz faces of vertex 309
    code.calculateSyndrome();
    code.clearFlipBits();
    code.setError({407, 276});
    code.calculateSyndrome();
    sweepEdges = code.findSweepEdges(309, "-xy");
    code.sweepHalfVertex(309, sweepEdges, "-xy", {"-xyz", "xz", "yz"});
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 277)
        {
            EXPECT_EQ(flipBits[i], 1);
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
    // xz -xyz and yz -xyz faces of vertex 309
    code.calculateSyndrome();
    code.clearFlipBits();
    code.setError({277, 276});
    code.calculateSyndrome();
    sweepEdges = code.findSweepEdges(309, "-xy");
    code.sweepHalfVertex(309, sweepEdges, "-xy", {"-xyz", "xz", "yz"});
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 407)
        {
            EXPECT_EQ(flipBits[i], 1);
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
}

TEST(sweepHalfVertex, handles_qubit_errors_xz)
{
    int l = 6;
    double p = 0.1;
    RhombicCode code = RhombicCode(l, p, p, false, 1);

    // One error
    // xyx -yz face of vertex 283
    code.setError({204});
    code.calculateSyndrome();
    auto sweepEdges = code.findSweepEdges(283, "xz");
    auto &flipBits = code.getFlipBits();
    code.sweepHalfVertex(283, sweepEdges, "xz", {"xyz", "-xy", "-yz"});
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 204)
        {
            EXPECT_EQ(flipBits[i], 1);
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
    // xyz -xy face of vertex 283
    code.clearSyndrome();
    code.clearFlipBits();
    code.setError({308});
    code.calculateSyndrome();
    sweepEdges = code.findSweepEdges(283, "xz");
    code.sweepHalfVertex(283, sweepEdges, "xz", {"xyz", "-xy", "-yz"});
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 308)
        {
            EXPECT_EQ(flipBits[i], 1);
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
    // -yz -xy face of vertex 283
    code.clearSyndrome();
    code.clearFlipBits();
    code.setError({310});
    code.calculateSyndrome();
    sweepEdges = code.findSweepEdges(283, "xz");
    code.sweepHalfVertex(283, sweepEdges, "xz", {"xyz", "-xy", "-yz"});
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 310)
        {
            EXPECT_EQ(flipBits[i], 1);
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }

    // Two errors
    // xyz -xz, -xz -yz faces of vertex 283
    code.clearSyndrome();
    code.clearFlipBits();
    code.setError({109, 113});
    code.calculateSyndrome();
    sweepEdges = code.findSweepEdges(283, "xz");
    code.sweepHalfVertex(283, sweepEdges, "xz", {"xyz", "-xy", "-yz"});
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 204)
        {
            EXPECT_EQ(flipBits[i], 1);
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
    // xyz -yz, xyz -xy faces of vertex 283
    code.clearSyndrome();
    code.clearFlipBits();
    code.setError({204, 308});
    code.calculateSyndrome();
    sweepEdges = code.findSweepEdges(283, "xz");
    code.sweepHalfVertex(283, sweepEdges, "xz", {"xyz", "-xy", "-yz"});
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 310)
        {
            EXPECT_EQ(flipBits[i], 1);
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
    // xyz -yz, -xy -yz faces of vertex 283
    code.clearSyndrome();
    code.clearFlipBits();
    code.setError({204, 310});
    code.calculateSyndrome();
    sweepEdges = code.findSweepEdges(283, "xz");
    code.sweepHalfVertex(283, sweepEdges, "xz", {"xyz", "-xy", "-yz"});
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 308)
        {
            EXPECT_EQ(flipBits[i], 1);
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
}

TEST(sweepHalfVertex, handles_qubit_errors_minus_xz)
{
    int l = 6;
    double p = 0.1;
    RhombicCode code = RhombicCode(l, p, p, false, 1);

    // One error
    // xy yz face of vertex 309
    code.setError({406});
    code.calculateSyndrome();
    auto sweepEdges = code.findSweepEdges(309, "-xz");
    auto &flipBits = code.getFlipBits();
    code.sweepHalfVertex(309, sweepEdges, "-xz", {"-xyz", "xy", "yz"});
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 406)
        {
            EXPECT_EQ(flipBits[i], 1);
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
    // xy -xyz face of vertex 309
    code.calculateSyndrome();
    code.clearFlipBits();
    code.setError({278});
    code.calculateSyndrome();
    sweepEdges = code.findSweepEdges(309, "-xz");
    code.sweepHalfVertex(309, sweepEdges, "-xz", {"-xyz", "xy", "yz"});
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 278)
        {
            EXPECT_EQ(flipBits[i], 1);
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
    // yz -xyz face of vertex 309
    code.calculateSyndrome();
    code.clearFlipBits();
    code.setError({276});
    code.calculateSyndrome();
    sweepEdges = code.findSweepEdges(309, "-xz");
    code.sweepHalfVertex(309, sweepEdges, "-xz", {"-xyz", "xy", "yz"});
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 276)
        {
            EXPECT_EQ(flipBits[i], 1);
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }

    // Two errors
    // xy yz and xy -xyz faces of vertex 309
    code.calculateSyndrome();
    code.clearFlipBits();
    code.setError({406, 278});
    code.calculateSyndrome();
    sweepEdges = code.findSweepEdges(309, "-xz");
    code.sweepHalfVertex(309, sweepEdges, "-xz", {"-xyz", "xy", "yz"});
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 276)
        {
            EXPECT_EQ(flipBits[i], 1);
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
    // xy yz and yz -xyz faces of vertex 309
    code.calculateSyndrome();
    code.clearFlipBits();
    code.setError({406, 276});
    code.calculateSyndrome();
    sweepEdges = code.findSweepEdges(309, "-xz");
    code.sweepHalfVertex(309, sweepEdges, "-xz", {"-xyz", "xy", "yz"});
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 278)
        {
            EXPECT_EQ(flipBits[i], 1);
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
    // xy -xyz and yz -xyz faces of vertex 309
    code.calculateSyndrome();
    code.clearFlipBits();
    code.setError({278, 276});
    code.calculateSyndrome();
    sweepEdges = code.findSweepEdges(309, "-xz");
    code.sweepHalfVertex(309, sweepEdges, "-xz", {"-xyz", "xy", "yz"});
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 406)
        {
            EXPECT_EQ(flipBits[i], 1);
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
}

TEST(sweepHalfVertex, handles_qubit_errors_yz)
{
    int l = 6;
    double p = 0.1;
    RhombicCode code = RhombicCode(l, p, p, false, 1);

    // One error
    // xyx -xz face of vertex 283
    code.setError({109});
    code.calculateSyndrome();
    auto sweepEdges = code.findSweepEdges(283, "yz");
    auto &flipBits = code.getFlipBits();
    code.sweepHalfVertex(283, sweepEdges, "yz", {"xyz", "-xy", "-xz"});
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 109)
        {
            EXPECT_EQ(flipBits[i], 1);
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
    // xyz -xy face of vertex 283
    code.clearSyndrome();
    code.clearFlipBits();
    code.setError({308});
    code.calculateSyndrome();
    sweepEdges = code.findSweepEdges(283, "yz");
    code.sweepHalfVertex(283, sweepEdges, "yz", {"xyz", "-xy", "-xz"});
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 308)
        {
            EXPECT_EQ(flipBits[i], 1);
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
    // -xz -xy face of vertex 283
    code.clearSyndrome();
    code.clearFlipBits();
    code.setError({309});
    code.calculateSyndrome();
    sweepEdges = code.findSweepEdges(283, "yz");
    code.sweepHalfVertex(283, sweepEdges, "yz", {"xyz", "-xy", "-xz"});
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 309)
        {
            EXPECT_EQ(flipBits[i], 1);
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }

    // Two errors
    // xyz -yz, -xz -yz faces of vertex 283
    code.clearSyndrome();
    code.clearFlipBits();
    code.setError({113, 204});
    code.calculateSyndrome();
    sweepEdges = code.findSweepEdges(283, "yz");
    code.sweepHalfVertex(283, sweepEdges, "yz", {"xyz", "-xy", "-xz"});
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 109)
        {
            EXPECT_EQ(flipBits[i], 1);
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
    // -xy xyz and xyz -xz faces of vertex 283
    code.clearSyndrome();
    code.clearFlipBits();
    code.setError({308, 109});
    code.calculateSyndrome();
    sweepEdges = code.findSweepEdges(283, "yz");
    code.sweepHalfVertex(283, sweepEdges, "yz", {"xyz", "-xy", "-xz"});
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 309)
        {
            EXPECT_EQ(flipBits[i], 1);
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
    // xyz -xz and -xz -xy faces of vertex 283
    code.clearSyndrome();
    code.clearFlipBits();
    code.setError({109, 309});
    code.calculateSyndrome();
    sweepEdges = code.findSweepEdges(283, "yz");
    code.sweepHalfVertex(283, sweepEdges, "yz", {"xyz", "-xy", "-xz"});
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 308)
        {
            EXPECT_EQ(flipBits[i], 1);
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
}

TEST(sweepHalfVertex, handles_qubit_errors_minus_yz)
{
    int l = 6;
    double p = 0.1;
    RhombicCode code = RhombicCode(l, p, p, false, 1);

    // One error
    // xy xz face of vertex 309
    code.setError({393});
    code.calculateSyndrome();
    auto sweepEdges = code.findSweepEdges(309, "-yz");
    auto &flipBits = code.getFlipBits();
    code.sweepHalfVertex(309, sweepEdges, "-yz", {"-xyz", "xy", "xz"});
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 393)
        {
            EXPECT_EQ(flipBits[i], 1);
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
    // xy -xyz face of vertex 309
    code.calculateSyndrome();
    code.clearFlipBits();
    code.setError({278});
    code.calculateSyndrome();
    sweepEdges = code.findSweepEdges(309, "-yz");
    code.sweepHalfVertex(309, sweepEdges, "-yz", {"-xyz", "xy", "xz"});
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 278)
        {
            EXPECT_EQ(flipBits[i], 1);
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
    // xz -xyz face of vertex 309
    code.calculateSyndrome();
    code.clearFlipBits();
    code.setError({277});
    code.calculateSyndrome();
    sweepEdges = code.findSweepEdges(309, "-yz");
    code.sweepHalfVertex(309, sweepEdges, "-yz", {"-xyz", "xy", "xz"});
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 277)
        {
            EXPECT_EQ(flipBits[i], 1);
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }

    // Two errors
    // xy xz and xy -xyz faces of vertex 309
    code.calculateSyndrome();
    code.clearFlipBits();
    code.setError({393, 278});
    code.calculateSyndrome();
    sweepEdges = code.findSweepEdges(309, "-yz");
    code.sweepHalfVertex(309, sweepEdges, "-yz", {"-xyz", "xy", "xz"});
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 277)
        {
            EXPECT_EQ(flipBits[i], 1);
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
    // xy yz and xz -xyz faces of vertex 309
    code.calculateSyndrome();
    code.clearFlipBits();
    code.setError({393, 277});
    code.calculateSyndrome();
    sweepEdges = code.findSweepEdges(309, "-yz");
    code.sweepHalfVertex(309, sweepEdges, "-yz", {"-xyz", "xy", "xz"});
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 278)
        {
            EXPECT_EQ(flipBits[i], 1);
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
    // xy -xyz and xz -xyz faces of vertex 309
    code.calculateSyndrome();
    code.clearFlipBits();
    code.setError({278, 277});
    code.calculateSyndrome();
    sweepEdges = code.findSweepEdges(309, "-yz");
    code.sweepHalfVertex(309, sweepEdges, "-yz", {"-xyz", "xy", "xz"});
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 393)
        {
            EXPECT_EQ(flipBits[i], 1);
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
}

TEST(sweepHalfVertex, handles_qubit_errors_xyz)
{
    int l = 6;
    double p = 0.1;
    RhombicCode code = RhombicCode(l, p, p, false, 1);

    // One error
    // xy xz face of vertex 309
    code.setError({393});
    code.calculateSyndrome();
    auto sweepEdges = code.findSweepEdges(309, "xyz");
    auto &flipBits = code.getFlipBits();
    code.sweepHalfVertex(309, sweepEdges, "xyz", {"xy", "xz", "yz"});
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 393)
        {
            EXPECT_EQ(flipBits[i], 1);
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
    // xy yz face of vertex 309
    code.calculateSyndrome();
    code.clearFlipBits();
    code.setError({406});
    code.calculateSyndrome();
    sweepEdges = code.findSweepEdges(309, "xyz");
    code.sweepHalfVertex(309, sweepEdges, "xyz", {"xy", "xz", "yz"});
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 406)
        {
            EXPECT_EQ(flipBits[i], 1);
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
    // xz yz face of vertex 309
    code.calculateSyndrome();
    code.clearFlipBits();
    code.setError({407});
    code.calculateSyndrome();
    sweepEdges = code.findSweepEdges(309, "xyz");
    code.sweepHalfVertex(309, sweepEdges, "xyz", {"xy", "xz", "yz"});
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 407)
        {
            EXPECT_EQ(flipBits[i], 1);
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }

    // Two errors
    // xy xz and xy yz faces of vertex 309
    code.calculateSyndrome();
    code.clearFlipBits();
    code.setError({393, 406});
    code.calculateSyndrome();
    sweepEdges = code.findSweepEdges(309, "xyz");
    code.sweepHalfVertex(309, sweepEdges, "xyz", {"xy", "xz", "yz"});
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 407)
        {
            EXPECT_EQ(flipBits[i], 1);
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
    // xy xz and xz yz faces of vertex 309
    code.calculateSyndrome();
    code.clearFlipBits();
    code.setError({393, 407});
    code.calculateSyndrome();
    sweepEdges = code.findSweepEdges(309, "xyz");
    code.sweepHalfVertex(309, sweepEdges, "xyz", {"xy", "xz", "yz"});
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 406)
        {
            EXPECT_EQ(flipBits[i], 1);
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
    // xz yz and xy yz faces of vertex 309
    code.calculateSyndrome();
    code.clearFlipBits();
    code.setError({406, 407});
    code.calculateSyndrome();
    sweepEdges = code.findSweepEdges(309, "xyz");
    code.sweepHalfVertex(309, sweepEdges, "xyz", {"xy", "xz", "yz"});
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 393)
        {
            EXPECT_EQ(flipBits[i], 1);
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
}

TEST(sweepHalfVertex, handles_qubit_errors_minus_xyz)
{
    int l = 6;
    double p = 0.1;
    RhombicCode code = RhombicCode(l, p, p, false, 1);

    // One error
    // -xz -yz face of vertex 283
    code.setError({113});
    code.calculateSyndrome();
    auto sweepEdges = code.findSweepEdges(283, "-xyz");
    auto &flipBits = code.getFlipBits();
    code.sweepHalfVertex(283, sweepEdges, "-xyz", {"-xy", "-yz", "-xz"});
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 113)
        {
            EXPECT_EQ(flipBits[i], 1);
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
    // -xz -xy face of vertex 283
    code.clearSyndrome();
    code.clearFlipBits();
    code.setError({309});
    code.calculateSyndrome();
    sweepEdges = code.findSweepEdges(283, "-xyz");
    code.sweepHalfVertex(283, sweepEdges, "-xyz", {"-xy", "-yz", "-xz"});
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 309)
        {
            EXPECT_EQ(flipBits[i], 1);
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
    // -yz -xy face of vertex 283
    code.clearSyndrome();
    code.clearFlipBits();
    code.setError({310});
    code.calculateSyndrome();
    sweepEdges = code.findSweepEdges(283, "-xyz");
    code.sweepHalfVertex(283, sweepEdges, "-xyz", {"-xy", "-yz", "-xz"});
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 310)
        {
            EXPECT_EQ(flipBits[i], 1);
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }

    // Two errors
    // xyz -xz, xyz -yz faces of vertex 283
    code.clearSyndrome();
    code.clearFlipBits();
    code.setError({109, 204});
    code.calculateSyndrome();
    sweepEdges = code.findSweepEdges(283, "-xyz");
    code.sweepHalfVertex(283, sweepEdges, "-xyz", {"-xy", "-yz", "-xz"});
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 113)
        {
            EXPECT_EQ(flipBits[i], 1);
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
    // -xz -yz and -yz -xy faces of vertex 283
    code.clearSyndrome();
    code.clearFlipBits();
    code.setError({113, 310});
    code.calculateSyndrome();
    sweepEdges = code.findSweepEdges(283, "-xyz");
    code.sweepHalfVertex(283, sweepEdges, "-xyz", {"-xy", "-yz", "-xz"});
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 309)
        {
            EXPECT_EQ(flipBits[i], 1);
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
    // -yz -xy and -xy -xz faces of vertex 283
    code.clearSyndrome();
    code.clearFlipBits();
    code.setError({113, 309});
    code.calculateSyndrome();
    sweepEdges = code.findSweepEdges(283, "-xyz");
    code.sweepHalfVertex(283, sweepEdges, "-xyz", {"-xy", "-yz", "-xz"});
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 310)
        {
            EXPECT_EQ(flipBits[i], 1);
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
}

TEST(sweepHalfVertex, handles_measurement_errors_xz)
{
    int l = 6;
    double p = 0.1;
    RhombicCode code = RhombicCode(l, p, p, false, 1);

    // Three sweep edges at vertex 283
    // xyz, -yz and -xy edges
    std::vector<int8_t> syndrome;
    syndrome.assign(14 * l * l * l, 0);
    syndrome[1981] = 1;
    syndrome[480] = 1;
    syndrome[723] = 1;
    code.setSyndrome(syndrome);
    auto &flipBits = code.getFlipBits();
    auto sweepEdges = code.findSweepEdges(283, "xz");
    code.sweepHalfVertex(283, sweepEdges, "xz", {"xyz", "-xy", "-yz"});
    EXPECT_TRUE(flipBits[310] ^ flipBits[308] ^ flipBits[204]);
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 204 || i == 308 || i == 310)
        {
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
}

TEST(sweepHalfVertex, handles_measurement_errors_yz)
{
    int l = 6;
    double p = 0.1;
    RhombicCode code = RhombicCode(l, p, p, false, 1);

    // Three sweep edges at vertex 283
    // xyz, -xy and -xz edges
    std::vector<int8_t> syndrome;
    syndrome.assign(14 * l * l * l, 0);
    syndrome[1981] = 1;
    syndrome[723] = 1;
    syndrome[265] = 1;
    code.setSyndrome(syndrome);
    auto &flipBits = code.getFlipBits();
    auto sweepEdges = code.findSweepEdges(283, "yz");
    code.sweepHalfVertex(283, sweepEdges, "yz", {"xyz", "-xy", "-xz"});
    EXPECT_TRUE(flipBits[109] ^ flipBits[308] ^ flipBits[309]);
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 109 || i == 309 || i == 308)
        {
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
}

TEST(sweepHalfVertex, handles_measurement_errors_minus_xyz)
{
    int l = 6;
    double p = 0.1;
    RhombicCode code = RhombicCode(l, p, p, false, 1);

    // Three sweep edges at vertex 283
    // -xy, -yz and -xz edges
    std::vector<int8_t> syndrome;
    syndrome.assign(14 * l * l * l, 0);
    syndrome[723] = 1;
    syndrome[480] = 1;
    syndrome[265] = 1;
    code.setSyndrome(syndrome);
    auto &flipBits = code.getFlipBits();
    auto sweepEdges = code.findSweepEdges(283, "-xyz");
    code.sweepHalfVertex(283, sweepEdges, "-xyz", {"-xy", "-yz", "-xz"});
    EXPECT_TRUE(flipBits[113] ^ flipBits[309] ^ flipBits[310]);
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 113 || i == 309 || i == 310)
        {
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
}

TEST(sweepHalfVertex, handles_measurement_errors_minus_xy)
{
    int l = 6;
    double p = 0.1;
    RhombicCode code = RhombicCode(l, p, p, false, 1);

    // Three sweep edges at vertex 309
    // -xyz, yz and xz edges
    std::vector<int8_t> syndrome;
    syndrome.assign(14 * l * l * l, 0);
    syndrome[651] = 1;
    syndrome[2167] = 1;
    syndrome[2169] = 1;
    code.setSyndrome(syndrome);
    auto &flipBits = code.getFlipBits();
    auto sweepEdges = code.findSweepEdges(309, "-xy");
    code.sweepHalfVertex(309, sweepEdges, "-xy", {"-xyz", "yz", "xz"});
    EXPECT_TRUE(flipBits[276] ^ flipBits[277] ^ flipBits[407]);
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 276 || i == 277 || i == 407)
        {
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
}

TEST(sweepHalfVertex, handles_measurement_errors_minus_xz)
{
    int l = 6;
    double p = 0.1;
    RhombicCode code = RhombicCode(l, p, p, false, 1);

    // Three sweep edges at vertex 309
    // -xyz, yz and xz edges
    std::vector<int8_t> syndrome;
    syndrome.assign(14 * l * l * l, 0);
    syndrome[651] = 1;
    syndrome[2167] = 1;
    syndrome[2165] = 1;
    code.setSyndrome(syndrome);
    auto &flipBits = code.getFlipBits();
    auto sweepEdges = code.findSweepEdges(309, "-xz");
    code.sweepHalfVertex(309, sweepEdges, "-xz", {"-xyz", "yz", "xy"});
    EXPECT_TRUE(flipBits[276] ^ flipBits[278] ^ flipBits[406]);
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 276 || i == 278 || i == 406)
        {
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
}

TEST(sweepHalfVertex, handles_measurement_errors_minus_yz)
{
    int l = 6;
    double p = 0.1;
    RhombicCode code = RhombicCode(l, p, p, false, 1);

    // Three sweep edges at vertex 309
    // -xyz, yz and xz edges
    std::vector<int8_t> syndrome;
    syndrome.assign(14 * l * l * l, 0);
    syndrome[651] = 1;
    syndrome[2169] = 1;
    syndrome[2165] = 1;
    code.setSyndrome(syndrome);
    auto &flipBits = code.getFlipBits();
    auto sweepEdges = code.findSweepEdges(309, "-yz");
    code.sweepHalfVertex(309, sweepEdges, "-yz", {"-xyz", "xy", "xz"});
    EXPECT_TRUE(flipBits[277] ^ flipBits[278] ^ flipBits[393]);
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 277 || i == 278 || i == 393)
        {
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
}

TEST(sweepHalfVertex, handles_measurement_errors_xyz)
{
    int l = 6;
    double p = 0.1;
    RhombicCode code = RhombicCode(l, p, p, false, 1);

    // Three sweep edges at vertex 309
    // -xyz, yz and xz edges
    std::vector<int8_t> syndrome;
    syndrome.assign(14 * l * l * l, 0);
    syndrome[2167] = 1;
    syndrome[2169] = 1;
    syndrome[2165] = 1;
    code.setSyndrome(syndrome);
    auto &flipBits = code.getFlipBits();
    auto sweepEdges = code.findSweepEdges(309, "xyz");
    code.sweepHalfVertex(309, sweepEdges, "xyz", {"xy", "yz", "xz"});
    EXPECT_TRUE(flipBits[406] ^ flipBits[407] ^ flipBits[393]);
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 406 || i == 407 || i == 393)
        {
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
}

TEST(sweep, handles_qubit_errors_xyz)
{
    int l = 4;
    double p = 0.1;
    RhombicCode code = RhombicCode(l, p, p, false, 1);

    code.setError({44, 45, 151});
    code.calculateSyndrome();
    auto &syndrome = code.getSyndrome();
    auto &flipBits = code.getFlipBits();
    auto &error = code.getError();
    code.sweep("xyz", true);
    code.calculateSyndrome();
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 45)
        {
            EXPECT_EQ(flipBits[i], 1);
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
    EXPECT_TRUE(error.find(45) == error.end());
    EXPECT_TRUE(error.find(44) != error.end());
    EXPECT_TRUE(error.find(151) != error.end());
    for (int i = 0; i < syndrome.size(); ++i)
    {
        if (i == 811 || i == 357 || i == 363 || i == 107 || i == 105 || i == 555)
        {
            EXPECT_EQ(syndrome[i], 1);
        }
        else
        {
            EXPECT_EQ(syndrome[i], 0);
        }
    }
    code.sweep("xyz", true);
    code.calculateSyndrome();
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 44 || i == 151)
        {
            EXPECT_EQ(flipBits[i], 1);
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
    EXPECT_TRUE(error.find(44) == error.end());
    EXPECT_TRUE(error.find(151) == error.end());
    for (int i = 0; i < syndrome.size(); ++i)
    {
        EXPECT_EQ(syndrome[i], 0);
    }
}

TEST(sweep, handles_qubit_errors_minus_xyz)
{
    int l = 4;
    double p = 0.1;
    RhombicCode code = RhombicCode(l, p, p, false, 1);

    code.setError({44, 45, 151});
    code.calculateSyndrome();
    auto &syndrome = code.getSyndrome();
    auto &flipBits = code.getFlipBits();
    auto &error = code.getError();
    code.sweep("-xyz", true);
    code.calculateSyndrome();
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 44 || i == 151)
        {
            EXPECT_EQ(flipBits[i], 1);
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
    EXPECT_TRUE(error.find(45) != error.end());
    EXPECT_TRUE(error.find(44) == error.end());
    EXPECT_TRUE(error.find(151) == error.end());
    for (int i = 0; i < syndrome.size(); ++i)
    {
        if (i == 363 || i == 107 || i == 884 || i == 888)
        {
            EXPECT_EQ(syndrome[i], 1);
        }
        else
        {
            EXPECT_EQ(syndrome[i], 0);
        }
    }
    code.sweep("-xyz", true);
    code.calculateSyndrome();
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 45)
        {
            EXPECT_EQ(flipBits[i], 1);
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
    EXPECT_TRUE(error.find(45) == error.end());
    for (int i = 0; i < syndrome.size(); ++i)
    {
        EXPECT_EQ(syndrome[i], 0);
    }
}

TEST(sweep, handles_qubit_errors_xy)
{
    int l = 4;
    double p = 0.1;
    RhombicCode code = RhombicCode(l, p, p, false, 1);

    code.setError({44, 45, 151});
    code.calculateSyndrome();
    auto &syndrome = code.getSyndrome();
    auto &flipBits = code.getFlipBits();
    auto &error = code.getError();
    code.sweep("xy", true);
    code.calculateSyndrome();
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 44 || i == 45)
        {
            EXPECT_EQ(flipBits[i], 1);
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
    EXPECT_TRUE(error.find(45) == error.end());
    EXPECT_TRUE(error.find(44) == error.end());
    EXPECT_TRUE(error.find(151) != error.end());
    for (int i = 0; i < syndrome.size(); ++i)
    {
        if (i == 357 || i == 811 || i == 889 || i == 363)
        {
            EXPECT_EQ(syndrome[i], 1);
        }
        else
        {
            EXPECT_EQ(syndrome[i], 0);
        }
    }
    code.sweep("xy", true);
    code.calculateSyndrome();
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 151)
        {
            EXPECT_EQ(flipBits[i], 1);
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
    EXPECT_TRUE(error.find(151) == error.end());
    for (int i = 0; i < syndrome.size(); ++i)
    {
        EXPECT_EQ(syndrome[i], 0);
    }
}

TEST(sweep, handles_qubit_errors_minus_xy)
{
    int l = 4;
    double p = 0.1;
    RhombicCode code = RhombicCode(l, p, p, false, 1);

    code.setError({44, 45, 151});
    code.calculateSyndrome();
    auto &syndrome = code.getSyndrome();
    auto &flipBits = code.getFlipBits();
    auto &error = code.getError();
    code.sweep("-xy", true);
    code.calculateSyndrome();
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 151)
        {
            EXPECT_EQ(flipBits[i], 1);
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
    EXPECT_TRUE(error.find(45) != error.end());
    EXPECT_TRUE(error.find(44) != error.end());
    EXPECT_TRUE(error.find(151) == error.end());
    for (int i = 0; i < syndrome.size(); ++i)
    {
        if (i == 363 || i == 889 || i == 884 || i == 888 || i == 105 || i == 555)
        {
            EXPECT_EQ(syndrome[i], 1);
        }
        else
        {
            EXPECT_EQ(syndrome[i], 0);
        }
    }
    code.sweep("-xy", true);
    code.calculateSyndrome();
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 45 || i == 44)
        {
            EXPECT_EQ(flipBits[i], 1);
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
    EXPECT_TRUE(error.find(45) == error.end());
    EXPECT_TRUE(error.find(44) == error.end());
    for (int i = 0; i < syndrome.size(); ++i)
    {
        EXPECT_EQ(syndrome[i], 0);
    }
}

TEST(sweep, handles_qubit_errors_xz)
{
    int l = 6;
    double p = 0.1;
    RhombicCode code = RhombicCode(l, p, p, false, 1);

    code.setError({259, 478, 350});
    code.calculateSyndrome();
    auto &syndrome = code.getSyndrome();
    auto &flipBits = code.getFlipBits();
    auto &error = code.getError();
    code.sweep("xz", true);
    code.calculateSyndrome();
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 259 || i == 478)
        {
            EXPECT_EQ(flipBits[i], 1);
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
    EXPECT_TRUE(error.find(350) != error.end());
    EXPECT_TRUE(error.find(259) == error.end());
    EXPECT_TRUE(error.find(478) == error.end());
    for (int i = 0; i < syndrome.size(); ++i)
    {
        if (i == 812 || i == 814 || i == 2326 || i == 2072)
        {
            EXPECT_EQ(syndrome[i], 1);
        }
        else
        {
            EXPECT_EQ(syndrome[i], 0);
        }
    }
    code.sweep("xz", true);
    code.calculateSyndrome();
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 350)
        {
            EXPECT_EQ(flipBits[i], 1);
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
    EXPECT_TRUE(error.find(350) == error.end());
    for (int i = 0; i < syndrome.size(); ++i)
    {
        EXPECT_EQ(syndrome[i], 0);
    }
}

TEST(sweep, handles_qubit_errors_minus_xz)
{
    int l = 6;
    double p = 0.1;
    RhombicCode code = RhombicCode(l, p, p, false, 1);

    code.setError({259, 478, 350});
    code.calculateSyndrome();
    auto &syndrome = code.getSyndrome();
    auto &flipBits = code.getFlipBits();
    auto &error = code.getError();
    code.sweep("-xz", true);
    code.calculateSyndrome();
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 348 || i == 389)
        {
            EXPECT_EQ(flipBits[i], 1);
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
    EXPECT_TRUE(error.find(350) != error.end());
    EXPECT_TRUE(error.find(259) != error.end());
    EXPECT_TRUE(error.find(478) != error.end());
    EXPECT_TRUE(error.find(348) != error.end());
    EXPECT_TRUE(error.find(389) != error.end());
    for (int i = 0; i < syndrome.size(); ++i)
    {
        if (i == 816 || i == 814 || i == 602 || i == 608 ||
            i == 2118 || i == 902 || i == 1108 || i == 2317)
        {
            EXPECT_EQ(syndrome[i], 1);
        }
        else
        {
            EXPECT_EQ(syndrome[i], 0);
        }
    }
    // Sweeping gets too complex to work out on paper past this point
    // After three sweeps (total) the configuration is mapped to a stabiliser
    code.sweep("-xz", true);
    code.calculateSyndrome();
    code.sweep("-xz", true);
    code.calculateSyndrome();
    for (int i = 0; i < syndrome.size(); ++i)
    {
        EXPECT_EQ(syndrome[i], 0);
    }
}

TEST(sweep, handles_qubit_errors_yz)
{
    int l = 6;
    double p = 0.1;
    RhombicCode code = RhombicCode(l, p, p, false, 1);

    code.setError({259, 478, 350});
    code.calculateSyndrome();
    auto &syndrome = code.getSyndrome();
    auto &flipBits = code.getFlipBits();
    auto &error = code.getError();
    code.sweep("yz", true);
    code.calculateSyndrome();
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 351 || i == 389)
        {
            EXPECT_EQ(flipBits[i], 1);
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
    EXPECT_TRUE(error.find(350) != error.end());
    EXPECT_TRUE(error.find(259) != error.end());
    EXPECT_TRUE(error.find(478) != error.end());
    EXPECT_TRUE(error.find(351) != error.end());
    EXPECT_TRUE(error.find(389) != error.end());
    for (int i = 0; i < syndrome.size(); ++i)
    {
        if (i == 812 || i == 2071 || i == 602 || i == 2067 ||
            i == 2118 || i == 902 || i == 1108 || i == 2328)
        {
            EXPECT_EQ(syndrome[i], 1);
        }
        else
        {
            EXPECT_EQ(syndrome[i], 0);
        }
    }
    // Sweeping gets too complex to work out on paper past this point
    // After three sweeps (total) the configuration is mapped to a stabiliser
    // Guessed this by symmetry with above test!
    code.sweep("yz", true);
    code.calculateSyndrome();
    code.sweep("yz", true);
    code.calculateSyndrome();
    for (int i = 0; i < syndrome.size(); ++i)
    {
        EXPECT_EQ(syndrome[i], 0);
    }
}

TEST(sweep, handles_qubit_errors_minus_yz)
{
    // Same as XZ test by symmetry
    int l = 6;
    double p = 0.1;
    RhombicCode code = RhombicCode(l, p, p, false, 1);

    code.setError({259, 478, 350});
    code.calculateSyndrome();
    auto &syndrome = code.getSyndrome();
    auto &flipBits = code.getFlipBits();
    auto &error = code.getError();
    code.sweep("-yz", true);
    code.calculateSyndrome();
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 259 || i == 478)
        {
            EXPECT_EQ(flipBits[i], 1);
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
    EXPECT_TRUE(error.find(350) != error.end());
    EXPECT_TRUE(error.find(259) == error.end());
    EXPECT_TRUE(error.find(478) == error.end());
    for (int i = 0; i < syndrome.size(); ++i)
    {
        if (i == 812 || i == 814 || i == 2326 || i == 2072)
        {
            EXPECT_EQ(syndrome[i], 1);
        }
        else
        {
            EXPECT_EQ(syndrome[i], 0);
        }
    }
    code.sweep("-yz", true);
    code.calculateSyndrome();
    for (int i = 0; i < flipBits.size(); ++i)
    {
        if (i == 350)
        {
            EXPECT_EQ(flipBits[i], 1);
        }
        else
        {
            EXPECT_EQ(flipBits[i], 0);
        }
    }
    EXPECT_TRUE(error.find(350) == error.end());
    for (int i = 0; i < syndrome.size(); ++i)
    {
        EXPECT_EQ(syndrome[i], 0);
    }
}

TEST(buildLogical, correct_steps_lattice_size_4)
{
    int l = 4;
    double p = 0.1;
    RhombicCode code = RhombicCode(l, p, p, false, 1);
    auto logicals = code.getLogicals();
    vint expectedZ1 = {151, 4, 145, 10};
    EXPECT_EQ(logicals.at(0), expectedZ1);
    vint expectedZ2 = {180, 3, 156, 27};
    EXPECT_EQ(logicals.at(1), expectedZ2);
    vint expectedZ3 = {151, 58, 55, 154};
    EXPECT_EQ(logicals.at(2), expectedZ3);
}

TEST(checkCorrection, handles_stabilizers)
{
    int l = 4;
    double p = 0.1;
    RhombicCode code = RhombicCode(l, p, p, false, 1);

    // No error
    EXPECT_TRUE(code.checkCorrection());
    // Stabiliser error
    code.setError({0, 2, 3, 19, 20, 22, 23, 29, 63, 64, 156, 157});
    EXPECT_TRUE(code.checkCorrection());
}

TEST(checkCorrection, handles_logical_x_operators)
{
    int l = 4;
    double p = 0.1;
    RhombicCode code = RhombicCode(l, p, p, false, 1);

    std::set<int> logicalX3 = {0, 1, 58, 87,
                               24, 25, 82, 63,
                               6, 7, 52, 93,
                               12, 13, 64, 51,
                               30, 31, 76, 69,
                               36, 37, 88, 75,
                               18, 19, 70, 57,
                               42, 43, 94, 81};
    code.setError(logicalX3);
    EXPECT_FALSE(code.checkCorrection());
    std::set<int> logicalX2 = {0, 2, 3, 23,
                               6, 8, 9, 17,
                               96, 98, 99, 119,
                               48, 50, 51, 65,
                               54, 56, 57, 71,
                               144, 146, 147, 161,
                               102, 104, 105, 113,
                               150, 152, 153, 167};
    code.setError(logicalX2);
    EXPECT_FALSE(code.checkCorrection());
    std::set<int> logicalX1 = {1, 2, 4, 5,
                               25, 26, 28, 29,
                               97, 98, 100, 101,
                               61, 62, 64, 65,
                               121, 122, 124, 125,
                               85, 86, 88, 89,
                               157, 158, 160, 161,
                               181, 182, 184, 185};
    code.setError(logicalX1);
    EXPECT_FALSE(code.checkCorrection());
}

TEST(sweep, multiple_sweeps_per_syndrome)
{
    // No test for rhombic boundaries because there is some randomness in the definition of the rule on the boundaries
    int l = 4;
    double p = 0.1;
    int testRate = 2;
    RhombicCode highRateCode = RhombicCode(l, p, p, false, testRate);
    RhombicCode lowRateCode = RhombicCode(l, p, p, false, 1);
    std::vector<std::set<int>> testErrors = {{22}, {1}, {17}, {41, 42, 43}, {50, 27, 47, 11, 1}, {20, 10}, {11, 15, 6, 2, 22, 45, 40, 21, 0, 3, 33, 5}};
    for (auto &error : testErrors)
    {
        highRateCode.setError(error);
        lowRateCode.setError(error);
        highRateCode.calculateSyndrome();
        lowRateCode.calculateSyndrome();

        for (int i = 0; i < testRate; ++i)
        {
            highRateCode.sweep("xz", false);
        }
        auto highRateSyndrome = highRateCode.getSyndrome();

        for (int i = 0; i < testRate; ++i)
        {
            lowRateCode.sweep("xz", false);
            lowRateCode.calculateSyndrome();
        }
        auto lowRateSyndrome = lowRateCode.getSyndrome();

        auto &finalErrorLowRate = lowRateCode.getError();
        auto &finalErrorHighRate = highRateCode.getError();
        EXPECT_EQ(finalErrorLowRate, finalErrorHighRate);
        EXPECT_EQ(lowRateSyndrome, highRateSyndrome);
    }
}