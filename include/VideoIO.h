#pragma once

#include <string>
#include <vector>
#include <opencv2/opencv.hpp>

class VideoIO {
public:
    VideoIO(const std::string& path);
    ~VideoIO();

    std::vector<cv::Mat> readAll();
    std::vector<cv::Mat> readRange(int start, int end);
    void write(const std::string& path, const std::vector<cv::Mat>& frames);
    void writeFps(const std::string& path, const std::vector<cv::Mat>& frames, int newFps);
    bool extractFrame(int frameIdx, const std::string& outputPath);

    int w, h, fps, fc;
private:
    cv::VideoCapture cap;
};
