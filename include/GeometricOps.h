#pragma once

#include <vector>
#include <opencv2/opencv.hpp>

class GeometricOps {
public:
    static std::vector<cv::Mat> crop(const std::vector<cv::Mat>& frames, int x, int y, int w, int h);
    static std::vector<cv::Mat> resize(const std::vector<cv::Mat>& frames, int w, int h);
    static std::vector<cv::Mat> rotate(const std::vector<cv::Mat>& frames, double angle, int w, int h);
    static std::vector<cv::Mat> flip(const std::vector<cv::Mat>& frames, int direction);
    static std::vector<cv::Mat> zoom(const std::vector<cv::Mat>& frames, double factor, int centerX = -1, int centerY = -1);
    static std::vector<cv::Mat> pan(const std::vector<cv::Mat>& frames, int dx, int dy);
};
