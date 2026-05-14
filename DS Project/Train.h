#ifndef TRAIN_H
#define TRAIN_H

#include "Utils.h"
#include "Coach.h"
#include "Seat.h"

struct AVLNode {
    int trainID;
    char name[100];
    CoachList* coaches;
    SeatManager* seats;
    AVLNode* left;
    AVLNode* right;
    int height;

    AVLNode() : trainID(0), coaches(nullptr), seats(nullptr), left(nullptr), right(nullptr), height(1) {
        name[0] = '\0';
    }

    AVLNode(int id, const char* trainName) : trainID(id), left(nullptr), right(nullptr), height(1) {
        strCopyN(name, trainName, 100);
        coaches = new CoachList();
        seats = nullptr;
    }

    ~AVLNode() {
        delete coaches;
        delete seats;
    }
};

class TrainRegistry {
private:
    AVLNode* root;
    int count;

    int getNodeHeight(AVLNode* node) const;
    int getBalanceFactor(AVLNode* node) const;
    AVLNode* rotateRight(AVLNode* node);
    AVLNode* rotateLeft(AVLNode* node);
    AVLNode* rebalance(AVLNode* node);

    AVLNode* insertHelper(AVLNode* node, int id, const char* name, bool& added);
    AVLNode* removeHelper(AVLNode* node, int id, bool& removed);
    AVLNode* findMinNode(AVLNode* node) const;
    AVLNode* findMaxNode(AVLNode* node) const;
    AVLNode* searchHelper(AVLNode* node, int id) const;

    void inOrderHelper(AVLNode* node) const;
    void preOrderHelper(AVLNode* node) const;
    void postOrderHelper(AVLNode* node) const;
    void freeTree(AVLNode* node);
    int countNodes(AVLNode* node) const;

public:
    TrainRegistry();
    ~TrainRegistry();

    bool insert(int id, const char* name);
    bool remove(int id);
    AVLNode* search(int id) const;

    void inOrder() const;
    void preOrder() const;
    void postOrder() const;

    void displayAll() const;
    void displayTrainDetails(int id) const;

    int getCount() const;
    bool isEmpty() const;
    void clear();

    AVLNode* getRoot() const {
        return root;
    }
};

#endif
