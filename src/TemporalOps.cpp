#include "../include/TemporalOps.h"
#include <algorithm>

using namespace cv;

std::vector<Mat> TemporalOps::reverse(const std::vector<Mat>& frames) {
    std::vector<Mat> result = frames;
    std::reverse(result.begin(), result.end());
    return result;
}

std::vector<Mat> TemporalOps::speedUp(const std::vector<Mat>& frames, double factor) {
    std::vector<Mat> result;
    int step = (int)factor;
    if (step < 1) step = 1;
    for (size_t i = 0; i < frames.size(); i += step) result.push_back(frames[i].clone());
    return result;
}

std::vector<Mat> TemporalOps::freeze(const std::vector<Mat>& frames, int frameIdx, int duration) {
    if (frames.empty() || frameIdx >= (int)frames.size()) return frames;
    std::vector<Mat> result;
    for (int i = 0; i < frameIdx; i++) result.push_back(frames[i].clone());
    Mat freezeFrame = frames[frameIdx].clone();
    for (int i = 0; i < duration; i++) result.push_back(freezeFrame.clone());
    for (size_t i = frameIdx; i < frames.size(); i++) result.push_back(frames[i].clone());
    return result;
}

std::vector<Mat> TemporalOps::loop(const std::vector<Mat>& frames, int count) {
    std::vector<Mat> result;
    for (int c = 0; c < count; c++) for (const auto& f : frames) result.push_back(f.clone());
    return result;
}

std::vector<Mat> TemporalOps::duplicate(const std::vector<Mat>& frames, int factor) {
    std::vector<Mat> result;
    for (const auto& f : frames) for (int i = 0; i < factor; ++i) result.push_back(f.clone());
    return result;
}
