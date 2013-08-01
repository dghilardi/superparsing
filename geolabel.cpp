#include "geolabel.h"
#include "math.h"
#include <stdio.h>

GeoLabel::GeoLabel(string filename){
    imgName = string(MAT_PATH)+filename+string(MAT_EXT);
    mat_t *geoLabelf = Mat_Open(imgName.c_str(), MAT_ACC_RDONLY);
    if(geoLabelf==NULL) cerr << MSG_ERROR_LOADING_MAT << imgName << endl;
    matvar_t *matVar;

    /*
    // Print variables names
    while((matVar=Mat_VarReadNextInfo(geoLabelf))!=NULL){
        cout << "DataName: " << matVar->name << endl;
        Mat_VarFree(matVar);
        matVar = NULL;
    }
    */

    //Load labeled image from file
    int start[2]={0,0}, stride[2]={0,0}, edge[2];
    ushort ptr[257][256];
    matVar = Mat_VarReadInfo(geoLabelf, "S");
    int resultLoadFile;
    if(matVar!=NULL){
        edge[0] = matVar->dims[0]+1;
        edge[1] = matVar->dims[1];
        resultLoadFile = Mat_VarReadData(geoLabelf, matVar, ptr, start, stride, edge);
    }
    if(matVar==NULL || resultLoadFile!=0){
        cerr << "ERROR! impossible to load file" << endl;
        throw ERROR_LOADING_MAT;
    }

    //Create opencv matrix containing labeled image
    imageLabeled.create(matVar->dims[0], matVar->dims[1], CV_16SC1);
    for(int x=0; x<imageLabeled.cols; ++x){
        for(int y=0; y<imageLabeled.rows; ++y){
            imageLabeled.at<short>(y,x) = ptr[x][y];
            //cout << imageLabeled.at<short>(y,x) <<" - "<< ptr[x][y] << endl;
        }
    }

    Mat_VarFree(matVar);

    //Load label names and put them in the vector
    matVar = Mat_VarReadInfo(geoLabelf, "names");

    if(matVar!=NULL){
        for(uint i=0; i<matVar->dims[1]; ++i){
            matvar_t *cell = Mat_VarGetCell(matVar, i);

            Mat_VarReadDataAll(geoLabelf, cell);
            string *newLabel = new string((char*)cell->data);
            labels.push_back(newLabel);

            //Clean string from strange chars at end
            while(newLabel->at(newLabel->length()-1)<'0') newLabel->erase(--newLabel->end());
            free(cell);
        }
    }
    if(matVar==NULL || resultLoadFile!=0){
        cerr << "ERROR! impossible to load"<< (resultLoadFile!=0?" from":"") <<" file" << endl;
        throw ERROR_LOADING_MAT;
    }
    //Mat_VarFree(matVar);

    Mat_Close(geoLabelf);
}

GeoLabel::~GeoLabel(){
    for(int i=0; i<labels.size(); ++i){
        delete labels[i];
    }
}

/**
 * @brief GeoLabel::getLabeledImg Get an opencv Mat that assign to each pixel a label (or no one)
 * @return pointer to the Mat
 */
cv::Mat *GeoLabel::getLabeledImg(){
    return &imageLabeled;
}

/**
 * @brief GeoLabel::getLabel Return label associated to specified index
 * @param index number representing a label
 * @return String with the name of the label
 */
string GeoLabel::getLabel(int index){
    if(index>0 && index<=(int)labels.size()) return *labels[index-1];
    if(index==-1) return NO_LABEL;
    //Index out of vector size
    cerr << MSG_ERROR_LABEL_INDEX << index << endl;
    throw ERROR_LABEL_INDEX;
}

void GeoLabel::viewLabeledImg(){
    int maxValue = labels.size();
    cv::Mat toVisualize(imageLabeled.size(), CV_8UC1);

    for(int x=0; x<imageLabeled.cols; ++x){
        for(int y=0; y<imageLabeled.rows; ++y){
            toVisualize.at<uchar>(y,x) = 255*(imageLabeled.at<short>(y,x)+1)/(maxValue+1);
        }
    }

    for(int i=1; i<=labels.size(); ++i) cout <<255*(i+1)/(maxValue+1) <<". "<< getLabel(i) << endl;
    cvNamedWindow(imgName.c_str(),2);
    imshow(imgName.c_str(),toVisualize);
}
