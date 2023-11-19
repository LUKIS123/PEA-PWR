#include "AutomaticTests.h"

AutomaticTests::AutomaticTests(RandomDataGenerator::generator *generator,
                               Matrix *matrix,
                               BruteForce *bruteForce, BranchAndBound *branchAndBound) :
        generator(generator),
        matrix(matrix),
        bruteForce(bruteForce),
        branchAndBound(branchAndBound) {
}

AutomaticTests::~AutomaticTests() = default;

void AutomaticTests::initialize() {
    matrix->testing = true;
    system("CLS");
    std::cout << "AUTOMATIC TESTS Initialization..." << std::endl;
    std::cout << std::endl << "Set number of tests..." << std::endl;

    std::cout << "Tests: ";
    std::cin >> testNumber;
    while (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Bad entry... Enter a NUMBER: ";
        std::cin >> testNumber;
    }
    std::cout << "Vertices count: ";
    std::cin >> graphSize;
    while (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Bad entry... Enter a NUMBER: ";
        std::cin >> graphSize;
    }
    std::cout << "Maximum graph edge weight: ";
    std::cin >> maxGraphDist;
    while (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Bad entry... Enter a NUMBER: ";
        std::cin >> maxGraphDist;
    }
    std::cout << "Time limit (ms): ";
    std::cin >> testTimeLimit;
    while (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Bad entry... Enter a NUMBER: ";
        std::cin >> testTimeLimit;
    }
    matrix->distanceValueLimit = maxGraphDist;
    matrix->generate(graphSize, generator);
}

void AutomaticTests::menu() {
    ActionResult::automaticTestsMenu status = ActionResult::automaticTestsMenu::MENU_TEST;
    while (status != ActionResult::automaticTestsMenu::END_TEST) {
        switch (status) {
            case ActionResult::MENU_TEST:
                status = ConsoleView::automaticTestsMenu();
                break;
            case ActionResult::TEST_BRUTE_FORCE:
                testBruteForce();
                status = ActionResult::MENU_TEST;
                break;
            case ActionResult::TEST_BRANCH_AND_BOUND:
                testBranchAndBound();
                status = ActionResult::MENU_TEST;
                break;
            case ActionResult::SET_TEST_COUNT:
                updateTestCount();
                status = ActionResult::MENU_TEST;
                break;
            case ActionResult::END_TEST:
                break;
        }
    }
    matrix->testing = false;
}

void AutomaticTests::updateTestCount() {
    system("CLS");
    std::cout << "AUTOMATIC TESTS Initialization..." << std::endl;
    std::cout << std::endl << "Set number of tests..." << std::endl;
    while (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Bad entry... Enter a NUMBER: ";
        std::cin >> testNumber;
    }
    std::cout << "Tests: ";
    std::cin >> testNumber;
}

void AutomaticTests::testBruteForce() {
    std::string fileName = "./BruteForce.csv";
    std::string cols = "us,ms,s";
    std::vector<double> resultsUS;
    std::vector<double> resultsMS;
    std::vector<double> resultsS;

    double results;
    for (int i = 0; i < testNumber; i++) {
        std::promise<double> p;
        std::atomic<int> done = 0;
        auto f = p.get_future();
        std::jthread thr(run_bf_fun, bruteForce, matrix, std::ref(done), std::move(p));
        std::jthread stop_thr(stop_thr_fun, std::ref(thr), std::ref(done), testTimeLimit);
        stop_thr.join();
        thr.join();
        if (done == 1) {
            try {
                results = f.get();
                resultsUS.push_back(results);
                resultsMS.push_back(results / 1000);
                resultsS.push_back(results / 1000000);
            }
            catch (...) {
            }
        }
        matrix->generateAnew(generator);
    }
    DataFileUtility::saveAutomaticTestResults(fileName, resultsUS, resultsMS, resultsS, cols);
    std::cout << "Done!" << std::endl;
    system("PAUSE");
}

void AutomaticTests::testBranchAndBound() {
    std::string fileName = "./BBLittle.csv";
    std::string cols = "us,ms,s";
    std::vector<double> resultsUS;
    std::vector<double> resultsMS;
    std::vector<double> resultsS;

    double results;
    for (int i = 0; i < testNumber; i++) {
        std::promise<double> p;
        std::atomic<int> done = 0;
        auto f = p.get_future();
        std::jthread thr(run_bib_fun, branchAndBound, matrix, std::ref(done), std::move(p));
        std::jthread stop_thr(stop_thr_fun, std::ref(thr), std::ref(done), testTimeLimit);
        stop_thr.join();
        thr.join();
        if (done == 1) {
            try {
                results = f.get();
                resultsUS.push_back(results);
                resultsMS.push_back(results / 1000);
                resultsS.push_back(results / 1000000);
            }
            catch (...) {
            }
        }
        matrix->generateAnew(generator);
    }
    DataFileUtility::saveAutomaticTestResults(fileName, resultsUS, resultsMS, resultsS, cols);
    std::cout << "Done!" << std::endl;
    system("PAUSE");
}
