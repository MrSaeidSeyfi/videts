#include "../include/ColorOps.h"

using namespace cv;

std::vector<Mat> ColorOps::gray(const std::vector<Mat>& frames) {
    std::vector<Mat> result;
    for (const auto& f : frames) {
        Mat g, bgr;
        cvtColor(f, g, COLOR_BGR2GRAY);
        cvtColor(g, bgr, COLOR_GRAY2BGR);
        result.push_back(bgr);
    }
    return result;
}

std::vector<Mat> ColorOps::invert(const std::vector<Mat>& frames) {
    std::vector<Mat> result;
    for (const auto& f : frames) {
        Mat inv; bitwise_not(f, inv); result.push_back(inv);
    }
    return result;
}

std::vector<Mat> ColorOps::brightness(const std::vector<Mat>& frames, int value) {
    std::vector<Mat> result;
    for (const auto& f : frames) { Mat bright; f.convertTo(bright, -1, 1, value); result.push_back(bright); }
    return result;
}

std::vector<Mat> ColorOps::contrast(const std::vector<Mat>& frames, double value) {
    std::vector<Mat> result;
    for (const auto& f : frames) { Mat cont; f.convertTo(cont, -1, value, 0); result.push_back(cont); }
    return result;
}

std::vector<Mat> ColorOps::hueShift(const std::vector<Mat>& frames, int shift) {
    std::vector<Mat> result;
    for (const auto& f : frames) {
        Mat hsv; cvtColor(f, hsv, COLOR_BGR2HSV); std::vector<Mat> channels; split(hsv, channels);
        channels[0].convertTo(channels[0], CV_32F); channels[0] = channels[0] + shift;
        Mat temp; channels[0].copyTo(temp); temp = temp / 180.0; temp.forEach<float>([](float &p, const int*) { p = std::floor(p); }); temp = temp * 180.0;
        channels[0] = cv::max(channels[0] - temp, 0.0f);
        channels[0].convertTo(channels[0], CV_8U); Mat merged; merge(channels, merged); Mat bgr; cvtColor(merged, bgr, COLOR_HSV2BGR); result.push_back(bgr);
    }
    return result;
}

std::vector<Mat> ColorOps::saturation(const std::vector<Mat>& frames, double factor) {
    std::vector<Mat> result;
    for (const auto& f : frames) {
        Mat hsv; cvtColor(f, hsv, COLOR_BGR2HSV); std::vector<Mat> channels; split(hsv, channels);
        channels[1].convertTo(channels[1], CV_32F); channels[1] = channels[1] * factor; channels[1] = min(channels[1], 255.0); channels[1] = max(channels[1], 0.0); channels[1].convertTo(channels[1], CV_8U);
        Mat merged; merge(channels, merged); Mat bgr; cvtColor(merged, bgr, COLOR_HSV2BGR); result.push_back(bgr);
    }
    return result;
}

std::vector<Mat> ColorOps::sepia(const std::vector<Mat>& frames) {
    std::vector<Mat> result; Mat sepiaMatrix = (Mat_<float>(3,3) << 0.272f,0.534f,0.131f, 0.349f,0.686f,0.168f, 0.393f,0.769f,0.189f);
    for (const auto& f : frames) { Mat sepiaFrame; transform(f, sepiaFrame, sepiaMatrix); sepiaFrame = min(sepiaFrame, 255.0); sepiaFrame = max(sepiaFrame, 0.0); sepiaFrame.convertTo(sepiaFrame, CV_8U); result.push_back(sepiaFrame); }
    return result;
}

std::vector<Mat> ColorOps::warm(const std::vector<Mat>& frames, int intensity) {
    std::vector<Mat> result; for (const auto& f : frames) { std::vector<Mat> channels; split(f, channels); channels[0].convertTo(channels[0], CV_32F); channels[1].convertTo(channels[1], CV_32F); channels[2].convertTo(channels[2], CV_32F); channels[2] = min(channels[2] + intensity, 255.0); channels[0] = max(channels[0] - intensity * 0.5, 0.0); channels[0].convertTo(channels[0], CV_8U); channels[1].convertTo(channels[1], CV_8U); channels[2].convertTo(channels[2], CV_8U); Mat merged; merge(channels, merged); result.push_back(merged); }
    return result;
}

std::vector<Mat> ColorOps::cool(const std::vector<Mat>& frames, int intensity) {
    std::vector<Mat> result; for (const auto& f : frames) { std::vector<Mat> channels; split(f, channels); channels[0].convertTo(channels[0], CV_32F); channels[1].convertTo(channels[1], CV_32F); channels[2].convertTo(channels[2], CV_32F); channels[0] = min(channels[0] + intensity, 255.0); channels[2] = max(channels[2] - intensity * 0.5, 0.0); channels[0].convertTo(channels[0], CV_8U); channels[1].convertTo(channels[1], CV_8U); channels[2].convertTo(channels[2], CV_8U); Mat merged; merge(channels, merged); result.push_back(merged); }
    return result;
}

std::vector<Mat> ColorOps::posterize(const std::vector<Mat>& frames, int levels) {
    std::vector<Mat> result; int step = 256 / levels; for (const auto& f : frames) { Mat quantized; f.convertTo(quantized, CV_32F); quantized = (quantized / step) * step; quantized = min(quantized, 255.0); quantized = max(quantized, 0.0); quantized.convertTo(quantized, CV_8U); result.push_back(quantized); } return result;
}
