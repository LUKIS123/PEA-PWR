#include "List.h"

List::~List() {
    removeAll();
}

List::ListElem *List::getHead() {
    return head;
}

List::ListElem *List::getTail() {
    return tail;
}

unsigned List::getSize() {
    return size;
}

void List::removeAll() {
    ListElem *current = head;
    while (current != nullptr) {
        ListElem *temp = current;
        current = current->next;
        delete temp;
        size--;
    }
    head = nullptr;
    tail = nullptr;
}

void List::pushEnd(int data) {
    size++;
    ListElem *node = new ListElem;
    node->data = data;

    if (head == nullptr) {
        head = node;
        tail = node;
        return;
    }

    node->prev = tail;
    tail->next = node;
    tail = node;
}

void List::displayFromFront() {
    ListElem *current = head;
    while (current != nullptr) {
        std::cout << current->data << ", ";
        current = current->next;
    }
    if (size == 0) {
        std::cout << "List is empty!";
    }
    std::cout << std::endl;
}

List::ListElem *List::getByIndex(int index) {
    if (size == 0) {
        std::cout << "List is empty! ";
        return nullptr;
    }
    if (index <= 0) {
        return head;
    }
    if (index >= size - 1) {
        return tail;
    }

    ListElem *current = head;
    for (int i = 0; i < index; i++) {
        current = current->next;
        if (current == nullptr) {
            return nullptr;
        }
    }
    return current;
}

int List::popEnd() {
    if (head == nullptr) {
        return NULL;
    }
    size--;
    int value = tail->data;

    if (tail->prev == nullptr) {
        delete tail;
        head = nullptr;
        tail = nullptr;
        return value;
    }

    ListElem *previous = tail->prev;
    previous->next = nullptr;
    delete tail;
    tail = previous;

    return value;
}

void List::copyOf(List *list) {
    if (size != 0) {
        removeAll();
    }
    ListElem *elem = list->getHead();
    while (elem != nullptr) {
        pushEnd(elem->data);
        elem = elem->next;
        size++;
    }
}
