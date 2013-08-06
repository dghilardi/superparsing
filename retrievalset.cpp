#include "retrievalset.h"

RetrievalSet::RetrievalSet(vector<string> &names) : imgNames(names), matchResults()
{
}

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
            for(uint k=0; k<setSuperPixelsToLabel->size(); ++k){
                //Ad ogni superpixel della query image assegno un'etichetta
                SuperPixel *superPixelToLabel = (*setSuperPixelsToLabel)[k];
                checkSuperPixel(superPixelToLabel, setSuperPixel, *(matchResults[k]));
            }
        }
    }

    for(uint k=0; k<setSuperPixelsToLabel->size(); ++k){
        SuperPixel *superPixelToLabel = (*setSuperPixelsToLabel)[k];
        superPixelToLabel->setLabel(matchResults[k]->getBestLabel());
        RetrImage tmp(imgNames[0]);
        cout << "Assign: " << tmp.matchLabel(matchResults[k]->getBestLabel()) << endl;
    }

    for(int i=0; i<matchResults.size(); ++i) delete matchResults[i];
}

/**
 * @brief RetrievalSet::checkSuperPixel
 * @param toLabel Superpixel da etichettare
 * @param inSet Superpixel preso da un'immagine del rertieval set
 * @param spixelResults
 */
void RetrievalSet::checkSuperPixel(SuperPixel *toLabel, SuperPixel *inSet, GlobLikelihood &spixelResults){
    double tk_mask=10, tk_relH=0.1, tk_sift=0.2, tk_color=0.2;

    int actualLabel = inSet->getLabel();

    double maskDistance = toLabel->getMaskDistance(*inSet);
    spixelResults.mask.incTotClass(actualLabel);
    if(maskDistance<tk_mask) spixelResults.mask.foundMatch(actualLabel);
    if(maskDistance<tk_mask) cout << "Found mask" << endl;

    double relHDistance = toLabel->getRelHeightDistance(*inSet);
    spixelResults.relH.incTotClass(actualLabel);
    if(relHDistance<tk_relH) spixelResults.relH.foundMatch(actualLabel);
    if(relHDistance<tk_relH) cout << "found relh" << endl;

    double siftDistance = toLabel->getSiftDistance(*inSet);
    spixelResults.quantSIFT.incTotClass(actualLabel);
    if(siftDistance<tk_sift) spixelResults.quantSIFT.foundMatch(actualLabel);
    if(abs(siftDistance)<tk_sift) cout << "found sift: "<<siftDistance << endl;

    double colDistance = toLabel->getColorDistance(*inSet);
    spixelResults.colorHist.incTotClass(actualLabel);
    if(colDistance<tk_color) spixelResults.colorHist.foundMatch(actualLabel);
    if(abs(colDistance)<tk_color) cout << "found color: " <<colDistance<< endl;
}
