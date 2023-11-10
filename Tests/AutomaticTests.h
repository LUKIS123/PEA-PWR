#ifndef PEA_PWR_AUTOMATICTESTS_H
#define PEA_PWR_AUTOMATICTESTS_H

#include "iostream"
#include "limits"
#include <algorithm>
#include <vector>
#include "../FileUtils/DataFileUtility.h"
#include "../Flow/ActionResult.h"
#include "../Flow/ConsoleView.h"
#include "../RandomDataGenerator/RandomDataGenerator.h"
#include "../MeasureTime/Timer.h"
#include "../Matrix/Matrix.h"
#include "../Algorithms/BruteForce.h"
#include "../Algorithms/BranchAndBound.h"
#include "../Algorithms/DynamicProgramming.h"

class AutomaticTests {
private:
    int testNumber = 0;
    int maxGraphDist = INT_MAX;
    int graphSize = 0;

    RandomDataGenerator *randomDataGenerator = nullptr;
    RandomDataGenerator::generator *generator = nullptr;
    Matrix *matrix = nullptr;
    BruteForce *bruteForce = nullptr;
    BranchAndBound *branchAndBound = nullptr;
    DynamicProgramming *dynamicProgramming = nullptr;

public:
    AutomaticTests(RandomDataGenerator *randomDataGenerator, RandomDataGenerator::generator *generator, Matrix *matrix,
                   BruteForce *bruteForce, BranchAndBound *branchAndBound, DynamicProgramming *dynamicProgramming);

    virtual ~AutomaticTests();

    void initialize();

    void menu();

    void testBruteForce();

    void updateTestCount();

    void testBranchAndBound();

    void testDynamic();
};


#endif //PEA_PWR_AUTOMATICTESTS_H
