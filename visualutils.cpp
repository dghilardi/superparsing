#include "visualutils.h"

VisualUtils::VisualUtils()
{
}

void VisualUtils::hslToRgb(cv::Vec3f hsl, cv::Vec3b &rgb){
    double r, g, b, v;
    r = hsl[2];
    g = hsl[2];
    b = hsl[2];
    v = hsl[2]<=0.5 ? (hsl[2]*(1.0*hsl[1])) : (hsl[2]+hsl[1]-hsl[2]*hsl[1]);
    if(v>0){
        double m, sv;
        int sextant;
        double fract, vsf, mid1, mid2;

        m=hsl[2]+hsl[2]-v;
        sv = (v-m)/v;
        hsl[0] *= 6.0;
        sextant = (int)hsl[0];
        fract = hsl[0]-sextant;
        vsf = v*sv*fract;
        mid1 = m+vsf;
        mid2 = v-vsf;
        switch(sextant){
            case 0:
            r=v;
            g=mid1;
            b=m;
            break;
        case 1:
            r=mid2;
            g=v;
            b=m;
            break;
        case 2:
            r=m;
            g=v;
            b=mid1;
            break;
        case 3:
            r=m;
            g=mid2;
            b=v;
            break;
        case 4:
            r=mid1;
            g=m;
            b=v;
            break;
        case 5:
            r=v;
            g=m;
            b=mid2;
            break;
        }
    }
    cout << r << g  << b << endl;
    rgb[0] = (uchar)(r*255);
    rgb[1] = (uchar)(g*255);
    rgb[2] = (uchar)(b*255);

}

void VisualUtils::findDistinctColor(int index, int total, cv::Vec3b &result){
    VisualUtils::hslToRgb(cv::Vec3f(index/(float)total, 0.5, 0.5), result);
}

void VisualUtils::colorLabels(cv::Mat &labeled, cv::Mat &result, set<int> &labels){
    result.create(labeled.rows, labeled.cols+100, CV_8UC3);
    result.setTo(cv::Scalar::all(0));
    int nlabels = labels.size();
    map<int, cv::Vec3b> labelColors;
    int i=0, wmargin=labeled.cols+5;
    for(set<int>::iterator it=labels.begin(); it!=labels.end(); ++it){
        cv::Vec3b tmp;
        findDistinctColor(i, nlabels, tmp);
        labelColors[*it] = tmp;

        int hmargin = 15*i+5;
        cv::rectangle(result, cv::Point(wmargin, hmargin), cv::Point(wmargin+10, hmargin+10), cv::Scalar(tmp[0], tmp[1], tmp[2]), CV_FILLED);
        cv::putText(result, GeoLabel::getLabel(*it), cv::Point(wmargin+15,hmargin+10), 0, 0.5, cv::Scalar::all(255), 1, CV_AA);
        i++;
    }
    for(int x=0; x<labeled.cols; ++x){ for(int y=0; y<labeled.rows; ++y){
            cv::Point3_<uchar> *p = result.ptr<cv::Point3_<uchar> >(y,x);
            assert(labelColors.find(labeled.at<uchar>(y,x))!=labelColors.end());
            cv::Vec3b color = labelColors[(int)labeled.at<uchar>(y,x)];
            p->x = color[0];
            p->y = color[1];
            p->z = color[2];
    }}
}
