#include "retrimage.h"
#include "opencv2/highgui/highgui.hpp"

#define IMG_NAME "insidecity_art920"

using namespace std;

int main(){
    RetrImage retrievalSetImage(IMG_NAME);
    retrievalSetImage.show();

    cv::waitKey();
    return 0;
}

