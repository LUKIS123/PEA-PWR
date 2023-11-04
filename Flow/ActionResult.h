#ifndef PEA_PWR_ACTIONRESULT_H
#define PEA_PWR_ACTIONRESULT_H

class ActionResult {
public:
    enum mainMenu {
        END = 0,
        LOAD_DATA = 1,
        GENERATE_DATA = 2,
        DISPLAY_DATA_BUFFER = 3,
        RUN_ALGORITHMS = 4,
        RUN_TESTS = 5,
        BACK_TO_MENU = 6
    };
    enum algorithmsMenu {
        END_ALGORITHMS_MENU = 0,
        BACK_TO_ALGORITHMS_MENU = 1,
        RUN_BRUTE_FORCE = 2,
        RUN_BRUTE_FORCE_OPT = 3,
        RUN_BRANCH_AND_BOUND = 4,
        RUN_DYNAMIC = 5,
        DISPLAY_LATEST_RESULTS = 6
    };
    enum automaticTestsMenu {
        END_TEST = 0,
        MENU_TEST = 1,
        SET_TEST_COUNT = 2,
        TEST_BRUTE_FORCE = 3,
        TEST_BRANCH_AND_BOUND = 4,
        TEST_DYNAMIC = 5
    };
};


#endif //PEA_PWR_ACTIONRESULT_H
