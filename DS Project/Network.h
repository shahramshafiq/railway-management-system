#ifndef NETWORK_H
#define NETWORK_H

#include "Utils.h"

const int MAX_STATIONS = 15;
const int INF = 999999;

struct EdgeNode {
    int destID;
    int distance;
    int travelTime;
    EdgeNode* next;

    EdgeNode() : destID(-1), distance(0), travelTime(0), next(nullptr) {}
    EdgeNode(int dest, int dist, int time) : destID(dest), distance(dist), travelTime(time), next(nullptr) {}
};

struct Station {
    int id;
    char name[100];
    bool active;

    Station() : id(-1), active(false) { 
        name[0] = '\0'; 
    }
};

class RailwayNetwork {
private:
    Station stations[MAX_STATIONS];
    EdgeNode* adjList[MAX_STATIONS];
    int stationCount;

    void runDijkstra(int source, int destination, bool useDistance) const;

public:
    RailwayNetwork();
    ~RailwayNetwork();

    int addStation(const char* name);
    bool addStationWithID(int id, const char* name);
    bool removeStation(int id);
    int findStationByName(const char* name) const;
    Station* getStation(int id);

    bool addRoute(int source, int destination, int distance, int travelTime);
    bool removeRoute(int source, int destination);

    void shortestPathByDistance(int source, int destination) const;
    void shortestPathByTime(int source, int destination) const;

    bool isConnected(int source, int destination) const;

    void displayNetwork() const;
    void displayStations() const;
    void displayAdjacencyMatrix() const;

    int getStationCount() const;
    bool isValidStation(int id) const;
    void clear();

    Station* getStations() {
        return stations;
    }
    const Station* getStations() const {
        return stations;
    }
    EdgeNode** getAdjList() {
        return adjList;
    }
    const EdgeNode* const* getAdjList() const {
        return adjList;
    }
};

#endif
