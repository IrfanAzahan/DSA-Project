#ifndef QUEUE_HPP
#define QUEUE_HPP

#include <iostream>
#include <string>
using namespace std;

struct Booking {
    string date;
    int hour;
    string room;
    string lecturer;
    string course;
};

class nodeQueue {
    public:
    char item;
    nodeQueue *next;
};

class Queue {
    public:
    nodeQueue *backPtr, *frontPtr;

    void createQueue() {
        backPtr = NULL;
        frontPtr = NULL;
    }

    void destroyQueue() {
        nodeQueue *temp = frontPtr;
        while(temp) {
            frontPtr = temp->next;
            delete temp;
            temp = frontPtr;
        }
    }

    bool isEmpty() {
        return (backPtr == NULL && frontPtr == NULL);
    }

    void enQueue(string value) {
        nodeQueue *newNode = new nodeQueue;
        newNode->item = value[0];
        newNode->next = nullptr;

        if (backPtr == nullptr) {
            frontPtr = backPtr = newNode;
        } else {
            backPtr->next = newNode;
            backPtr = newNode;
        }
    }

    void deQueue() {
        if (isEmpty()) {
            cout << "Queue is already empty.\n";
            return;
        } else {
            nodeQueue *temp = frontPtr;
            frontPtr = frontPtr->next;
            delete temp;

            if (frontPtr == nullptr) {
                backPtr = nullptr;
            }
        } 
    }

    void getFront() {
        if (isEmpty()) {
            cout << "Queue is empty.\n";
            return;
        }
        cout << frontPtr->item << endl;
    }

    void getRear() {
        if (isEmpty()) {
            cout << "Queue is empty.\n";
            return;
        }
        cout << backPtr->item << endl;
    }
};

class BookingNode {
    public:
    Booking* item;
    BookingNode *next;
};

class WaitlistQueue {
    public:
    BookingNode *backPtr, *frontPtr;

    void createQueue() {
        backPtr = NULL;
        frontPtr = NULL;
    }

    void destroyQueue() {
        BookingNode *temp = frontPtr;
        while(temp) {
            frontPtr = temp->next;
            delete temp->item;
            delete temp;
            temp = frontPtr;
        }
        backPtr = NULL;
        frontPtr = NULL;
    }

    bool isEmpty() {
        return (backPtr == NULL && frontPtr == NULL);
    }

    void enQueue(Booking b) {
        BookingNode *newNode = new BookingNode;
        newNode->item = new Booking(b);
        newNode->next = nullptr;

        if (backPtr == nullptr) {
            frontPtr = backPtr = newNode;
        } else {
            backPtr->next = newNode;
            backPtr = newNode;
        }
    }

    Booking* deQueue() {
        if (isEmpty()) {
            return nullptr;
        } else {
            BookingNode *temp = frontPtr;
            Booking* booking = temp->item;
            frontPtr = frontPtr->next;
            delete temp;

            if (frontPtr == nullptr) {
                backPtr = nullptr;
            }
            return booking;
        } 
    }

    Booking* getFront() {
        if (isEmpty()) {
            return nullptr;
        }
        return frontPtr->item;
    }

    Booking* getRear() {
        if (isEmpty()) {
            return nullptr;
        }
        return backPtr->item;
    }

    int getSize() {
        int count = 0;
        BookingNode* temp = frontPtr;
        while (temp) {
            count++;
            temp = temp->next;
        }
        return count;
    }

    void display() {
        if (isEmpty()) {
            cout << "No one in waitlist.\n";
            return;
        }
        
        BookingNode* temp = frontPtr;
        int position = 1;
        while (temp) {
            cout << position << ". Lecturer: " << temp->item->lecturer 
                 << " | Course: " << temp->item->course << "\n";
            temp = temp->next;
            position++;
        }
    }
};

#endif