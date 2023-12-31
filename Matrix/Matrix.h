#ifndef PEA_PWR_MATRIX_H
#define PEA_PWR_MATRIX_H

#include <iostream>
#include "../RandomDataGenerator/RandomDataGenerator.h"

class Matrix {
private:
    int **matrix = nullptr;
    int size = 0;

public:
    bool testing = false;
    bool exists = false;
    int distanceValueLimit = 1000;

    Matrix();

    virtual ~Matrix();

    int **getMatrix();

    int getSize();

    void generate(int newSize, RandomDataGenerator::generator *numberGenerator);

    void generateAnew(RandomDataGenerator::generator *numberGenerator);

    void loadFromFile(const std::string &filename);

    void displayMatrix();

private:
    void clearData();
};

#endif //PEA_PWR_MATRIX_H
