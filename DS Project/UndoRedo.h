#ifndef UNDOREDO_H
#define UNDOREDO_H

#include "Utils.h"

enum ActionType {
    ACT_NONE = 0,
    ACT_ADD_TRAIN,
    ACT_REMOVE_TRAIN,
    ACT_ADD_COACH_FRONT,
    ACT_ADD_COACH_BACK,
    ACT_ADD_COACH_AT,
    ACT_REMOVE_COACH,
    ACT_REVERSE_COACHES,
    ACT_BOOK_SEAT,
    ACT_CANCEL_SEAT,
    ACT_ADD_STATION,
    ACT_REMOVE_STATION,
    ACT_ADD_ROUTE,
    ACT_REMOVE_ROUTE
};

struct ActionData {
    int trainID;
    char trainName[100];
    int coachID;
    char coachType[50];
    int coachSeatCount;
    int coachPosition;
    int seatNumber;
    char passengerName[100];
    int stationID;
    char stationName[100];
    int srcStation;
    int destStation;
    int distance;
    int travelTime;

    ActionData() {
        trainID = 0;
        trainName[0] = '\0';
        coachID = 0;
        coachType[0] = '\0';
        coachSeatCount = 0;
        coachPosition = 0;
        seatNumber = 0;
        passengerName[0] = '\0';
        stationID = 0;
        stationName[0] = '\0';
        srcStation = 0;
        destStation = 0;
        distance = 0;
        travelTime = 0;
    }
};

struct ActionNode {
    ActionType type;
    ActionData data;
    ActionNode* next;

    ActionNode() : type(ACT_NONE), next(nullptr) {}
    ActionNode(ActionType actionType, const ActionData& actionData) : type(actionType), data(actionData), next(nullptr) {}
};

class TrainRegistry;
class RailwayNetwork;

class UndoRedoManager {
private:
    ActionNode* undoTop;
    ActionNode* redoTop;
    int undoCount;
    int redoCount;

    void pushToUndo(ActionType type, const ActionData& data);
    void pushToRedo(ActionType type, const ActionData& data);
    ActionNode popFromUndo();
    ActionNode popFromRedo();
    void clearStack(ActionNode*& stackTop, int& stackCount);

    bool executeAction(ActionType type, const ActionData& data, TrainRegistry& registry, RailwayNetwork& network);
    ActionType getOppositeAction(ActionType type) const;

public:
    UndoRedoManager();
    ~UndoRedoManager();

    void recordAction(ActionType type, const ActionData& data);
    bool undo(TrainRegistry& registry, RailwayNetwork& network);
    bool redo(TrainRegistry& registry, RailwayNetwork& network);

    bool canUndo() const;
    bool canRedo() const;
    void clearRedo();
    void clearAll();

    void displayStatus() const;
};

#endif
