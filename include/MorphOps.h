#pragma once

#include <vector>
#include <opencv2/opencv.hpp>

class MorphOps {
public:
    static std::vector<cv::Mat> dilate(const std::vector<cv::Mat>& frames, int kernelSize);
    static std::vector<cv::Mat> erode(const std::vector<cv::Mat>& frames, int kernelSize);
    static std::vector<cv::Mat> open(const std::vector<cv::Mat>& frames, int kernelSize);
    static std::vector<cv::Mat> close(const std::vector<cv::Mat>& frames, int kernelSize);
};
