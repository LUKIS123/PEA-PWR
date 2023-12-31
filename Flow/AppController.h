#ifndef PEA_PWR_APPCONTROLLER_H
#define PEA_PWR_APPCONTROLLER_H

#include <iostream>
#include "ConsoleView.h"
#include "ActionResult.h"
#include "LatestAlgorithmRun.h"
#include "../RandomDataGenerator/RandomDataGenerator.h"
#include "../MeasureTime/Timer.h"
#include "../Tests/AutomaticTests.h"
#include "../Matrix/Matrix.h"
#include "../Algorithms/BruteForce.h"
#include "../Algorithms/BranchAndBound.h"
#include "../Algorithms/DynamicProgramming.h"


class AppController {
private:
    RandomDataGenerator *randomDataGenerator = nullptr;
    RandomDataGenerator::generator *generator = nullptr;
    AutomaticTests *automaticTests = nullptr;
    Matrix *matrix = nullptr;
    BruteForce *bruteForce = nullptr;
    BranchAndBound *branchAndBound = nullptr;
    DynamicProgramming *dynamicProgramming = nullptr;

    std::string algorithmTypes[4] = {
            "BRUTE FORCE",
            "BRUTE FORCE OPTIMIZED",
            "BRANCH AND BOUND",
            "DYNAMIC PROGRAMMING"
    };
    LatestAlgorithm latestRun = LatestAlgorithm::NONE;
    double latestTimerResult = 0.0;

public:
    AppController();

    virtual ~AppController();

    void clearMemory();

    void mainIndex();

    void loadDataFile();

    void generateMatrix();

    void algorithmMenu();

    void testsMenu();

    void runBruteForce(bool ifOptimizedAlgorithm);

    void runBranchAndBound();

    void runDynamic();

    void displayLatestResults();
};


#endif //PEA_PWR_APPCONTROLLER_H
