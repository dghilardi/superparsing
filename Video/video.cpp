#include "video.h"

Video::Video(string path) : vid(path){
}

void Video::getImages(vector<IplImage *> &result, int maxNumber){
    assert(result.empty());
    cv::Mat image;
    for(int i=0; i<maxNumber; ++i){
        vid >> image;
        IplImage *tmpimg = new IplImage(image);
        result.push_back(cvCloneImage(tmpimg));
        delete tmpimg;
    }
}
