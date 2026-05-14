#ifndef LOGGER_H
#define LOGGER_H

#include "Utils.h"

struct LogEntry {
    char action[256];
    char module[50];
    char timestamp[30];
    LogEntry* next;

    LogEntry() : next(nullptr) {
        action[0] = '\0';
        module[0] = '\0';
        timestamp[0] = '\0';
    }

    LogEntry(const char* actionText, const char* moduleName) : next(nullptr) {
        strCopyN(action, actionText, 256);
        strCopyN(module, moduleName, 50);
        getCurrentTimestamp(timestamp, 30);
    }
};

class OperationLog {
private:
    LogEntry* top;
    int count;

public:
    OperationLog();
    ~OperationLog();

    void push(const char* action, const char* module);
    LogEntry pop();
    LogEntry* peek() const;

    void displayAll() const;
    void displayByModule(const char* module) const;
    void displayRecent(int howMany) const;

    int getSize() const;
    bool isEmpty() const;
    void clear();

    LogEntry* getTop() const {
        return top;
    }
};

#endif
