#pragma once

#include <vector>
#include <opencv2/opencv.hpp>

class ColorOps {
public:
    static std::vector<cv::Mat> gray(const std::vector<cv::Mat>& frames);
    static std::vector<cv::Mat> invert(const std::vector<cv::Mat>& frames);
    static std::vector<cv::Mat> brightness(const std::vector<cv::Mat>& frames, int value);
    static std::vector<cv::Mat> contrast(const std::vector<cv::Mat>& frames, double value);
    static std::vector<cv::Mat> hueShift(const std::vector<cv::Mat>& frames, int shift);
    static std::vector<cv::Mat> saturation(const std::vector<cv::Mat>& frames, double factor);
    static std::vector<cv::Mat> sepia(const std::vector<cv::Mat>& frames);
    static std::vector<cv::Mat> warm(const std::vector<cv::Mat>& frames, int intensity);
    static std::vector<cv::Mat> cool(const std::vector<cv::Mat>& frames, int intensity);
    static std::vector<cv::Mat> posterize(const std::vector<cv::Mat>& frames, int levels);
};
