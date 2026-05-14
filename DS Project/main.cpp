#include "Train.h"
#include "Coach.h"
#include "Seat.h"
#include "Network.h"
#include "Logger.h"
#include "UndoRedo.h"
#include "FileIO.h"
#include <cstdio>

using namespace std;

TrainRegistry registry;
RailwayNetwork network;
OperationLog logger;
UndoRedoManager undoRedo;

void mainMenu();
void trainRegistryMenu();
void coachManagementMenu();
void railwayNetworkMenu();
void seatingChartMenu();
void operationLogMenu();
void undoRedoMenu();
void saveLoadMenu();

int main() {
    enableColors();
    clearScreen();
    printBanner();

    cout << CLR_DIM << "  Initializing system..." << CLR_RESET << endl << endl;
    logger.push("System initialized", "System");

    pauseScreen();
    mainMenu();

    return 0;
}

void mainMenu() {
    while (true) {
        clearScreen();

        cout << endl;
        cout << CLR_CYAN << CLR_BOLD << "    THE IRON NEXUS" << CLR_RESET << CLR_DIM << " | Railway Management System" << CLR_RESET << endl;

        printRailwayArt();

        printMenuHeader("MAIN MENU");
        printMenuOption(1, "Train Registry");
        printMenuOption(2, "Coach Management");
        printMenuOption(3, "Railway Track Network");
        printMenuOption(4, "Seating Chart");
        printMenuOption(5, "Operation Log");
        printMenuOption(6, "Undo / Redo");
        printMenuOption(7, "Save / Load Data");
        printMenuOption(0, "Exit System");
        printMenuFooter();

        int choice;
        cout << "  > Select option: ";
        if (!readInt(choice)) {
            printError("Invalid input. Please enter a number.");
            pauseScreen();
            continue;
        }

        if (choice == 1) {
            trainRegistryMenu();
        }
        else if (choice == 2) {
            coachManagementMenu();
        }
        else if (choice == 3) {
            railwayNetworkMenu();
        }
        else if (choice == 4) {
            seatingChartMenu();
        }
        else if (choice == 5) {
            operationLogMenu();
        }
        else if (choice == 6) {
            undoRedoMenu();
        }
        else if (choice == 7) {
            saveLoadMenu();
        }
        else if (choice == 0) {
            clearScreen();
            cout << endl;
            printDoubleSeparator(55);
            cout << CLR_BOLD << CLR_YELLOW << "  Thank you for using The Iron Nexus!" << CLR_RESET << endl;
            cout << CLR_DIM << "  Railway Authority - System Shutdown" << CLR_RESET << endl;
            printDoubleSeparator(55);
            cout << endl;
            logger.push("System shutdown", "System");
            return;
        }
        else {
            printError("Invalid option. Please try again.");
            pauseScreen();
        }
    }
}

void trainRegistryMenu() {
    while (true) {
        clearScreen();
        printMenuHeader("TRAIN REGISTRY");
        printSubMenuDeco();
        printMenuOption(1, "Register New Train");
        printMenuOption(2, "Remove Train");
        printMenuOption(3, "Search Train by ID");
        printMenuOption(4, "Display All Trains (In-Order)");
        printMenuOption(5, "Display All Trains (Pre-Order)");
        printMenuOption(6, "Display All Trains (Post-Order)");
        printMenuOption(7, "View Train Details");
        printMenuOption(0, "Back to Main Menu");
        printMenuFooter();

        int choice;
        cout << "  > Select option: ";
        if (!readInt(choice)) {
            printError("Invalid input. Please enter a number.");
            pauseScreen();
            continue;
        }

        if (choice == 1) {
            int id;
            cout << "  > Enter Train ID: ";
            if (!readInt(id)) {
                printError("Invalid input. Please enter a number.");
                pauseScreen();
                continue;
            }
            if (id <= 0) {
                printError("Train ID must be a positive number.");
                pauseScreen();
                continue;
            }
            if (registry.search(id)) {
                printError("A train with this ID already exists.");
                pauseScreen();
                continue;
            }
            char trainName[100];
            cout << "  > Enter Train Name: ";
            cin.getline(trainName, 100);
            if (isBlankString(trainName)) {
                printError("Train name cannot be empty.");
                pauseScreen();
                continue;
            }
            if (registry.insert(id, trainName)) {
                printSuccess("Train registered successfully.");
                char logMessage[256];
                snprintf(logMessage, 256, "Registered train %d: %s", id, trainName);
                logger.push(logMessage, "Train");

                ActionData actionInfo;
                actionInfo.trainID = id;
                strCopyN(actionInfo.trainName, trainName, 100);
                undoRedo.recordAction(ACT_ADD_TRAIN, actionInfo);
            }
            pauseScreen();
        }
        else if (choice == 2) {
            int id;
            cout << "  > Enter Train ID to remove: ";
            if (!readInt(id)) {
                printError("Invalid input. Please enter a number.");
                pauseScreen();
                continue;
            }
            AVLNode* train = registry.search(id);
            if (train) {
                ActionData actionInfo;
                actionInfo.trainID = id;
                strCopyN(actionInfo.trainName, train->name, 100);

                if (registry.remove(id)) {
                    printSuccess("Train removed successfully.");
                    char logMessage[256];
                    snprintf(logMessage, 256, "Removed train %d", id);
                    logger.push(logMessage, "Train");
                    undoRedo.recordAction(ACT_REMOVE_TRAIN, actionInfo);
                }
            }
            else {
                printError("Train not found.");
            }
            pauseScreen();
        }
        else if (choice == 3) {
            int id;
            cout << "  > Enter Train ID to search: ";
            if (!readInt(id)) {
                printError("Invalid input. Please enter a number.");
                pauseScreen();
                continue;
            }
            AVLNode* train = registry.search(id);
            if (train) {
                printSuccess("Train found!");
                registry.displayTrainDetails(id);
            }
            else {
                printError("Train not found in registry.");
            }
            pauseScreen();
        }
        else if (choice == 4) {
            registry.inOrder();
            pauseScreen();
        }
        else if (choice == 5) {
            registry.preOrder();
            pauseScreen();
        }
        else if (choice == 6) {
            registry.postOrder();
            pauseScreen();
        }
        else if (choice == 7) {
            int id;
            cout << "  > Enter Train ID: ";
            if (!readInt(id)) {
                printError("Invalid input. Please enter a number.");
                pauseScreen();
                continue;
            }
            registry.displayTrainDetails(id);
            pauseScreen();
        }
        else if (choice == 0) {
            return;
        }
        else {
            printError("Invalid option.");
            pauseScreen();
        }
    }
}

void coachManagementMenu() {
    while (true) {
        clearScreen();
        printMenuHeader("COACH MANAGEMENT");
        printSubMenuDeco();
        printMenuOption(1, "Add Coach at Front");
        printMenuOption(2, "Add Coach at Back");
        printMenuOption(3, "Insert Coach at Position");
        printMenuOption(4, "Remove Coach by ID");
        printMenuOption(5, "Traverse Forward (Engine -> Caboose)");
        printMenuOption(6, "Traverse Backward (Caboose -> Engine)");
        printMenuOption(7, "Reverse Train Orientation");
        printMenuOption(8, "View Coach Details");
        printMenuOption(0, "Back to Main Menu");
        printMenuFooter();

        int choice;
        cout << "  > Select option: ";
        if (!readInt(choice)) {
            printError("Invalid input. Please enter a number.");
            pauseScreen();
            continue;
        }

        if (choice == 0) {
            return;
        }

        if (choice < 1 || choice > 8) {
            printError("Invalid option.");
            pauseScreen();
            continue;
        }

        int trainID;
        cout << "  > Enter Train ID: ";
        if (!readInt(trainID)) {
            printError("Invalid input. Please enter a number.");
            pauseScreen();
            continue;
        }
        AVLNode* train = registry.search(trainID);
        if (!train) {
            printError("Train not found. Please register the train first.");
            pauseScreen();
            continue;
        }

        cout << CLR_DIM << "  Working with train: " << CLR_WHITE << train->name << CLR_DIM << " (ID: " << trainID << ")" << CLR_RESET << endl << endl;

        if (choice == 1) {
            int coachID;
            cout << "  > Enter Coach ID: ";
            if (!readInt(coachID)) {
                printError("Invalid input. Please enter a number.");
                pauseScreen();
                continue;
            }
            if (coachID <= 0) {
                printError("Coach ID must be a positive number.");
                pauseScreen();
                continue;
            }
            if (train->coaches->coachExists(coachID)) {
                printError("Coach ID already exists in this train.");
                pauseScreen();
                continue;
            }
            char coachType[50];
            cout << "  > Enter Coach Type (Economy/Business/VIP): ";
            cin.getline(coachType, 50);
            if (isBlankString(coachType)) {
                printError("Coach type cannot be empty.");
                pauseScreen();
                continue;
            }
            int seats;
            cout << "  > Enter Seat Count: ";
            if (!readInt(seats)) {
                printError("Invalid input. Please enter a number.");
                pauseScreen();
                continue;
            }
            if (seats <= 0) {
                printError("Seat count must be a positive number.");
                pauseScreen();
                continue;
            }
            if (train->coaches->addFront(coachID, coachType, seats)) {
                printSuccess("Coach added at front.");
                char logMessage[256];
                snprintf(logMessage, 256, "Added coach %d (front) to train %d", coachID, trainID);
                logger.push(logMessage, "Coach");

                ActionData actionInfo;
                actionInfo.trainID = trainID;
                actionInfo.coachID = coachID;
                strCopyN(actionInfo.coachType, coachType, 50);
                actionInfo.coachSeatCount = seats;
                undoRedo.recordAction(ACT_ADD_COACH_FRONT, actionInfo);
            }
        }
        else if (choice == 2) {
            int coachID;
            cout << "  > Enter Coach ID: ";
            if (!readInt(coachID)) {
                printError("Invalid input. Please enter a number.");
                pauseScreen();
                continue;
            }
            if (coachID <= 0) {
                printError("Coach ID must be a positive number.");
                pauseScreen();
                continue;
            }
            if (train->coaches->coachExists(coachID)) {
                printError("Coach ID already exists in this train.");
                pauseScreen();
                continue;
            }
            char coachType[50];
            cout << "  > Enter Coach Type (Economy/Business/VIP): ";
            cin.getline(coachType, 50);
            if (isBlankString(coachType)) {
                printError("Coach type cannot be empty.");
                pauseScreen();
                continue;
            }
            int seats;
            cout << "  > Enter Seat Count: ";
            if (!readInt(seats)) {
                printError("Invalid input. Please enter a number.");
                pauseScreen();
                continue;
            }
            if (seats <= 0) {
                printError("Seat count must be a positive number.");
                pauseScreen();
                continue;
            }
            if (train->coaches->addBack(coachID, coachType, seats)) {
                printSuccess("Coach added at back.");
                char logMessage[256];
                snprintf(logMessage, 256, "Added coach %d (back) to train %d", coachID, trainID);
                logger.push(logMessage, "Coach");

                ActionData actionInfo;
                actionInfo.trainID = trainID;
                actionInfo.coachID = coachID;
                strCopyN(actionInfo.coachType, coachType, 50);
                actionInfo.coachSeatCount = seats;
                undoRedo.recordAction(ACT_ADD_COACH_BACK, actionInfo);
            }
        }
        else if (choice == 3) {
            int position;
            cout << "  > Enter Position (0-indexed): ";
            if (!readInt(position)) {
                printError("Invalid input. Please enter a number.");
                pauseScreen();
                continue;
            }
            if (position < 0 || position > train->coaches->getCount()) {
                printError("Position is out of range.");
                pauseScreen();
                continue;
            }
            int coachID;
            cout << "  > Enter Coach ID: ";
            if (!readInt(coachID)) {
                printError("Invalid input. Please enter a number.");
                pauseScreen();
                continue;
            }
            if (coachID <= 0) {
                printError("Coach ID must be a positive number.");
                pauseScreen();
                continue;
            }
            if (train->coaches->coachExists(coachID)) {
                printError("Coach ID already exists in this train.");
                pauseScreen();
                continue;
            }
            char coachType[50];
            cout << "  > Enter Coach Type (Economy/Business/VIP): ";
            cin.getline(coachType, 50);
            if (isBlankString(coachType)) {
                printError("Coach type cannot be empty.");
                pauseScreen();
                continue;
            }
            int seats;
            cout << "  > Enter Seat Count: ";
            if (!readInt(seats)) {
                printError("Invalid input. Please enter a number.");
                pauseScreen();
                continue;
            }
            if (seats <= 0) {
                printError("Seat count must be a positive number.");
                pauseScreen();
                continue;
            }
            if (train->coaches->insertAt(position, coachID, coachType, seats)) {
                printSuccess("Coach inserted successfully.");
                char logMessage[256];
                snprintf(logMessage, 256, "Inserted coach %d at pos %d in train %d", coachID, position, trainID);
                logger.push(logMessage, "Coach");

                ActionData actionInfo;
                actionInfo.trainID = trainID;
                actionInfo.coachID = coachID;
                strCopyN(actionInfo.coachType, coachType, 50);
                actionInfo.coachSeatCount = seats;
                actionInfo.coachPosition = position;
                undoRedo.recordAction(ACT_ADD_COACH_AT, actionInfo);
            }
        }
        else if (choice == 4) {
            int coachID;
            cout << "  > Enter Coach ID to remove: ";
            if (!readInt(coachID)) {
                printError("Invalid input. Please enter a number.");
                pauseScreen();
                continue;
            }
            CoachNode* coach = train->coaches->findCoach(coachID);
            if (coach) {
                ActionData actionInfo;
                actionInfo.trainID = trainID;
                actionInfo.coachID = coachID;
                strCopyN(actionInfo.coachType, coach->type, 50);
                actionInfo.coachSeatCount = coach->seatCount;

                if (train->coaches->removeByID(coachID)) {
                    printSuccess("Coach removed.");
                    char logMessage[256];
                    snprintf(logMessage, 256, "Removed coach %d from train %d", coachID, trainID);
                    logger.push(logMessage, "Coach");
                    undoRedo.recordAction(ACT_REMOVE_COACH, actionInfo);
                }
            }
            else {
                printError("Coach not found.");
            }
        }
        else if (choice == 5) {
            train->coaches->traverseForward();
        }
        else if (choice == 6) {
            train->coaches->traverseBackward();
        }
        else if (choice == 7) {
            train->coaches->reverse();
            printSuccess("Train orientation reversed.");
            char logMessage[256];
            snprintf(logMessage, 256, "Reversed coaches in train %d", trainID);
            logger.push(logMessage, "Coach");

            ActionData actionInfo;
            actionInfo.trainID = trainID;
            undoRedo.recordAction(ACT_REVERSE_COACHES, actionInfo);
        }
        else if (choice == 8) {
            train->coaches->displayDetailed();
        }
        pauseScreen();
    }
}

void railwayNetworkMenu() {
    while (true) {
        clearScreen();
        printMenuHeader("RAILWAY TRACK NETWORK");
        printSubMenuDeco();
        printMenuOption(1, "Add Station");
        printMenuOption(2, "Remove Station");
        printMenuOption(3, "Add Route");
        printMenuOption(4, "Remove Route");
        printMenuOption(5, "Shortest Path (by Distance)");
        printMenuOption(6, "Shortest Path (by Travel Time)");
        printMenuOption(7, "Check Connectivity");
        printMenuOption(8, "Display Network");
        printMenuOption(9, "Display Adjacency Matrix");
        printMenuOption(10, "List All Stations");
        printMenuOption(0, "Back to Main Menu");
        printMenuFooter();

        int choice;
        cout << "  > Select option: ";
        if (!readInt(choice)) {
            printError("Invalid input. Please enter a number.");
            pauseScreen();
            continue;
        }

        if (choice == 1) {
            char stationName[100];
            cout << "  > Enter Station Name: ";
            cin.getline(stationName, 100);
            if (isBlankString(stationName)) {
                printError("Station name cannot be empty.");
                pauseScreen();
                continue;
            }
            int id = network.addStation(stationName);
            if (id >= 0) {
                printSuccess("Station added successfully.");
                char logMessage[256];
                snprintf(logMessage, 256, "Added station '%s' (ID: %d)", stationName, id);
                logger.push(logMessage, "Network");

                ActionData actionInfo;
                actionInfo.stationID = id;
                strCopyN(actionInfo.stationName, stationName, 100);
                undoRedo.recordAction(ACT_ADD_STATION, actionInfo);
            }
            pauseScreen();
        }
        else if (choice == 2) {
            network.displayStations();
            int id;
            cout << "  > Enter Station ID to remove: ";
            if (!readInt(id)) {
                printError("Invalid input. Please enter a number.");
                pauseScreen();
                continue;
            }
            Station* station = network.getStation(id);
            if (station) {
                ActionData actionInfo;
                actionInfo.stationID = id;
                strCopyN(actionInfo.stationName, station->name, 100);

                if (network.removeStation(id)) {
                    printSuccess("Station removed.");
                    char logMessage[256];
                    snprintf(logMessage, 256, "Removed station ID %d", id);
                    logger.push(logMessage, "Network");
                    undoRedo.recordAction(ACT_REMOVE_STATION, actionInfo);
                }
            }
            else {
                printError("Station not found.");
            }
            pauseScreen();
        }
        else if (choice == 3) {
            network.displayStations();
            int source;
            cout << "  > Enter Source Station ID: ";
            if (!readInt(source)) {
                printError("Invalid input. Please enter a number.");
                pauseScreen();
                continue;
            }
            if (!network.isValidStation(source)) {
                printError("Invalid source station ID.");
                pauseScreen();
                continue;
            }
            int destination;
            cout << "  > Enter Destination Station ID: ";
            if (!readInt(destination)) {
                printError("Invalid input. Please enter a number.");
                pauseScreen();
                continue;
            }
            if (!network.isValidStation(destination)) {
                printError("Invalid destination station ID.");
                pauseScreen();
                continue;
            }
            if (source == destination) {
                printError("Source and destination cannot be the same.");
                pauseScreen();
                continue;
            }
            int dist;
            cout << "  > Enter Distance (km): ";
            if (!readInt(dist)) {
                printError("Invalid input. Please enter a number.");
                pauseScreen();
                continue;
            }
            if (dist <= 0) {
                printError("Distance must be a positive number.");
                pauseScreen();
                continue;
            }
            int travelTime;
            cout << "  > Enter Travel Time (min): ";
            if (!readInt(travelTime)) {
                printError("Invalid input. Please enter a number.");
                pauseScreen();
                continue;
            }
            if (travelTime <= 0) {
                printError("Travel time must be a positive number.");
                pauseScreen();
                continue;
            }
            if (network.addRoute(source, destination, dist, travelTime)) {
                printSuccess("Route added.");
                char logMessage[256];
                snprintf(logMessage, 256, "Added route %d <-> %d (dist: %d km, time: %d min)", source, destination, dist, travelTime);
                logger.push(logMessage, "Network");

                ActionData actionInfo;
                actionInfo.srcStation = source;
                actionInfo.destStation = destination;
                actionInfo.distance = dist;
                actionInfo.travelTime = travelTime;
                undoRedo.recordAction(ACT_ADD_ROUTE, actionInfo);
            }
            pauseScreen();
        }
        else if (choice == 4) {
            network.displayStations();
            int source;
            cout << "  > Enter Source Station ID: ";
            if (!readInt(source)) {
                printError("Invalid input. Please enter a number.");
                pauseScreen();
                continue;
            }
            if (!network.isValidStation(source)) {
                printError("Invalid source station ID.");
                pauseScreen();
                continue;
            }
            int destination;
            cout << "  > Enter Destination Station ID: ";
            if (!readInt(destination)) {
                printError("Invalid input. Please enter a number.");
                pauseScreen();
                continue;
            }
            if (!network.isValidStation(destination)) {
                printError("Invalid destination station ID.");
                pauseScreen();
                continue;
            }

            EdgeNode** adjacencyList = network.getAdjList();
            EdgeNode* edge = adjacencyList[source];
            ActionData actionInfo;
            actionInfo.srcStation = source;
            actionInfo.destStation = destination;
            while (edge) {
                if (edge->destID == destination) {
                    actionInfo.distance = edge->distance;
                    actionInfo.travelTime = edge->travelTime;
                    break;
                }
                edge = edge->next;
            }

            if (network.removeRoute(source, destination)) {
                printSuccess("Route removed.");
                char logMessage[256];
                snprintf(logMessage, 256, "Removed route %d <-> %d", source, destination);
                logger.push(logMessage, "Network");
                undoRedo.recordAction(ACT_REMOVE_ROUTE, actionInfo);
            }
            pauseScreen();
        }
        else if (choice == 5) {
            network.displayStations();
            int source;
            cout << "  > Enter Source Station ID: ";
            if (!readInt(source)) {
                printError("Invalid input. Please enter a number.");
                pauseScreen();
                continue;
            }
            if (!network.isValidStation(source)) {
                printError("Invalid source station ID.");
                pauseScreen();
                continue;
            }
            int destination;
            cout << "  > Enter Destination Station ID: ";
            if (!readInt(destination)) {
                printError("Invalid input. Please enter a number.");
                pauseScreen();
                continue;
            }
            if (!network.isValidStation(destination)) {
                printError("Invalid destination station ID.");
                pauseScreen();
                continue;
            }
            network.shortestPathByDistance(source, destination);
            pauseScreen();
        }
        else if (choice == 6) {
            network.displayStations();
            int source;
            cout << "  > Enter Source Station ID: ";
            if (!readInt(source)) {
                printError("Invalid input. Please enter a number.");
                pauseScreen();
                continue;
            }
            if (!network.isValidStation(source)) {
                printError("Invalid source station ID.");
                pauseScreen();
                continue;
            }
            int destination;
            cout << "  > Enter Destination Station ID: ";
            if (!readInt(destination)) {
                printError("Invalid input. Please enter a number.");
                pauseScreen();
                continue;
            }
            if (!network.isValidStation(destination)) {
                printError("Invalid destination station ID.");
                pauseScreen();
                continue;
            }
            network.shortestPathByTime(source, destination);
            pauseScreen();
        }
        else if (choice == 7) {
            network.displayStations();
            int station1;
            cout << "  > Enter Station 1 ID: ";
            if (!readInt(station1)) {
                printError("Invalid input. Please enter a number.");
                pauseScreen();
                continue;
            }
            if (!network.isValidStation(station1)) {
                printError("Invalid station ID.");
                pauseScreen();
                continue;
            }
            int station2;
            cout << "  > Enter Station 2 ID: ";
            if (!readInt(station2)) {
                printError("Invalid input. Please enter a number.");
                pauseScreen();
                continue;
            }
            if (!network.isValidStation(station2)) {
                printError("Invalid station ID.");
                pauseScreen();
                continue;
            }
            if (network.isConnected(station1, station2)) {
                printSuccess("Stations are connected.");
            }
            else {
                printWarning("Stations are NOT connected.");
            }
            pauseScreen();
        }
        else if (choice == 8) {
            network.displayNetwork();
            pauseScreen();
        }
        else if (choice == 9) {
            network.displayAdjacencyMatrix();
            pauseScreen();
        }
        else if (choice == 10) {
            network.displayStations();
            pauseScreen();
        }
        else if (choice == 0) {
            return;
        }
        else {
            printError("Invalid option.");
            pauseScreen();
        }
    }
}

void seatingChartMenu() {
    while (true) {
        clearScreen();
        printMenuHeader("SEATING CHART");
        printSubMenuDeco();
        printMenuOption(1, "Initialize Seat Chart for a Train");
        printMenuOption(2, "Book a Seat");
        printMenuOption(3, "Cancel a Booking");
        printMenuOption(4, "Check Seat Status");
        printMenuOption(5, "Display All Seats (Sorted - BST)");
        printMenuOption(6, "Display Booked Seats");
        printMenuOption(7, "Display Available Seats");
        printMenuOption(8, "Display Hash Table View");
        printMenuOption(0, "Back to Main Menu");
        printMenuFooter();

        int choice;
        cout << "  > Select option: ";
        if (!readInt(choice)) {
            printError("Invalid input. Please enter a number.");
            pauseScreen();
            continue;
        }

        if (choice == 0) {
            return;
        }

        if (choice < 1 || choice > 8) {
            printError("Invalid option.");
            pauseScreen();
            continue;
        }

        int trainID;
        cout << "  > Enter Train ID: ";
        if (!readInt(trainID)) {
            printError("Invalid input. Please enter a number.");
            pauseScreen();
            continue;
        }
        AVLNode* train = registry.search(trainID);
        if (!train) {
            printError("Train not found.");
            pauseScreen();
            continue;
        }

        cout << CLR_DIM << "  Train: ";
        cout << CLR_WHITE << train->name;
        cout << CLR_DIM << " (ID: " << trainID << ")" << CLR_RESET << endl;

        if (choice == 1) {
            int seatTotal = train->coaches->getTotalSeats();
            if (seatTotal == 0) {
                printError("No coaches attached. Add coaches first.");
            }
            else {
                if (train->seats && train->seats->isInitialized()) {
                    printWarning("Seat chart already initialized. Reinitializing will clear all bookings.");
                    int confirm;
                    cout << "  > Continue? (1=Yes, 0=No): ";
                    if (!readInt(confirm)) {
                        printError("Invalid input.");
                        pauseScreen();
                        continue;
                    }
                    if (confirm != 1) {
                        pauseScreen();
                        continue;
                    }
                }
                delete train->seats;
                train->seats = new SeatManager();
                if (train->seats->initialize(seatTotal)) {
                    char statusMessage[256];
                    snprintf(statusMessage, 256, "Seat chart initialized with %d seats.", seatTotal);
                    printSuccess(statusMessage);
                    char logMessage[256];
                    snprintf(logMessage, 256, "Initialized %d seats for train %d", seatTotal, trainID);
                    logger.push(logMessage, "Seat");
                }
            }
        }
        else if (choice == 2) {
            if (!train->seats || !train->seats->isInitialized()) {
                printError("Seat chart not initialized. Use option 1 first.");
            }
            else {
                int seatNum;
                cout << "  > Enter Seat Number: ";
                if (!readInt(seatNum)) {
                    printError("Invalid input. Please enter a number.");
                    pauseScreen();
                    continue;
                }
                if (seatNum < 1 || seatNum > train->seats->getTotalSeats()) {
                    printError("Seat number out of range.");
                    pauseScreen();
                    continue;
                }
                Seat* existingSeat = train->seats->checkSeat(seatNum);
                if (!existingSeat) {
                    printError("Seat does not exist.");
                    pauseScreen();
                    continue;
                }
                if (existingSeat->isBooked) {
                    printError("This seat is already booked.");
                    pauseScreen();
                    continue;
                }
                char passenger[100];
                cout << "  > Enter Passenger Name: ";
                cin.getline(passenger, 100);
                if (isBlankString(passenger)) {
                    printError("Passenger name cannot be empty.");
                    pauseScreen();
                    continue;
                }
                if (train->seats->bookSeat(seatNum, passenger)) {
                    printSuccess("Seat booked successfully.");
                    char logMessage[256];
                    snprintf(logMessage, 256, "Booked seat %d for '%s' on train %d", seatNum, passenger, trainID);
                    logger.push(logMessage, "Seat");

                    ActionData actionInfo;
                    actionInfo.trainID = trainID;
                    actionInfo.seatNumber = seatNum;
                    strCopyN(actionInfo.passengerName, passenger, 100);
                    undoRedo.recordAction(ACT_BOOK_SEAT, actionInfo);
                }
            }
        }
        else if (choice == 3) {
            if (!train->seats || !train->seats->isInitialized()) {
                printError("Seat chart not initialized.");
            }
            else {
                int seatNum;
                cout << "  > Enter Seat Number to cancel: ";
                if (!readInt(seatNum)) {
                    printError("Invalid input. Please enter a number.");
                    pauseScreen();
                    continue;
                }
                if (seatNum < 1 || seatNum > train->seats->getTotalSeats()) {
                    printError("Seat number out of range.");
                    pauseScreen();
                    continue;
                }

                ActionData actionInfo;
                actionInfo.trainID = trainID;
                actionInfo.seatNumber = seatNum;
                Seat* seatData = train->seats->checkSeat(seatNum);
                if (seatData && seatData->isBooked) {
                    strCopyN(actionInfo.passengerName, seatData->passengerName, 100);
                }

                if (train->seats->cancelSeat(seatNum)) {
                    printSuccess("Booking cancelled.");
                    char logMessage[256];
                    snprintf(logMessage, 256, "Cancelled seat %d on train %d", seatNum, trainID);
                    logger.push(logMessage, "Seat");
                    undoRedo.recordAction(ACT_CANCEL_SEAT, actionInfo);
                }
            }
        }
        else if (choice == 4) {
            if (!train->seats || !train->seats->isInitialized()) {
                printError("Seat chart not initialized.");
            }
            else {
                int seatNum;
                cout << "  > Enter Seat Number: ";
                if (!readInt(seatNum)) {
                    printError("Invalid input. Please enter a number.");
                    pauseScreen();
                    continue;
                }
                Seat* seatData = train->seats->checkSeat(seatNum);
                if (!seatData) {
                    printError("Seat does not exist.");
                }
                else {
                    cout << endl;
                    printSeparator('-', 40);
                    cout << "  " << CLR_CYAN << "Seat #" << CLR_BOLD << seatData->seatNumber << CLR_RESET << endl;
                    if (seatData->isBooked) {
                        cout << "  " << CLR_CYAN << "Status: " << CLR_RED << "BOOKED" << CLR_RESET << endl;
                        cout << "  " << CLR_CYAN << "Passenger: " << CLR_WHITE << seatData->passengerName << CLR_RESET << endl;
                    }
                    else {
                        cout << "  " << CLR_CYAN << "Status: " << CLR_GREEN << "AVAILABLE" << CLR_RESET << endl;
                    }
                    printSeparator('-', 40);
                }
            }
        }
        else if (choice == 5) {
            if (!train->seats || !train->seats->isInitialized()) {
                printError("Seat chart not initialized.");
            }
            else {
                train->seats->displayAllSorted();
            }
        }
        else if (choice == 6) {
            if (!train->seats || !train->seats->isInitialized()) {
                printError("Seat chart not initialized.");
            }
            else {
                train->seats->displayBooked();
            }
        }
        else if (choice == 7) {
            if (!train->seats || !train->seats->isInitialized()) {
                printError("Seat chart not initialized.");
            }
            else {
                train->seats->displayAvailable();
            }
        }
        else if (choice == 8) {
            if (!train->seats || !train->seats->isInitialized()) {
                printError("Seat chart not initialized.");
            }
            else {
                train->seats->getHashTable()->displayAll();
            }
        }
        pauseScreen();
    }
}

void operationLogMenu() {
    while (true) {
        clearScreen();
        printMenuHeader("OPERATION LOG");
        printSubMenuDeco();
        printMenuOption(1, "View All Logs");
        printMenuOption(2, "View Recent Logs");
        printMenuOption(3, "Filter by Module");
        printMenuOption(4, "View Most Recent Entry");
        printMenuOption(5, "Delete Most Recent Entry (Pop)");
        printMenuOption(6, "Log Statistics");
        printMenuOption(0, "Back to Main Menu");
        printMenuFooter();

        int choice;
        cout << "  > Select option: ";
        if (!readInt(choice)) {
            printError("Invalid input. Please enter a number.");
            pauseScreen();
            continue;
        }

        if (choice == 1) {
            logger.displayAll();
            pauseScreen();
        }
        else if (choice == 2) {
            int howMany;
            cout << "  > How many recent entries? ";
            if (!readInt(howMany)) {
                printError("Invalid input. Please enter a number.");
                pauseScreen();
                continue;
            }
            if (howMany <= 0) {
                printError("Count must be a positive number.");
                pauseScreen();
                continue;
            }
            logger.displayRecent(howMany);
            pauseScreen();
        }
        else if (choice == 3) {
            cout << CLR_DIM << "  Modules: Train, Coach, Seat, Network, System" << CLR_RESET << endl;
            char moduleName[50];
            cout << "  > Enter module name: ";
            cin.getline(moduleName, 50);
            if (isBlankString(moduleName)) {
                printError("Module name cannot be empty.");
                pauseScreen();
                continue;
            }
            logger.displayByModule(moduleName);
            pauseScreen();
        }
        else if (choice == 4) {
            LogEntry* latest = logger.peek();
            if (latest) {
                cout << endl;
                printSeparator('-', 55);
                cout << CLR_BOLD << CLR_CYAN << "  Most Recent Entry:" << CLR_RESET << endl;
                cout << "  " << CLR_GRAY << latest->timestamp << CLR_RESET;
                cout << "  [" << CLR_YELLOW << latest->module << CLR_RESET << "] ";
                cout << CLR_WHITE << latest->action << CLR_RESET << endl;
                printSeparator('-', 55);
            }
            else {
                printWarning("Log is empty.");
            }
            pauseScreen();
        }
        else if (choice == 5) {
            if (logger.isEmpty()) {
                printWarning("Log is empty. Nothing to delete.");
            }
            else {
                LogEntry* latest = logger.peek();
                cout << endl;
                cout << CLR_DIM << "  About to delete: [" << CLR_YELLOW << latest->module << CLR_RESET << CLR_DIM << "] " << CLR_WHITE << latest->action << CLR_RESET << endl;
                int confirm;
                cout << "  > Confirm deletion? (1=Yes, 0=No): ";
                if (!readInt(confirm)) {
                    printError("Invalid input.");
                    pauseScreen();
                    continue;
                }
                if (confirm == 1) {
                    logger.pop();
                    printSuccess("Most recent log entry deleted.");
                }
                else {
                    printInfo("Deletion cancelled.");
                }
            }
            pauseScreen();
        }
        else if (choice == 6) {
            cout << endl;
            printSeparator('-', 40);
            cout << CLR_BOLD << CLR_CYAN << "  Log Statistics" << CLR_RESET << endl;
            cout << "  " << CLR_WHITE << "Total entries: " << CLR_GREEN << logger.getSize() << CLR_RESET << endl;
            printSeparator('-', 40);
            pauseScreen();
        }
        else if (choice == 0) {
            return;
        }
        else {
            printError("Invalid option.");
            pauseScreen();
        }
    }
}

void undoRedoMenu() {
    while (true) {
        clearScreen();
        printMenuHeader("UNDO / REDO");
        printSubMenuDeco();
        undoRedo.displayStatus();
        cout << endl;
        printMenuOption(1, "Undo Last Action");
        printMenuOption(2, "Redo Last Undone Action");
        printMenuOption(3, "Clear All History");
        printMenuOption(0, "Back to Main Menu");
        printMenuFooter();

        int choice;
        cout << "  > Select option: ";
        if (!readInt(choice)) {
            printError("Invalid input. Please enter a number.");
            pauseScreen();
            continue;
        }

        if (choice == 1) {
            if (undoRedo.undo(registry, network)) {
                logger.push("Undo performed", "System");
            }
            pauseScreen();
        }
        else if (choice == 2) {
            if (undoRedo.redo(registry, network)) {
                logger.push("Redo performed", "System");
            }
            pauseScreen();
        }
        else if (choice == 3) {
            undoRedo.clearAll();
            printSuccess("Undo/Redo history cleared.");
            pauseScreen();
        }
        else if (choice == 0) {
            return;
        }
        else {
            printError("Invalid option.");
            pauseScreen();
        }
    }
}

void saveLoadMenu() {
    while (true) {
        clearScreen();
        printMenuHeader("SAVE / LOAD DATA");
        printSubMenuDeco();
        printMenuOption(1, "Save All Data");
        printMenuOption(2, "Load All Data");
        printMenuOption(3, "Save Train Registry Only");
        printMenuOption(4, "Load Train Registry Only");
        printMenuOption(5, "Save Network Only");
        printMenuOption(6, "Load Network Only");
        printMenuOption(7, "Save Operation Log Only");
        printMenuOption(8, "Load Operation Log Only");
        printMenuOption(9, "Save BST Structure (Bonus)");
        printMenuOption(0, "Back to Main Menu");
        printMenuFooter();

        int choice;
        cout << "  > Select option: ";
        if (!readInt(choice)) {
            printError("Invalid input. Please enter a number.");
            pauseScreen();
            continue;
        }

        if (choice == 1) {
            FileIO::saveAll(registry, network, logger);
            logger.push("All data saved to files", "System");
            pauseScreen();
        }
        else if (choice == 2) {
            FileIO::loadAll(registry, network, logger);
            logger.push("All data loaded from files", "System");
            pauseScreen();
        }
        else if (choice == 3) {
            FileIO::saveTrains(registry, "data/trains.dat");
            logger.push("Train registry saved", "System");
            pauseScreen();
        }
        else if (choice == 4) {
            FileIO::loadTrains(registry, "data/trains.dat");
            logger.push("Train registry loaded", "System");
            pauseScreen();
        }
        else if (choice == 5) {
            FileIO::saveNetwork(network, "data/network.dat");
            logger.push("Network saved", "System");
            pauseScreen();
        }
        else if (choice == 6) {
            FileIO::loadNetwork(network, "data/network.dat");
            logger.push("Network loaded", "System");
            pauseScreen();
        }
        else if (choice == 7) {
            FileIO::saveLog(logger, "data/log.dat");
            pauseScreen();
        }
        else if (choice == 8) {
            FileIO::loadLog(logger, "data/log.dat");
            pauseScreen();
        }
        else if (choice == 9) {
            FileIO::saveSeatBSTStructure(registry, "data/bst_structure.dat");
            logger.push("BST structure saved (hierarchical)", "System");
            pauseScreen();
        }
        else if (choice == 0) {
            return;
        }
        else {
            printError("Invalid option.");
            pauseScreen();
        }
    }
}
