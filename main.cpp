#include <algorithm>
#include <string>
#include <ctime>
#include "./Flow/AppController.h"

int main() {
    // Zakladamy ze graf jest pelny i asymetryczny
    srand(time(NULL));

    auto *appController = new AppController();
    appController->mainIndex();
    delete appController;

    return 0;
}
