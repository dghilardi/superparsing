#ifndef CLASSLIKELIHOOD_H
#define CLASSLIKELIHOOD_H

#include <map>
#include <cmath>
#include <string>
#include <vector>
#include <boost/thread.hpp>

#include "debugHeader.h"

using namespace std;
class ClassLikelihood
{
    map<int,int> numMatches; //Maps the number of the class to the number of matches founded for this superpixel
    boost::mutex mtxNumMatches;
    static map<int,int> totNum; //Maps the number of the class to the total number of superpixels of that class
    static boost::mutex mtxTotNum;
    string featureName;
public:
    ClassLikelihood(string feature);
    static void incTotClass(int classID);
    void foundMatch(int classID);
    double getLogLikelihood(int classID);
    int getLabelsNumber();
    void getKeys(vector<int> &keys);
};
#endif // CLASSLIKELIHOOD_H
