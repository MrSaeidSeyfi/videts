#include "../include/FilterOps.h"

using namespace cv;

std::vector<Mat> FilterOps::blur(const std::vector<Mat>& frames, int kernel) {
    std::vector<Mat> result;
    for (const auto& f : frames) { Mat blurred; GaussianBlur(f, blurred, Size(kernel, kernel), 0); result.push_back(blurred); }
    return result;
}

std::vector<Mat> FilterOps::sharpen(const std::vector<Mat>& frames) {
    std::vector<Mat> result; Mat k = (Mat_<float>(3,3) << -1,-1,-1,-1,9,-1,-1,-1,-1);
    for (const auto& f : frames) { Mat sharp; filter2D(f, sharp, -1, k); result.push_back(sharp); }
    return result;
}

std::vector<Mat> FilterOps::edge(const std::vector<Mat>& frames) {
    std::vector<Mat> result; for (const auto& f : frames) { Mat e; Canny(f, e, 100, 200); Mat bgr; cvtColor(e, bgr, COLOR_GRAY2BGR); result.push_back(bgr); } return result;
}

std::vector<Mat> FilterOps::denoise(const std::vector<Mat>& frames) { std::vector<Mat> result; for (const auto& f : frames) { Mat denoised; fastNlMeansDenoisingColored(f, denoised, 10, 10, 7, 21); result.push_back(denoised); } return result; }

std::vector<Mat> FilterOps::threshold(const std::vector<Mat>& frames, int thresh) { std::vector<Mat> result; for (const auto& f : frames) { Mat g; cvtColor(f, g, COLOR_BGR2GRAY); Mat th; cv::threshold(g, th, thresh, 255, THRESH_BINARY); Mat bgr; cvtColor(th, bgr, COLOR_GRAY2BGR); result.push_back(bgr); } return result; }

std::vector<Mat> FilterOps::emboss(const std::vector<Mat>& frames) { std::vector<Mat> result; Mat k = (Mat_<float>(3,3) << -2,-1,0,-1,1,1,0,1,2); for (const auto& f : frames) { Mat gray; cvtColor(f, gray, COLOR_BGR2GRAY); Mat embossed; filter2D(gray, embossed, -1, k); Mat bgr; cvtColor(embossed, bgr, COLOR_GRAY2BGR); result.push_back(bgr); } return result; }

std::vector<Mat> FilterOps::cartoon(const std::vector<Mat>& frames) { std::vector<Mat> result; for (const auto& f : frames) { Mat gray; cvtColor(f, gray, COLOR_BGR2GRAY); medianBlur(gray, gray, 5); Mat edges; adaptiveThreshold(gray, edges, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 9, 9); Mat color; bilateralFilter(f, color, 9, 300, 300); Mat cartoon; color.copyTo(cartoon, edges); result.push_back(cartoon); } return result; }

std::vector<Mat> FilterOps::vignette(const std::vector<Mat>& frames, double intensity) { std::vector<Mat> result; for (const auto& f : frames) { int h = f.rows, w = f.cols; Mat kernelX = getGaussianKernel(w, w * intensity, CV_32F); Mat kernelY = getGaussianKernel(h, h * intensity, CV_32F); Mat kernel = kernelY * kernelX.t(); double minVal, maxVal; minMaxLoc(kernel, &minVal, &maxVal); kernel = kernel / maxVal; Mat mask; cvtColor(kernel, mask, COLOR_GRAY2BGR); Mat vignetted; f.convertTo(vignetted, CV_32F); multiply(vignetted, mask, vignetted); vignetted = min(vignetted, 255.0); vignetted = max(vignetted, 0.0); vignetted.convertTo(vignetted, CV_8U); result.push_back(vignetted); } return result; }

std::vector<Mat> FilterOps::motionBlur(const std::vector<Mat>& frames, int size, double angle) { std::vector<Mat> result; Mat k = Mat::zeros(size, size, CV_32F); int mid = (size - 1) / 2; for (int i = 0; i < size; i++) k.at<float>(mid, i) = 1.0f; Point2f center(size / 2.0f - 0.5f, size / 2.0f - 0.5f); Mat M = getRotationMatrix2D(center, angle, 1.0); warpAffine(k, k, M, Size(size, size)); cv::Scalar sumVal = cv::sum(k); k = k / sumVal[0]; for (const auto& f : frames) { Mat blurred; filter2D(f, blurred, -1, k); result.push_back(blurred); } return result; }
