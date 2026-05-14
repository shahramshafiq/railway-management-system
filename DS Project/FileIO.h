#ifndef FILEIO_H
#define FILEIO_H

#include "Train.h"
#include "Network.h"
#include "Logger.h"
#include <fstream>

class FileIO {
private:
    static void saveTrainNode(ofstream& out, AVLNode* node);
    static void loadTrainNode(ifstream& in, TrainRegistry& registry);
    static void saveBSTPreOrder(ofstream& out, BSTNode* node);

public:
    static bool saveTrains(const TrainRegistry& registry, const char* filename);
    static bool saveNetwork(const RailwayNetwork& network, const char* filename);
    static bool saveLog(const OperationLog& log, const char* filename);

    static bool loadTrains(TrainRegistry& registry, const char* filename);
    static bool loadNetwork(RailwayNetwork& network, const char* filename);
    static bool loadLog(OperationLog& log, const char* filename);

    static bool saveAll(const TrainRegistry& registry, const RailwayNetwork& network, const OperationLog& log);
    static bool loadAll(TrainRegistry& registry, RailwayNetwork& network, OperationLog& log);

    static bool saveSeatBSTStructure(const TrainRegistry& registry, const char* filename);
    static void ensureDataDir();
};

#endif
