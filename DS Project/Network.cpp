#include "Network.h"
#include <iomanip>

using namespace std;

RailwayNetwork::RailwayNetwork() : stationCount(0) {
    for (int i = 0; i < MAX_STATIONS; i++) {
        adjList[i] = nullptr;
    }
}

RailwayNetwork::~RailwayNetwork() {
    clear();
}

int RailwayNetwork::addStation(const char* name) {
    if (stationCount >= MAX_STATIONS) {
        printError("Maximum station limit reached (15).");
        return -1;
    }
    for (int i = 0; i < MAX_STATIONS; i++) {
        if (stations[i].active && strEqualsNoCase(stations[i].name, name)) {
            printError("Station with this name already exists.");
            return -1;
        }
    }
    int slotIndex = -1;
    for (int i = 0; i < MAX_STATIONS; i++) {
        if (!stations[i].active) {
            slotIndex = i;
            break;
        }
    }
    if (slotIndex == -1) {
        return -1;
    }

    stations[slotIndex].id = slotIndex;
    strCopyN(stations[slotIndex].name, name, 100);
    stations[slotIndex].active = true;
    stationCount++;
    return slotIndex;
}

bool RailwayNetwork::removeStation(int id) {
    if (!isValidStation(id)) {
        printError("Invalid station ID.");
        return false;
    }

    EdgeNode* currentEdge = adjList[id];
    while (currentEdge) {
        EdgeNode* toDelete = currentEdge;
        currentEdge = currentEdge->next;
        delete toDelete;
    }
    adjList[id] = nullptr;

    for (int i = 0; i < MAX_STATIONS; i++) {
        if (i == id) { 
            continue; 
        }
        if (!stations[i].active) { 
            continue; 
        }

        EdgeNode* previous = nullptr;
        EdgeNode* edge = adjList[i];
        while (edge) {
            if (edge->destID == id) {
                if (previous) {
                    previous->next = edge->next;
                }
                else {
                    adjList[i] = edge->next;
                }
                EdgeNode* toDelete = edge;
                edge = edge->next;
                delete toDelete;
            } 
            else {
                previous = edge;
                edge = edge->next;
            }
        }
    }

    stations[id].active = false;
    stations[id].name[0] = '\0';
    stationCount--;
    return true;
}

bool RailwayNetwork::addStationWithID(int id, const char* name) {
    if (id < 0 || id >= MAX_STATIONS) {
        return false;
    }
    if (stations[id].active) {
        return false;
    }
    if (stationCount >= MAX_STATIONS) {
        return false;
    }
    stations[id].id = id;
    strCopyN(stations[id].name, name, 100);
    stations[id].active = true;
    stationCount++;
    return true;
}

int RailwayNetwork::findStationByName(const char* name) const {
    for (int i = 0; i < MAX_STATIONS; i++) {
        if (stations[i].active && strEqualsNoCase(stations[i].name, name)) {
            return i;
        }
    }
    return -1;
}

Station* RailwayNetwork::getStation(int id) {
    if (id < 0 || id >= MAX_STATIONS || !stations[id].active) {
        return nullptr;
    }
    return &stations[id];
}

bool RailwayNetwork::addRoute(int source, int destination, int distance, int travelTime) {
    if (!isValidStation(source) || !isValidStation(destination)) {
        printError("Invalid station ID(s).");
        return false;
    }
    if (source == destination) {
        printError("Cannot create a route from a station to itself.");
        return false;
    }
    if (distance <= 0 || travelTime <= 0) {
        printError("Distance and travel time must be positive.");
        return false;
    }

    EdgeNode* currentEdge = adjList[source];
    while (currentEdge) {
        if (currentEdge->destID == destination) {
            printError("Route already exists. Remove it first to update.");
            return false;
        }
        currentEdge = currentEdge->next;
    }

    EdgeNode* forwardEdge = new EdgeNode(destination, distance, travelTime);
    forwardEdge->next = adjList[source];
    adjList[source] = forwardEdge;

    EdgeNode* backwardEdge = new EdgeNode(source, distance, travelTime);
    backwardEdge->next = adjList[destination];
    adjList[destination] = backwardEdge;

    return true;
}

bool RailwayNetwork::removeRoute(int source, int destination) {
    if (!isValidStation(source) || !isValidStation(destination)) {
        printError("Invalid station ID(s).");
        return false;
    }

    bool removedForward = false;
    bool removedBackward = false;

    EdgeNode* previous = nullptr;
    EdgeNode* currentEdge = adjList[source];
    while (currentEdge) {
        if (currentEdge->destID == destination) {
            if (previous) {
                previous->next = currentEdge->next;
            } 
            else {
                adjList[source] = currentEdge->next;
            }
            delete currentEdge;
            removedForward = true;
            break;
        }
        previous = currentEdge;
        currentEdge = currentEdge->next;
    }

    previous = nullptr;
    currentEdge = adjList[destination];
    while (currentEdge) {
        if (currentEdge->destID == source) {
            if (previous) {
                previous->next = currentEdge->next;
            } 
            else {
                adjList[destination] = currentEdge->next;
            }
            delete currentEdge;
            removedBackward = true;
            break;
        }
        previous = currentEdge;
        currentEdge = currentEdge->next;
    }

    if (!removedForward && !removedBackward) {
        printError("Route does not exist.");
        return false;
    }
    return true;
}

// dijkstra with inlined min-distance search instead of helper function
void RailwayNetwork::runDijkstra(int source, int destination, bool useDistance) const {
    int distances[MAX_STATIONS];
    bool visited[MAX_STATIONS];
    int parent[MAX_STATIONS];

    for (int i = 0; i < MAX_STATIONS; i++) {
        distances[i] = INF;
        visited[i] = false;
        parent[i] = -1;
    }
    distances[source] = 0;

    for (int round = 0; round < stationCount; round++) {
        // inline min search
        int minValue = INF;
        int closest = -1;
        for (int i = 0; i < MAX_STATIONS; i++) {
            if (stations[i].active && !visited[i] && distances[i] < minValue) {
                minValue = distances[i];
                closest = i;
            }
        }
        if (closest == -1) {
            break;
        }
        visited[closest] = true;

        if (closest == destination) {
            break;
        }

        EdgeNode* edge = adjList[closest];
        while (edge) {
            int neighbor = edge->destID;
            int weight = useDistance ? edge->distance : edge->travelTime;
            if (!visited[neighbor] && distances[closest] + weight < distances[neighbor]) {
                distances[neighbor] = distances[closest] + weight;
                parent[neighbor] = closest;
            }
            edge = edge->next;
        }
    }

    if (distances[destination] == INF) {
        printError("No route exists between these stations.");
        return;
    }

    // reconstruct path by walking parent array backwards
    int path[MAX_STATIONS];
    int pathLength = 0;
    int current = destination;
    while (current != -1) {
        path[pathLength] = current;
        pathLength++;
        current = parent[current];
    }

    cout << endl;
    printDoubleSeparator(70);
    cout << CLR_BOLD << CLR_GREEN << "  Shortest Path (" << (useDistance ? "Distance" : "Travel Time") << ") >> Path Found!" << CLR_RESET << endl;
    printSeparator('-', 70);

    // visual ASCII route map
    cout << endl << "  ";
    for (int i = pathLength - 1; i >= 0; i--) {
        cout << CLR_BOLD << CLR_YELLOW << "[" << stations[path[i]].name << "]" << CLR_RESET;
        if (i > 0) {
            int from = path[i];
            int to = path[i - 1];
            EdgeNode* seg = adjList[from];
            while (seg && seg->destID != to) {
                seg = seg->next;
            }
            if (seg) {
                if (useDistance) {
                    cout << CLR_CYAN << "──" << seg->distance << "km──" << CLR_RESET;
                } else {
                    cout << CLR_CYAN << "──" << seg->travelTime << "min──" << CLR_RESET;
                }
            } else {
                cout << CLR_CYAN << "────" << CLR_RESET;
            }
        }
    }
    cout << endl << endl;

    // summary line
    cout << "  " << CLR_CYAN << "Total " << (useDistance ? "Distance" : "Time") << ": ";
    cout << CLR_BOLD << CLR_WHITE << distances[destination] << (useDistance ? " km" : " min") << CLR_RESET;
    cout << "   " << CLR_CYAN << "Stops: " << CLR_WHITE << (pathLength - 1) << " hop(s)" << CLR_RESET << endl;
    cout << endl;

    // numbered segment table
    printSeparator('-', 70);
    cout << CLR_BOLD << CLR_CYAN;
    cout << "  #   From                 To                   Dist      Time" << CLR_RESET << endl;
    printSeparator('-', 70);
    for (int i = pathLength - 1; i > 0; i--) {
        int from = path[i];
        int to = path[i - 1];
        EdgeNode* seg = adjList[from];
        while (seg && seg->destID != to) {
            seg = seg->next;
        }
        int segNum = pathLength - 1 - i + 1;
        cout << "  " << CLR_DIM << segNum << CLR_RESET << "   ";
        cout << CLR_YELLOW << setw(21) << left << stations[from].name << CLR_RESET;
        cout << CLR_YELLOW << setw(21) << left << stations[to].name << CLR_RESET;
        if (seg) {
            cout << CLR_GREEN << setw(4) << right << seg->distance << " km  " << CLR_RESET;
            cout << CLR_BLUE << setw(4) << right << seg->travelTime << " min" << CLR_RESET;
        }
        cout << endl;
    }
    printSeparator('-', 70);
    cout << CLR_BOLD << "  " << setw(44) << left << "TOTAL" << CLR_RESET;
    if (useDistance) {
        cout << CLR_GREEN << CLR_BOLD << setw(4) << right << distances[destination] << " km" << CLR_RESET << endl;
    } else {
        cout << CLR_BLUE << CLR_BOLD << setw(4) << right << distances[destination] << " min" << CLR_RESET << endl;
    }
    printDoubleSeparator(70);
}

void RailwayNetwork::shortestPathByDistance(int source, int destination) const {
    if (!isValidStation(source) || !isValidStation(destination)) {
        printError("Invalid station ID(s).");
        return;
    }
    if (source == destination) {
        printWarning("Source and destination are the same station.");
        return;
    }
    runDijkstra(source, destination, true);
}

void RailwayNetwork::shortestPathByTime(int source, int destination) const {
    if (!isValidStation(source) || !isValidStation(destination)) {
        printError("Invalid station ID(s).");
        return;
    }
    if (source == destination) {
        printWarning("Source and destination are the same station.");
        return;
    }
    runDijkstra(source, destination, false);
}

bool RailwayNetwork::isConnected(int source, int destination) const {
    if (!isValidStation(source) || !isValidStation(destination)) {
        return false;
    }
    if (source == destination) {
        return true;
    }

    bool visited[MAX_STATIONS];
    for (int i = 0; i < MAX_STATIONS; i++) {
        visited[i] = false;
    }

    int queue[MAX_STATIONS];
    int front = 0;
    int back = 0;
    queue[back] = source;
    back++;
    visited[source] = true;

    while (front < back) {
        int current = queue[front];
        front++;
        EdgeNode* edge = adjList[current];
        while (edge) {
            if (edge->destID == destination) {
                return true;
            }
            if (!visited[edge->destID]) {
                visited[edge->destID] = true;
                queue[back] = edge->destID;
                back++;
            }
            edge = edge->next;
        }
    }
    return false;
}

void RailwayNetwork::displayStations() const {
    if (stationCount == 0) {
        printWarning("No stations in the network.");
        return;
    }
    cout << endl;
    printDoubleSeparator(45);
    cout << CLR_BOLD << CLR_CYAN << "  ID    Station Name" << CLR_RESET << endl;
    printSeparator('-', 45);
    for (int i = 0; i < MAX_STATIONS; i++) {
        if (stations[i].active) {
            cout << "  " << CLR_GREEN << setw(4) << left << stations[i].id << CLR_RESET << "  " << CLR_WHITE << stations[i].name << CLR_RESET << endl;
        }
    }
    printSeparator('-', 45);
    cout << CLR_DIM << "  Total: " << stationCount << " stations" << CLR_RESET << endl;
    printDoubleSeparator(45);
}

void RailwayNetwork::displayNetwork() const {
    if (stationCount == 0) {
        printWarning("No stations in the network.");
        return;
    }
    cout << endl;
    printDoubleSeparator(68);
    cout << CLR_BOLD << CLR_CYAN << "  Railway Network (Adjacency List)  ";
    cout << CLR_DIM << "[" << stationCount << " stations]" << CLR_RESET << endl;
    printSeparator('=', 68);

    for (int i = 0; i < MAX_STATIONS; i++) {
        if (!stations[i].active) {
            continue;
        }
        cout << endl;
        cout << "  " << CLR_YELLOW << CLR_BOLD << ">> " << stations[i].name << CLR_RESET;
        cout << CLR_DIM << "  (ID: " << i << ")" << CLR_RESET << endl;

        cout << CLR_YELLOW << CLR_DIM << "  o====o====o====o" << CLR_RESET << endl;

        EdgeNode* edge = adjList[i];
        if (!edge) {
            cout << "    " << CLR_DIM << "(no track connections)" << CLR_RESET << endl;
        }
        while (edge) {
            cout << "    " << CLR_GREEN << "\xe2\x94\x9c\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80 ";
            cout << CLR_WHITE << CLR_BOLD << stations[edge->destID].name << CLR_RESET;
            cout << CLR_DIM << "  [" << CLR_RESET;
            cout << CLR_GREEN << edge->distance << " km" << CLR_RESET;
            cout << CLR_DIM << " | " << CLR_RESET;
            cout << CLR_BLUE << edge->travelTime << " min" << CLR_RESET;
            cout << CLR_DIM << "]" << CLR_RESET << endl;
            edge = edge->next;
        }
    }
    cout << endl;
    printDoubleSeparator(68);
}

void RailwayNetwork::displayAdjacencyMatrix() const {
    if (stationCount == 0) {
        printWarning("No stations in the network.");
        return;
    }

    int activeIDs[MAX_STATIONS];
    int activeCount = 0;
    for (int i = 0; i < MAX_STATIONS; i++) {
        if (stations[i].active) {
            activeIDs[activeCount] = i;
            activeCount++;
        }
    }

    cout << endl;
    cout << CLR_BOLD << CLR_CYAN << "  Adjacency Matrix (Distance in km):" << CLR_RESET << endl << endl;

    cout << "       ";
    for (int col = 0; col < activeCount; col++) {
        char shortName[6];
        strCopyN(shortName, stations[activeIDs[col]].name, 6);
        cout << CLR_YELLOW << setw(6) << shortName << CLR_RESET;
    }
    cout << endl << "       ";
    for (int col = 0; col < activeCount; col++) {
        cout << "------";
    }
    cout << endl;

    for (int row = 0; row < activeCount; row++) {
        char shortName[6];
        strCopyN(shortName, stations[activeIDs[row]].name, 6);
        cout << "  " << CLR_YELLOW << setw(5) << left << shortName << CLR_RESET << "| ";

        for (int col = 0; col < activeCount; col++) {
            int dist = 0;
            if (row != col) {
                EdgeNode* edge = adjList[activeIDs[row]];
                while (edge) {
                    if (edge->destID == activeIDs[col]) {
                        dist = edge->distance;
                        break;
                    }
                    edge = edge->next;
                }
            }
            if (row == col) {
                cout << CLR_DIM << setw(6) << "   0" << CLR_RESET;
            }
            else if (dist > 0) {
                cout << CLR_GREEN << setw(6) << dist << CLR_RESET;
            }
            else {
                cout << CLR_DIM << setw(6) << "  INF" << CLR_RESET;
            }
        }
        cout << endl;
    }
    cout << endl;
}

int RailwayNetwork::getStationCount() const {
    return stationCount;
}

bool RailwayNetwork::isValidStation(int id) const {
    if (id < 0 || id >= MAX_STATIONS) {
        return false;
    }
    return stations[id].active;
}

void RailwayNetwork::clear() {
    for (int i = 0; i < MAX_STATIONS; i++) {
        EdgeNode* currentEdge = adjList[i];
        while (currentEdge) {
            EdgeNode* toDelete = currentEdge;
            currentEdge = currentEdge->next;
            delete toDelete;
        }
        adjList[i] = nullptr;
        stations[i].active = false;
        stations[i].name[0] = '\0';
    }
    stationCount = 0;
}
