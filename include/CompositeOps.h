#pragma once

#include <vector>
#include <string>
#include <opencv2/opencv.hpp>

class CompositeOps {
public:
    static std::vector<cv::Mat> stackHorizontal(const std::vector<cv::Mat>& frames1, const std::vector<cv::Mat>& frames2);
    static std::vector<cv::Mat> overlay(const std::vector<cv::Mat>& frames1, const std::vector<cv::Mat>& frames2, int x, int y);
    static std::vector<cv::Mat> fadeIn(const std::vector<cv::Mat>& frames, int duration);
    static std::vector<cv::Mat> fadeOut(const std::vector<cv::Mat>& frames, int duration);
    static std::vector<cv::Mat> crossfade(const std::vector<cv::Mat>& frames1, const std::vector<cv::Mat>& frames2, int duration);
    static std::vector<cv::Mat> stackVertical(const std::vector<cv::Mat>& frames1, const std::vector<cv::Mat>& frames2);
    static std::vector<cv::Mat> textOverlay(const std::vector<cv::Mat>& frames, const std::string& text, int x, int y, double scale, const cv::Scalar& color, int thickness);
};
