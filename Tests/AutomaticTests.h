#ifndef PEA_PWR_AUTOMATICTESTS_H
#define PEA_PWR_AUTOMATICTESTS_H

#include "iostream"
#include "limits"
#include "../Flow/ActionResult.h"
#include "../Flow/ConsoleView.h"
#include "../RandomDataGenerator/RandomDataGenerator.h"
#include "../MeasureTime/Timer.h"
#include "../Matrix/Matrix.h"
#include "../Algorithms/BruteForce.h"

class AutomaticTests {
private:
    int testNumber = 0;
    int maxGraphDist = INT_MAX;

    RandomDataGenerator *randomDataGenerator = nullptr;
    RandomDataGenerator::generator *generator = nullptr;
    Matrix *matrix = nullptr;
    BruteForce *bruteForce = nullptr;

public:
    AutomaticTests(RandomDataGenerator *randomDataGenerator, RandomDataGenerator::generator *generator, Matrix *matrix,
                   BruteForce *bruteForce);

    virtual ~AutomaticTests();

    void initialize();

    void menu();

    void testBruteForce();

    void updateTestCount();
};


#endif //PEA_PWR_AUTOMATICTESTS_H
