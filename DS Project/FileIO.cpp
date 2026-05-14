#define _CRT_SECURE_NO_WARNINGS
#include "FileIO.h"
#include <iomanip>

using namespace std;

void FileIO::ensureDataDir() {
#ifdef _WIN32
    system("if not exist data mkdir data");
#else
    system("mkdir -p data");
#endif
}

void FileIO::saveTrainNode(ofstream& out, AVLNode* node) {
    if (!node) {
        return;
    }

    out << "TRAIN " << node->trainID << " " << node->name << endl;

    int coachTotal = node->coaches->getCount();
    out << "COACHES " << coachTotal << endl;
    CoachNode* currentCoach = node->coaches->getHead();
    while (currentCoach) {
        out << "COACH " << currentCoach->coachID << " " << currentCoach->seatCount << " " << currentCoach->type << endl;
        currentCoach = currentCoach->next;
    }

    if (node->seats && node->seats->isInitialized()) {
        int seatTotal = node->seats->getTotalSeats();
        out << "SEATS " << seatTotal << endl;

        SeatHashTable* hashTable = node->seats->getHashTable();
        HashSlot* slots = hashTable->getTable();
        int slotCount = hashTable->getTableSize();
        for (int i = 0; i < slotCount; i++) {
            if (slots[i].occupied) {
                out << "SEAT " << slots[i].seat.seatNumber << " " << (slots[i].seat.isBooked ? 1 : 0) << " " << (slots[i].seat.isBooked ? slots[i].seat.passengerName : "-") << endl;
            }
        }
        out << "END_SEATS" << endl;
    } else {
        out << "SEATS 0" << endl;
    }

    out << "END_TRAIN" << endl;

    saveTrainNode(out, node->left);
    saveTrainNode(out, node->right);
}

bool FileIO::saveTrains(const TrainRegistry& registry, const char* filename) {
    ensureDataDir();
    ofstream out(filename);
    if (!out.is_open()) {
        printError("Could not open file for saving trains.");
        return false;
    }

    out << "IRON_NEXUS_TRAINS" << endl;
    out << "COUNT " << registry.getCount() << endl;
    saveTrainNode(out, registry.getRoot());
    out << "END_FILE" << endl;
    out.close();

    printSuccess("Train registry saved successfully.");
    return true;
}

void FileIO::loadTrainNode(ifstream& in, TrainRegistry& registry) {
    char tag[50];
    int trainID;
    char trainName[100];

    in >> tag;
    if (!strEquals(tag, "TRAIN")) {
        return;
    }

    in >> trainID;
    in.ignore();
    in.getline(trainName, 100);

    // skip leading whitespace in the name
    char* nameStart = trainName;
    while (*nameStart == ' ') {
        nameStart++;
    }

    registry.insert(trainID, nameStart);
    AVLNode* train = registry.search(trainID);
    if (!train) {
        return;
    }

    int coachTotal;
    in >> tag >> coachTotal;

    for (int i = 0; i < coachTotal; i++) {
        int coachID, seatCount;
        char coachType[50];
        in >> tag >> coachID >> seatCount;
        in.ignore();
        in.getline(coachType, 50);
        char* typeStart = coachType;
        while (*typeStart == ' ') {
            typeStart++;
        }
        train->coaches->addBack(coachID, typeStart, seatCount);
    }

    int seatTotal;
    in >> tag >> seatTotal;

    if (seatTotal > 0) {
        train->seats = new SeatManager();
        train->seats->initialize(seatTotal);

        char line[256];
        in.ignore();
        while (in.getline(line, 256)) {
            if (strEquals(line, "END_SEATS")) {
                break;
            }

            int seatNum, booked;
            char passenger[100];
            if (sscanf(line, "SEAT %d %d %99[^\n]", &seatNum, &booked, passenger) >= 2) {
                if (booked) {
                    char* passengerStart = passenger;
                    while (*passengerStart == ' ') passengerStart++;
                    train->seats->bookSeat(seatNum, passengerStart);
                }
            }
        }
    } else {
        in.ignore();
    }

    in >> tag;
}

bool FileIO::loadTrains(TrainRegistry& registry, const char* filename) {
    ifstream in(filename);
    if (!in.is_open()) {
        printError("Could not open train data file.");
        return false;
    }

    char tag[50];
    in >> tag;
    if (!strEquals(tag, "IRON_NEXUS_TRAINS")) {
        printError("Invalid train data file format.");
        in.close();
        return false;
    }

    int totalTrains;
    in >> tag >> totalTrains;

    registry.clear();
    for (int i = 0; i < totalTrains; i++) {
        loadTrainNode(in, registry);
    }

    in >> tag;
    in.close();

    printSuccess("Train registry loaded successfully.");
    return true;
}

bool FileIO::saveNetwork(const RailwayNetwork& network, const char* filename) {
    ensureDataDir();

    ofstream out(filename);
    if (!out.is_open()) {
        printError("Could not open file for saving network.");
        return false;
    }

    out << "IRON_NEXUS_NETWORK" << endl;
    out << "STATIONS " << network.getStationCount() << endl;

    const Station* allStations = network.getStations();
    for (int i = 0; i < MAX_STATIONS; i++) {
        if (allStations[i].active) {
            out << "STATION " << i << " " << allStations[i].name << endl;
        }
    }

    out << "ROUTES" << endl;
    const EdgeNode* const* adjacencyList = network.getAdjList();
    // only save routes where source < dest to avoid duplicates
    for (int i = 0; i < MAX_STATIONS; i++) {
        if (!allStations[i].active) {
            continue;
        }
        const EdgeNode* edge = adjacencyList[i];
        while (edge) {
            if (i < edge->destID) {
                out << "ROUTE " << i << " " << edge->destID << " ";
                out << edge->distance << " " << edge->travelTime << endl;
            }
            edge = edge->next;
        }
    }
    out << "END_ROUTES" << endl;
    out << "END_FILE" << endl;
    out.close();

    printSuccess("Railway network saved successfully.");
    return true;
}

bool FileIO::loadNetwork(RailwayNetwork& network, const char* filename) {
    ifstream in(filename);
    if (!in.is_open()) {
        printError("Could not open network data file.");
        return false;
    }

    char tag[50];
    in >> tag;
    if (!strEquals(tag, "IRON_NEXUS_NETWORK")) {
        printError("Invalid network data file format.");
        in.close();
        return false;
    }

    network.clear();

    int totalStations;
    in >> tag >> totalStations;

    for (int i = 0; i < totalStations; i++) {
        int id;
        char stationName[100];
        in >> tag >> id;
        in.ignore();
        in.getline(stationName, 100);
        char* nameStart = stationName;
        while (*nameStart == ' ') {
            nameStart++;
        }
        network.addStationWithID(id, nameStart);
    }

    in >> tag;

    char line[256];
    in.ignore();
    while (in.getline(line, 256)) {
        if (strEquals(line, "END_ROUTES")) {
            break;
        }
        int source, destination, dist, time;
        if (sscanf(line, "ROUTE %d %d %d %d", &source, &destination, &dist, &time) == 4) {
            network.addRoute(source, destination, dist, time);
        }
    }

    in >> tag;
    in.close();

    printSuccess("Railway network loaded successfully.");
    return true;
}

bool FileIO::saveLog(const OperationLog& log, const char* filename) {
    ensureDataDir();
    ofstream out(filename);
    if (!out.is_open()) {
        printError("Could not open file for saving log.");
        return false;
    }

    out << "IRON_NEXUS_LOG" << endl;
    out << "ENTRIES " << log.getSize() << endl;

    LogEntry* currentEntry = log.getTop();
    int total = log.getSize();
    if (total > 0) {
        // collect entries to reverse their order (oldest first in file)
        LogEntry** allEntries = new LogEntry*[total];
        int pos = 0;
        while (currentEntry && pos < total) {
            allEntries[pos] = currentEntry;
            pos++;
            currentEntry = currentEntry->next;
        }
        for (int i = pos - 1; i >= 0; i--) {
            out << "LOG|" << allEntries[i]->timestamp << "|" << allEntries[i]->module << "|" << allEntries[i]->action << endl;
        }
        delete[] allEntries;
    }

    out << "END_FILE" << endl;
    out.close();

    printSuccess("Operation log saved successfully.");
    return true;
}

bool FileIO::loadLog(OperationLog& log, const char* filename) {
    ifstream in(filename);
    if (!in.is_open()) {
        printError("Could not open log data file.");
        return false;
    }

    char tag[50];
    in >> tag;
    if (!strEquals(tag, "IRON_NEXUS_LOG")) {
        printError("Invalid log data file format.");
        in.close();
        return false;
    }

    int totalEntries;
    in >> tag >> totalEntries;
    in.ignore();

    log.clear();

    for (int i = 0; i < totalEntries; i++) {
        char line[512];
        in.getline(line, 512);

        char timeText[30] = "";
        char moduleText[50] = "";
        char actionText[256] = "";

        char* parser = line;
        // skip "LOG|" prefix
        if (parser[0] == 'L' && parser[1] == 'O' && parser[2] == 'G' && parser[3] == '|') {
            parser += 4;
        }

        // extract timestamp up to next |
        char* separator = strchr(parser, '|');
        if (separator) {
            int length = (int)(separator - parser);
            for (int j = 0; j < length && j < 29; j++) {
                timeText[j] = parser[j];
            }
            timeText[length < 29 ? length : 29] = '\0';
            parser = separator + 1;
        }

        // extract module up to next |
        separator = strchr(parser, '|');
        if (separator) {
            int length = (int)(separator - parser);
            for (int j = 0; j < length && j < 49; j++) {
                moduleText[j] = parser[j];
            }
            moduleText[length < 49 ? length : 49] = '\0';
            parser = separator + 1;
        }

        // rest is the action text
        strCopyN(actionText, parser, 256);

        log.push(actionText, moduleText);
    }

    in.close();
    printSuccess("Operation log loaded successfully.");
    return true;
}

bool FileIO::saveAll(const TrainRegistry& registry, const RailwayNetwork& network,
                     const OperationLog& log) {
    ensureDataDir();
    bool allGood = true;
    allGood = saveTrains(registry, "data/trains.dat") && allGood;
    allGood = saveNetwork(network, "data/network.dat") && allGood;
    allGood = saveLog(log, "data/log.dat") && allGood;

    if (allGood)
        printSuccess("All data saved successfully to data/ directory.");
    else
        printWarning("Some data may not have been saved correctly.");
    return allGood;
}

bool FileIO::loadAll(TrainRegistry& registry, RailwayNetwork& network, OperationLog& log) {
    bool allGood = true;
    allGood = loadTrains(registry, "data/trains.dat") && allGood;
    allGood = loadNetwork(network, "data/network.dat") && allGood;
    allGood = loadLog(log, "data/log.dat") && allGood;

    if (allGood)
        printSuccess("All data loaded successfully from data/ directory.");
    else
        printWarning("Some data may not have been loaded correctly.");
    return allGood;
}

void FileIO::saveBSTPreOrder(ofstream& out, BSTNode* node) {
    if (!node) {
        out << "NULL" << endl;
        return;
    }
    out << "NODE " << node->seat.seatNumber << " " << (node->seat.isBooked ? 1 : 0) << " " << (node->seat.isBooked ? node->seat.passengerName : "-") << endl;
    saveBSTPreOrder(out, node->left);
    saveBSTPreOrder(out, node->right);
}

bool FileIO::saveSeatBSTStructure(const TrainRegistry& registry, const char* filename) {
    ensureDataDir();
    ofstream out(filename);
    if (!out.is_open()) {
        printError("Could not open file for BST structure save.");
        return false;
    }

    out << "IRON_NEXUS_BST_STRUCTURE" << endl;

    // iterative pre-order traversal of AVL tree using array as stack
    AVLNode* stack[1000];
    int stackTop = -1;
    AVLNode* currentNode = registry.getRoot();

    if (currentNode) {
        stack[++stackTop] = currentNode;
    }

    while (stackTop >= 0) {
        AVLNode* node = stack[stackTop--];
        if (node->right) {
            stack[++stackTop] = node->right;
        }
        if (node->left) {
            stack[++stackTop] = node->left;
        }

        if (node->seats && node->seats->isInitialized()) {
            out << "TRAIN_BST " << node->trainID << " " << node->name << endl;
            saveBSTPreOrder(out, node->seats->getBST()->getRoot());
            out << "END_TRAIN_BST" << endl << endl;
        }
    }

    out << "END_FILE" << endl;
    out.close();

    printSuccess("BST structure saved successfully (hierarchical format).");
    return true;
}
