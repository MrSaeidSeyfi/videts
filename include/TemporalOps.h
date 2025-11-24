#pragma once

#include <vector>
#include <opencv2/opencv.hpp>

class TemporalOps {
public:
    static std::vector<cv::Mat> reverse(const std::vector<cv::Mat>& frames);
    static std::vector<cv::Mat> speedUp(const std::vector<cv::Mat>& frames, double factor);
    static std::vector<cv::Mat> freeze(const std::vector<cv::Mat>& frames, int frameIdx, int duration);
    static std::vector<cv::Mat> loop(const std::vector<cv::Mat>& frames, int count);
    static std::vector<cv::Mat> duplicate(const std::vector<cv::Mat>& frames, int factor);
};
