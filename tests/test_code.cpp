#include "code.h"
#include "gtest/gtest.h"
#include <string>
#include <iostream>

TEST(Code, ExceptsInvalidProbabilities)
{
    int latticeLength = 4;
    std::string latticeType = "rhombic";
    std::vector<std::pair<double, double>> errorProbabilities = {{2, 0.1}, {-2, 0.2}, {0.5, 3}, {0.8, -1}};
    for (const auto &errorPair : errorProbabilities)
    {
        EXPECT_THROW(Code code(latticeLength, latticeType, errorPair.first, errorPair.second), std::invalid_argument);
    }
}

TEST(Code, SyndromeCorrectSize)
{
    std::vector<int> latticeLengths = {4, 6, 8, 10};
    std::string latticeType = "rhombic";
    for (const int l : latticeLengths)
    {
        Code code(l, latticeType, 0.1, 0.1);
        vint syndrome = code.getSyndrome();
        EXPECT_EQ(syndrome.size(), 2 * 7 * l * l * l);
    }
}

TEST(calculateSyndrome, correctlyCalculates)
{
    int latticeLength = 6;
    std::string latticeType = "rhombic";
    double p = 0.1;
    double q = p;
    Code code = Code(latticeLength, latticeType, p, q);
    vint error = {0, 1};
    code.setError(error);
    code.calculateSyndrome();
    vint syndrome = code.getSyndrome();
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

TEST(calculateSyndrome, correctlyCalculatesStabiliserError)
{
    int latticeLength = 4;
    std::string latticeType = "rhombic";
    double p = 0.1;
    double q = p;
    Code code = Code(latticeLength, latticeType, p, q);
    code.setError({0, 2, 3, 19, 20, 22, 23, 29, 63, 64, 156, 157});
    code.calculateSyndrome();
    vint syndrome = code.getSyndrome();
    for (const int value : syndrome)
    {
        EXPECT_EQ(value, 0);
    }
}

TEST(generateDataError, handlesOneErrorProbability)
{
    int l = 4;
    std::string latticeType = "rhombic";
    double p = 1;
    Code code = Code(l, latticeType, p, p);
    code.generateDataError();
    EXPECT_EQ(code.getError().size(), 3 * l * l * l);
}

TEST(generateDataError, handlesZeroErrorProbability)
{
    int l = 6;
    std::string latticeType = "rhombic";
    double p = 0;
    Code code = Code(l, latticeType, p, p);
    code.generateDataError();
    EXPECT_EQ(code.getError().size(), 0);
}

TEST(generateMeasError, handlesOneErrorProbability)
{
    int l = 6;
    std::string latticeType = "rhombic";
    double p = 1;
    Code code = Code(l, latticeType, p, p);
    code.generateMeasError();
    vint syndrome = code.getSyndrome();
    for (const int value : syndrome)
    {
        EXPECT_EQ(value, 1);
    }
}

TEST(generateMeasError, handlesZeroErrorProbability)
{
    int l = 4;
    std::string latticeType = "rhombic";
    double p = 0;
    Code code = Code(l, latticeType, p, p);
    code.generateMeasError();
    vint syndrome = code.getSyndrome();
    for (const int value : syndrome)
    {
        EXPECT_EQ(value, 0);
    }
}

TEST(checkExtremalVertex, correctVerticesExtremalOneError)
{
    int l = 4;
    std::string latticeType = "rhombic";
    double p = 0.1;
    Code code = Code(l, latticeType, p, p);
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

TEST(checkExtremalVertex, correctVerticesExtremalTwoErrors)
{
    int l = 6;
    std::string latticeType = "rhombic";
    double p = 0.1;
    Code code = Code(l, latticeType, p, p);
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

TEST(localFlip, flipFaceOnceTwice)
{
    int l = 8;
    std::string latticeType = "rhombic";
    double p = 0.1;
    Code code = Code(l, latticeType, p, p);
    vint vs = {0, 72, 512, 519};
    code.localFlip(vs);
    vint &flipBits = code.getFlipBits();
    EXPECT_EQ(flipBits[0], 1);
    code.localFlip(vs);
    for (const int value : flipBits)
    {
        EXPECT_EQ(value, 0);
    }
}

TEST(findSweepEdges, correctEdgesOneError)
{
    int l = 4;
    std::string type = "rhombic";
    double p = 0.1;
    Code code = Code(l, type, p, p);
    code.setError({120});
    code.calculateSyndrome();

    // Full vertex
    vstr upEdgesInSynd = code.findSweepEdges(40, "xyz");
    EXPECT_TRUE(std::find(upEdgesInSynd.begin(), upEdgesInSynd.end(), "xyz") 
                != upEdgesInSynd.end());
    EXPECT_TRUE(std::find(upEdgesInSynd.begin(), upEdgesInSynd.end(), "yz") 
                != upEdgesInSynd.end());
    EXPECT_EQ(upEdgesInSynd.size(), 2);

    upEdgesInSynd = code.findSweepEdges(40, "yz");
    EXPECT_TRUE(std::find(upEdgesInSynd.begin(), upEdgesInSynd.end(), "xyz") 
                != upEdgesInSynd.end());
    EXPECT_TRUE(std::find(upEdgesInSynd.begin(), upEdgesInSynd.end(), "yz") 
                != upEdgesInSynd.end());
    EXPECT_EQ(upEdgesInSynd.size(), 2);

    upEdgesInSynd = code.findSweepEdges(40, "xz");
    EXPECT_TRUE(std::find(upEdgesInSynd.begin(), upEdgesInSynd.end(), "xyz") 
                != upEdgesInSynd.end());
    EXPECT_EQ(upEdgesInSynd.size(), 1);

    upEdgesInSynd = code.findSweepEdges(40, "xy");
    EXPECT_TRUE(std::find(upEdgesInSynd.begin(), upEdgesInSynd.end(), "xyz") 
                != upEdgesInSynd.end());
    EXPECT_EQ(upEdgesInSynd.size(), 1);

    upEdgesInSynd = code.findSweepEdges(40, "-xy");
    EXPECT_TRUE(std::find(upEdgesInSynd.begin(), upEdgesInSynd.end(), "yz") 
                != upEdgesInSynd.end());
    EXPECT_EQ(upEdgesInSynd.size(), 1);

    upEdgesInSynd = code.findSweepEdges(40, "-xz");
    EXPECT_TRUE(std::find(upEdgesInSynd.begin(), upEdgesInSynd.end(), "yz") 
                != upEdgesInSynd.end());
    EXPECT_EQ(upEdgesInSynd.size(), 1);

    upEdgesInSynd = code.findSweepEdges(40, "-xyz");
    EXPECT_EQ(upEdgesInSynd.size(), 0);

    upEdgesInSynd = code.findSweepEdges(40, "-yz");
    EXPECT_EQ(upEdgesInSynd.size(), 0);

    // Type 1 half vertex
    upEdgesInSynd = code.findSweepEdges(104, "xyz");
    EXPECT_TRUE(std::find(upEdgesInSynd.begin(), upEdgesInSynd.end(), "yz") 
                != upEdgesInSynd.end());
    EXPECT_EQ(upEdgesInSynd.size(), 1);

    upEdgesInSynd = code.findSweepEdges(104, "yz");
    EXPECT_EQ(upEdgesInSynd.size(), 0);

    upEdgesInSynd = code.findSweepEdges(104, "xz"); 
    EXPECT_EQ(upEdgesInSynd.size(), 0);

    upEdgesInSynd = code.findSweepEdges(104, "xy");
    EXPECT_EQ(upEdgesInSynd.size(), 0);

    upEdgesInSynd = code.findSweepEdges(104, "-xy");
    EXPECT_TRUE(std::find(upEdgesInSynd.begin(), upEdgesInSynd.end(), "-xyz") 
                != upEdgesInSynd.end());
    EXPECT_TRUE(std::find(upEdgesInSynd.begin(), upEdgesInSynd.end(), "yz") 
                != upEdgesInSynd.end());
    EXPECT_EQ(upEdgesInSynd.size(), 2);

    upEdgesInSynd = code.findSweepEdges(104, "-xz");
    EXPECT_TRUE(std::find(upEdgesInSynd.begin(), upEdgesInSynd.end(), "-xyz") 
                != upEdgesInSynd.end());
    EXPECT_TRUE(std::find(upEdgesInSynd.begin(), upEdgesInSynd.end(), "yz") 
                != upEdgesInSynd.end());
    EXPECT_EQ(upEdgesInSynd.size(), 2);

    upEdgesInSynd = code.findSweepEdges(104, "-xyz");
    EXPECT_EQ(upEdgesInSynd.size(), 0);

    upEdgesInSynd = code.findSweepEdges(104, "-yz");
    EXPECT_TRUE(std::find(upEdgesInSynd.begin(), upEdgesInSynd.end(), "-xyz") 
                != upEdgesInSynd.end());
    EXPECT_EQ(upEdgesInSynd.size(), 1);

    // Type 2 half vertex
    upEdgesInSynd = code.findSweepEdges(107, "xyz");
    EXPECT_EQ(upEdgesInSynd.size(), 0);

    upEdgesInSynd = code.findSweepEdges(107, "yz");
    EXPECT_TRUE(std::find(upEdgesInSynd.begin(), upEdgesInSynd.end(), "xyz") 
                != upEdgesInSynd.end());
    EXPECT_EQ(upEdgesInSynd.size(), 1);

    upEdgesInSynd = code.findSweepEdges(107, "xz"); 
    EXPECT_TRUE(std::find(upEdgesInSynd.begin(), upEdgesInSynd.end(), "xyz") 
                != upEdgesInSynd.end());
    EXPECT_TRUE(std::find(upEdgesInSynd.begin(), upEdgesInSynd.end(), "-yz") 
                != upEdgesInSynd.end());
    EXPECT_EQ(upEdgesInSynd.size(), 2);

    upEdgesInSynd = code.findSweepEdges(107, "xy");
    EXPECT_TRUE(std::find(upEdgesInSynd.begin(), upEdgesInSynd.end(), "xyz") 
                != upEdgesInSynd.end());
    EXPECT_TRUE(std::find(upEdgesInSynd.begin(), upEdgesInSynd.end(), "-yz") 
                != upEdgesInSynd.end());
    EXPECT_EQ(upEdgesInSynd.size(), 2);

    upEdgesInSynd = code.findSweepEdges(107, "-xy");
    EXPECT_EQ(upEdgesInSynd.size(), 0);

    upEdgesInSynd = code.findSweepEdges(107, "-xz");
    EXPECT_EQ(upEdgesInSynd.size(), 0);

    upEdgesInSynd = code.findSweepEdges(107, "-xyz");
    EXPECT_TRUE(std::find(upEdgesInSynd.begin(), upEdgesInSynd.end(), "-yz") 
                != upEdgesInSynd.end());
    EXPECT_EQ(upEdgesInSynd.size(), 1);

    upEdgesInSynd = code.findSweepEdges(107, "-yz");
    EXPECT_EQ(upEdgesInSynd.size(), 0);
}

TEST(faceVertices, handlesReasonableInput)
{
    int l = 6;
    std::string type = "rhombic";
    double p = 0.1;
    Code code = Code(l, type, p, p);   

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

TEST(faceVertices, exceptsTooManyDirections)
{
    int l = 6;
    std::string type = "rhombic";
    double p = 0.1;
    Code code = Code(l, type, p, p);   
    EXPECT_THROW(code.faceVertices(0, {"xyz", "-xy", "xy", "xz"}), std::invalid_argument);
}

TEST(faceVertices, exceptsInvalidSigns)
{
    int l = 6;
    std::string type = "rhombic";
    double p = 0.1;
    Code code = Code(l, type, p, p);   
    EXPECT_THROW(code.faceVertices(0, {"xyz", "-xy", "xy"}), std::invalid_argument);
    EXPECT_THROW(code.faceVertices(0, {"xyz", "xy", "-xy"}), std::invalid_argument);
}

TEST(faceVertices, exceptsInvalidDirections)
{
    int l = 6;
    std::string type = "rhombic";
    double p = 0.1;
    Code code = Code(l, type, p, p);   
    EXPECT_THROW(code.faceVertices(0, {"xyz", "xy", "xz"}), std::invalid_argument);
    EXPECT_THROW(code.faceVertices(0, {"xyz", "-xy", "xy"}), std::invalid_argument);
}

TEST(sweepFullVertex, handlesQubitErrorsXY)
{
    int l = 4;
    std::string type = "rhombic";
    double p = 0.1;
    Code code = Code(l, type, p, p);  

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
    sweepEdges = code.findSweepEdges(27, "-xz");
    code.clearFlipBits();
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
    sweepEdges = code.findSweepEdges(27, "xyz");
    code.clearFlipBits();
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
    sweepEdges = code.findSweepEdges(27, "-yz");
    code.clearFlipBits();
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
    sweepEdges = code.findSweepEdges(27, "-xyz");
    code.clearFlipBits();
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
    sweepEdges = code.findSweepEdges(27, "xz");
    code.clearFlipBits();
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
    sweepEdges = code.findSweepEdges(27, "yz");
    code.clearFlipBits();
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
}

TEST(sweepFullVertex, handlesMeasurementErrorsXY)
{
    int l = 4;
    std::string type = "rhombic";
    double p = 0.1;
    Code code = Code(l, type, p, p);
    
    // 3 sweep edges around vertex
    // xy, xyz, -yz edges of vertex 27
    vint syndrome;
    syndrome.assign(14 * l * l * l, 0);
    syndrome[189] = 1;
    syndrome[191] = 1;
    syndrome[501] = 1;
    code.setSyndrome(syndrome);
    vstr sweepEdges = code.findSweepEdges(27, "xy");
    vint &flipBits = code.getFlipBits();
    code.sweepFullVertex(27, sweepEdges, "xy", {"xyz", "-xz", "-yz"});
    EXPECT_TRUE(flipBits[80] ^ flipBits[82]); 
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

    // 4 sweep edges around vertex
    // xy, xyz, -xz, -yz edges of vertex 27
    code.clearFlipBits();
    syndrome.assign(14 * l * l * l, 0);
    syndrome[191] = 1;
    syndrome[189] = 1;
    syndrome[501] = 1;
    syndrome[524] = 1;
    code.setSyndrome(syndrome);
    sweepEdges = code.findSweepEdges(27, "xy");
    code.sweepFullVertex(27, sweepEdges, "xy", {"xyz", "-xz", "-yz"});
    EXPECT_TRUE(flipBits[80] && flipBits[81] && flipBits[82]); 
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

TEST(sweepHalfVertex, handlesQubitErrorsXY)
{
    int l = 6;
    std::string type = "rhombic";
    double p = 0.1;
    Code code = Code(l, type, p, p);
    
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
    sweepEdges = code.findSweepEdges(283, "yz");
    code.clearFlipBits();
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
    sweepEdges = code.findSweepEdges(283, "xz");
    code.clearFlipBits();
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
    sweepEdges = code.findSweepEdges(283, "-xyz");
    code.clearFlipBits();
    code.sweepHalfVertex(283, sweepEdges, "-xyz", {"-xy", "-xz", "-yz"});
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
    sweepEdges = code.findSweepEdges(283, "xz");
    code.clearFlipBits();
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
    sweepEdges = code.findSweepEdges(283, "-xyz");
    code.clearFlipBits();
    code.sweepHalfVertex(283, sweepEdges, "-xyz", {"-xy", "-xz", "-yz"});
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
    sweepEdges = code.findSweepEdges(283, "yz");
    code.clearFlipBits();
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
}

TEST(sweepHalfVertex, handlesMeasurementErrorsXY)
{
    int l = 6;
    std::string type = "rhombic";
    double p = 0.1;
    Code code = Code(l, type, p, p);
    
    // Three sweep edges at vertex 283
    // xyz, -yz and -xz edges
    vint syndrome;
    syndrome.assign(14 * l * l * l, 0);
    syndrome[1981] = 1;
    syndrome[480] = 1;
    syndrome[265] = 1;
    code.setSyndrome(syndrome);
    auto sweepEdges = code.findSweepEdges(283, "xy");
    code.sweepHalfVertex(283, sweepEdges, "xy", {"xyz", "-xz", "-yz"});
    auto &flipBits = code.getFlipBits();
    EXPECT_TRUE(flipBits[109] ^ flipBits[113] ^ flipBits[204]);
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
    sweepEdges = code.findSweepEdges(283, "yz");
    code.clearFlipBits();
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
    sweepEdges = code.findSweepEdges(283, "-xyz");
    code.clearFlipBits();
    code.sweepHalfVertex(283, sweepEdges, "-xyz", {"-xy", "-xz", "-yz"});
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
    sweepEdges = code.findSweepEdges(283, "xz");   
    code.clearFlipBits();
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
}