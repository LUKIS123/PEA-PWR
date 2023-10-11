#ifndef PEA_PWR_CONSOLEVIEW_H
#define PEA_PWR_CONSOLEVIEW_H


#include "iostream"
#include "ActionResult.h"

class ConsoleView {
public:
    ConsoleView() = default;

    virtual ~ConsoleView() = default;

    static ActionResult::mainMenu mainMenu();
};


#endif //PEA_PWR_CONSOLEVIEW_H
