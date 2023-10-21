#include <algorithm>
#include <string>
#include <ctime>
#include <unistd.h>
#include <stdio.h>
#include <Windows.h>
#include "./Flow/AppController.h"
#include "./MeasureTime/Timer.h"

int main() {
    // Zakladamy ze graf jest pelny i asymetryczny
    srand(time(NULL));

    auto *appController = new AppController();
    appController->mainIndex();
    delete appController;

//    long long s = Timer::read_QPC();
//    Sleep(2000);
//    long long e = Timer::read_QPC();
//
//    std::cout << "sec " << Timer::getSecondsElapsed(s, e);

    return 0;
}
