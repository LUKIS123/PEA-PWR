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
