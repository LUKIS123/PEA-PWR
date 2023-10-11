#include "Matrix.h"
#include "fstream"

Matrix::Matrix() = default;

Matrix::~Matrix() {
    clearData();
}

int **Matrix::getMatrix() {
    return matrix;
}

// Metoda generuje macierz dla asymetrycznego grafu pełnego o zadanej liczbie wierzcholkow
void Matrix::generate(int newSize, RandomDataGenerator::generator *numberGenerator) {
    clearData();
    numberGenerator->setDist(distanceValueLimit);
    size = newSize;

    matrix = new int *[size];
    for (int i = 0; i < size; i++) {
        matrix[i] = new int[size];
    }
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (i == j) {
                matrix[i][j] = -1;
            } else {
                matrix[i][j] = numberGenerator->getRandomNum();
            }
        }
    }
    exists = true;
    if (!testing) {
        displayMatrix();
    }
}

// Ladowanie macierzy z pliku
void Matrix::loadFromFile(const std::string &filename) {
    clearData();
    std::ifstream file(filename + ".txt");
    if (file.good()) {
        file >> size;
        matrix = new int *[size];
        for (int i = 0; i < size; i++) {
            matrix[i] = new int[size];
        }

        int value;
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                file >> value;
                if (i == j && value != -1) {
                    std::cout << "Data file error!" << std::endl;
                    exists = false;
                    return;
                }
                matrix[i][j] = value;
            }
        }
        file.close();
        exists = true;
        if (!testing) {
            displayMatrix();
        }
    } else {
        std::cout << "File error - OPEN" << std::endl;
    }
}

// Wyswietlanie macierzy
void Matrix::displayMatrix() {
    if (matrix == nullptr) {
        std::cout << "Matrix is empty" << std::endl;
        return;
    }
    std::cout << "\nMatrix: " << "[Size: " << size << "]" << std::endl;
    std::cout << "  ";
    for (int i = 0; i < size; i++) {
        if (i > 10) std::cout << " ";
        else std::cout << "  ";
        std::cout << i;
    }
    std::cout << "\n\n";
    for (int i = 0; i < size; i++) {
        std::cout << i;
        if (i < 10) std::cout << " ";
        for (int j = 0; j < size; j++) {
            std::cout << "  ";
            std::cout << matrix[i][j];
        }
        std::cout << std::endl;
    }
}

// Zwalnianie pamieci
void Matrix::clearData() {
    if (matrix != nullptr) {
        for (int i = 0; i < size; i++) {
            delete[] matrix[i];
        }
        delete[] matrix;
        matrix = nullptr;
    }
}
