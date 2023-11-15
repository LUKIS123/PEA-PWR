#include <algorithm>
#include <ctime>
#include "./Flow/AppController.h"

// Zakladamy ze graf jest pelny i asymetryczny
int main() {
    srand(time(NULL));

    auto *appController = new AppController();
    appController->mainIndex();
    delete appController;

    return 0;
}
