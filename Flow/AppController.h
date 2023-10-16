#ifndef PEA_PWR_APPCONTROLLER_H
#define PEA_PWR_APPCONTROLLER_H

#include "iostream"
#include "ConsoleView.h"
#include "ActionResult.h"
#include "../RandomDataGenerator/RandomDataGenerator.h"
#include "../MeasureTime/Timer.h"
#include "../Matrix/Matrix.h"
#include "../Algorithms/BruteForce.h"

class AppController {
private:
    RandomDataGenerator *randomDataGenerator = nullptr;
    RandomDataGenerator::generator *generator = nullptr;
    Matrix *matrix = nullptr;
    BruteForce *bruteForce = nullptr;

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

    void runBruteForce();

    void displayLatestResults();
};


#endif //PEA_PWR_APPCONTROLLER_H
