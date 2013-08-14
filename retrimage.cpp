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
    SuperPixel *superPixelsMap[labelsMat->cols][labelsMat->rows]; //Maps each pixel to the superpixel instance
    for(int x=0; x<labelsMat->cols; ++x){
        for(int y=0; y<labelsMat->rows; ++y){
            imagePixels[pair<int,int>(x,y)] = false;
            superPixelsMap[x][y] = NULL;
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
            //Check if the neighbours have the same label
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
            SuperPixel *toAdd = new SuperPixel(currentSP, *image.getImage());
            superPixelList.push_back(toAdd);
            superPixelList.back()->setLabel(currentLabel);
            for(uint i=0; i<currentSP.size(); ++i){ //Map each pixel to the pointer of his superPixel
                assert(superPixelsMap[currentSP[i].x][currentSP[i].y]==NULL);
                superPixelsMap[currentSP[i].x][currentSP[i].y] = toAdd;
            }
            //cout << "Superpixel, label: " << currentLabel << "size: " << currentSP.size() <<"\t"<<labeledImg.getLabel(currentLabel)<< endl;
        }
    }

    //Set the adiacents to each instance of superPixel
    for(int x=0; x<labelsMat->cols-1; ++x){
        for(int y=0; y<labelsMat->rows-1; ++y){
            if(superPixelsMap[x][y]!=NULL){
                for(int dx=0; dx<2; ++dx){ for(int dy=0; dy<2; ++dy){
                    if(dx+dy>0 && superPixelsMap[x+dx][y+dy]!=NULL && superPixelsMap[x+dx][y+dy]!=superPixelsMap[x][y]){
                        superPixelsMap[x+dx][y+dy]->appendAdiacent(superPixelsMap[x][y]);
                        superPixelsMap[x][y]->appendAdiacent(superPixelsMap[x+dx][y+dy]);
                    }
                }}
            }
        }
    }
}

/**
 * @brief RetrImage::getSuperPixels Metodo per ottenere il vettore di superpixel
 */
vector<SuperPixel *> *RetrImage::getSuperPixels(){
    return &superPixelList;
}

string RetrImage::matchLabel(int id){
    return labeledImg.getLabel(id);
}

/**
 * @brief RetrImage::updateNeighbourStatistics For each superpixels of the retrieval image saves the neighbour statistics
 * @param statistics
 */
void RetrImage::updateNeighbourStatistics(NeighbourStat &statistics){
    for(uint i=0; i<superPixelList.size(); ++i){
        int actualClass = superPixelList[i]->getLabel();
        set<SuperPixel *> *adiacents = superPixelList[i]->getAdiacents();
        for(set<SuperPixel *>::iterator it=adiacents->begin(); it!=adiacents->end(); ++it){
            statistics.incNeigNum(actualClass, (*it)->getLabel());
        }
    }
}
