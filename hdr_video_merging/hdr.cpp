#include "hdr.h"

HdrCap::HdrCap()
{
}

HdrCap::~HdrCap()
{
}

vector<Mat> HdrCap::align_frames(Mat &img1, Mat &img2)
{
    vector<Mat> images;
    images.push_back(img1);
    images.push_back(img2);

    std::cout << img1.cols << 'x' << img1.rows << '\n';
    std::cout << img2.cols << 'x' << img2.rows << '\n';

    Ptr<AlignMTB> alignMTB = createAlignMTB();
    alignMTB->process(images, images);

    std::cout << images[0].cols << 'x' << images[0].rows << '\n';
    std::cout << images[1].cols << 'x' << images[1].rows << '\n';

    std::cout << '\n';

    return images;
}

Mat HdrCap::deghost_frames(Mat &img1, Mat &img2)
{
    //unsigned char *p = img2.ptr();
    //cout << img1.channels() << "\n";
    //cout << "Blue chanel: " << int(p[0]) << "\n";
    Mat gray;
    cvtColor(img1, gray, COLOR_RGB2GRAY);
    vector<Mat> bitmaps = computeBitmaps(gray);
    unsigned char *p = bitmaps[0].ptr();
    cout << int(p[0]) << " " << int(p[1]) << " " << int(p[2]) << "\n";

    return bitmaps[0];
}

Mat HdrCap::merge_frames(Mat &img1, Mat &img2)
{
    vector<Mat> images;
    images.push_back(img1);
    images.push_back(img2);

    Ptr<MergeMertens> merge_mertens = createMergeMertens();

    Mat result;
    merge_mertens->process(images, result);

    return result;
}

vector<Mat> HdrCap::computeBitmaps(Mat &img)
{
    vector<Mat> bitmaps;
    Mat tb, eb;
    tb.create(img.size(), CV_8U);

    int median = getMedian(img);

    compare(img, median, tb, CMP_GT);
    compare(abs(img-median), exclude_range, eb, CMP_GT);
    bitmaps.push_back(tb);
    bitmaps.push_back(eb);

    return bitmaps;
}

int HdrCap::getMedian(Mat &img)
{
    int channels = 0;
    Mat hist;
    int hist_size = LDR_SIZE;
    float range[] = {0, LDR_SIZE} ;
    const float* ranges[] = {range};
    calcHist(&img, 1, &channels, Mat(), hist, 1, &hist_size, ranges);
    float *ptr = hist.ptr<float>();
    int median = 0, sum = 0;
    int thresh = int(img.total()) / 2;
    while(sum < thresh && median < LDR_SIZE) {
        sum += static_cast<int>(ptr[median]);
        median++;
    }

    return median;
}
