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

    Mat colorLabeled(Mat &labelImage, int nLabels);

    Mat imgToShowTransform(Mat &img);
    Mat imgToBitmapTransform(Mat &img);

    std::vector<Mat> getWeights(Mat &img1, Mat &img2);
    Mat getWeightedMap(Mat &img);
    double exponential_euclidean(double channel, double sigma);
    double max_channel(double r, double g, double b);
    int min_channel(int r, int g, int b);

private:
    string* path;
    VideoCapture *hdr_capture;
    VideoCapture *hdr_tonemaped_capture;
};

#endif // HDR_H
