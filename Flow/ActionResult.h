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

};


#endif //PEA_PWR_ACTIONRESULT_H
