#include "UndoRedo.h"
#include "Train.h"
#include "Network.h"

using namespace std;

UndoRedoManager::UndoRedoManager() : undoTop(nullptr), redoTop(nullptr), undoCount(0), redoCount(0) {
}

UndoRedoManager::~UndoRedoManager() {
    clearAll();
}

void UndoRedoManager::pushToUndo(ActionType type, const ActionData& data) {
    ActionNode* node = new ActionNode(type, data);
    node->next = undoTop;
    undoTop = node;
    undoCount++;
}

void UndoRedoManager::pushToRedo(ActionType type, const ActionData& data) {
    ActionNode* node = new ActionNode(type, data);
    node->next = redoTop;
    redoTop = node;
    redoCount++;
}

ActionNode UndoRedoManager::popFromUndo() {
    ActionNode result;
    if (!undoTop) {
        return result;
    }
    ActionNode* toRemove = undoTop;
    result = *undoTop;
    result.next = nullptr;
    undoTop = undoTop->next;
    delete toRemove;
    undoCount--;
    return result;
}

ActionNode UndoRedoManager::popFromRedo() {
    ActionNode result;
    if (!redoTop) {
        return result;
    }
    ActionNode* toRemove = redoTop;
    result = *redoTop;
    result.next = nullptr;
    redoTop = redoTop->next;
    delete toRemove;
    redoCount--;
    return result;
}

void UndoRedoManager::clearStack(ActionNode*& stackTop, int& stackCount) {
    while (stackTop) {
        ActionNode* toRemove = stackTop;
        stackTop = stackTop->next;
        delete toRemove;
    }
    stackCount = 0;
}

// maps each action to its reverse using if-else instead of switch
ActionType UndoRedoManager::getOppositeAction(ActionType type) const {
    if (type == ACT_ADD_TRAIN) { 
        return ACT_REMOVE_TRAIN; 
    }
    if (type == ACT_REMOVE_TRAIN) { 
        return ACT_ADD_TRAIN; 
    }
    if (type == ACT_ADD_COACH_FRONT) { 
        return ACT_REMOVE_COACH; 
    }
    if (type == ACT_ADD_COACH_BACK) { 
        return ACT_REMOVE_COACH; 
    }
    if (type == ACT_ADD_COACH_AT) { 
        return ACT_REMOVE_COACH; 
    }
    if (type == ACT_REMOVE_COACH) { 
        return ACT_ADD_COACH_BACK; 
    }
    if (type == ACT_REVERSE_COACHES) { 
        return ACT_REVERSE_COACHES; 
    }
    if (type == ACT_BOOK_SEAT) { 
        return ACT_CANCEL_SEAT; 
    }
    if (type == ACT_CANCEL_SEAT) { 
        return ACT_BOOK_SEAT; 
    }
    if (type == ACT_ADD_STATION) { 
        return ACT_REMOVE_STATION; 
    }
    if (type == ACT_REMOVE_STATION) { 
        return ACT_ADD_STATION; 
    }
    if (type == ACT_ADD_ROUTE) { 
        return ACT_REMOVE_ROUTE; 
    }
    if (type == ACT_REMOVE_ROUTE) { 
        return ACT_ADD_ROUTE; 
    }
    return ACT_NONE;
}

bool UndoRedoManager::executeAction(ActionType type, const ActionData& data, TrainRegistry& registry, RailwayNetwork& network) {
    AVLNode* train = nullptr;

    if (type == ACT_ADD_TRAIN) {
        return registry.insert(data.trainID, data.trainName);
    }

    if (type == ACT_REMOVE_TRAIN) {
        return registry.remove(data.trainID);
    }

    if (type == ACT_ADD_COACH_FRONT) {
        train = registry.search(data.trainID);
        if (!train) {
            return false;
        }
        return train->coaches->addFront(data.coachID, data.coachType, data.coachSeatCount);
    }

    if (type == ACT_ADD_COACH_BACK) {
        train = registry.search(data.trainID);
        if (!train) {
            return false;
        }
        return train->coaches->addBack(data.coachID, data.coachType, data.coachSeatCount);
    }

    if (type == ACT_ADD_COACH_AT) {
        train = registry.search(data.trainID);
        if (!train) {
            return false;
        }
        return train->coaches->insertAt(data.coachPosition, data.coachID, data.coachType, data.coachSeatCount);
    }

    if (type == ACT_REMOVE_COACH) {
        train = registry.search(data.trainID);
        if (!train) {
            return false;
        }
        return train->coaches->removeByID(data.coachID);
    }

    if (type == ACT_REVERSE_COACHES) {
        train = registry.search(data.trainID);
        if (!train) {
            return false;
        }
        train->coaches->reverse();
        return true;
    }

    if (type == ACT_BOOK_SEAT) {
        train = registry.search(data.trainID);
        if (!train || !train->seats) {
            return false;
        }
        return train->seats->bookSeat(data.seatNumber, data.passengerName);
    }

    if (type == ACT_CANCEL_SEAT) {
        train = registry.search(data.trainID);
        if (!train || !train->seats) {
            return false;
        }
        return train->seats->cancelSeat(data.seatNumber);
    }

    if (type == ACT_ADD_STATION) {
        if (network.addStationWithID(data.stationID, data.stationName)) {
            return true;
        }
        return network.addStation(data.stationName) >= 0;
    }

    if (type == ACT_REMOVE_STATION) {
        return network.removeStation(data.stationID);
    }

    if (type == ACT_ADD_ROUTE) {
        return network.addRoute(data.srcStation, data.destStation, data.distance, data.travelTime);
    }

    if (type == ACT_REMOVE_ROUTE) {
        return network.removeRoute(data.srcStation, data.destStation);
    }

    return false;
}

void UndoRedoManager::recordAction(ActionType type, const ActionData& data) {
    pushToUndo(type, data);
    clearRedo();
}

bool UndoRedoManager::undo(TrainRegistry& registry, RailwayNetwork& network) {
    if (!canUndo()) {
        printError("Nothing to undo.");
        return false;
    }

    ActionNode action = popFromUndo();
    ActionType opposite = getOppositeAction(action.type);

    bool worked = executeAction(opposite, action.data, registry, network);
    if (worked) {
        pushToRedo(action.type, action.data);
        printSuccess("Undo successful.");
    } else {
        printError("Undo failed - could not reverse action.");
        pushToUndo(action.type, action.data);
    }
    return worked;
}

bool UndoRedoManager::redo(TrainRegistry& registry, RailwayNetwork& network) {
    if (!canRedo()) {
        printError("Nothing to redo.");
        return false;
    }

    ActionNode action = popFromRedo();

    bool worked = executeAction(action.type, action.data, registry, network);
    if (worked) {
        pushToUndo(action.type, action.data);
        printSuccess("Redo successful.");
    } else {
        printError("Redo failed - could not re-apply action.");
        pushToRedo(action.type, action.data);
    }
    return worked;
}

bool UndoRedoManager::canUndo() const {
    return undoTop != nullptr;
}

bool UndoRedoManager::canRedo() const {
    return redoTop != nullptr;
}

void UndoRedoManager::clearRedo() {
    clearStack(redoTop, redoCount);
}

void UndoRedoManager::clearAll() {
    clearStack(undoTop, undoCount);
    clearStack(redoTop, redoCount);
}

// uses an array of names instead of switch for displaying action type
void UndoRedoManager::displayStatus() const {
    const char* actionLabels[] = {
        "Unknown", "Add Train", "Remove Train", "Add Coach (Front)", "Add Coach (Back)", "Add Coach (Position)", "Remove Coach", "Reverse Coaches", "Book Seat", "Cancel Seat", "Add Station", "Remove Station", "Add Route", "Remove Route"
    };

    cout << endl;
    printSeparator('-', 45);
    cout << CLR_BOLD << CLR_CYAN << "  Undo/Redo Status" << CLR_RESET << endl;
    printSeparator('-', 45);
    cout << "  " << CLR_YELLOW << "Undo stack: " << CLR_WHITE << undoCount << " action(s)" << CLR_RESET << endl;
    cout << "  " << CLR_YELLOW << "Redo stack: " << CLR_WHITE << redoCount << " action(s)" << CLR_RESET << endl;

    if (undoTop) {
        int index = (int)undoTop->type;
        if (index < 0 || index > 13) {
            index = 0;
        }
        cout << "  " << CLR_GREEN << "Last action: " << CLR_WHITE << actionLabels[index] << CLR_RESET << endl;
    }
    printSeparator('-', 45);
}
