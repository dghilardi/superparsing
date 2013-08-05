#include "retrimage.h"
#include "queryimage.h"
#include "opencv2/highgui/highgui.hpp"

#define IMG_NAME "insidecity_art1161"

using namespace std;

int main(){
    RetrImage retrievalSetImage(IMG_NAME);
    retrievalSetImage.show();

    QueryImage queryImg(IMG_NAME);
    cv::waitKey();
    return 0;
}

