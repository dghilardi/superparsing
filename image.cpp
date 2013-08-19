#include "image.h"

Image::Image(string filename){
    imgName = string(IMG_PATH)+filename+string(IMG_EXT);
    image = cv::imread(imgName, CV_LOAD_IMAGE_COLOR);
}

Image::~Image(){
    //assert(*(image.refcount)==1);
    image.release();
}

void Image::show(){
    cvNamedWindow(imgName.c_str(),2);
    imshow(imgName.c_str(),image);
}

cv::Mat *Image::getImage(){
    return &image;
}
