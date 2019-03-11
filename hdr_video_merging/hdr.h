#ifndef HDR_H
#define HDR_H

#include "libs.h"

class HdrCap
{
public:
    HdrCap();
    ~HdrCap();

    vector<Mat> align_frames(Mat &img1, Mat &img2);
    Mat deghost_frames(Mat &img1, Mat &img2);
    Mat merge_frames(Mat &img1, Mat &img2);
    int tonemap_frames();

    bool add_frame();

protected:
    int exclude_range = 4;
    Mat computeBitmap(Mat &img);
    int getMedian(Mat &img);

    Mat getMotionMap(Mat &tb1, Mat &tb2);

    Mat morphClosing(Mat &src);
    Mat erosion(Mat &img, Mat &kernel);
    Mat dilation(Mat &img, Mat &kernel);
    bool checkPattern(Mat &img, Mat &kernel, size_t img_i, size_t img_j, int option);

    Mat imgToShowTransform(Mat &img);
    Mat imgToBitmapTransform(Mat &img);

private:
    string* path;
    VideoCapture *hdr_capture;
    VideoCapture *hdr_tonemaped_capture;
};

#endif // HDR_H
