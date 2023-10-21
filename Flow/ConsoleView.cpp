#include "ConsoleView.h"

ActionResult::mainMenu ConsoleView::mainMenu() {
    system("CLS");
    std::cout << "Menu: Choose from given options..." << std::endl;
    std::cout << "1 => Load matrix from file" << std::endl;
    std::cout << "2 => Generate matrix" << std::endl;
    std::cout << "3 => Display matrix" << std::endl;
    std::cout << "4 => ALGORITHMS" << std::endl;
    std::cout << "5 => TESTS" << std::endl;
    std::cout << "0 => Exit" << std::endl;

    std::cout << "Choice: ";
    int next;
    std::cin >> next;
    while (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Bad entry... Enter a NUMBER: ";
        std::cin >> next;
    }
    std::cout << std::endl;
    switch (next) {
        case 1:
            return ActionResult::LOAD_DATA;
        case 2:
            return ActionResult::GENERATE_DATA;
        case 3:
            return ActionResult::DISPLAY_DATA_BUFFER;
        case 4:
            return ActionResult::RUN_ALGORITHMS;
        case 5:
            return ActionResult::RUN_TESTS;
        case 0:
            return ActionResult::END;
        default:
            return ActionResult::BACK_TO_MENU;
    }
}

ActionResult::algorithmsMenu ConsoleView::algorithmsMenu() {
    system("CLS");
    std::cout << "Menu: Choose from given options..." << std::endl;
    std::cout << "1 => Run BRUTE FORCE" << std::endl;
    std::cout << "2 => Run BRUTE FORCE with Optimization" << std::endl;
    std::cout << "3 => Display latest results" << std::endl;
    std::cout << "0 => Return" << std::endl;

    std::cout << "Choice: ";
    int next;
    std::cin >> next;
    while (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Bad entry... Enter a NUMBER: ";
        std::cin >> next;
    }
    std::cout << std::endl;
    switch (next) {
        case 1:
            return ActionResult::RUN_BRUTE_FORCE;
        case 2:
            return ActionResult::RUN_BRUTE_FORCE_OPT;
        case 3:
            return ActionResult::DISPLAY_LATEST_RESULTS;
        case 0:
            return ActionResult::END_ALGORITHMS_MENU;
        default:
            return ActionResult::BACK_TO_ALGORITHMS_MENU;
    }
}

ActionResult::automaticTestsMenu ConsoleView::automaticTestsMenu() {
    system("CLS");
    std::cout << "AUTOMATIC TESTS: Choose from given options..." << std::endl;
    std::cout << "1 => Test BRUTE FORCE" << std::endl;
    std::cout << "2 => Test BRANCH AND BOUND" << std::endl;
    std::cout << "3 => Change Test Count" << std::endl;
    std::cout << "0 => Exit" << std::endl;

    std::cout << "Choice: ";
    int next = 0;
    std::cin >> next;
    while (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Bad entry... Enter a NUMBER: ";
        std::cin >> next;
    }

    switch (next) {
        case 1:
            return ActionResult::automaticTestsMenu::TEST_BRUTE_FORCE;
        case 2:
            return ActionResult::automaticTestsMenu::TEST_BRUTE_FORCE;
        case 3:
            return ActionResult::automaticTestsMenu::SET_TEST_COUNT;
        case 0:
            return ActionResult::automaticTestsMenu::END_TEST;
        default:
            return ActionResult::automaticTestsMenu::MENU_TEST;
    }
}
