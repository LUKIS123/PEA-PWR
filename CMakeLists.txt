cmake_minimum_required(VERSION 3.26)
project(PEA_PWR)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -static-libgcc -fno-rtti")

set(SOURCE_FILES main.cpp)

add_executable(PEA_PWR main.cpp
        Flow/ActionResult.h
        Flow/AppController.cpp
        Flow/AppController.h
        Matrix/Matrix.cpp
        Matrix/Matrix.h
        RandomDataGenerator/RandomDataGenerator.cpp
        RandomDataGenerator/RandomDataGenerator.h
        Flow/ConsoleView.cpp
        Flow/ConsoleView.h
        MeasureTime/Timer.cpp
        MeasureTime/Timer.h
        Algorithms/BruteForce.cpp
        Algorithms/BruteForce.h
        Algorithms/Utils/List.cpp
        Algorithms/Utils/List.h
        Tests/AutomaticTests.cpp
        Tests/AutomaticTests.h
        Flow/LatestAlgorithmRun.h
        Algorithms/BranchAndBound.h
        Algorithms/BranchAndBound.cpp
        Algorithms/Utils/BranchAndBoundNode.h
        Algorithms/Utils/BranchAndBoundNode.cpp
        Algorithms/DynamicProgramming.h
        Algorithms/DynamicProgramming.cpp
        FileUtils/DataFileUtility.cpp
        FileUtils/DataFileUtility.h
)
