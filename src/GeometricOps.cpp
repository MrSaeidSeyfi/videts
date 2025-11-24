#include "../include/GeometricOps.h"

using namespace cv;

std::vector<Mat> GeometricOps::crop(const std::vector<Mat>& frames, int x, int y, int w, int h) {
    std::vector<Mat> result;
    for (const auto& f : frames) {
        Rect roi(x, y, w, h);
        result.push_back(f(roi).clone());
    }
    return result;
}

std::vector<Mat> GeometricOps::resize(const std::vector<Mat>& frames, int w, int h) {
    std::vector<Mat> result;
    for (const auto& f : frames) {
        Mat resized;
        cv::resize(f, resized, Size(w, h));
        result.push_back(resized);
    }
    return result;
}

std::vector<Mat> GeometricOps::rotate(const std::vector<Mat>& frames, double angle, int w, int h) {
    std::vector<Mat> result;
    Point2f center(w / 2.0f, h / 2.0f);
    Mat M = getRotationMatrix2D(center, angle, 1.0);
    for (const auto& f : frames) {
        Mat rotated;
        warpAffine(f, rotated, M, Size(w, h));
        result.push_back(rotated);
    }
    return result;
}

std::vector<Mat> GeometricOps::flip(const std::vector<Mat>& frames, int direction) {
    std::vector<Mat> result;
    for (const auto& f : frames) {
        Mat flipped;
        cv::flip(f, flipped, direction);
        result.push_back(flipped);
    }
    return result;
}

std::vector<Mat> GeometricOps::zoom(const std::vector<Mat>& frames, double factor, int centerX, int centerY) {
    std::vector<Mat> result;
    for (const auto& f : frames) {
        int h = f.rows, w = f.cols;
        int cx = centerX == -1 ? w / 2 : centerX;
        int cy = centerY == -1 ? h / 2 : centerY;
        int newW = (int)(w / factor);
        int newH = (int)(h / factor);
        int x1 = std::max(0, cx - newW / 2);
        int y1 = std::max(0, cy - newH / 2);
        int x2 = std::min(w, x1 + newW);
        int y2 = std::min(h, y1 + newH);
        Rect roi(x1, y1, x2 - x1, y2 - y1);
        Mat cropped = f(roi);
        Mat resized;
        cv::resize(cropped, resized, Size(w, h));
        result.push_back(resized);
    }
    return result;
}

std::vector<Mat> GeometricOps::pan(const std::vector<Mat>& frames, int dx, int dy) {
    std::vector<Mat> result;
    for (const auto& f : frames) {
        int h = f.rows, w = f.cols;
        Mat M = (Mat_<float>(2,3) << 1, 0, dx, 0, 1, dy);
        Mat panned;
        warpAffine(f, panned, M, Size(w, h));
        result.push_back(panned);
    }
    return result;
}
