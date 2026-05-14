#include "Coach.h"

using namespace std;

CoachList::CoachList() : head(nullptr), tail(nullptr), count(0) {
}

CoachList::~CoachList() {
    clear();
}

bool CoachList::addFront(int id, const char* type, int seats) {
    if (id <= 0) {
        printError("Coach ID must be a positive number.");
        return false;
    }
    if (seats <= 0) {
        printError("Seat count must be a positive number.");
        return false;
    }
    if (coachExists(id)) {
        printError("Coach ID already exists in this train.");
        return false;
    }
    CoachNode* newNode = new CoachNode(id, type, seats);
    if (isEmpty()) {
        head = tail = newNode;
    } else {
        newNode->next = head;
        head->prev = newNode;
        head = newNode;
    }
    count++;
    return true;
}

bool CoachList::addBack(int id, const char* type, int seats) {
    if (id <= 0) {
        printError("Coach ID must be a positive number.");
        return false;
    }
    if (seats <= 0) {
        printError("Seat count must be a positive number.");
        return false;
    }
    if (coachExists(id)) {
        printError("Coach ID already exists in this train.");
        return false;
    }
    CoachNode* newNode = new CoachNode(id, type, seats);
    if (isEmpty()) {
        head = tail = newNode;
    } else {
        newNode->prev = tail;
        tail->next = newNode;
        tail = newNode;
    }
    count++;
    return true;
}

bool CoachList::insertAt(int position, int id, const char* type, int seats) {
    if (position < 0 || position > count) {
        printError("Position is out of range.");
        return false;
    }
    if (id <= 0) {
        printError("Coach ID must be a positive number.");
        return false;
    }
    if (seats <= 0) {
        printError("Seat count must be a positive number.");
        return false;
    }
    if (coachExists(id)) {
        printError("Coach ID already exists in this train.");
        return false;
    }
    if (position == 0) {
        return addFront(id, type, seats);
    }
    if (position == count) {
        return addBack(id, type, seats);
    }

    CoachNode* newNode = new CoachNode(id, type, seats);

    // walk from whichever end is closer to the target position
    CoachNode* target;
    if (position <= count / 2) {
        target = head;
        for (int step = 0; step < position; step++)
            target = target->next;
    } else {
        target = tail;
        for (int step = count - 1; step > position; step--) {
            target = target->prev;
        }
    }

    newNode->prev = target->prev;
    newNode->next = target;
    target->prev->next = newNode;
    target->prev = newNode;
    count++;
    return true;
}

bool CoachList::removeByID(int id) {
    CoachNode* target = findCoach(id);
    if (!target) {
        printError("Coach not found.");
        return false;
    }
    if (target == head && target == tail) {
        head = tail = nullptr;
    } else if (target == head) {
        head = head->next;
        head->prev = nullptr;
    } else if (target == tail) {
        tail = tail->prev;
        tail->next = nullptr;
    } else {
        target->prev->next = target->next;
        target->next->prev = target->prev;
    }
    delete target;
    count--;
    return true;
}

bool CoachList::removeFront() {
    if (isEmpty()) {
        printError("No coaches to remove.");
        return false;
    }
    CoachNode* toDelete = head;
    if (head == tail) {
        head = tail = nullptr;
    } else {
        head = head->next;
        head->prev = nullptr;
    }
    delete toDelete;
    count--;
    return true;
}

bool CoachList::removeBack() {
    if (isEmpty()) {
        printError("No coaches to remove.");
        return false;
    }
    CoachNode* toDelete = tail;
    if (head == tail) {
        head = tail = nullptr;
    } else {
        tail = tail->prev;
        tail->next = nullptr;
    }
    delete toDelete;
    count--;
    return true;
}

CoachNode* CoachList::findCoach(int id) const {
    CoachNode* currentNode = head;
    while (currentNode) {
        if (currentNode->coachID == id) {
            return currentNode;
        }
        currentNode = currentNode->next;
    }
    return nullptr;
}

bool CoachList::coachExists(int id) const {
    if (findCoach(id) != nullptr) {
        return true;
    }
    return false;
}

void CoachList::traverseForward() const {
    if (isEmpty()) {
        printWarning("No coaches in this train.");
        return;
    }
    cout << endl;
    cout << CLR_CYAN << "  Engine " << CLR_RESET;
    CoachNode* currentNode = head;
    while (currentNode) {
        cout << CLR_YELLOW << " --> " << CLR_RESET;
        cout << CLR_WHITE << "[";
        cout << CLR_GREEN << "C" << currentNode->coachID;
        cout << CLR_WHITE << " | ";
        cout << CLR_CYAN << currentNode->type;
        cout << CLR_WHITE << " | ";
        cout << CLR_MAGENTA << currentNode->seatCount << " seats";
        cout << CLR_WHITE << "]" << CLR_RESET;
        currentNode = currentNode->next;
    }
    cout << CLR_YELLOW << " --> " << CLR_RED << "Caboose" << CLR_RESET << endl;
}

void CoachList::traverseBackward() const {
    if (isEmpty()) {
        printWarning("No coaches in this train.");
        return;
    }
    cout << endl;
    cout << CLR_RED << "  Caboose " << CLR_RESET;
    CoachNode* currentNode = tail;
    while (currentNode) {
        cout << CLR_YELLOW << " --> " << CLR_RESET;
        cout << CLR_WHITE << "[";
        cout << CLR_GREEN << "C" << currentNode->coachID;
        cout << CLR_WHITE << " | ";
        cout << CLR_CYAN << currentNode->type;
        cout << CLR_WHITE << " | ";
        cout << CLR_MAGENTA << currentNode->seatCount << " seats";
        cout << CLR_WHITE << "]" << CLR_RESET;
        currentNode = currentNode->prev;
    }
    cout << CLR_YELLOW << " --> " << CLR_CYAN << "Engine" << CLR_RESET << endl;
}

void CoachList::reverse() {
    if (isEmpty()) {
        return;
    }
    if (count == 1) {
        return;
    }
    CoachNode* currentNode = head;
    while (currentNode) {
        CoachNode* holder = currentNode->prev;
        currentNode->prev = currentNode->next;
        currentNode->next = holder;
        currentNode = currentNode->prev;
    }
    CoachNode* holder = head;
    head = tail;
    tail = holder;
}

int CoachList::getTotalSeats() const {
    int total = 0;
    CoachNode* currentNode = head;
    while (currentNode) {
        total = total + currentNode->seatCount;
        currentNode = currentNode->next;
    }
    return total;
}

int CoachList::getCount() const {
    return count;
}

bool CoachList::isEmpty() const {
    return count == 0;
}

void CoachList::clear() {
    while (head) {
        CoachNode* toDelete = head;
        head = head->next;
        delete toDelete;
    }
    head = tail = nullptr;
    count = 0;
}

void CoachList::displayDetailed() const {
    if (isEmpty()) {
        printWarning("No coaches attached to this train.");
        return;
    }
    cout << endl;
    printDoubleSeparator(55);
    cout << CLR_BOLD << CLR_CYAN << "  Coach#   Type             Seats     Position" << CLR_RESET << endl;
    printSeparator('-', 55);

    CoachNode* currentNode = head;
    int position = 1;
    while (currentNode) {
        cout << "  " << CLR_GREEN << CLR_BOLD;
        cout.width(8);
        cout << left << currentNode->coachID << CLR_RESET;
        cout << " " << CLR_WHITE;
        char paddedType[18];
        strCopyN(paddedType, currentNode->type, 18);
        cout.width(17);
        cout << left << paddedType << CLR_RESET;
        cout << " " << CLR_MAGENTA;
        cout.width(9);
        cout << left << currentNode->seatCount << CLR_RESET;
        cout << " " << CLR_YELLOW << position << CLR_RESET;
        cout << endl;
        currentNode = currentNode->next;
        position++;
    }

    printSeparator('-', 55);
    cout << CLR_BOLD << "  Total Coaches: " << CLR_GREEN << count << CLR_RESET;
    cout << CLR_BOLD << "  |  Total Seats: " << CLR_MAGENTA << getTotalSeats() << CLR_RESET << endl;
    printDoubleSeparator(55);
}
