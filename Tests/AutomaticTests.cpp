#include "AutomaticTests.h"

AutomaticTests::AutomaticTests(RandomDataGenerator *randomDataGenerator,
                               RandomDataGenerator::generator *generator,
                               Matrix *matrix,
                               BruteForce *bruteForce) :
        randomDataGenerator(randomDataGenerator),
        generator(generator),
        matrix(matrix),
        bruteForce(bruteForce) {

}

AutomaticTests::~AutomaticTests() {
}

void AutomaticTests::initialize() {
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

    std::cout << "Maximum edge weight: ";
    std::cin >> maxGraphDist;
    while (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Bad entry... Enter a NUMBER: ";
        std::cin >> maxGraphDist;
    }
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
            case ActionResult::SET_TEST_COUNT:
                updateTestCount();
                status = ActionResult::MENU_TEST;
                break;
            case ActionResult::END_TEST:
                break;
        }
    }
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
    matrix->displayMatrix();
}
