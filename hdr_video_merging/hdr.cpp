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

    Ptr<AlignMTB> alignMTB = createAlignMTB();
    alignMTB->process(images, images);

    return images;
}

int HdrCap::deghost_frames(Mat &img1, Mat &img2, std::vector<Mat>& labeledMap)
{
    Mat img1_g, img2_g;
    GaussianBlur(img1, img1_g, Size(21, 21), 0, 0);
    GaussianBlur(img2, img2_g, Size(21, 21), 0, 0);

    Mat grayscale1, grayscale2;
    cvtColor(img1_g, grayscale1, COLOR_RGB2GRAY);
    cvtColor(img2_g, grayscale2, COLOR_RGB2GRAY);

    Mat tb1 = computeBitmap(grayscale1);
    Mat tb2 = computeBitmap(grayscale2);

    Mat motionMap = getMotionMap(tb1, tb2);

    Mat labeledComponents;
    int nLabels = connectedComponents(motionMap, labeledComponents);

    Mat coloredLabel = colorLabeled(labeledComponents, nLabels);

    //std::vector<Mat> labeledMap;
    //labeledMap.push_back(nLabels);
    labeledMap.push_back(labeledComponents);
    labeledMap.push_back(coloredLabel);

    return nLabels;
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
            mp[j] = (mp[j] == 1) ? 255 : 0;
        }
    }

    return motionMap;
}

Mat imgToBitmapTransform(Mat &img)
{
    Mat transformed = Mat(img.rows, img.cols, CV_8U);

    for(int i = 0; i < transformed.rows; i++) {
        uchar *mp = transformed.ptr(i),
            *imgp = img.ptr(i);
        for(int j = 0; j < transformed.cols; j++) {
            mp[j] = (int(imgp[j]) == 255) ? 1 : 0;
        }
    }

    return transformed;
}

Mat HdrCap::imgToShowTransform(Mat &img)
{
    Mat transformed = Mat(img.rows, img.cols, CV_8U);

    for(int i = 0; i < transformed.rows; i++) {
        uchar *mp = transformed.ptr(i),
            *imgp = img.ptr(i);
        for(int j = 0; j < transformed.cols; j++) {
            mp[j] = (int(imgp[j]) == 1) ? 255 : 0;
        }
    }

    return transformed;
}

Mat HdrCap::colorLabeled(Mat &labelImage, int nLabels)
{
    std::vector<Vec3b> colors(static_cast<size_t>(nLabels));
    colors[0] = Vec3b(0, 0, 0);//background
    for(int label = 1; label < nLabels; ++label){
        colors[static_cast<size_t>(label)] = Vec3b( (rand()&255), (rand()&255), (rand()&255) );
    }
    Mat dst(labelImage.size(), CV_8UC3);
    for(int r = 0; r < dst.rows; ++r){
        for(int c = 0; c < dst.cols; ++c){
            int label = labelImage.at<int>(r, c);
            Vec3b &pixel = dst.at<Vec3b>(r, c);
            pixel = colors[static_cast<size_t>(label)];
        }
    }

    return dst;
}

std::vector<Mat> HdrCap::getWeights(Mat &img1, Mat &img2)
{
    Mat weightMap1;
    Mat weightMap2;

    weightMap1 = getWeightedMap(img1);
    weightMap2 = getWeightedMap(img2);

    std::vector<Mat> weights;
    weights.push_back(weightMap1);
    weights.push_back(weightMap2);

    return weights;
}

Mat HdrCap::getWeightedMap(Mat &image)
{
    Mat img;
    image.convertTo(img, CV_32F, 1.0/255.0);

    Mat wMap(img.rows, img.cols, CV_32F);
    double w_c = 1, w_s = 1, w_e = 1;

    Mat grayscale, laplacianDst, absDst;
    cvtColor(img, grayscale, COLOR_RGB2GRAY);

    Laplacian(grayscale, laplacianDst, CV_32F);
    //cout << laplacianDst;
    absDst = abs(laplacianDst);
    //cout << "\n\n\n\n\n\n" << absDst;

    double w,
          w_contrast,
          w_saturation,
          w_exposedness,
          sigma = 0.2;
    for(int i = 0; i < wMap.rows; i++) {
        float *wp = wMap.ptr<float>(i);
        for(int j = 0; j < wMap.cols; j++) {
            // contrast
            w_contrast = pow(abs(laplacianDst.at<float>(i, j)), w_c);
            w = w_contrast;
//            cout << "laplacianDst.at<float>(i, j):" << laplacianDst.at<float>(i, j) << "\n";
//            cout << "abs(laplacianDst.at<float>(i, j)): " << abs(laplacianDst.at<float>(i, j)) << "\n";
//            cout << "contrast: " << w_contrast << "\n";

            // saturation
            double  r = double(img.at<Vec3f>(i, j)[0]),
                    g = double(img.at<Vec3f>(i, j)[1]),
                    b = double(img.at<Vec3f>(i, j)[2]);
//            cout << "red: " << r << "\n";
//            cout << "green: " << g << "\n";
//            cout << "blue: " << b << "\n";
            double mean = (r + g + b)/3.0;
            w_saturation = pow(sqrt((pow(r - mean, 2) + pow(g - mean, 2) +
                                    pow(b - mean, 2)) / 3), w_s);
//            cout << "w_saturation: " << w_saturation << "\n";
            w *= w_saturation;

            // well-exposedness
            double red_exp = exponential_euclidean(r, sigma),
                   green_exp = exponential_euclidean(g, sigma),
                   blue_exp = exponential_euclidean(b, sigma);
            //cout << "red channel exposedness: " << red_exp << "\n\n";

            w_exposedness = pow(red_exp * green_exp * blue_exp, w_e);
            w *= w_exposedness;
            w += 1e-12;
//            cout << "exposedness: " << w_exposedness << "\n\n";

//            cout << "w: " << w << "\n\n";
            wp[j] = float(w);
        }
    }

    return wMap;
}

double HdrCap::exponential_euclidean(double channel, double sigma)
{
    return exp(-pow(channel-0.5,2)/(2*pow(sigma, 2)));
}

double HdrCap::max_channel(double r, double g, double b)
{
    double max = r;
        if (g > max) {
            max = g;
        }
        if (b > max) {
            max = b;
        }
        return max;
}

int HdrCap::min_channel(int r, int g, int b)
{
    int min = r;
        if (g < min) {
            min = g;
        }
        if (b < min) {
            min = b;
        }
        return min;
}

Mat HdrCap::exposure_fusion(Mat &image1, Mat &image2, int nLabels, Mat &labeledMap)
{

    std::vector<Mat> images;
    images.push_back(image1);
    images.push_back(image2);
    Mat dst;
    Size size = image1.size();

    Mat weight_sum = Mat::zeros(size, CV_32F);
    std::vector<Mat> weights = getWeights(images[0], images[1]);
    weights = integrateMovementsToWeights(weights, nLabels, labeledMap);
    weight_sum += weights[0] + weights[1];
    //cout << weights[0] << "\n";
    //cout << weights[1] << "\n";
    //cout << weight_sum << "\n";


    size_t maxlevel = static_cast<size_t>(logf(static_cast<float>(min(size.width, size.height))) / logf(2.0f));
    std::vector<Mat> res_pyr(maxlevel + 1);

    for(size_t i = 0; i < images.size(); i++) {
        weights[i] /= weight_sum;
        Mat img;
        images[i].convertTo(img, CV_32F, 1.0/255.0);

        std::vector<Mat> img_pyr, weight_pyr;
        buildPyramid(img, img_pyr, int(maxlevel));
        buildPyramid(weights[i], weight_pyr, int(maxlevel));

        for(size_t lvl = 0; lvl < maxlevel; lvl++) {
            Mat up;
            pyrUp(img_pyr[lvl + 1], up, img_pyr[lvl].size());
            img_pyr[lvl] -= up;
        }
        for(size_t lvl = 0; lvl <= maxlevel; lvl++) {
            std::vector<Mat> splitted(3);
            split(img_pyr[lvl], splitted);
            for(size_t c = 0; c < 3; c++) {
                splitted[c] = splitted[c].mul(weight_pyr[lvl]);
            }
            merge(splitted, img_pyr[lvl]);
            if(res_pyr[lvl].empty()) {
                res_pyr[lvl] = img_pyr[lvl];
            } else {
                res_pyr[lvl] += img_pyr[lvl];
            }
        }
    }
    for(size_t lvl = maxlevel; lvl > 0; lvl--) {
        Mat up;
        pyrUp(res_pyr[lvl], up, res_pyr[lvl - 1].size());
        res_pyr[lvl - 1] += up;
    }
    dst.create(size, CV_32FC3);
    res_pyr[0].copyTo(dst);


    return dst;
}

std::vector<Mat> HdrCap::integrateMovementsToWeights(InputArrayOfArrays src, int nLabels, Mat &labeledMap)
{
    std::vector<Mat> weights;
    src.getMatVector(weights);

    std::vector<Mat> correctedWeights;

    std::vector<float> average1 = getAverageClastersWeights(weights[0], nLabels, labeledMap);
    std::vector<float> average2 = getAverageClastersWeights(weights[1], nLabels, labeledMap);

    for(size_t i = 1; i < size_t(nLabels); i++){
        if(average1[i] >= average2[i]){
            average1[i] = 1;
            average2[i] = 0;
        } else {
            average1[i] = 0;
            average2[i] = 1;
        }
    }

    for(int i = 0; i < weights[0].rows; i++){
        for(int j = 0; j < weights[0].cols; j++){
            if(labeledMap.at<float>(i, j) != 0.0f) {
                weights[0].at<float>(i, j) = average1[size_t(labeledMap.at<int>(i, j))];
                weights[1].at<float>(i, j) = average2[size_t(labeledMap.at<int>(i, j))];
            }
        }
    }

    correctedWeights.push_back(weights[0]);
    correctedWeights.push_back(weights[1]);

    return correctedWeights;
}

std::vector<float> HdrCap::getAverageClastersWeights(Mat weightMap, int nLabels, Mat &labeledMap)
{
    std::vector<std::vector<float>> aMeasurements(size_t(nLabels), vector<float>(2));
    for(size_t i = 0; i < size_t(nLabels); i++) {
        aMeasurements[i].push_back(0.0f);
        aMeasurements[i].push_back(0.0f);
    }

    for(int i = 0; i < labeledMap.rows; i++) {
        for(int j = 0; j < labeledMap.cols; j++) {
            if(labeledMap.at<int>(i, j) != 0) {
                aMeasurements[size_t(labeledMap.at<int>(i, j))][0] += weightMap.at<float>(i, j);
                aMeasurements[size_t(labeledMap.at<int>(i, j))][1] += 1.0f;
            }
        }
    }

    std::vector<float> average(size_t(nLabels), 0);

    for(size_t i = 0; i < size_t(nLabels); i++) {
        average[i] = aMeasurements[i][0] / aMeasurements[i][1];
    }

    return average;
}
