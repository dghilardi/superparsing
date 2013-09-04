#include "retrievalset.h"

RetrievalSet::RetrievalSet()
{
}

RetrievalSet::~RetrievalSet(){
    for(int i=0; i<matchResults.size(); ++i) delete matchResults[i];
}
string RetrievalSet::getNextTrainingPath(){
    string ani("-/|\\");
    stringstream str;
    str << (trainingPos*100/trainingSet.size()) <<"%\t"<<ani.at(trainingPos%ani.size())<<"\t"<<trainingPos<<"\r";
    neigTraining.lock();
        cout.flush();
        cout << str.str();
        if(trainingPos>=trainingSet.size()){
            neigTraining.unlock();
            return "";
        }
        string path = trainingSet[trainingPos].asString();
        trainingPos++;
    neigTraining.unlock();
    return path;
}

void RetrievalSet::training(NeighbourStat *result,  RetrievalSet *setInstance){
    while(true){
        string path = setInstance->getNextTrainingPath();
        if(path.empty()) break;
        RetrImage setImage(path);
        setImage.updateNeighbourStatistics(*result);
        /*
        if((trainingPos%200)==0){
            for(int x=0; x<33; x++){
                for(int y=0; y<33; ++y){
                    cout <<((y==0) ? "\n" : "") <<setw(3)<<setfill(' ')<< result.getNeighbourNum(x,y) << " ";
                }
            }
            cout << endl << endl;
        }*/
    }
}

void RetrievalSet::computeNeighbourStatistics(NeighbourStat &result, string imgListPath, int threadNum){
    ifstream ifs(imgListPath.c_str());
    string content((istreambuf_iterator<char>(ifs)), istreambuf_iterator<char>());
    Json::Value root;
    Json::Reader reader;
    bool parsedSuccess = reader.parse(content, root, false);

    GlobalSettings::imgPath = root["imgPath"].asString();
    GlobalSettings::labelsPath = root["labelsPath"].asString();
    trainingSet = root["dataset"];

    if(!parsedSuccess){
        cerr << MSG_ERROR_PARSE_JSON << imgListPath << endl;
        throw ERROR_PARSE_JSON;
    }
    cout<<"Computing Neighborhood statistics:"<<endl;
    trainingPos = 0;
    boost::thread threads[threadNum];
    for(int i=0; i<threadNum; ++i){
        threads[i] = boost::thread(&RetrievalSet::training, &result, this);
    }
    for(int i=0; i<threadNum; ++i){
        threads[i].join();
    }
    //training(&result, &dataSet);
    result.saveToFile();
}

void RetrievalSet::assignLabels(QueryImage &query)
{
    vector<SuperPixel *> *setSuperPixelsToLabel = query.getSuperPixels();
    for(uint k=0; k<setSuperPixelsToLabel->size(); ++k){
        SuperPixel *superPixelToLabel = (*setSuperPixelsToLabel)[k];
        superPixelToLabel->setLabel(matchResults[k]->getBestLabel());

        //Necessarie solo per stampare
        //cout << "Assign: " << tmp.matchLabel(matchResults[k]->getBestLabel()) << endl;
        //superPixelToLabel->show();
    }
    query.showSrc();
    query.showLabeling();
    cout << "ERRORE: " << query.checkResults()*100 << "%\n";
}

void RetrievalSet::computeImage(int nThreads, bool useMRF, ThreadSafeStringSet &nameSet, NeighbourStat &stat, string queryPath)
{
    QueryImage query(queryPath);
    //query.showSrc();
    //cv::waitKey();
    vector<SuperPixel *> *setSuperPixelsToLabel = query.getSuperPixels();
    for(uint i=0; i<setSuperPixelsToLabel->size(); ++i) matchResults.push_back(new GlobLikelihood());
    //computeLabels(query, &nameSet, &matchResults);
    boost::thread threadList[nThreads];
    for(int i=0; i<nThreads; ++i){
        threadList[i] = boost::thread(&computeLabels, &query, &nameSet, &matchResults);
    }
    for(int i=0; i<nThreads; ++i) threadList[i].join();


    //Ad ogni superpixel assegno la label col valore migliore
    assignLabels(query);

    if(useMRF) applyMRF(query, stat);
}

void RetrievalSet::computeVideo(int nThreads, bool useMRF, ThreadSafeStringSet &nameSet, NeighbourStat &stat, string queryPath){
    QueryVideo query(queryPath);
    vector<SuperVoxel *> *setSuperVoxelToLabel = query.getSuperVoxelsList();
    vector<map<int, SuperPixel *> *> setSuperPixelsToLabel;
    vector<SuperVoxelLikelihood *> statLikelihood;
    for(uint i=0; i<setSuperVoxelToLabel->size(); ++i){
        setSuperPixelsToLabel.push_back((*setSuperVoxelToLabel)[i]->getSuperPixels());
        statLikelihood.push_back(new SuperVoxelLikelihood());
    }
    boost::thread threadList[nThreads];
    for(int i=0; i<nThreads; ++i){
        threadList[i] = boost::thread(&computeLabelsMulti, &setSuperPixelsToLabel, &nameSet, &statLikelihood);
    }
    for(int i=0; i<nThreads; ++i) threadList[i].join();
    if(useMRF){
        vector<SuperVoxel *> *setSuperVoxels = query.getSuperVoxelsList();
        MRF::computeMRF<SuperVoxel, SuperVoxelLikelihood>(*setSuperVoxels, statLikelihood, stat);
    }
}

void RetrievalSet::computeInstance(string instancePath, NeighbourStat &stat, bool useMRF, int nThreads, bool isQueryImage){
    vector<string> imgNames;
    ifstream ifs(instancePath.c_str());
    string content((istreambuf_iterator<char>(ifs)), istreambuf_iterator<char>());
    Json::Value root;
    Json::Reader reader;
    bool parsedSuccess = reader.parse(content, root, false);

    GlobalSettings::imgPath = root["imgPath"].asString();
    GlobalSettings::labelsPath = root["labelsPath"].asString();

    Json::Value dataSet = root["dataset"];
    for(uint i=0; i<dataSet.size(); ++i){
        imgNames.push_back(dataSet[i].asString());
    }

    string queryPath = root["query"].asString();

    parsedSuccess = parsedSuccess && imgNames.size()>0 && !queryPath.empty();
    //Reading failure
    if(!parsedSuccess){
        cerr << MSG_ERROR_PARSE_JSON << instancePath << endl;
        throw ERROR_PARSE_JSON;
    }

    ThreadSafeStringSet nameSet(imgNames);

    if(isQueryImage)computeImage(nThreads, useMRF, nameSet, stat, queryPath);
    else computeVideo(nThreads, useMRF, nameSet, stat, queryPath);
}

void RetrievalSet::computeLabelsMulti(vector<map<int, SuperPixel *> *> *setSuperPixelsToLabel, ThreadSafeStringSet *imgNames, vector<SuperVoxelLikelihood *> *matchResults){
    while(true){
        string setImagePath = imgNames->getNext();
        if(setImagePath.empty()) break;
        RetrImage setImage(setImagePath);

        vector<SuperPixel *> *setImgSuperPixels = setImage.getSuperPixels();
        for(int j=0; j<setImgSuperPixels->size(); ++j){
            SuperPixel *setSuperPixel = (*setImgSuperPixels)[j];
            ClassLikelihood::incTotClass(setSuperPixel->getLabel());
            for(int i=0; i<setSuperPixelsToLabel->size(); ++i){
                for(map<int, SuperPixel *>::iterator it=(*setSuperPixelsToLabel)[i]->begin(); it!=(*setSuperPixelsToLabel)[i]->end(); ++it){
                    SuperPixel *spToLabel = it->second;
                    checkSuperPixel(spToLabel, setSuperPixel, *((*matchResults)[i]->getGlobLikelihood(it->first, spToLabel->getWeight())));
                }
            }
        }
    }
}

/**
 * @brief RetrievalSet::LabelImg Insert label for the given image
 * @param imgToLabel segmented image in which it will insert labels
 */
void RetrievalSet::computeLabels(QueryImage *imgToLabel, ThreadSafeStringSet *imgNames, vector<GlobLikelihood *> *matchResults){

    //Recupero il vettore di superpixel della query image
    vector<SuperPixel *> *setSuperPixelsToLabel = imgToLabel->getSuperPixels();
    //Faccio scorrere i superpixels del retrieval set e della query image, provo i match e salvo le statistiche

    //Elaboro le immagini del retrieval set
    while(true){
        string setImagePath = imgNames->getNext();
        if(setImagePath.empty()) break;
        //Recupero i superpixel della i-esima immagine del retrieval set
        RetrImage setImage(setImagePath);
        //setImage.show();
        //cv::waitKey();
        vector<SuperPixel *> *setImgSuperPixels = setImage.getSuperPixels();
        for(uint j=0; j<setImgSuperPixels->size(); ++j){
            SuperPixel *setSuperPixel = (*setImgSuperPixels)[j];
            ClassLikelihood::incTotClass(setSuperPixel->getLabel());
            for(uint k=0; k<setSuperPixelsToLabel->size(); ++k){
                //Per ogni superpixel dell'immagine calcolo i valori riguardanti il numero di match per ogni classe
                SuperPixel *superPixelToLabel = (*setSuperPixelsToLabel)[k];
                checkSuperPixel(superPixelToLabel, setSuperPixel, *(matchResults->at(k)));
            }
        }
    }
}

/**
 * @brief RetrievalSet::checkSuperPixel Compute the distance of two superpixels given within all the features and saves results
 * @param toLabel Superpixel da etichettare
 * @param inSet Superpixel preso da un'immagine del rertieval set
 * @param spixelResults Instance of the class (relative for the superpixel to label) which takes count
 *                      of the number of match for each feature, each class and total number of superpixels per class
 */
void RetrievalSet::checkSuperPixel(SuperPixel *toLabel, SuperPixel *inSet, GlobLikelihood &spixelResults){
    double tk_mask=15, tk_relH=0.15, tk_sift=0.1, tk_color=0.1; //Valori casuali solo per provare

    int actualLabel = inSet->getLabel(); //Indice del label del superpixel preso dal dataset
    //cout << actualLabel << endl;
    if(actualLabel==9) throw;

    double maskDistance = toLabel->getMaskDistance(*inSet);
    if(maskDistance<tk_mask) spixelResults.mask.foundMatch(actualLabel);
    //if(maskDistance<tk_mask) cout << "Found mask: " << inSet->getLabel() <<" dist: "<<maskDistance << endl;
    //else cout << "No mask" << endl;

    double relHDistance = toLabel->getRelHeightDistance(*inSet);
    if(relHDistance<tk_relH) spixelResults.relH.foundMatch(actualLabel);
    //if(relHDistance<tk_relH) cout << "found relh: " << inSet->getLabel() << endl;
    //else cout << "No distance" << endl;

    double siftDistance = toLabel->getSiftDistance(*inSet);
    if(1-siftDistance<tk_sift) spixelResults.quantSIFT.foundMatch(actualLabel);
    //if(abs(siftDistance)<tk_sift) cout << "found sift: "<< inSet->getLabel() << endl;
    //else cout << "No SIFT" << endl;

    double colDistance = toLabel->getColorDistance(*inSet);
    if(1-colDistance<tk_color) spixelResults.colorHist.foundMatch(actualLabel);
    //if(abs(colDistance)<tk_color) cout << "found color: " << inSet->getLabel() << endl;
    //else cout << "No Color" << endl;
}

void RetrievalSet::applyMRF(QueryImage &imgToLabel, NeighbourStat &stat){
    vector<SuperPixel *> *setSuperPixelsToLabel = imgToLabel.getSuperPixels();
    MRF::computeMRF<SuperPixel>(*setSuperPixelsToLabel, matchResults, stat);
    cout << "ERRORE: " << imgToLabel.checkResults()*100 << "%\n";
}
