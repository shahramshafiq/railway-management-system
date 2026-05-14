#include "Logger.h"

using namespace std;

OperationLog::OperationLog() : top(nullptr), count(0) {
}

OperationLog::~OperationLog() {
    clear();
}

void OperationLog::push(const char* action, const char* module) {
    LogEntry* entry = new LogEntry(action, module);
    entry->next = top;
    top = entry;
    count++;
}

LogEntry OperationLog::pop() {
    LogEntry result;
    if (isEmpty()) {
        printError("Log is empty.");
        return result;
    }
    LogEntry* toRemove = top;
    result = *top;
    result.next = nullptr;
    top = top->next;
    delete toRemove;
    count--;
    return result;
}

LogEntry* OperationLog::peek() const {
    if (isEmpty()) {
        return nullptr;
    }
    return top;
}

void OperationLog::displayAll() const {
    if (isEmpty()) {
        printWarning("Operation log is empty.");
        return;
    }
    cout << endl;
    printDoubleSeparator(70);
    cout << CLR_BOLD << CLR_CYAN;
    cout << "  #     Timestamp            Module          Action" << CLR_RESET << endl;
    printSeparator('-', 70);

    LogEntry* currentEntry = top;
    int number = 1;
    while (currentEntry) {
        cout << "  " << CLR_DIM;
        cout.width(5);
        cout << left << number << CLR_RESET;
        cout << " " << CLR_GRAY << currentEntry->timestamp << CLR_RESET;
        cout << "  " << CLR_YELLOW;
        cout.width(14);
        cout << left << currentEntry->module << CLR_RESET;
        cout << "  " << CLR_WHITE << currentEntry->action << CLR_RESET << endl;
        currentEntry = currentEntry->next;
        number++;
    }

    printSeparator('-', 70);
    cout << CLR_DIM << "  Total entries: " << count << CLR_RESET << endl;
    printDoubleSeparator(70);
}

void OperationLog::displayByModule(const char* module) const {
    if (isEmpty()) {
        printWarning("Operation log is empty.");
        return;
    }

    cout << endl;
    cout << CLR_BOLD << CLR_CYAN << "  Logs for module: " << CLR_YELLOW << module << CLR_RESET << endl;
    printSeparator('-', 60);

    LogEntry* currentEntry = top;
    int matched = 0;
    while (currentEntry) {
        if (strEqualsNoCase(currentEntry->module, module)) {
            cout << "  " << CLR_GRAY << currentEntry->timestamp << CLR_RESET << "  " << CLR_WHITE << currentEntry->action << CLR_RESET << endl;
            matched++;
        }
        currentEntry = currentEntry->next;
    }

    if (matched == 0) {
        printWarning("No log entries found for this module.");
    } else {
        printSeparator('-', 60);
        cout << CLR_DIM << "  Found " << matched << " entries." << CLR_RESET << endl;
    }
}

void OperationLog::displayRecent(int howMany) const {
    if (isEmpty()) {
        printWarning("Operation log is empty.");
        return;
    }
    if (howMany <= 0) {
        printError("Count must be a positive number.");
        return;
    }

    cout << endl;
    cout << CLR_BOLD << CLR_CYAN << "  Most Recent " << howMany << " Operations:" << CLR_RESET << endl;
    printSeparator('-', 60);

    LogEntry* currentEntry = top;
    int shown = 0;
    while (currentEntry && shown < howMany) {
        cout << "  " << CLR_GRAY << currentEntry->timestamp << CLR_RESET;
        cout << "  [" << CLR_YELLOW << currentEntry->module << CLR_RESET << "] ";
        cout << CLR_WHITE << currentEntry->action << CLR_RESET << endl;
        currentEntry = currentEntry->next;
        shown++;
    }
    printSeparator('-', 60);
}

int OperationLog::getSize() const {
    return count;
}

bool OperationLog::isEmpty() const {
    return top == nullptr;
}

void OperationLog::clear() {
    while (top) {
        LogEntry* toRemove = top;
        top = top->next;
        delete toRemove;
    }
    top = nullptr;
    count = 0;
}
