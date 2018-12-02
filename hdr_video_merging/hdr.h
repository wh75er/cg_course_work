#ifndef HDR_H
#define HDR_H

#include "libs.h"

class HdrCap
{
public:
    HdrCap();
    ~HdrCap();

    vector<Mat> align_frames(Mat &img1, Mat &img2);
    int deghost_frames();
    int merge_frames();
    int tonemap_frames();

    bool add_frame();

private:
    string* path;
    VideoCapture *cap;
};

#endif // HDR_H
