#include "Seat.h"
#include <iomanip>

using namespace std;

SeatHashTable::SeatHashTable() : table(nullptr), tableSize(0), count(0) {}

SeatHashTable::SeatHashTable(int size) : count(0) {
    tableSize = size;
    table = new HashSlot[tableSize];
}

SeatHashTable::~SeatHashTable() {
    delete[] table;
    table = nullptr;
}

void SeatHashTable::init(int size) {
    delete[] table;
    tableSize = size;
    count = 0;
    table = new HashSlot[tableSize];
}

// multiplicative hash to spread seat numbers better
int SeatHashTable::hashFunction(int seatNumber) const {
    return ((seatNumber * 7) + 3) % tableSize;
}

// linear probing using iteration count instead of sentinel check
int SeatHashTable::findSlot(int seatNumber) const {
    int startIndex = hashFunction(seatNumber);
    for (int attempt = 0; attempt < tableSize; attempt++) {
        int index = (startIndex + attempt) % tableSize;
        if (!table[index].occupied) {
            if (!table[index].deleted) {
                return -1;
            }
        }
        if (table[index].occupied) {
            if (table[index].seat.seatNumber == seatNumber) {
                return index;
            }
        }
    }
    return -1;
}

bool SeatHashTable::insertSeat(int seatNumber) {
    if (count >= tableSize) {
        printError("Hash table is full.");
        return false;
    }
    if (findSlot(seatNumber) != -1) {
        return false;
    }

    int index = hashFunction(seatNumber);
    while (table[index].occupied) {
        index = (index + 1) % tableSize;
    }
    table[index].seat = Seat(seatNumber);
    table[index].occupied = true;
    table[index].deleted = false;
    count++;
    return true;
}

bool SeatHashTable::bookSeat(int seatNumber, const char* passenger) {
    int index = findSlot(seatNumber);
    if (index == -1) {
        printError("Seat does not exist.");
        return false;
    }
    if (table[index].seat.isBooked) {
        printError("Seat is already booked.");
        return false;
    }
    table[index].seat.isBooked = true;
    strCopyN(table[index].seat.passengerName, passenger, 100);
    return true;
}

bool SeatHashTable::cancelSeat(int seatNumber) {
    int index = findSlot(seatNumber);
    if (index == -1) {
        printError("Seat does not exist.");
        return false;
    }
    if (!table[index].seat.isBooked) {
        printError("Seat is not booked.");
        return false;
    }
    table[index].seat.isBooked = false;
    table[index].seat.passengerName[0] = '\0';
    return true;
}

Seat* SeatHashTable::checkSeat(int seatNumber) {
    int index = findSlot(seatNumber);
    if (index == -1) {
        return nullptr;
    }
    return &table[index].seat;
}

void SeatHashTable::displayAll() const {
    cout << endl;
    cout << CLR_BOLD << CLR_CYAN << "  Hash Table View (Size: " << tableSize << ", Occupied: " << count << ")" << CLR_RESET << endl;
    printSeparator('-', 55);

    for (int index = 0; index < tableSize; index++) {
        if (table[index].occupied) {
            cout << "  [" << CLR_DIM << setw(4) << index << CLR_RESET << "] Seat " << CLR_BOLD << setw(4) << table[index].seat.seatNumber << CLR_RESET << " ";
            if (table[index].seat.isBooked) {
                cout << CLR_RED << "BOOKED" << CLR_RESET << " -> " << CLR_WHITE << table[index].seat.passengerName << CLR_RESET;
            } else {
                cout << CLR_GREEN << "AVAILABLE" << CLR_RESET;
            }
            cout << endl;
        }
    }
    printSeparator('-', 55);
}

int SeatHashTable::getBookedCount() const {
    int booked = 0;
    for (int index = 0; index < tableSize; index++) {
        if (table[index].occupied) {
            if (table[index].seat.isBooked) {
                booked++;
            }
        }
    }
    return booked;
}

int SeatHashTable::getAvailableCount() const {
    return count - getBookedCount();
}

int SeatHashTable::getCount() const {
    return count;
}

SeatBST::SeatBST() : root(nullptr), count(0) {
}

SeatBST::~SeatBST() {
    clear();
}

BSTNode* SeatBST::insertHelper(BSTNode* node, int seatNumber) {
    if (!node) {
        count++;
        return new BSTNode(seatNumber);
    }
    if (seatNumber < node->seat.seatNumber) {
        node->left = insertHelper(node->left, seatNumber);
    } else if (seatNumber > node->seat.seatNumber) {
        node->right = insertHelper(node->right, seatNumber);
    }
    return node;
}

// predecessor approach: find max in left subtree
BSTNode* SeatBST::findMaxNode(BSTNode* node) const {
    while (node) {
        if (node->right == nullptr) {
            break;
        }
        node = node->right;
    }
    return node;
}

BSTNode* SeatBST::removeHelper(BSTNode* node, int seatNumber) {
    if (!node) {
        return nullptr;
    }
    if (seatNumber < node->seat.seatNumber) {
        node->left = removeHelper(node->left, seatNumber);
    } else if (seatNumber > node->seat.seatNumber) {
        node->right = removeHelper(node->right, seatNumber);
    } else {
        // leaf node: no children
        if (!node->left) {
            if (!node->right) {
                delete node;
                count--;
                return nullptr;
            }
            // only right child
            BSTNode* replacement = node->right;
            delete node;
            count--;
            return replacement;
        }
        if (!node->right) {
            // only left child
            BSTNode* replacement = node->left;
            delete node;
            count--;
            return replacement;
        }
        // two children: use predecessor (max of left subtree)
        BSTNode* predecessor = findMaxNode(node->left);
        node->seat = predecessor->seat;
        node->left = removeHelper(node->left, predecessor->seat.seatNumber);
    }
    return node;
}

BSTNode* SeatBST::searchHelper(BSTNode* node, int seatNumber) const {
    if (!node) {
        return nullptr;
    }
    if (seatNumber == node->seat.seatNumber) {
        return node;
    }
    if (seatNumber < node->seat.seatNumber) {
        return searchHelper(node->left, seatNumber);
    }
    return searchHelper(node->right, seatNumber);
}

void SeatBST::inOrderHelper(BSTNode* node) const {
    if (!node) {
        return;
    }
    inOrderHelper(node->left);
    cout << "  Seat " << CLR_BOLD << setw(4) << node->seat.seatNumber << CLR_RESET << " ";
    if (node->seat.isBooked) {
        cout << CLR_RED << "[BOOKED]" << CLR_RESET;
        cout << " -> " << CLR_WHITE << node->seat.passengerName << CLR_RESET;
    } else {
        cout << CLR_GREEN << "[AVAILABLE]" << CLR_RESET;
    }
    cout << endl;
    inOrderHelper(node->right);
}

void SeatBST::displayBookedHelper(BSTNode* node) const {
    if (!node) {
        return;
    }
    displayBookedHelper(node->left);
    if (node->seat.isBooked) {
        cout << "  Seat " << CLR_BOLD << setw(4) << node->seat.seatNumber << CLR_RESET << " -> " << CLR_WHITE << node->seat.passengerName << CLR_RESET << endl;
    }
    displayBookedHelper(node->right);
}

void SeatBST::displayAvailableHelper(BSTNode* node) const {
    if (!node) {
        return;
    }
    displayAvailableHelper(node->left);
    if (!node->seat.isBooked) {
        cout << "  Seat " << CLR_BOLD << CLR_GREEN << setw(4) << node->seat.seatNumber << CLR_RESET << endl;
    }
    displayAvailableHelper(node->right);
}

void SeatBST::freeTree(BSTNode* node) {
    if (!node) {
        return;
    }
    freeTree(node->left);
    freeTree(node->right);
    delete node;
}

int SeatBST::countBookedHelper(BSTNode* node) const {
    if (!node) {
        return 0;
    }
    int thisNode = node->seat.isBooked ? 1 : 0;
    return thisNode + countBookedHelper(node->left) + countBookedHelper(node->right);
}

bool SeatBST::insert(int seatNumber) {
    int before = count;
    root = insertHelper(root, seatNumber);
    return count > before;
}

bool SeatBST::remove(int seatNumber) {
    if (!search(seatNumber)) {
        return false;
    }
    root = removeHelper(root, seatNumber);
    return true;
}

BSTNode* SeatBST::search(int seatNumber) const {
    return searchHelper(root, seatNumber);
}

bool SeatBST::bookSeat(int seatNumber, const char* passenger) {
    BSTNode* node = search(seatNumber);
    if (!node) {
        return false;
    }
    if (node->seat.isBooked) {
        return false;
    }
    node->seat.isBooked = true;
    strCopyN(node->seat.passengerName, passenger, 100);
    return true;
}

bool SeatBST::cancelSeat(int seatNumber) {
    BSTNode* node = search(seatNumber);
    if (!node) {
        return false;
    }
    if (!node->seat.isBooked) {
        return false;
    }
    node->seat.isBooked = false;
    node->seat.passengerName[0] = '\0';
    return true;
}

void SeatBST::inOrder() const {
    if (!root) {
        printWarning("No seats in BST.");
        return;
    }
    cout << endl;
    cout << CLR_BOLD << CLR_CYAN << "  BST In-Order Traversal (Sorted View):" << CLR_RESET << endl;
    printSeparator('-', 55);
    inOrderHelper(root);
    printSeparator('-', 55);
}

void SeatBST::displayBooked() const {
    if (!root) {
        printWarning("No seats in BST.");
        return;
    }
    cout << endl;
    cout << CLR_BOLD << CLR_RED << "  Booked Seats:" << CLR_RESET << endl;
    printSeparator('-', 45);
    displayBookedHelper(root);
    printSeparator('-', 45);
    cout << CLR_DIM << "  Total booked: " << getBookedCount() << CLR_RESET << endl;
}

void SeatBST::displayAvailable() const {
    if (!root) {
        printWarning("No seats in BST.");
        return;
    }
    cout << endl;
    cout << CLR_BOLD << CLR_GREEN << "  Available Seats:" << CLR_RESET << endl;
    printSeparator('-', 45);
    displayAvailableHelper(root);
    printSeparator('-', 45);
    cout << CLR_DIM << "  Total available: " << (count - getBookedCount()) << CLR_RESET << endl;
}

int SeatBST::getBookedCount() const {
    return countBookedHelper(root);
}

int SeatBST::getCount() const {
    return count;
}

bool SeatBST::isEmpty() const {
    return root == nullptr;
}

void SeatBST::clear() {
    freeTree(root);
    root = nullptr;
    count = 0;
}

void SeatBST::preOrderSaveHelper(BSTNode* node, ostream& out) const {
    if (!node) {
        return;
    }
    out << node->seat.seatNumber << " "
        << (node->seat.isBooked ? 1 : 0) << " "
        << (node->seat.isBooked ? node->seat.passengerName : "-") << endl;
    preOrderSaveHelper(node->left, out);
    preOrderSaveHelper(node->right, out);
}

void SeatBST::preOrderSave(ostream& out) const {
    preOrderSaveHelper(root, out);
}

SeatManager::SeatManager() : hashTable(nullptr), bst(nullptr), totalSeats(0), initialized(false) {
}

SeatManager::~SeatManager() {
    delete hashTable;
    hashTable = nullptr;
    delete bst;
    bst = nullptr;
}

bool SeatManager::initialize(int numSeats) {
    if (numSeats <= 0) {
        printError("Number of seats must be positive.");
        return false;
    }
    delete hashTable;
    delete bst;

    totalSeats = numSeats;
    // find a prime size larger than double the seat count for low load factor
    int hashSize = numSeats * 2 + 1;
    while (true) {
        bool isPrime = true;
        if (hashSize < 2) { 
            hashSize = 2; 
            break; 
        }
        for (int divisor = 2; divisor * divisor <= hashSize; divisor++) {
            if (hashSize % divisor == 0) { 
                isPrime = false; 
                break; 
            }
        }
        if (isPrime) {
            break;
        }
        hashSize++;
    }

    hashTable = new SeatHashTable(hashSize);
    bst = new SeatBST();

    for (int seatNum = 1; seatNum <= numSeats; seatNum++) {
        hashTable->insertSeat(seatNum);
        bst->insert(seatNum);
    }

    initialized = true;
    return true;
}

bool SeatManager::bookSeat(int seatNumber, const char* passenger) {
    if (!initialized) {
        printError("Seat chart not initialized yet.");
        return false;
    }
    if (seatNumber < 1 || seatNumber > totalSeats) {
        printError("Seat number out of range.");
        return false;
    }
    bool hashResult = hashTable->bookSeat(seatNumber, passenger);
    if (!hashResult) {
        return false;
    }
    bst->bookSeat(seatNumber, passenger);
    return true;
}

bool SeatManager::cancelSeat(int seatNumber) {
    if (!initialized) {
        printError("Seat chart not initialized.");
        return false;
    }
    if (seatNumber < 1 || seatNumber > totalSeats) {
        printError("Seat number out of range.");
        return false;
    }
    bool hashResult = hashTable->cancelSeat(seatNumber);
    if (!hashResult) {
        return false;
    }
    bst->cancelSeat(seatNumber);
    return true;
}

Seat* SeatManager::checkSeat(int seatNumber) const {
    if (!initialized) {
        return nullptr;
    }
    if (seatNumber < 1 || seatNumber > totalSeats) {
        return nullptr;
    }
    return hashTable->checkSeat(seatNumber);
}

void SeatManager::displayAllSorted() const {
    if (!initialized) {
        printError("Seat chart not initialized.");
        return;
    }
    bst->inOrder();
    cout << CLR_BOLD << "  Summary: ";
    cout << CLR_RED << getBookedCount() << " booked" << CLR_RESET;
    cout << CLR_BOLD << " / ";
    cout << CLR_GREEN << (totalSeats - getBookedCount()) << " available" << CLR_RESET;
    cout << CLR_BOLD << " / " << totalSeats << " total" << CLR_RESET << endl;
}

void SeatManager::displayBooked() const {
    if (!initialized) {
        printError("Seat chart not initialized.");
        return;
    }
    bst->displayBooked();
}

void SeatManager::displayAvailable() const {
    if (!initialized) {
        printError("Seat chart not initialized.");
        return;
    }
    bst->displayAvailable();
}

int SeatManager::getTotalSeats() const {
    return totalSeats;
}

int SeatManager::getBookedCount() const {
    if (!initialized) {
        return 0;
    }
    return hashTable->getBookedCount();
}

bool SeatManager::isInitialized() const {
    return initialized;
}
