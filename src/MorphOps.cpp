#include "../include/MorphOps.h"

using namespace cv;

std::vector<Mat> MorphOps::dilate(const std::vector<Mat>& frames, int kernelSize) {
    std::vector<Mat> result; Mat k = getStructuringElement(MORPH_RECT, Size(kernelSize, kernelSize));
    for (const auto& f : frames) { Mat dilated; cv::dilate(f, dilated, k); result.push_back(dilated); }
    return result;
}

std::vector<Mat> MorphOps::erode(const std::vector<Mat>& frames, int kernelSize) {
    std::vector<Mat> result; Mat k = getStructuringElement(MORPH_RECT, Size(kernelSize, kernelSize));
    for (const auto& f : frames) { Mat eroded; cv::erode(f, eroded, k); result.push_back(eroded); }
    return result;
}

std::vector<Mat> MorphOps::open(const std::vector<Mat>& frames, int kernelSize) {
    std::vector<Mat> result; Mat k = getStructuringElement(MORPH_RECT, Size(kernelSize, kernelSize));
    for (const auto& f : frames) { Mat opened; morphologyEx(f, opened, MORPH_OPEN, k); result.push_back(opened); }
    return result;
}

std::vector<Mat> MorphOps::close(const std::vector<Mat>& frames, int kernelSize) {
    std::vector<Mat> result; Mat k = getStructuringElement(MORPH_RECT, Size(kernelSize, kernelSize));
    for (const auto& f : frames) { Mat closed; morphologyEx(f, closed, MORPH_CLOSE, k); result.push_back(closed); }
    return result;
}
