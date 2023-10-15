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
        DISPLAY_LATEST_RESULTS = 3
    };
};


#endif //PEA_PWR_ACTIONRESULT_H
