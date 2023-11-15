#ifndef PEA_PWR_AUTOMATICTESTS_H
#define PEA_PWR_AUTOMATICTESTS_H

#include <iostream>
#include <limits>
#include <algorithm>
#include <vector>
#include <thread>
#include <future>
#include <chrono>
#include "../FileUtils/DataFileUtility.h"
#include "../Flow/ActionResult.h"
#include "../Flow/ConsoleView.h"
#include "../RandomDataGenerator/RandomDataGenerator.h"
#include "../MeasureTime/Timer.h"
#include "../Matrix/Matrix.h"
#include "../Algorithms/BruteForce.h"
#include "../Algorithms/BranchAndBound.h"
#include "../Algorithms/DynamicProgramming.h"


class AutomaticTests {
private:
    int testNumber = 0;
    int maxGraphDist = INT_MAX;
    int graphSize = 0;
    int testTimeLimit = 120000;

    RandomDataGenerator::generator *generator = nullptr;
    Matrix *matrix = nullptr;
    BruteForce *bruteForce = nullptr;
    BranchAndBound *branchAndBound = nullptr;
    DynamicProgramming *dynamicProgramming = nullptr;

public:
    AutomaticTests(RandomDataGenerator::generator *generator, Matrix *matrix,
                   BruteForce *bruteForce, BranchAndBound *branchAndBound, DynamicProgramming *dynamicProgramming);

    virtual ~AutomaticTests();

    void initialize();

    void menu();

    void testBruteForce();

    void updateTestCount();

    void testBranchAndBound();

    void testDynamic();

    // Multithreading
    static void stop_thr_fun(std::jthread &th, std::atomic<int> &stop, int testTimeLimit) {
        Sleep(testTimeLimit);
        if (stop == 1) {
            return;
        }
        if (!th.joinable()) {
            return;
        }
        stop_source ss = th.get_stop_source();
        if (ss.stop_possible()) {
            stop = 2;
            th.request_stop();
        }
    }

    static void
    run_bf_fun(BruteForce *bruteForce, Matrix *pMatrix, std::atomic<int> &finished, std::promise<double> &&p) {
        long long int start, end;
        start = Timer::read_QPC();
        bruteForce->mainFun(pMatrix, pMatrix->getSize(), false);
        end = Timer::read_QPC();
        if (finished != 2) {
            finished = 1;
        }
        p.set_value(Timer::getMicroSecondsElapsed(start, end));
    };

    static void
    run_bib_fun(BranchAndBound *branchAndBound, Matrix *pMatrix, std::atomic<int> &finished, std::promise<double> &&p) {
        long long int start, end;
        start = Timer::read_QPC();
        branchAndBound->mainFun(pMatrix, pMatrix->getSize());
        end = Timer::read_QPC();
        if (finished != 2) {
            finished = 1;
        }
        p.set_value(Timer::getMicroSecondsElapsed(start, end));
    };

    static void
    run_dynamic_fun(DynamicProgramming *dynamicProgramming, Matrix *pMatrix, std::atomic<int> &finished,
                    std::promise<double> &&p) {
        long long int start, end;
        start = Timer::read_QPC();
        dynamicProgramming->mainFun(pMatrix, pMatrix->getSize());
        end = Timer::read_QPC();
        if (finished != 2) {
            finished = 1;
        }
        p.set_value(Timer::getMicroSecondsElapsed(start, end));
    };

};

#endif //PEA_PWR_AUTOMATICTESTS_H
