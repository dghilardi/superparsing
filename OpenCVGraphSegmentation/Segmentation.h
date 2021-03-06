//
//  Segmentation.h
//
//  Created by Salik Syed on 8/16/11.
// 
//

/*
Copyright (C) 2010 Salik Syed

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
*/

#pragma once
#include <opencv/cv.h>
#include <opencv2/highgui/highgui.hpp>
#include <vector>


using namespace std;

typedef struct {
    int x;
    int y;
    int f;
}Pixel;

void segmentImage(IplImage* input, vector<vector<Pixel> > & resultBuffer);
void segmentVideo(vector<cv::Mat> &input, vector<vector<Pixel> > &resultBuffer);

