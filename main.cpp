#include "retrimage.h"
#include "queryimage.h"
#include "retrievalset.h"
#include "neighbourstat.h"
#include <getopt.h>

#include "opencv2/highgui/highgui.hpp"

#define INST_NAME "../instance.json"
#define STAT_FILE_NAME "statistics.dat"

using namespace std;

void help(string progName){
    cout << "usage: " << progName << " [ options ]" << endl;
    cout << "  -i <file>    --instance <file>   files containing names of image of the retrieval set and query image" << endl;
    cout << "  -r           --retrain           Redo the training phase" << endl;
    cout << "  -h           --help              Gives this help" << endl;
}

int main(int argc, char **argv){
    string instancePath;
    string trainingSetPath;
    static struct option long_options[] = {
        {"instance", 1, 0, 'i'},
        {"retrain", 1, 0, 'r'},
        {"help", 0, 0, 'h'},
        {NULL, 0, NULL, 0}
    };
    if(argc!=1){
        int c, option_index=0;
        while((c=getopt_long(argc, argv, "q:r:h", long_options, &option_index))!=-1){
            //int this_option_optind = optind ? optind : 1;
            cout << "arg: " << c << endl;
            switch(c){
            case 'i':
                instancePath = optarg;
                break;
            case 'r':
                trainingSetPath = optarg;
                break;
            case 'h':
                help(argv[0]);
                return 0;
                break;
            case 0:
                break;
            default:
                help(argv[0]);
                return 1;
                break;
            }
        }
    }else{
        instancePath = INST_NAME;
    }
    NeighbourStat neighbourStatistics(STAT_FILE_NAME);
    if(!trainingSetPath.empty()){
        neighbourStatistics.reset();
        RetrievalSet::computeNeighbourStatistics(neighbourStatistics, trainingSetPath);
    }
    if(neighbourStatistics.isEmpty()){
        cerr << "ERROR! no statistics about neighbour founded." << endl;
        return 1;
    }
    if(!instancePath.empty()){
        RetrievalSet db;
        db.computeInstance(instancePath);
        cv::waitKey();
    }
    return 0;
}

