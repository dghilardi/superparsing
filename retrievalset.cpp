#include "retrievalset.h"

RetrievalSet::RetrievalSet(vector<string> &names) : imgNames(names), matchResults()
{
}

/**
 * @brief RetrievalSet::LabelImg Insert label for the given image
 * @param imgToLabel segmented image in which it will insert labels
 */
void RetrievalSet::LabelImg(QueryImage &imgToLabel){
    //Recupero il vettore di superpixel della query image
    vector<SuperPixel *> *setSuperPixelsToLabel = imgToLabel.getSuperPixels();
    //Ad ogni superpixel assegno la migliore feature che lo descrive
    for(uint i=0; i<setSuperPixelsToLabel->size(); ++i) matchResults.push_back(new GlobLikelihood());
    //Elaboro le immagini del retrieval set
    for(uint i=0; i<imgNames.size(); ++i){
        //Recupero i superpixel della i-esima immagine del retrieval set
        RetrImage setImage(imgNames[i]);
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
    }

    //Ad ogni superpixel assegno la label col valore migliore
    for(uint k=0; k<setSuperPixelsToLabel->size(); ++k){
        SuperPixel *superPixelToLabel = (*setSuperPixelsToLabel)[k];
        superPixelToLabel->setLabel(matchResults[k]->getBestLabel());
        //Necessarie solo per stampare
        RetrImage tmp(imgNames[0]);
        cout << "Assign: " << tmp.matchLabel(matchResults[k]->getBestLabel()) << endl;
    }

    //Libero la memoria
    for(int i=0; i<matchResults.size(); ++i) delete matchResults[i];
}

/**
 * @brief RetrievalSet::checkSuperPixel Compute the distance of two superpixels given within all the features and saves results
 * @param toLabel Superpixel da etichettare
 * @param inSet Superpixel preso da un'immagine del rertieval set
 * @param spixelResults Instance of the class (relative for the superpixel to label) which takes count
 *                      of the number of match for each feature, each class and total number of superpixels per class
 */
void RetrievalSet::checkSuperPixel(SuperPixel *toLabel, SuperPixel *inSet, GlobLikelihood &spixelResults){
    double tk_mask=10, tk_relH=0.1, tk_sift=0.2, tk_color=0.2; //Valori casuali solo per provare

    int actualLabel = inSet->getLabel(); //Indice del label del superpixel preso dal dataset

    double maskDistance = toLabel->getMaskDistance(*inSet);
    if(maskDistance<tk_mask) spixelResults.mask.foundMatch(actualLabel);
    if(maskDistance<tk_mask) cout << "Found mask" << endl;

    double relHDistance = toLabel->getRelHeightDistance(*inSet);
    if(relHDistance<tk_relH) spixelResults.relH.foundMatch(actualLabel);
    if(relHDistance<tk_relH) cout << "found relh" << endl;

    double siftDistance = toLabel->getSiftDistance(*inSet);
    if(siftDistance<tk_sift) spixelResults.quantSIFT.foundMatch(actualLabel);
    if(abs(siftDistance)<tk_sift) cout << "found sift: "<<siftDistance << endl;

    double colDistance = toLabel->getColorDistance(*inSet);
    if(colDistance<tk_color) spixelResults.colorHist.foundMatch(actualLabel);
    if(abs(colDistance)<tk_color) cout << "found color: " <<colDistance<< endl;
}
