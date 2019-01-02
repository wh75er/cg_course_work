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
    Mat grayscale1, grayscale2;
    cvtColor(img1, grayscale1, COLOR_RGB2GRAY);
    cvtColor(img2, grayscale2, COLOR_RGB2GRAY);

    Mat tb1 = computeBitmap(grayscale1);
    Mat tb2 = computeBitmap(grayscale2);

    Mat motionMap = getMotionMap(tb1, tb2);

    return motionMap;
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

Mat HdrCap::computeBitmap(Mat &img)
{
    Mat tb;
    tb.create(img.size(), CV_8U);

    int median = getMedian(img);

    compare(img, median, tb, CMP_GT);

    return tb;
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

Mat HdrCap::getMotionMap(Mat &tb1, Mat &tb2)
{
    // Add bitmaps to get motionMap
    //	0 and 2 - pixel is static, 1 - otherwise
    Mat motionMap = Mat(tb1.rows, tb1.cols, CV_8U);

    for(int i = 0; i < motionMap.rows; i++) {
        uchar *mp = motionMap.ptr(i),
            *tbp1 = tb1.ptr(i),
            *tbp2 = tb2.ptr(i);
        for(int j = 0; j < motionMap.cols; j++) {
            int a = (int(tbp1[j]) == 255) ? 1 : 0;
            int b = (int(tbp2[j]) == 255) ? 1 : 0;
            mp[j] = uchar(a + b);
        }
    }

    Mat temp = morphOpening(motionMap);

    return temp;
}

Mat HdrCap::morphOpening(Mat &src)
{
    Mat	structElement = Mat(3, 3, CV_8U, Scalar(1));
    cout << "M = "<< endl << " "  << structElement << endl << endl;

    Mat erosed = erosion(src, structElement);

    return erosed;
}

Mat HdrCap::erosion(Mat &img, Mat &kernel)
{
    Mat erased = Mat(img.rows, img.cols, CV_8U, Scalar(0));
    size_t src_i = size_t(kernel.rows/2);
    size_t src_j = size_t(kernel.cols/2);

    uchar *ersd = erased.data;
    size_t ersd_step = erased.step1();

    for(size_t i = src_i; i < size_t(img.rows)-src_i; i++) {
        for(size_t j = src_j; j < size_t(img.cols)-src_j; j++) {
            if(checkPattern(img, kernel, i, j) == true)
                ersd[i*ersd_step + j] = 0;
            else
                ersd[i*ersd_step + j] = 255;
        }
    }

    return erased;
}

Mat HdrCap::dilation(Mat &img, Mat &kernel)
{
    Mat extended = Mat(img.rows, img.cols, CV_8U);

    return extended;

}

bool HdrCap::checkPattern(Mat &img, Mat &kernel, size_t img_i, size_t img_j)
{
    uchar *core = kernel.data,
          *row = img.data;
    size_t core_step = kernel.step1(),
           row_step = img.step1(),
           core_i = 0,
           core_j = 0;
    bool result = true;

    for(size_t i = img_i-1; i < img_i+1; i++) {
        core_j = 0;
        for(size_t j = img_j-1; j < img_j+1; j++) {
            if(core[core_i*core_step+core_j] != row[i*row_step+j])
                result = false;
            core_j++;
        }
        core_i++;
    }

    return result;
}
