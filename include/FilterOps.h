#pragma once

#include <vector>
#include <opencv2/opencv.hpp>

class FilterOps {
public:
    static std::vector<cv::Mat> blur(const std::vector<cv::Mat>& frames, int kernel);
    static std::vector<cv::Mat> sharpen(const std::vector<cv::Mat>& frames);
    static std::vector<cv::Mat> edge(const std::vector<cv::Mat>& frames);
    static std::vector<cv::Mat> denoise(const std::vector<cv::Mat>& frames);
    static std::vector<cv::Mat> threshold(const std::vector<cv::Mat>& frames, int thresh);
    static std::vector<cv::Mat> emboss(const std::vector<cv::Mat>& frames);
    static std::vector<cv::Mat> cartoon(const std::vector<cv::Mat>& frames);
    static std::vector<cv::Mat> vignette(const std::vector<cv::Mat>& frames, double intensity);
    static std::vector<cv::Mat> motionBlur(const std::vector<cv::Mat>& frames, int size, double angle);
};
