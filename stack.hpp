#ifndef STACK_HPP
#define STACK_HPP

#include <iostream>
#include <string>
#include "queue.hpp"
using namespace std;

class BookingNode_Stack {
    public:
    Booking item;
    BookingNode_Stack* next;
};

class BookingStack {
    private:
    BookingNode_Stack* topPtr;
    int count;

    public:
    BookingStack() {
        topPtr = NULL;
        count = 0;
    }

    ~BookingStack() {
        destroyStack();
    }

    void destroyStack() {
        BookingNode_Stack* temp;
        while (topPtr != NULL) {
            temp = topPtr;
            topPtr = topPtr->next;
            delete temp;
        }
        count = 0;
    }

    bool isEmpty() {
        return (topPtr == NULL);
    }

    void push(Booking b) {
        BookingNode_Stack* newNode = new BookingNode_Stack;
        newNode->item = b;
        newNode->next = topPtr;
        topPtr = newNode;
        count++;
    }

    Booking pop() {
        if (isEmpty()) {
            cout << "Stack is empty!\n";
            return Booking(); 
        }
        
        BookingNode_Stack* temp = topPtr;
        Booking booking = temp->item;
        topPtr = topPtr->next;
        delete temp;
        count--;
        return booking;
    }

    Booking peek() {
        if (isEmpty()) {
            cout << "Stack is empty!\n";
            return Booking();
        }
        return topPtr->item;
    }

    int size() {
        return count;
    }

    void display() {
        if (isEmpty()) {
            cout << "No booking history.\n";
            return;
        }

        BookingNode_Stack* temp = topPtr;
        while (temp != NULL) {
            cout << "| " << setw(6) << temp->item.date
                 << " | " << setw(2) << temp->item.hour << ":00"
                 << " | " << setw(6) << temp->item.room
                 << " | " << setw(12) << temp->item.lecturer
                 << " | " << setw(10) << temp->item.course
                 << " |\n";
            temp = temp->next;
        }
    }
};

#endif