#include "../include/CompositeOps.h"
#include <algorithm>

using namespace cv;

std::vector<Mat> CompositeOps::stackHorizontal(const std::vector<Mat>& frames1, const std::vector<Mat>& frames2) {
    std::vector<Mat> result; size_t minSize = std::min(frames1.size(), frames2.size());
    for (size_t i = 0; i < minSize; i++) { Mat resized; resize(frames2[i], resized, Size(frames1[i].cols, frames1[i].rows)); Mat stacked; hconcat(frames1[i], resized, stacked); result.push_back(stacked); }
    return result;
}

std::vector<Mat> CompositeOps::overlay(const std::vector<Mat>& frames1, const std::vector<Mat>& frames2, int x, int y) {
    std::vector<Mat> result;
    for (size_t i = 0; i < frames1.size(); i++) {
        Mat f = frames1[i].clone();
        if (i < frames2.size()) {
            Mat f2 = frames2[i]; int h = f2.rows, w = f2.cols;
            if (x + w <= f.cols && y + h <= f.rows) { Rect roi(x, y, w, h); f2.copyTo(f(roi)); }
        }
        result.push_back(f);
    }
    return result;
}

std::vector<Mat> CompositeOps::fadeIn(const std::vector<Mat>& frames, int duration) {
    std::vector<Mat> result; int fadeFrames = std::min(duration, (int)frames.size());
    for (size_t i = 0; i < frames.size(); i++) { double alpha = (fadeFrames > 0) ? std::min(1.0, (double)i / fadeFrames) : 1.0; Mat faded; frames[i].convertTo(faded, CV_32F); faded = faded * alpha; faded = min(faded, 255.0); faded = max(faded, 0.0); faded.convertTo(faded, CV_8U); result.push_back(faded); }
    return result;
}

std::vector<Mat> CompositeOps::fadeOut(const std::vector<Mat>& frames, int duration) {
    std::vector<Mat> result; int fadeFrames = std::min(duration, (int)frames.size()); int startIdx = frames.size() - fadeFrames;
    for (size_t i = 0; i < frames.size(); i++) { double alpha; if (i >= (size_t)startIdx) alpha = (fadeFrames > 0) ? 1.0 - (double)(i - startIdx) / fadeFrames : 0.0; else alpha = 1.0; Mat faded; frames[i].convertTo(faded, CV_32F); faded = faded * alpha; faded = min(faded, 255.0); faded = max(faded, 0.0); faded.convertTo(faded, CV_8U); result.push_back(faded); }
    return result;
}

std::vector<Mat> CompositeOps::crossfade(const std::vector<Mat>& frames1, const std::vector<Mat>& frames2, int duration) {
    std::vector<Mat> result; int fadeLen = std::min(duration, (int)std::min(frames1.size(), frames2.size()));
    for (size_t i = 0; i < frames1.size() - fadeLen; i++) result.push_back(frames1[i].clone());
    for (int i = 0; i < fadeLen; i++) {
        int idx1 = frames1.size() - fadeLen + i; int idx2 = i;
        if (idx1 < (int)frames1.size() && idx2 < (int)frames2.size()) {
            double alpha = (double)i / fadeLen; Mat f1, f2; frames1[idx1].convertTo(f1, CV_32F); frames2[idx2].convertTo(f2, CV_32F);
            if (f1.size() != f2.size()) resize(f2, f2, f1.size()); Mat blended = f1 * (1 - alpha) + f2 * alpha; blended = min(blended, 255.0); blended = max(blended, 0.0); blended.convertTo(blended, CV_8U); result.push_back(blended);
        }
    }
    for (size_t i = fadeLen; i < frames2.size(); i++) result.push_back(frames2[i].clone());
    return result;
}

std::vector<Mat> CompositeOps::stackVertical(const std::vector<Mat>& frames1, const std::vector<Mat>& frames2) {
    std::vector<Mat> result; size_t minSize = std::min(frames1.size(), frames2.size());
    for (size_t i = 0; i < minSize; i++) { Mat resized; resize(frames2[i], resized, Size(frames1[i].cols, frames1[i].rows)); Mat stacked; vconcat(frames1[i], resized, stacked); result.push_back(stacked); }
    return result;
}

std::vector<Mat> CompositeOps::textOverlay(const std::vector<Mat>& frames, const std::string& text, int x, int y, double scale, Scalar color, int thickness) {
    std::vector<Mat> result; for (const auto& f : frames) { Mat frame = f.clone(); putText(frame, text, Point(x, y), FONT_HERSHEY_SIMPLEX, scale, color, thickness); result.push_back(frame); } return result;
}
