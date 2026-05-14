#ifndef COACH_H
#define COACH_H

#include "Utils.h"

struct CoachNode {
    int coachID;
    char type[50];
    int seatCount;
    CoachNode* next;
    CoachNode* prev;

    CoachNode() : coachID(0), seatCount(0), next(nullptr), prev(nullptr) {
        type[0] = '\0';
    }

    CoachNode(int id, const char* coachType, int seats) : coachID(id), seatCount(seats), next(nullptr), prev(nullptr) {
        strCopyN(type, coachType, 50);
    }
};

class CoachList {
private:
    CoachNode* head;
    CoachNode* tail;
    int count;

public:
    CoachList();
    ~CoachList();

    bool addFront(int id, const char* type, int seats);
    bool addBack(int id, const char* type, int seats);
    bool insertAt(int position, int id, const char* type, int seats);

    bool removeByID(int id);
    bool removeFront();
    bool removeBack();

    CoachNode* findCoach(int id) const;
    bool coachExists(int id) const;

    void traverseForward() const;
    void traverseBackward() const;

    void reverse();
    int getTotalSeats() const;
    int getCount() const;
    bool isEmpty() const;
    void clear();

    void displayDetailed() const;

    CoachNode* getHead() const {
        return head;
    }
    CoachNode* getTail() const {
        return tail;
    }
};

#endif
