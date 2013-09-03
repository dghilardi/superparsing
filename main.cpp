#include "mrf.h"
#include "retrimage.h"
#include "queryimage.h"
#include "retrievalset.h"
#include "neighbourstat.h"
#include <getopt.h>
#include "Video/queryvideo.h"

#include "opencv2/highgui/highgui.hpp"

#define INST_NAME "../instance.json"
#define STAT_FILE_NAME "statistics.dat"

using namespace std;

void help(string progName){
    cout << "usage: " << progName << " [ options ]" << endl;
    cout << "  -i <file>    --image-inst <file>   files containing names of image of the retrieval set and query image" << endl;
    cout << "  -v <file>    --video-inst <file>   files containing names of image of the retrieval set and query video" << endl;
    cout << "  -r <file>    --retrain    <file>   Redo the training phase on the specified set" << endl;
    cout << "  -m           --mrf                 Use markov random fields (needs to be trained)" << endl;
    cout << "  -h           --help                Gives this help" << endl;
}

int main(int argc, char **argv){
    string imgInstancePath;
    string vidInstancePath;
    string trainingSetPath;
    bool useMRF = false;
    static struct option long_options[] = {
        {"image-inst", 1, 0, 'i'},
        {"video-inst", 1, 0, 'v'},
        {"retrain", 1, 0, 'r'},
        {"help", 0, 0, 'h'},
        {"mrf", 0, 0,'m'},
        {NULL, 0, NULL, 0}
    };
    if(argc!=1){
        int c, option_index=0;
        while((c=getopt_long(argc, argv, "i:v:r:hm", long_options, &option_index))!=-1){
            //int this_option_optind = optind ? optind : 1;
            cout << "arg: " << (char)c << endl;
            switch(c){
            case 'i':
                imgInstancePath = optarg;
                break;
            case 'v':
                vidInstancePath = optarg;
                break;
            case 'r':
                trainingSetPath = optarg;
                break;
            case 'm':
                useMRF = true;
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
        imgInstancePath = INST_NAME;
    }
    NeighbourStat neighbourStatistics(STAT_FILE_NAME);
    RetrievalSet imSet;
    if(!trainingSetPath.empty()){
        neighbourStatistics.reset();
        imSet.computeNeighbourStatistics(neighbourStatistics, trainingSetPath, 4);
    }
    if(neighbourStatistics.isEmpty() && useMRF){
        cerr << "ERROR! no statistics about neighbour founded." << endl;
        return 1;
    }
    if(!imgInstancePath.empty()){
        imSet.computeInstance(imgInstancePath, neighbourStatistics, useMRF, 4);
        cv::waitKey();
    }
    if(!vidInstancePath.empty()){
        QueryVideo prova(vidInstancePath);
    }
    return 0;
}

