#ifndef SEAT_H
#define SEAT_H

#include "Utils.h"

struct Seat {
    int seatNumber;
    bool isBooked;
    char passengerName[100];

    Seat() : seatNumber(-1), isBooked(false) { 
        passengerName[0] = '\0'; 
    }
    Seat(int number) : seatNumber(number), isBooked(false) { 
        passengerName[0] = '\0'; 
    }
};

struct HashSlot {
    Seat seat;
    bool occupied;
    bool deleted;

    HashSlot() : occupied(false), deleted(false) {}
};

class SeatHashTable {
private:
    HashSlot* table;
    int tableSize;
    int count;

    int hashFunction(int seatNumber) const;
    int findSlot(int seatNumber) const;

public:
    SeatHashTable();
    SeatHashTable(int size);
    ~SeatHashTable();

    void init(int size);

    bool insertSeat(int seatNumber);
    bool bookSeat(int seatNumber, const char* passenger);
    bool cancelSeat(int seatNumber);
    Seat* checkSeat(int seatNumber);

    void displayAll() const;
    int getBookedCount() const;
    int getAvailableCount() const;
    int getCount() const;
    int getTableSize() const {
        return tableSize;
    }

    HashSlot* getTable() const {
        return table;
    }
};

struct BSTNode {
    Seat seat;
    BSTNode* left;
    BSTNode* right;

    BSTNode() : left(nullptr), right(nullptr) {}
    BSTNode(int seatNumber) : seat(seatNumber), left(nullptr), right(nullptr) {}
};

class SeatBST {
private:
    BSTNode* root;
    int count;

    BSTNode* insertHelper(BSTNode* node, int seatNumber);
    BSTNode* removeHelper(BSTNode* node, int seatNumber);
    BSTNode* findMaxNode(BSTNode* node) const;
    BSTNode* searchHelper(BSTNode* node, int seatNumber) const;
    void inOrderHelper(BSTNode* node) const;
    void displayBookedHelper(BSTNode* node) const;
    void displayAvailableHelper(BSTNode* node) const;
    void freeTree(BSTNode* node);
    void preOrderSaveHelper(BSTNode* node, ostream& out) const;
    int countBookedHelper(BSTNode* node) const;

public:
    SeatBST();
    ~SeatBST();

    bool insert(int seatNumber);
    bool remove(int seatNumber);
    BSTNode* search(int seatNumber) const;

    bool bookSeat(int seatNumber, const char* passenger);
    bool cancelSeat(int seatNumber);

    void inOrder() const;
    void displayBooked() const;
    void displayAvailable() const;
    int getBookedCount() const;

    int getCount() const;
    bool isEmpty() const;
    void clear();

    BSTNode* getRoot() const {
        return root;
    }
    void preOrderSave(ostream& out) const;
};

class SeatManager {
private:
    SeatHashTable* hashTable;
    SeatBST* bst;
    int totalSeats;
    bool initialized;

public:
    SeatManager();
    ~SeatManager();

    bool initialize(int numSeats);
    bool bookSeat(int seatNumber, const char* passenger);
    bool cancelSeat(int seatNumber);
    Seat* checkSeat(int seatNumber) const;

    void displayAllSorted() const;
    void displayBooked() const;
    void displayAvailable() const;

    int getTotalSeats() const;
    int getBookedCount() const;
    bool isInitialized() const;

    SeatHashTable* getHashTable() const {
        return hashTable;
    }
    SeatBST* getBST() const {
        return bst;
    }
};

#endif
