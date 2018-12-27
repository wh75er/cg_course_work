#ifndef HDR_H
#define HDR_H

#include "libs.h"

class HdrCap
{
public:
    HdrCap();
    ~HdrCap();

    vector<Mat> align_frames(Mat &img1, Mat &img2);
    int deghost_frames(Mat &img1, Mat &img2);
    Mat merge_frames(Mat &img1, Mat &img2);
    int tonemap_frames();

    bool add_frame();

protected:
    Mat computeBitmap(Mat &img);
    int getMedian(Mat &img);

private:
    string* path;
    VideoCapture *hdr_capture;
    VideoCapture *hdr_tonemaped_capture;
};

#endif // HDR_H
