#ifndef PEA_PWR_APPCONTROLLER_H
#define PEA_PWR_APPCONTROLLER_H

#include "iostream"
#include "ConsoleView.h"
#include "ActionResult.h"
#include "../RandomDataGenerator/RandomDataGenerator.h"
#include "../Matrix/Matrix.h"

class AppController {
private:
    RandomDataGenerator *randomDataGenerator = nullptr;
    RandomDataGenerator::generator *generator = nullptr;
    Matrix *matrix = nullptr;

public:
    AppController();

    virtual ~AppController();

    void clearMemory();

    void mainIndex();

    void loadDataFile();

    void generateMatrix();

    void algorithmMenu();

    void testsMenu();
};


#endif //PEA_PWR_APPCONTROLLER_H
