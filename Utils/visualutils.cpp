#include "visualutils.h"

#define SQUARE_SIZE 10
#define LEG_COL_WIDTH 100
#define LEG_PADDING 5
#define LEG_LIST_PADDING 8

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
    //cout << r << g  << b << endl;
    rgb[0] = (uchar)(r*255);
    rgb[1] = (uchar)(g*255);
    rgb[2] = (uchar)(b*255);

}

void VisualUtils::findDistinctColor(int index, int total, cv::Vec3b &result){
    int subtotal, subindex;
    double lightness, saturation;
    if(index<total/5){
        subtotal = total/5;
        subindex = index;
        lightness = 0.4;
        saturation = 0.6;
    }else if(index<total/2){
        subtotal = total/2-total/5;
        subindex = index - total/5;
        lightness = 0.6;
        saturation = 0.4;
    }else{
        subtotal = total/2;
        subindex = index-total/2;
        lightness = 0.6;
        saturation = 0.6;
    }
    VisualUtils::hslToRgb(cv::Vec3f(subindex/(float)subtotal, saturation, lightness), result);
}

void VisualUtils::colorLabels(cv::Mat &labeled, cv::Mat &result, set<int> &labels){
    int legPerCol = (labeled.rows-LEG_PADDING) / (SQUARE_SIZE+LEG_LIST_PADDING);
    int legendSize = (labels.size()/legPerCol+1)*LEG_COL_WIDTH;
    result.create(labeled.rows, labeled.cols+legendSize, CV_8UC3);
    result.setTo(cv::Scalar::all(0));
    int nlabels = labels.size();
    map<int, cv::Vec3b> labelColors;
    int i=0, wmargin=labeled.cols+LEG_PADDING;
    for(set<int>::iterator it=labels.begin(); it!=labels.end(); ++it){
        cv::Vec3b tmp;
        findDistinctColor(i, nlabels, tmp);
        labelColors[*it] = tmp;

        int hmargin = (SQUARE_SIZE+LEG_LIST_PADDING)*(i%legPerCol)+LEG_PADDING;
        int hpadding = (i/legPerCol)*LEG_COL_WIDTH;
        cv::rectangle(result, cv::Point(wmargin+hpadding, hmargin), cv::Point(wmargin+SQUARE_SIZE+hpadding, hmargin+SQUARE_SIZE), cv::Scalar(tmp[0], tmp[1], tmp[2]), CV_FILLED);
        cv::putText(result, GeoLabel::getLabel(*it), cv::Point(wmargin+15+hpadding,hmargin+10), 0, 0.5, cv::Scalar::all(255), 1, CV_AA);
        i++;
    }
    for(int x=0; x<labeled.cols; ++x){ for(int y=0; y<labeled.rows; ++y){
            cv::Point3_<uchar> *p = result.ptr<cv::Point3_<uchar> >(y,x);
            //cout << "LABEL: " <<(int)labeled.at<uchar>(y,x)<<endl;
            assert(labelColors.find(labeled.at<uchar>(y,x))!=labelColors.end());
            cv::Vec3b color = labelColors[(int)labeled.at<uchar>(y,x)];
            p->x = color[0];
            p->y = color[1];
            p->z = color[2];
    }}
}
