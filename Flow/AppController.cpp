#include "AppController.h"
#include <limits>
#include <string>
#include <cmath>

AppController::AppController() {
    randomDataGenerator = new RandomDataGenerator();
    generator = new RandomDataGenerator::generator();
    generator->setGen();
    matrix = new Matrix();
    bruteForce = new BruteForce();
    branchAndBound = new BranchAndBound();
    dynamicProgramming = new DynamicProgramming();
    // Testy
    automaticTests = new AutomaticTests(randomDataGenerator, generator, matrix, bruteForce, branchAndBound,
                                        dynamicProgramming);
}

AppController::~AppController() {
    clearMemory();
}

void AppController::clearMemory() {
    delete randomDataGenerator;
    delete generator;
    delete matrix;
    delete bruteForce;
    delete branchAndBound;
    delete automaticTests;
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
    if (matrix->getSize() < 2) {
        std::cout << "Wrong input!";
        system("PAUSE");
        return;
    }
    ActionResult::algorithmsMenu status = ActionResult::BACK_TO_ALGORITHMS_MENU;
    while (status != ActionResult::END_ALGORITHMS_MENU) {
        switch (status) {
            case ActionResult::BACK_TO_ALGORITHMS_MENU:
                status = ConsoleView::algorithmsMenu();
                break;
            case ActionResult::RUN_BRUTE_FORCE:
                runBruteForce(false);
                status = ActionResult::BACK_TO_ALGORITHMS_MENU;
                break;
            case ActionResult::RUN_BRUTE_FORCE_OPT:
                runBruteForce(true);
                status = ActionResult::BACK_TO_ALGORITHMS_MENU;
                break;
            case ActionResult::RUN_BRANCH_AND_BOUND:
                runBranchAndBound();
                status = ActionResult::BACK_TO_ALGORITHMS_MENU;
                break;
            case ActionResult::RUN_DYNAMIC:
                runDynamic();
                status = ActionResult::BACK_TO_ALGORITHMS_MENU;
                break;
            case ActionResult::DISPLAY_LATEST_RESULTS:
                displayLatestResults();
                status = ActionResult::BACK_TO_ALGORITHMS_MENU;
                break;
            case ActionResult::END_ALGORITHMS_MENU:
                break;
        }
    }
}

void AppController::runBruteForce(bool ifOptimizedAlgorithm) {
    bruteForce->clearData();
    if (ifOptimizedAlgorithm) {
        long long start = Timer::read_QPC();
        bruteForce->mainFun(matrix, matrix->getSize(), true);
        long long end = Timer::read_QPC();
        bruteForce->displayLatestResults();
        latestTimerResult = Timer::getMicroSecondsElapsed(start, end);
        latestRun = LatestAlgorithm::BRUTE_FORCE_OPT;
    } else {
        long long start = Timer::read_QPC();
        bruteForce->mainFun(matrix, matrix->getSize(), false);
        long long end = Timer::read_QPC();
        bruteForce->displayLatestResults();
        latestTimerResult = Timer::getMicroSecondsElapsed(start, end);
        latestRun = LatestAlgorithm::BRUTE_FORCE;
    }

    std::cout << "Timer: " << latestTimerResult << " us" << std::endl;
    std::cout << "     : " << latestTimerResult / 1000 << " ms" << std::endl;
    std::cout << "     : " << latestTimerResult / 1000000 << " s" << std::endl;
    system("PAUSE");
}

void AppController::runBranchAndBound() {
    long long start = Timer::read_QPC();
    branchAndBound->mainFun(matrix, matrix->getSize());
    long long end = Timer::read_QPC();
    latestTimerResult = Timer::getMicroSecondsElapsed(start, end);
    latestRun = LatestAlgorithm::BRANCH_AND_BOUND;

    branchAndBound->displayLatestResults();
    std::cout << "Timer: " << latestTimerResult << " us" << std::endl;
    std::cout << "     : " << latestTimerResult / 1000 << " ms" << std::endl;
    std::cout << "     : " << latestTimerResult / 1000000 << " s" << std::endl;
    system("PAUSE");
}

void AppController::runDynamic() {
    long long start = Timer::read_QPC();
    dynamicProgramming->mainFun(matrix, matrix->getSize());
    long long end = Timer::read_QPC();
    latestTimerResult = Timer::getMicroSecondsElapsed(start, end);
    latestRun = LatestAlgorithm::DYNAMIC;

    dynamicProgramming->displayLatestResults();
    std::cout << "Timer: " << latestTimerResult << " us" << std::endl;
    std::cout << "     : " << latestTimerResult / 1000 << " ms" << std::endl;
    std::cout << "     : " << latestTimerResult / 1000000 << " s" << std::endl;
    system("PAUSE");
}

void AppController::displayLatestResults() {
    std::cout << "algorithm: ";
    switch (latestRun) {
        case BRUTE_FORCE:
            std::cout << algorithmTypes[0] << std::endl;
            break;
        case BRUTE_FORCE_OPT:
            std::cout << algorithmTypes[1] << std::endl;
            break;
        case BRANCH_AND_BOUND:
            std::cout << algorithmTypes[2] << std::endl;
            break;
        case DYNAMIC:
            std::cout << algorithmTypes[3] << std::endl;
            break;
        default:
            break;
    }
    if (latestRun == LatestAlgorithm::BRUTE_FORCE_OPT || latestRun == LatestAlgorithm::BRUTE_FORCE) {
        bruteForce->displayLatestResults();
    }
    if (latestRun == LatestAlgorithm::BRANCH_AND_BOUND) {
        branchAndBound->displayLatestResults();
    }
    if (latestRun == LatestAlgorithm::DYNAMIC) {
        dynamicProgramming->displayLatestResults();
    }
    std::cout << "Timer: " << latestTimerResult << " us" << std::endl;
    std::cout << "     : " << latestTimerResult / 1000 << " ms" << std::endl;
    std::cout << "     : " << latestTimerResult / 1000000 << " s" << std::endl;
    system("PAUSE");
}

void AppController::testsMenu() {
    automaticTests->initialize();
    automaticTests->menu();
}
