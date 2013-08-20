#include "classlikelihood.h"

#define EPSILON 0.01

ClassLikelihood::ClassLikelihood(string feature){
    featureName = feature;
}

map<int,int> ClassLikelihood::totNum; //Static declaration
boost::mutex ClassLikelihood::mtxTotNum;

/**
 * @brief ClassLikelihood::incTotClass increment the value of the times we found the label in the database
 * @param classID id of the label
 */
void ClassLikelihood::incTotClass(int classID){
    mtxTotNum.lock();
    ++totNum[classID];
    mtxTotNum.unlock();
}

/**
 * @brief ClassLikelihood::foundMatch increment the number of times that we have a match of that class within the feature contained in the istance of this class
 * @param classID id of the label found
 */
void ClassLikelihood::foundMatch(int classID){
    mtxNumMatches.lock();
    ++numMatches[classID];
    mtxNumMatches.unlock();
}

/**
 * @brief ClassLikelihood::getLogLikelihood Compute the log of the probability of the pixel to be in that class
 *        (regarding the feature that the class refers to) log(P(fik|c)/P(fik|not c))
 * @param classID the class of which we want to compute the probability
 * @return log(P(fik|c)/P(fik|not c))
 */
double ClassLikelihood::getLogLikelihood(int classID){
    int totNotC = 0;
    int matchNotC = 0;
    bool existstotnum = false, existsnummatch = false;
    if(totNum.size()>0) for(map<int,int>::iterator it = totNum.begin(); it!=totNum.end(); ++it){
        if(it->first != classID) totNotC+=it->second;
        else existstotnum = true;
    }
    if(numMatches.size()>0) for(map<int,int>::iterator it = numMatches.begin(); it != numMatches.end(); ++it){
        if(it->first != classID) matchNotC+=it->second;
        else existsnummatch = true;
    }
    double frac1, frac2;
    if(!existstotnum) frac1 = 0; //?
    else frac1 = totNotC/totNum[classID];

    if(!existsnummatch) frac2 = (EPSILON)/(double)(matchNotC+EPSILON);
    else frac2 = (numMatches[classID]+EPSILON)/(double)(matchNotC+EPSILON);

    return log(frac1*frac2);
}

/**
 * @brief ClassLikelihood::getLabelsNumber
 * @return Total number of labels present
 */
int ClassLikelihood::getLabelsNumber(){
    return totNum.size();
}

/**
 * @brief ClassLikelihood::getKeys Compute a vector that contains all the feature id with associated a value
 * @param keys vector that will be filled
 */
void ClassLikelihood::getKeys(vector<int> &keys){
    for(map<int,int>::iterator it = totNum.begin(); it!=totNum.end(); ++it) keys.push_back(it->first);
}
