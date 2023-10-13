#include "AppController.h"
#include <limits>
#include <algorithm>
#include <cctype>
#include <string>
#include <cmath>

AppController::AppController() {
    randomDataGenerator = new RandomDataGenerator();
    generator = new RandomDataGenerator::generator();
    generator->setGen();

    matrix = new Matrix();
}

AppController::~AppController() {
    clearMemory();
}

void AppController::clearMemory() {
    delete randomDataGenerator;
    delete generator;
    delete matrix;
}

void AppController::mainIndex() {
    ActionResult::mainMenu status = ActionResult::BACK_TO_MENU;
    while (status != ActionResult::END) {
        switch (status) {
            case ActionResult::BACK_TO_MENU:
                status = ConsoleView::mainMenu();
                break;
            case ActionResult::LOAD_DATA:
                loadDataFile();
                status = ActionResult::BACK_TO_MENU;
                break;
            case ActionResult::GENERATE_DATA:
                generateMatrix();
                status = ActionResult::BACK_TO_MENU;
                break;
            case ActionResult::DISPLAY_DATA_BUFFER:
                system("CLS");
                matrix->displayMatrix();
                system("PAUSE");
                status = ActionResult::BACK_TO_MENU;
                break;
            case ActionResult::RUN_ALGORITHMS:
                algorithmMenu();
                status = ActionResult::BACK_TO_MENU;
                break;
            case ActionResult::RUN_TESTS:
                testsMenu();
                status = ActionResult::BACK_TO_MENU;
                break;
            case ActionResult::END:
                break;
        }
    }
    std::cout << "Exiting Application..." << std::endl;
}

void AppController::loadDataFile() {
    std::string fileName;
    std::cout << "Place .txt file in executable folder and enter file name (WITHOUT .txt):" << std::endl;
    std::cin >> fileName;
    matrix->loadFromFile(fileName);
    system("PAUSE");
}

void AppController::generateMatrix() {
    int size, maxDistanceValue;
    std::cout << "Set size: ";
    std::cin >> size;
    while (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Bad entry... Enter a NUMBER: ";
        std::cin >> size;
    }
    std::cout << "Set max distanceToFinish: ";
    std::cin >> maxDistanceValue;
    while (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Bad entry... Enter a NUMBER: ";
        std::cin >> maxDistanceValue;
    }
    matrix->distanceValueLimit = maxDistanceValue;
    matrix->generate(size, generator);
    system("PAUSE");
}

void AppController::algorithmMenu() {
    bruteForce = new BruteForce(matrix, matrix->getSize());
    bruteForce->mainFun();

}

void AppController::testsMenu() {

}
