#include "queryimage.h"

#include <iostream>

/**
 * @brief QueryImage::QueryImage Costruttore della classe che si occupa di generare le feature per la query image
 * @param filename Nome della query image
 */
QueryImage::QueryImage(string filename, bool computeSIFT): image(filename), imgName(filename) {
    IplImage iplimg = *(image.getImage());
    vector<vector<Pixel> > superPixels;
    segmentImage(&iplimg, superPixels);

    cv::Mat segmented(image.getImage()->size(), CV_8UC1);
    int nSp = superPixels.size();
    for(int i=0; i<nSp; ++i){
        superPixelList.push_back(new SuperPixel(superPixels[i], *image.getImage(), computeSIFT));
        for(uint j=0; j<superPixels[i].size(); ++j){
            assert(superPixels[i][j].y<segmented.rows);
            assert(superPixels[i][j].x<segmented.cols);
            segmented.at<uchar>(superPixels[i][j].y, superPixels[i][j].x) = (uchar)255*i/nSp;
        }
    }
    SuperPixel::computeAdiacents(superPixelList, image.getImage()->rows, image.getImage()->cols);
    SuperPixel::computeWeight(superPixelList);

    cvNamedWindow("superPixels",2);
    imshow("superPixels",segmented);
}

QueryImage::~QueryImage(){
    for(int i=0; i<superPixelList.size(); ++i){
        delete superPixelList[i];
    }
}

/**
 * @brief QueryImage::getSuperPixels Metodo per ottenere il vettore di superpixel della queryimage
 */
vector<SuperPixel *> *QueryImage::getSuperPixels(){
    return &superPixelList;
}

/**
 * @brief QueryImage::buildObtainedMat Build the mat representing the obtained labeling
 * @param result Mat in which the function will print the result
 */
void QueryImage::buildObtainedMat(cv::Mat &result, set<int> *usedLabels){
    result.create(image.getImage()->size(), CV_8UC1);
    result.setTo(cv::Scalar::all(0));
    if(usedLabels!=NULL) usedLabels->clear();
    for(uint i=0; i<superPixelList.size(); ++i){
        if(usedLabels!=NULL) usedLabels->insert(superPixelList[i]->getLabel());
        superPixelList[i]->printToMat(result);
    }
}

/**
 * @brief QueryImage::showLabeling Show the labeling made to the current image
 */
void QueryImage::showLabeling(){
    cv::Mat labeledImage, toVisualize;
    set<int> usedLabels;
    buildObtainedMat(labeledImage, &usedLabels);
    VisualUtils::colorLabels(labeledImage, toVisualize, usedLabels);
    cvNamedWindow("LABELED IMAGE",CV_WINDOW_AUTOSIZE);
    imshow("LABELED IMAGE",toVisualize);
}

void QueryImage::showSrc(){
    image.show();
}

double QueryImage::checkResults(){
    cv::Mat resultingImg;
    cv::Mat difference(image.getImage()->size(), CV_8UC1, cv::Scalar::all(0));
    buildObtainedMat(resultingImg);
    GeoLabel correctLabeling(imgName);
    //Check that the two Mat have the same size
    assert(correctLabeling.getLabeledImg()->size() == resultingImg.size());
    int diff = 0;
    for(int x=0; x<resultingImg.cols; ++x){ for(int y=0; y<resultingImg.rows; ++y){
            if(resultingImg.at<uchar>(y,x)!=(uchar)correctLabeling.getLabeledImg()->at<short>(y,x)) ++diff;
            else difference.at<uchar>(y,x) = 255;
    }}
    cvNamedWindow("DIFFERENCE", 2);
    imshow("DIFFERENCE", difference);
    return diff/(double)(resultingImg.rows*resultingImg.cols);
}
