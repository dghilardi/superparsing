#include "retrievalset.h"

RetrievalSet::RetrievalSet() : matchResults(), statNeig("data.json")
{
}

void RetrievalSet::computeNeighbourStatistics(NeighbourStat &result, string imgListPath){
    ifstream ifs(imgListPath.c_str());
    string content((istreambuf_iterator<char>(ifs)), istreambuf_iterator<char>());
    Json::Value root;
    Json::Reader reader;
    bool parsedSuccess = reader.parse(content, root, false);

    Json::Value dataSet = root["dataset"];

    if(!parsedSuccess){
        cerr << MSG_ERROR_PARSE_JSON << imgListPath << endl;
        throw ERROR_PARSE_JSON;
    }
    cout<<"Computing Neighborhood statistics:"<<endl;
    string ani("-/|\\");
    for(uint i=0; i<dataSet.size(); ++i){
        string path = dataSet[i].asString();
        RetrImage setImage(path);
        setImage.updateNeighbourStatistics(result);

        stringstream str;
        str << (i*100/dataSet.size()) <<"%\t"<<ani.at(i%ani.size())<<"\t"<<i<<"\r";
        cout << str.str();
        cout.flush();
        result.saveToFile();
    }
    result.saveToFile();
}

void RetrievalSet::computeInstance(string instancePath){
    ifstream ifs(instancePath.c_str());
    string content((istreambuf_iterator<char>(ifs)), istreambuf_iterator<char>());
    Json::Value root;
    Json::Reader reader;
    bool parsedSuccess = reader.parse(content, root, false);

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

    QueryImage query(queryPath);
    //query.showSrc();
    //cv::waitKey();
    LabelImg(query);
}

/**
 * @brief RetrievalSet::LabelImg Insert label for the given image
 * @param imgToLabel segmented image in which it will insert labels
 */
void RetrievalSet::LabelImg(QueryImage &imgToLabel){
    //Recupero il vettore di superpixel della query image
    vector<SuperPixel *> *setSuperPixelsToLabel = imgToLabel.getSuperPixels();
    //Faccio scorrere i superpixels del retrieval set e della query image, provo i match e salvo le statistiche
    for(uint i=0; i<setSuperPixelsToLabel->size(); ++i) matchResults.push_back(new GlobLikelihood());

    //Resetto statistiche vicini
    statNeig.reset();
    //Elaboro le immagini del retrieval set
    for(uint i=0; i<imgNames.size(); ++i){
        //Recupero i superpixel della i-esima immagine del retrieval set
        RetrImage setImage(imgNames[i]);
        //setImage.show();
        //cv::waitKey();
        vector<SuperPixel *> *setImgSuperPixels = setImage.getSuperPixels();
        for(uint j=0; j<setImgSuperPixels->size(); ++j){
            SuperPixel *setSuperPixel = (*setImgSuperPixels)[j];
            ClassLikelihood::incTotClass(setSuperPixel->getLabel());
            for(uint k=0; k<setSuperPixelsToLabel->size(); ++k){
                //Per ogni superpixel dell'immagine calcolo i valori riguardanti il numero di match per ogni classe
                SuperPixel *superPixelToLabel = (*setSuperPixelsToLabel)[k];
                checkSuperPixel(superPixelToLabel, setSuperPixel, *(matchResults[k]));
            }
        }
        stringstream str;
        str << (i*100/imgNames.size()) <<"%\r";
        cout << str.str();
        cout.flush();

        //Save statistics about neighnours
        setImage.updateNeighbourStatistics(statNeig);
    }
    cout << endl;
    //Necessario solo per stampare
    //RetrImage tmp(imgNames[0]);

    //Ad ogni superpixel assegno la label col valore migliore
    for(uint k=0; k<setSuperPixelsToLabel->size(); ++k){
        SuperPixel *superPixelToLabel = (*setSuperPixelsToLabel)[k];
        superPixelToLabel->setLabel(matchResults[k]->getBestLabel());

        //Necessarie solo per stampare
        //cout << "Assign: " << tmp.matchLabel(matchResults[k]->getBestLabel()) << endl;
        //superPixelToLabel->show();
    }
    imgToLabel.showSrc();
    imgToLabel.showLabeling();
    cout << "ERRORE: " << imgToLabel.checkResults()*100 << "%\n";
    //Libero la memoria
    for(int i=0; i<matchResults.size(); ++i) delete matchResults[i];

    //Salvo statistiche su file
    statNeig.saveToFile();
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
