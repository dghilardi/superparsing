#include "queryimage.h"

#include <iostream>

QueryImage::QueryImage(string filename): image(filename) {
    IplImage iplimg = *(image.getImage());
    segmentImage(&iplimg, superPixels);

    cv::Mat segmented(image.getImage()->size(), CV_8UC1);
    int nSp = superPixels.size();
    for(int i=0; i<nSp; ++i){
        for(int j=0; j<superPixels[i].size(); ++j){
            segmented.at<uchar>(superPixels[i][j].y, superPixels[i][j].x) = 255*i/nSp;
        }
    }

    cvNamedWindow("superPixels",2);
    imshow("superPixels",segmented);
}
