#include "queryimage.h"

#include <iostream>

/**
 * @brief QueryImage::QueryImage Costruttore della classe che si occupa di generare le feature per la query image
 * @param filename Nome della query image
 */
QueryImage::QueryImage(string filename): image(filename), imgName(filename) {
    IplImage iplimg = *(image.getImage());
    vector<vector<Pixel> > superPixels;
    segmentImage(&iplimg, superPixels);

    cv::Mat segmented(image.getImage()->size(), CV_8UC1);
    int nSp = superPixels.size();
    for(int i=0; i<nSp; ++i){
        superPixelList.push_back(new SuperPixel(superPixels[i], *image.getImage()));
        for(int j=0; j<superPixels[i].size(); ++j){
            segmented.at<uchar>(superPixels[i][j].y, superPixels[i][j].x) = 255*i/nSp;
        }
    }
    cout << "Distance: mask: "<<superPixelList[3]->getMaskDistance(*superPixelList[0]) << "relHeight: " <<superPixelList[3]->getRelHeightDistance(*superPixelList[0])<< endl;
    cout << "SIFT Distance: " << superPixelList[3]->getSiftDistance(*superPixelList[0]) << endl;
    cout << "Color Distance: " << superPixelList[3]->getColorDistance(*superPixelList[0]) << endl;

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
void QueryImage::buildObtainedMat(cv::Mat &result){
    result.create(image.getImage()->size(), CV_8UC1);
    result.setTo(cv::Scalar::all(0));
    for(uint i=0; i<superPixelList.size(); ++i) superPixelList[i]->printToMat(result);
}

/**
 * @brief QueryImage::showLabeling Show the labeling made to the current image
 */
void QueryImage::showLabeling(){
    cv::Mat labeledImage;
    buildObtainedMat(labeledImage);
    cvNamedWindow("LABELED IMAGE",2);
    imshow("LABELED IMAGE",labeledImage);
}

void QueryImage::showSrc(){
    image.show();
}

double QueryImage::checkResults(){
    cv::Mat resultingImg;
    buildObtainedMat(resultingImg);
    GeoLabel correctLabeling(imgName);
    //Check that the two Mat have the same size
    assert(correctLabeling.getLabeledImg()->size() == resultingImg.size());
    int diff = 0;
    for(int x=0; x<resultingImg.cols; ++x){ for(int y=0; y<resultingImg.rows; ++y){
            if(resultingImg.at<uchar>(y,x)!=(uchar)correctLabeling.getLabeledImg()->at<short>(y,x)) ++diff;
    }}
    return diff/(double)(resultingImg.rows*resultingImg.cols);
}
