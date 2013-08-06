#include "retrimage.h"

/**
 * @brief RetrImage::RetrImage Costruttore della classe che si occupa di elaborare un'immagine del retrieval set
 * @param imgName Il nome dell'immagine del retrieval set
 */
RetrImage::RetrImage(string imgName): image(imgName), labeledImg(imgName) {
    buildSuperPixels();
}

/**
 * @brief RetrImage::show
 */
void RetrImage::show(){
    labeledImg.viewLabeledImg();
    image.show();
}

/**
 * @brief RetrImage::buildSuperPixels Metodo che estrapola i superpixel
 */
void RetrImage::buildSuperPixels()
{
    list<pair<int,int> > toCheck;
    map<pair<int,int>,bool> imagePixels;
    cv::Mat *labelsMat = labeledImg.getLabeledImg();
    for(int x=0; x<labelsMat->cols; ++x){
        for(int y=0; y<labelsMat->rows; ++y){
            imagePixels[pair<int,int>(x,y)] = false;
        }
    }
    //Search for regions and create SuperPixel
    while(imagePixels.size()>0){
        int x = imagePixels.begin()->first.first;
        int y = imagePixels.begin()->first.second;
        vector<Pixel> currentSP;
        short currentLabel = labelsMat->at<short>(y,x);
        toCheck.push_back(imagePixels.begin()->first);
        currentSP.push_back(Pixel());
        currentSP.back().x=x;
        currentSP.back().y=y;
        imagePixels.erase(imagePixels.begin());
        while(toCheck.size()>0){
            int nx = toCheck.begin()->first;
            int ny = toCheck.begin()->second;
            toCheck.erase(toCheck.begin());
            for(int dx=-1; dx<=1; ++dx){
                for(int dy=-1; dy<=1; ++dy){
                    pair<int,int> p(nx+dx,ny+dy);
                    if(imagePixels.count(p) && labelsMat->at<short>(ny+dy,nx+dx)==currentLabel){
                        toCheck.push_back(imagePixels.find(p)->first);
                        imagePixels.erase(imagePixels.find(p));
                        currentSP.push_back(Pixel());
                        currentSP.back().x=nx+dx;
                        currentSP.back().y=ny+dy;
                    }
                }
            }
        }
        if(currentLabel!=0){
            superPixelList.push_back(new SuperPixel(currentSP, *image.getImage()));
            superPixelList.back()->setLabel(currentLabel);
            //cout << "Superpixel, label: " << currentLabel << "size: " << currentSP.size() <<"\t"<<labeledImg.getLabel(currentLabel)<< endl;
        }
    }
}

/**
 * @brief RetrImage::getSuperPixels Metodo per ottenere il vettore di superpixel
 */
vector<SuperPixel *> *RetrImage::getSuperPixels(){
    return &superPixelList;
}
