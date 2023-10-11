#include "RandomDataGenerator.h"

int RandomDataGenerator::generateRandomInteger() {
    std::mt19937 rng(seed);
    std::uniform_int_distribution<std::mt19937::result_type> dist(1, limit);
    return dist(rng);
}

void RandomDataGenerator::setLimit(int newLimit) {
    limit = newLimit;
}
