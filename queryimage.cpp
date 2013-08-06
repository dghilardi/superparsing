#include "queryimage.h"

#include <iostream>

QueryImage::QueryImage(string filename): image(filename) {
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

vector<SuperPixel *> *QueryImage::getSuperPixels(){
    return &superPixelList;
}
