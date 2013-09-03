#include "video.h"

Video::Video(string path) : vid(path){
}

void Video::getImages(vector<cv::Mat> &result, int maxNumber){
    assert(result.empty());
    cv::Mat image;
    for(int i=0; i<maxNumber; ++i){
        vid >> image;
        result.push_back(image.clone());
    }
}
