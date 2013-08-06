#include "classlikelihood.h"

#define EPSILON 0.1

ClassLikelihood::ClassLikelihood(string feature){
    featureName = feature;
}

map<string, map<int,int> > ClassLikelihood::totNum; //Static declaration

/**
 * @brief ClassLikelihood::incTotClass increment the value of the times we found the label in the database
 * @param classID id of the label
 */
void ClassLikelihood::incTotClass(int classID){ //Questa può forse essere cambiata per non tener conto della feature in considerazione...
    ++totNum[featureName][classID];
}

/**
 * @brief ClassLikelihood::foundMatch increment the number of times that we have a match of that class within the feature contained in the istance of this class
 * @param classID id of the label found
 */
void ClassLikelihood::foundMatch(int classID){
    ++numMatches[classID];
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
    for(map<int,int>::iterator it = totNum[featureName].begin(); it!=totNum[featureName].end(); ++it) totNotC+=it->second;
    for(map<int,int>::iterator it = numMatches.begin(); it != numMatches.end(); ++it) matchNotC+=it->second;
    totNotC -= totNum[featureName][classID];
    matchNotC -= numMatches[classID];
    double frac1 = (totNum[featureName][classID]+EPSILON)/(double)(totNotC+EPSILON);
    double frac2 = matchNotC/(double)numMatches[classID];
    return log(frac1*frac2);
}

/**
 * @brief ClassLikelihood::getLabelsNumber
 * @return Total number of labels present
 */
int ClassLikelihood::getLabelsNumber(){
    return totNum[featureName].size();
}

/**
 * @brief ClassLikelihood::getKeys Compute a vector that contains all the feature id with associated a value
 * @param keys vector that will be filled
 */
void ClassLikelihood::getKeys(vector<int> &keys){
    for(map<int,int>::iterator it = totNum[featureName].begin(); it!=totNum[featureName].end(); ++it) keys.push_back(it->first);
}
