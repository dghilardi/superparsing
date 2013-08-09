#include "superpixel.h"
#include <opencv2/nonfree/nonfree.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv/cv.hpp>
#include <math.h>

SuperPixel::SuperPixel(vector<Pixel> &list, cv::Mat &srcImg): maskFeature(0), siftHist(1,100,CV_32F,cv::Scalar(0)), pixelCoordList(list), label(-1){
    //Compute the bounding box of the superpixel
    int minX = list[0].x, minY = list[0].y;
    int maxX = minX, maxY = minY;
    for(uint i=1; i<list.size(); ++i){
        if(list[i].x>maxX) maxX = list[i].x;
        if(list[i].x<minX) minX = list[i].x;
        if(list[i].y>maxY) maxY = list[i].y;
        if(list[i].y<minY) minY = list[i].y;
    }

    //Ritaglio il superpixel dall'immagine originale
    int dimX = maxX-minX+1;
    int dimY = maxY-minY+1;
    //cv::Mat superPixelImg(dimY, dimX, CV_8UC3, cv::Scalar::all(0));
    superPixelImg.create(dimY, dimX, CV_8UC3);
    superPixelImg.setTo(cv::Scalar::all(0));
    for(uint i=0; i<list.size(); ++i){
        int x = list[i].x;
        int y = list[i].y;
        cv::Point3_<uchar>* src = srcImg.ptr<cv::Point3_<uchar> >(y,x);
        cv::Point3_<uchar>* dst = superPixelImg.ptr<cv::Point3_<uchar> >(y-minY,x-minX);
        dst->x=(uchar)src->x; //blue
        dst->y=(uchar)src->y; //green
        dst->z=(uchar)src->z; //red
    }

    /*
    cvNamedWindow("SUPERPIXEL",2);
    imshow("SUPERPIXEL",superPixelImg);
    cv::waitKey();
    */

    computeMaskFeature(list, minX, minY, maxX, maxY);
    relHeightFeature = (srcImg.rows-minY)/(double)srcImg.rows;

    computeSiftFeature(superPixelImg);
    computeColorFeature(superPixelImg);;
}

SuperPixel::~SuperPixel(){
    delete colorHist[0];
    delete colorHist[1];
    delete colorHist[2];
}

int SuperPixel::getLabel(){
    return label;
}

void SuperPixel::setLabel(int newLabel){
    label = newLabel;
}

/**
 * @brief SuperPixel::computeShapeFeature Get an 8x8 mask of the shape of the superpixel (64bit)
 */
void SuperPixel::computeMaskFeature(vector<Pixel> &pixelList, int minX, int minY, int maxX, int maxY){
    //Parameters for the computation of the mask-feature
    int dimX = maxX-minX;
    int dimY = maxY-minY;
    double divisorX = (dimX+0.00001)/(double)8, divisorY=(dimY+0.00001)/(double)8;
    int bucket[8][8] = {0};

    //Fill buckets to check if a cell of the feature will be considered set or unset
    for(uint i=0; i<pixelList.size(); ++i){
        int x = pixelList[i].x;
        int y = pixelList[i].y;
        int posx = (int)((x-minX)/divisorX);
        int posy = (int)((y-minY)/divisorY);

        if(divisorX>0 && divisorY>0)
            bucket[posx][posy]++;
    }

    for(int i=0; i<8; ++i){ for(int j=0; j<8; ++j){
            int actualXindex = (int)(i*divisorX);
            int successorXindex = (int)((i+1)*divisorX);
            if(((int)(successorXindex/divisorX))!=i+1) ++successorXindex;
            if(((int)(actualXindex/divisorX))!=i) ++actualXindex;

            int actualYindex = (int)(j*divisorY);
            int successorYindex = (int)((j+1)*divisorY);
            if(((int)(successorYindex/divisorY))!=j+1) successorYindex++;
            if(((int)(actualYindex/divisorY))!=j) actualYindex++;

            int bucketSize = (successorXindex-actualXindex)*(successorYindex-actualYindex);

            if(bucket[i][j]>bucketSize/2){
                maskFeature ^= ((uint64)1 << (8*i+j));
                if(8*i+j>63) throw;
            }
            //bitset<64> bits(maskFeature);
            //cout << bits << endl;
            //cout <<(i+j==0?"\n--":"")<<(j==0?"\n":"")<< ((bucket[i][j]>bucketSize/2)?(char)'\u2660':' ');
    }}

}

/**
 * @brief SuperPixel::getMaskDistance Compute the shape-distance between two SuperPixels
 * @param otherSP The SuperPixel to confront with
 * @return  The number of different cells in the mask
 */
int SuperPixel::getMaskDistance(SuperPixel &otherSP){
    int distance = 0;
    uint64 axorb = otherSP.maskFeature^maskFeature;
    //cout << maskFeature << " XOR " << otherSP.maskFeature << " = " << axorb << endl;
    while(axorb){
      distance++;
      axorb &= axorb-1;
    }
    return distance;
}

float SuperPixel::getRelHeightDistance(SuperPixel &otherSP){
    return abs(otherSP.relHeightFeature-relHeightFeature);
}

/**
 * @brief SuperPixel::computeSiftFeature Calcola la SIFT feature del superpixel.
 * @param superPixelImg Matrice contenente il superpixel
 */
void SuperPixel::computeSiftFeature(cv::Mat &superPixelImg){
    //Individuo i Keypoints del superPixel usando SIFT
    cv::SiftFeatureDetector detector;
    std::vector<cv::KeyPoint> keypoints;
    detector.detect(superPixelImg,keypoints);

    /*Calcolo istogramma quantizzato degli orientamenti dei gradienti.
     *Considero come istogramma un vettore di 100 posizioni, ognuna delle quali
     *rappresenta un intervallo di 36 gradi e contiene il numero di keypoints
     *aventi orientamento appartenente a tale intervallo
     */
    //int* colorHist;
    //colorHist = (int*) calloc(128, sizeof(int));
    /*
    for(int i=0; i<100; i++){
        //siftHist[i,0]=0;
        siftHist.at<int>(0,i)=0;
    }*/
    for(uint i=0; i<keypoints.size(); i++){
        int bucket = std::floor(keypoints[i].angle/(36));
        siftHist.at<float>(0,bucket)++;
    }
}

/**
 * @brief SuperPixel::getSiftDistance Calcola la distanza della SIFT feature
 * @param otherSP Il superpixel rispetto al quale calcolare la distanza
 * @return La misura della distanza
 */
double SuperPixel::getSiftDistance(SuperPixel &otherSP){
    //Calcolo la distanza in base alla correlazione dei due istogrammi
    double distance = cv::compareHist(siftHist,otherSP.siftHist,CV_COMP_CORREL);
    return distance;
}

/**
 * @brief SuperPixel::computeColorFeature Calcola la Color feature
 * @param superPixelImg Matrice contenente il superpixel
 */
void SuperPixel::computeColorFeature(cv::Mat &superPixelImg){
    //Separo il superpixel in 3 canali RGB
    vector<cv::Mat> rgb_planes;
    cv::split(superPixelImg, rgb_planes);
    //Stabilisco il numero di bins e il range di valori
    int histSize = 11;
    float range[] = {0,256};
    const float* histRange = {range};
    //Imposto flag per avere una distribuzione uniforme dei bins e affiché gli istogrammi siano inizialmente vuoti
    bool uniform = true;
    bool accumulate = false;
    //Istanzio i 3 istogrammi (1 per canale)
    cv::Mat *r_hist, *g_hist, *b_hist;
    r_hist = new cv::Mat;
    g_hist = new cv::Mat;
    b_hist = new cv::Mat;
    //Calcolo i 3 istogrammi
    calcHist( &rgb_planes[0], 1, 0, cv::Mat(), *b_hist, 1, &histSize, &histRange, uniform, accumulate );
    calcHist( &rgb_planes[1], 1, 0, cv::Mat(), *g_hist, 1, &histSize, &histRange, uniform, accumulate );
    calcHist( &rgb_planes[2], 1, 0, cv::Mat(), *r_hist, 1, &histSize, &histRange, uniform, accumulate );
    //Creo un array di array contenente i tre istogrammi e lo ritorno
    //cv::Mat* hists;
    //hists = (cv::Mat*) calloc(3, sizeof(cv::Mat));
    colorHist[0] = r_hist;
    colorHist[1] = g_hist;
    colorHist[2] = b_hist;
}

/**
 * @brief SuperPixel::getColorDistance Calcola la distanza della Color feature
 * @param otherSP Il superpixel rispetto al quale calcolare la distanza
 * @return La misura della distanza
 */
double SuperPixel::getColorDistance(SuperPixel &otherSP){
    //Calcolo la distanza in base alla correlazione dei due istogrammi
    double dist_r = cv::compareHist(*colorHist[0],*otherSP.colorHist[0],CV_COMP_CORREL);
    double dist_g = cv::compareHist(*colorHist[1],*otherSP.colorHist[1],CV_COMP_CORREL);
    double dist_b = cv::compareHist(*colorHist[2],*otherSP.colorHist[2],CV_COMP_CORREL);
    double avg = (dist_r+dist_g+dist_b)/3.0;
    return avg;
}

void SuperPixel::show(){
    cvNamedWindow("SUPERPIXEL",2);
    imshow("SUPERPIXEL",superPixelImg);
    cv::waitKey();
}

/**
 * @brief SuperPixel::printToMat Print to the given cv::Mat the region regarding the superpixel
 * @param result destination cv::Mat
 */
void SuperPixel::printToMat(cv::Mat &result){
    if(label==-1){
        cerr << "ERROR! the superPixel hasn't set a label" << endl;
        throw 1;
    }else{
        for(uint i=0; i<pixelCoordList.size(); ++i){
            result.at<uchar>(pixelCoordList[i].y, pixelCoordList[i].x) = (uchar)255*label/33;
        }
    }
}

