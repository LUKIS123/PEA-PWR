//
// Created by lukas on 12.10.2023.
//

#ifndef PEA_PWR_LIST_H
#define PEA_PWR_LIST_H

#include "iostream"

class List {
public:
    struct ListElem {
        int data = 0;
        ListElem *prev = nullptr;
        ListElem *next = nullptr;
    };

    List() = default;

    virtual ~List();

    ListElem *getHead();

    ListElem *getTail();

    unsigned getSize();

    ListElem *getByIndex(int index);

    void removeAll();

    void pushEnd(int data);

    int popEnd();

    void displayFromFront();

private:
    ListElem *head = nullptr;
    ListElem *tail = nullptr;
    unsigned size = 0;
};

#endif //PEA_PWR_LIST_H
