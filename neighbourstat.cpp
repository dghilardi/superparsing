#include "neighbourstat.h"

NeighbourStat::NeighbourStat(string path): filename(path)
{
    ifstream ifs(filename.c_str());
    string content((istreambuf_iterator<char>(ifs)), istreambuf_iterator<char>());
    Json::Value root;
    Json::Reader reader;
    bool parsedSuccess = reader.parse(content, root, false);

    //Reading failure
    if(!parsedSuccess){
        cout << "Failed to parse JSON" << endl;
        cout << reader.getFormatedErrorMessages() << endl;
    }

    statArray = root[STAT_NAME];
}

/**
 * @brief NeighbourStat::getNeighbourNum Gets the number of times that the class idA is adiacent to the class idB
 * @param idA
 * @param idB
 * @return
 */
int NeighbourStat::getNeighbourNum(int idA, int idB){
    int min = idA<idB ? idA : idB;
    int max = idA>idB ? idA : idB;
    return statArray[min][max].asInt();
}

/**
 * @brief NeighbourStat::incNeigNum Increment the number of times that the class idA is adiacent to the class idB
 * @param idA
 * @param idB
 */
void NeighbourStat::incNeigNum(int idA, int idB){
    int min = idA<idB ? idA : idB;
    int max = idA>idB ? idA : idB;
    statArray[min][max] = statArray[min][max].asInt()+1;
}

/**
 * @brief NeighbourStat::saveToFile Save statistics to file
 */
void NeighbourStat::saveToFile(){
    Json::Value root;
    root[STAT_NAME] = statArray;

    Json::StyledWriter writer;
    string fileContent = writer.write(root);

    ofstream output(filename.c_str());
    output << fileContent;
    output.close();
}

/**
 * @brief NeighbourStat::reset Reset statistics
 */
void NeighbourStat::reset(){
    statArray.clear();
}

/**
 * @brief conditionalNeigProb probability to find idA given adiacent idB: P(idA|idB)
 * @param idA
 * @param idB
 * @return
 */
double NeighbourStat::conditionalNeigProb(int idA, int idB){
    int foundTimes = getNeighbourNum(idA, idB);
    int totNum = 0;
    for(int i=0; i<idB; ++i) totNum += statArray[i][idB].asInt();
    for(int i=idB+1; i<statArray.size(); ++i) totNum += statArray[idB][i].asInt();

    double prob = totNum>0 ? foundTimes/(double)totNum : 0;
    assert(prob<=1);
    return prob;
}
