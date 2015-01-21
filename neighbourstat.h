#ifndef NEIGHBOURSTAT_H
#define NEIGHBOURSTAT_H

#include <boost/thread.hpp>
#include <json/json.h>
#include <fstream>
#include <string>

#define STAT_NAME "NeighbourStat"

using namespace std;
class NeighbourStat
{
    string filename;
    Json::Value statArray;
    boost::mutex mtx;
public:
    NeighbourStat(string path);
    void saveToFile();
    int getNeighbourNum(int idA, int idB);
    void incNeigNum(int idA, int idB);
    void reset();
    double conditionalNeigProb(int idA, int idB);
    bool isEmpty();
};

#endif // NEIGHBOURSTAT_H
