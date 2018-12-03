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
