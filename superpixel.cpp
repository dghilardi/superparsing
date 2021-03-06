#include "superpixel.h"
#include <opencv2/nonfree/nonfree.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv/cv.hpp>
#include <math.h>
#include "debugHeader.h"
SuperPixel::SuperPixel(vector<Pixel> &list, cv::Mat &srcImg, bool computeSIFT): maskFeature(0), pixelCoordList(list), weight(0){
    //Compute the bounding box of the superpixel
    label=-1;
    int minX = list[0].x, minY = list[0].y;
    int maxX = minX, maxY = minY;
    for(uint i=1; i<list.size(); ++i){
        if(list[i].x>maxX) maxX = list[i].x;
        if(list[i].x<minX) minX = list[i].x;
        if(list[i].y>maxY) maxY = list[i].y;
        if(list[i].y<minY) minY = list[i].y;
    }
    topLeftCorner.x = minX;
    topLeftCorner.y = minY;

    //Ritaglio il superpixel dall'immagine originale
    int dimX = maxX-minX+1;
    int dimY = maxY-minY+1;
    //cv::Mat superPixelImg(dimY, dimX, CV_8UC3, cv::Scalar::all(0));
    superPixelImg = srcImg(cv::Rect(minX, minY, dimX, dimY));

    //superPixelImg.create(dimY, dimX, CV_8UC3);
    //superPixelImg.setTo(cv::Scalar::all(0));
    mask.create(dimY, dimX, CV_8UC1);
    mask.setTo(cv::Scalar::all(0));
    for(uint i=0; i<list.size(); ++i){
        int x = list[i].x;
        int y = list[i].y;
        //cv::Point3_<uchar>* src = srcImg.ptr<cv::Point3_<uchar> >(y,x);
        //cv::Point3_<uchar>* dst = superPixelImg.ptr<cv::Point3_<uchar> >(y-minY,x-minX);
        //dst->x=(uchar)src->x; //blue
        //dst->y=(uchar)src->y; //green
        //dst->z=(uchar)src->z; //red
        mask.at<uchar>(y-minY,x-minX) = 255;
    }


    /*
    cvNamedWindow("SUPERPIXEL",2);
    imshow("SUPERPIXEL",superPixelImg);
    cv::waitKey();
    */

        computeMaskFeature(list, minX, minY, maxX, maxY);
        relHeightFeature = (srcImg.rows-minY)/(double)srcImg.rows;

        //computeSiftFeature(superPixelImg);
    if(computeSIFT){
        QuantizedSift *quantization = QuantizedSift::getInstance();
        computeSIFTDescriptor(quantization);
    }
        computeColorFeature(superPixelImg);

}

SuperPixel::SuperPixel(cv::Mat &img, cv::Mat &spMask, bool computeSIFT):label(-1){
    superPixelImg = img;
    mask = spMask;

    maskFeature = 0;
    relHeightFeature = -1;

        //computeSiftFeature(superPixelImg);
    if(computeSIFT){
        QuantizedSift *quantization = QuantizedSift::getInstance();
        computeSIFTDescriptor(quantization);
    }
        computeColorFeature(superPixelImg);
}

SuperPixel::~SuperPixel(){
    /*
    assert(*(colorHist[0]->refcount)==1);
    assert(*(colorHist[1]->refcount)==1);
    assert(*(colorHist[2]->refcount)==1);
    assert(*(siftHist.refcount)==1);
    */
    delete colorHist[0];
    delete colorHist[1];
    delete colorHist[2];
}

int SuperPixel::getLabel(){
    return label;
}
cv::Mat *SuperPixel::getMask(){
    return &mask;
}

cv::Point SuperPixel::getTopLeftCorner(){
    return topLeftCorner;
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
            //cout <<(i+j==0?"\n--":"") << (j==0?"\n":"") << ((bucket[j][i]>bucketSize/2)?(char)'#':' ');
    }}
    //show();

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
    /*
    bitset<64> one(maskFeature), two(otherSP.maskFeature);
    cout << one << endl << two << endl <<distance<<endl << endl;
    if(distance<10){
        show();
        cv::waitKey(100);
        otherSP.show();
        cv::waitKey(500);
    }*/
    return distance;
}

float SuperPixel::getRelHeightDistance(SuperPixel &otherSP){
    return abs(otherSP.relHeightFeature-relHeightFeature);
}

/**
 * @brief SuperPixel::computeSiftFeature Calcola la SIFT feature del superpixel.
 * @param superPixelImg Matrice contenente il superpixel
 */
void SuperPixel::computeSiftFeature(cv::Mat &descriptor, bool computeDense){
    cv::initModule_nonfree();
    //Individuo i Keypoints del superPixel usando SIFT
    //cv::Ptr<cv::FeatureDetector> detector = cv::FeatureDetector::create("SIFT");
    //cv::GridAdaptedFeatureDetector gridDetector(detector, 1000, 8, 8);
    //cv::Ptr<FeatureDetector> detector = FeatureDetector::create("Dense");

    //cv::DenseFeatureDetector *denseDetector = new cv::DenseFeatureDetector(1,1,0.1,10);
    //cv::SiftFeatureDetector *siftDetector = new cv::SiftFeatureDetector;    

    cv::FeatureDetector *detector;
    if(computeDense) detector = new cv::DenseFeatureDetector(1,1,0.1,10);
    else detector = new cv::SiftFeatureDetector();
    std::vector<cv::KeyPoint> keypoints;
    detector->detect(superPixelImg, keypoints, mask);

    cv::DescriptorExtractor *extractor = new cv::SIFT();
    //cv::Mat descriptor;
    extractor->compute(superPixelImg, keypoints, descriptor);

    //cout << "rows: " << descriptor.rows << " cols: " << descriptor.cols << endl;
    //cout << descriptor << endl;
    //cv::KeyPoint abc = keypoints[0];
    //cout << keypoints.size() << endl;
    //throw;
    /*
      vector matches;
      cv::BruteForceMatcher<cv::L2> matcher;
    */

    /*Calcolo istogramma degli orientamenti dei gradienti.
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
    /*for(uint i=0; i<keypoints.size(); i++){
        int bucket = std::floor(keypoints[i].angle/(3.6));
        siftHist.at<float>(0,bucket)++;
    }*/
    delete extractor;
    delete detector;
}

void SuperPixel::computeSIFTDescriptor(QuantizedSift *quantization){
    cv::Mat descriptor;
    computeSiftFeature(descriptor);
    int keyPointNumber = descriptor.rows;
    for(int i=0; i<QUANTIZATION_SIZE; ++i){
        siftDescriptor[i]=0;
    }
    for(int i=0; i<keyPointNumber; ++i){
        int dicIndex = quantization->getIndex(descriptor.row(i));
        assert(dicIndex<QUANTIZATION_SIZE);
        siftDescriptor[dicIndex] += 1/(float)keyPointNumber;
    }
}

/**
 * @brief SuperPixel::getSiftDistance Calcola la distanza della SIFT feature
 * @param otherSP Il superpixel rispetto al quale calcolare la distanza
 * @return La misura della distanza
 */
double SuperPixel::getSiftDistance(SuperPixel &otherSP){
    //Calcolo la distanza in base alla correlazione dei due istogrammi
    double distance = 0;
    for(int i=0; i<QUANTIZATION_SIZE; ++i){
        distance+=abs(pow(otherSP.siftDescriptor[i]-siftDescriptor[i],2));
    }
    return sqrt(distance);
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
    calcHist( &rgb_planes[0], 1, 0, mask, *b_hist, 1, &histSize, &histRange, uniform, accumulate );
    calcHist( &rgb_planes[1], 1, 0, mask, *g_hist, 1, &histSize, &histRange, uniform, accumulate );
    calcHist( &rgb_planes[2], 1, 0, mask, *r_hist, 1, &histSize, &histRange, uniform, accumulate );
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
        //cout <<"Label: " <<label << endl;
        for(uint i=0; i<pixelCoordList.size(); ++i){
            result.at<uchar>(pixelCoordList[i].y, pixelCoordList[i].x) = (uchar)label;
        }
    }
}

/**
 * @brief SuperPixel::appendAdiacent Insert a superpixel as a adiacent
 * @param toAppend SuperPixel to insert as adiacent
 */
void SuperPixel::appendAdiacent(SuperPixel *toAppend){
    adiacentsSet.insert(toAppend);
}
/**
 * @brief SuperPixel::getAdiacents return pointer to the set of adiacents
 * @return
 */
const set<SuperPixel *> *SuperPixel::getAdiacents(){
    return &adiacentsSet;
}

const vector<Pixel> *SuperPixel::getCoordList(){
    return &pixelCoordList;
}

void SuperPixel::computeAdiacents(vector<SuperPixel *> &spList, int height, int width){
    SuperPixel *superPixelsMap[width][height];
    for(int i=0; i<width; ++i) for(int j=0; j<height; ++j) superPixelsMap[i][j] = NULL;
    for(uint i=0; i<spList.size(); ++i){
        vector<Pixel> pixCoord = spList[i]->pixelCoordList;
        for(uint j=0; j<pixCoord.size(); ++j){
            int x = pixCoord[j].x;
            int y = pixCoord[j].y;
            assert(x<width && y<height);
            superPixelsMap[x][y] = spList[i];
        }
    }

    for(int x=0; x<width-1; ++x){
        for(int y=0; y<height-1; ++y){
            if(superPixelsMap[x][y]!=NULL){
                if(y>0 && superPixelsMap[x+1][y-1]!=NULL && superPixelsMap[x+1][y-1]!=superPixelsMap[x][y]){
                    superPixelsMap[x+1][y-1]->appendAdiacent(superPixelsMap[x][y]);
                    superPixelsMap[x][y]->appendAdiacent(superPixelsMap[x+1][y-1]);
                }
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

double SuperPixel::getWeight(){
    return weight;
}

void SuperPixel::computeWeight(vector<SuperPixel *> &spList){
    double dimMean=0;
    int nSP = spList.size();
    for(int i=0; i<nSP; ++i){
        int dim = spList[i]->pixelCoordList.size();
        dimMean += dim/(double)nSP;
    }
    for(int i=0; i<nSP; ++i) spList[i]->weight = spList[i]->pixelCoordList.size()/dimMean;
}
