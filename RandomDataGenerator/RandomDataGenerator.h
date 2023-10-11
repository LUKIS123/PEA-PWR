#ifndef PEA_PWR_RANDOMDATAGENERATOR_H
#define PEA_PWR_RANDOMDATAGENERATOR_H

#include "iostream"
#include "random"

class RandomDataGenerator {
private:
    int seed = 0;
    int limit = 100;
public:
    struct generator {
        std::uniform_int_distribution<> dist;
        std::random_device rd;
        std::mt19937 gen;

        void setGen() {
            gen = std::mt19937(rd());
        }

        void setDist(int newLimit) {
            dist = std::uniform_int_distribution<>(1, newLimit);
        }

        int getRandomNum() {
            return dist(gen);
        }
    };

    RandomDataGenerator() = default;

    ~RandomDataGenerator() = default;

    int generateRandomInteger();

    void setLimit(int newLimit);
};

#endif //PEA_PWR_RANDOMDATAGENERATOR_H
