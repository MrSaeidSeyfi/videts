#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/videoio.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>
#include <sstream>

using namespace cv;
using namespace std;

// VideoIO class
class VideoIO {
public:
    VideoCapture cap;
    int w, h, fps, fc;
    
    VideoIO(const string& path) {
        cap.open(path);
        if (!cap.isOpened()) {
            cerr << "Error: Cannot open video " << path << endl;
            exit(1);
        }
        w = (int)cap.get(CAP_PROP_FRAME_WIDTH);
        h = (int)cap.get(CAP_PROP_FRAME_HEIGHT);
        fps = (int)cap.get(CAP_PROP_FPS);
        fc = (int)cap.get(CAP_PROP_FRAME_COUNT);
    }
    
    vector<Mat> readAll() {
        vector<Mat> frames;
        Mat frame;
        while (cap.read(frame)) {
            frames.push_back(frame.clone());
        }
        return frames;
    }
    
    vector<Mat> readRange(int start, int end) {
        cap.set(CAP_PROP_POS_FRAMES, start);
        vector<Mat> frames;
        Mat frame;
        for (int i = 0; i < (end - start); i++) {
            if (!cap.read(frame)) break;
            frames.push_back(frame.clone());
        }
        return frames;
    }
    
    void write(const string& path, const vector<Mat>& frames) {
        if (frames.empty()) return;
        int h = frames[0].rows;
        int w = frames[0].cols;
        VideoWriter writer(path, VideoWriter::fourcc('m', 'p', '4', 'v'), fps, Size(w, h));
        for (const auto& frame : frames) {
            writer.write(frame);
        }
        writer.release();
    }
    
    void writeFps(const string& path, const vector<Mat>& frames, int newFps) {
        if (frames.empty()) return;
        int h = frames[0].rows;
        int w = frames[0].cols;
        VideoWriter writer(path, VideoWriter::fourcc('m', 'p', '4', 'v'), newFps, Size(w, h));
        for (const auto& frame : frames) {
            writer.write(frame);
        }
        writer.release();
    }
    
    bool extractFrame(int frameIdx, const string& outputPath) {
        cap.set(CAP_PROP_POS_FRAMES, frameIdx);
        Mat frame;
        if (cap.read(frame)) {
            imwrite(outputPath, frame);
            return true;
        }
        return false;
    }
    
    ~VideoIO() {
        cap.release();
    }
};

// GeometricOps class
class GeometricOps {
public:
    static vector<Mat> crop(const vector<Mat>& frames, int x, int y, int w, int h) {
        vector<Mat> result;
        for (const auto& f : frames) {
            Rect roi(x, y, w, h);
            result.push_back(f(roi).clone());
        }
        return result;
    }
    
    static vector<Mat> resize(const vector<Mat>& frames, int w, int h) {
        vector<Mat> result;
        for (const auto& f : frames) {
            Mat resized;
            cv::resize(f, resized, Size(w, h));
            result.push_back(resized);
        }
        return result;
    }
    
    static vector<Mat> rotate(const vector<Mat>& frames, double angle, int w, int h) {
        vector<Mat> result;
        Point2f center(w / 2.0f, h / 2.0f);
        Mat M = getRotationMatrix2D(center, angle, 1.0);
        for (const auto& f : frames) {
            Mat rotated;
            warpAffine(f, rotated, M, Size(w, h));
            result.push_back(rotated);
        }
        return result;
    }
    
    static vector<Mat> flip(const vector<Mat>& frames, int direction) {
        vector<Mat> result;
        for (const auto& f : frames) {
            Mat flipped;
            cv::flip(f, flipped, direction);
            result.push_back(flipped);
        }
        return result;
    }
    
    static vector<Mat> zoom(const vector<Mat>& frames, double factor, int centerX = -1, int centerY = -1) {
        vector<Mat> result;
        for (const auto& f : frames) {
            int h = f.rows, w = f.cols;
            if (centerX == -1) centerX = w / 2;
            if (centerY == -1) centerY = h / 2;
            int newW = (int)(w / factor);
            int newH = (int)(h / factor);
            int x1 = max(0, centerX - newW / 2);
            int y1 = max(0, centerY - newH / 2);
            int x2 = min(w, x1 + newW);
            int y2 = min(h, y1 + newH);
            Rect roi(x1, y1, x2 - x1, y2 - y1);
            Mat cropped = f(roi);
            Mat resized;
            cv::resize(cropped, resized, Size(w, h));
            result.push_back(resized);
        }
        return result;
    }
    
    static vector<Mat> pan(const vector<Mat>& frames, int dx, int dy) {
        vector<Mat> result;
        for (const auto& f : frames) {
            int h = f.rows, w = f.cols;
            Mat M = (Mat_<float>(2, 3) << 1, 0, dx, 0, 1, dy);
            Mat panned;
            warpAffine(f, panned, M, Size(w, h));
            result.push_back(panned);
        }
        return result;
    }
};

// TemporalOps class
class TemporalOps {
public:
    static vector<Mat> reverse(const vector<Mat>& frames) {
        vector<Mat> result = frames;
        std::reverse(result.begin(), result.end());
        return result;
    }
    
    static vector<Mat> speedUp(const vector<Mat>& frames, double factor) {
        vector<Mat> result;
        int step = (int)factor;
        for (size_t i = 0; i < frames.size(); i += step) {
            result.push_back(frames[i].clone());
        }
        return result;
    }
    
    static vector<Mat> freeze(const vector<Mat>& frames, int frameIdx, int duration) {
        if (frames.empty() || frameIdx >= (int)frames.size()) {
            return frames;
        }
        vector<Mat> result;
        for (int i = 0; i < frameIdx; i++) {
            result.push_back(frames[i].clone());
        }
        Mat freezeFrame = frames[frameIdx].clone();
        for (int i = 0; i < duration; i++) {
            result.push_back(freezeFrame.clone());
        }
        for (size_t i = frameIdx; i < frames.size(); i++) {
            result.push_back(frames[i].clone());
        }
        return result;
    }
    
    static vector<Mat> loop(const vector<Mat>& frames, int count) {
        vector<Mat> result;
        for (int c = 0; c < count; c++) {
            for (const auto& f : frames) {
                result.push_back(f.clone());
            }
        }
        return result;
    }
    
    static vector<Mat> duplicate(const vector<Mat>& frames, int factor) {
        vector<Mat> result;
        for (const auto& f : frames) {
            for (int i = 0; i < factor; i++) {
                result.push_back(f.clone());
            }
        }
        return result;
    }
};

// ColorOps class
class ColorOps {
public:
    static vector<Mat> gray(const vector<Mat>& frames) {
        vector<Mat> result;
        for (const auto& f : frames) {
            Mat g;
            cvtColor(f, g, COLOR_BGR2GRAY);
            Mat bgr;
            cvtColor(g, bgr, COLOR_GRAY2BGR);
            result.push_back(bgr);
        }
        return result;
    }
    
    static vector<Mat> invert(const vector<Mat>& frames) {
        vector<Mat> result;
        for (const auto& f : frames) {
            Mat inv;
            bitwise_not(f, inv);
            result.push_back(inv);
        }
        return result;
    }
    
    static vector<Mat> brightness(const vector<Mat>& frames, int value) {
        vector<Mat> result;
        for (const auto& f : frames) {
            Mat bright;
            f.convertTo(bright, -1, 1, value);
            result.push_back(bright);
        }
        return result;
    }
    
    static vector<Mat> contrast(const vector<Mat>& frames, double value) {
        vector<Mat> result;
        for (const auto& f : frames) {
            Mat cont;
            f.convertTo(cont, -1, value, 0);
            result.push_back(cont);
        }
        return result;
    }
    
    static vector<Mat> hueShift(const vector<Mat>& frames, int shift) {
        vector<Mat> result;
        for (const auto& f : frames) {
            Mat hsv;
            cvtColor(f, hsv, COLOR_BGR2HSV);
            vector<Mat> channels;
            split(hsv, channels);
            channels[0].convertTo(channels[0], CV_32F);
            channels[0] = channels[0] + shift;

            Mat temp;
            channels[0].copyTo(temp);
            temp = temp / 180.0;
            temp.forEach<float>([](float &p, const int*) { p = std::floor(p); });
            temp = temp * 180.0;
            channels[0] = cv::max(channels[0] - temp, 0.0f);

            channels[0].convertTo(channels[0], CV_8U);
            Mat merged;
            merge(channels, merged);
            Mat bgr;
            cvtColor(merged, bgr, COLOR_HSV2BGR);
            result.push_back(bgr);
        }
        return result;
    }
    
    static vector<Mat> saturation(const vector<Mat>& frames, double factor) {
        vector<Mat> result;
        for (const auto& f : frames) {
            Mat hsv;
            cvtColor(f, hsv, COLOR_BGR2HSV);
            vector<Mat> channels;
            split(hsv, channels);
            channels[1].convertTo(channels[1], CV_32F);
            channels[1] = channels[1] * factor;
            channels[1] = min(channels[1], 255.0);
            channels[1] = max(channels[1], 0.0);
            channels[1].convertTo(channels[1], CV_8U);
            Mat merged;
            merge(channels, merged);
            Mat bgr;
            cvtColor(merged, bgr, COLOR_HSV2BGR);
            result.push_back(bgr);
        }
        return result;
    }
    
    static vector<Mat> sepia(const vector<Mat>& frames) {
        vector<Mat> result;
        Mat sepiaMatrix = (Mat_<float>(3, 3) << 
            0.272f, 0.534f, 0.131f,
            0.349f, 0.686f, 0.168f,
            0.393f, 0.769f, 0.189f);
        for (const auto& f : frames) {
            Mat sepiaFrame;
            transform(f, sepiaFrame, sepiaMatrix);
            sepiaFrame = min(sepiaFrame, 255.0);
            sepiaFrame = max(sepiaFrame, 0.0);
            sepiaFrame.convertTo(sepiaFrame, CV_8U);
            result.push_back(sepiaFrame);
        }
        return result;
    }
    
    static vector<Mat> warm(const vector<Mat>& frames, int intensity) {
        vector<Mat> result;
        for (const auto& f : frames) {
            vector<Mat> channels;
            split(f, channels);
            channels[0].convertTo(channels[0], CV_32F);
            channels[1].convertTo(channels[1], CV_32F);
            channels[2].convertTo(channels[2], CV_32F);
            channels[2] = min(channels[2] + intensity, 255.0);
            channels[0] = max(channels[0] - intensity * 0.5, 0.0);
            channels[0].convertTo(channels[0], CV_8U);
            channels[1].convertTo(channels[1], CV_8U);
            channels[2].convertTo(channels[2], CV_8U);
            Mat merged;
            merge(channels, merged);
            result.push_back(merged);
        }
        return result;
    }
    
    static vector<Mat> cool(const vector<Mat>& frames, int intensity) {
        vector<Mat> result;
        for (const auto& f : frames) {
            vector<Mat> channels;
            split(f, channels);
            channels[0].convertTo(channels[0], CV_32F);
            channels[1].convertTo(channels[1], CV_32F);
            channels[2].convertTo(channels[2], CV_32F);
            channels[0] = min(channels[0] + intensity, 255.0);
            channels[2] = max(channels[2] - intensity * 0.5, 0.0);
            channels[0].convertTo(channels[0], CV_8U);
            channels[1].convertTo(channels[1], CV_8U);
            channels[2].convertTo(channels[2], CV_8U);
            Mat merged;
            merge(channels, merged);
            result.push_back(merged);
        }
        return result;
    }
    
    static vector<Mat> posterize(const vector<Mat>& frames, int levels) {
        vector<Mat> result;
        int step = 256 / levels;
        for (const auto& f : frames) {
            Mat quantized;
            f.convertTo(quantized, CV_32F);
            quantized = (quantized / step) * step;
            quantized = min(quantized, 255.0);
            quantized = max(quantized, 0.0);
            quantized.convertTo(quantized, CV_8U);
            result.push_back(quantized);
        }
        return result;
    }
};

// FilterOps class
class FilterOps {
public:
    static vector<Mat> blur(const vector<Mat>& frames, int kernel) {
        vector<Mat> result;
        for (const auto& f : frames) {
            Mat blurred;
            GaussianBlur(f, blurred, Size(kernel, kernel), 0);
            result.push_back(blurred);
        }
        return result;
    }
    
    static vector<Mat> sharpen(const vector<Mat>& frames) {
        vector<Mat> result;
        Mat k = (Mat_<float>(3, 3) << -1, -1, -1, -1, 9, -1, -1, -1, -1);
        for (const auto& f : frames) {
            Mat sharp;
            filter2D(f, sharp, -1, k);
            result.push_back(sharp);
        }
        return result;
    }
    
    static vector<Mat> edge(const vector<Mat>& frames) {
        vector<Mat> result;
        for (const auto& f : frames) {
            Mat e;
            Canny(f, e, 100, 200);
            Mat bgr;
            cvtColor(e, bgr, COLOR_GRAY2BGR);
            result.push_back(bgr);
        }
        return result;
    }
    
    static vector<Mat> denoise(const vector<Mat>& frames) {
        vector<Mat> result;
        for (const auto& f : frames) {
            Mat denoised;
            fastNlMeansDenoisingColored(f, denoised, 10, 10, 7, 21);
            result.push_back(denoised);
        }
        return result;
    }
    
    static vector<Mat> threshold(const vector<Mat>& frames, int thresh) {
        vector<Mat> result;
        for (const auto& f : frames) {
            Mat g;
            cvtColor(f, g, COLOR_BGR2GRAY);
            Mat th;
            cv::threshold(g, th, thresh, 255, THRESH_BINARY);
            Mat bgr;
            cvtColor(th, bgr, COLOR_GRAY2BGR);
            result.push_back(bgr);
        }
        return result;
    }
    
    static vector<Mat> emboss(const vector<Mat>& frames) {
        vector<Mat> result;
        Mat k = (Mat_<float>(3, 3) << -2, -1, 0, -1, 1, 1, 0, 1, 2);
        for (const auto& f : frames) {
            Mat gray;
            cvtColor(f, gray, COLOR_BGR2GRAY);
            Mat embossed;
            filter2D(gray, embossed, -1, k);
            Mat bgr;
            cvtColor(embossed, bgr, COLOR_GRAY2BGR);
            result.push_back(bgr);
        }
        return result;
    }
    
    static vector<Mat> cartoon(const vector<Mat>& frames) {
        vector<Mat> result;
        for (const auto& f : frames) {
            Mat gray;
            cvtColor(f, gray, COLOR_BGR2GRAY);
            medianBlur(gray, gray, 5);
            Mat edges;
            adaptiveThreshold(gray, edges, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 9, 9);
            Mat color;
            bilateralFilter(f, color, 9, 300, 300);
            Mat cartoon;
            color.copyTo(cartoon, edges);
            result.push_back(cartoon);
        }
        return result;
    }
    
    static vector<Mat> vignette(const vector<Mat>& frames, double intensity) {
        vector<Mat> result;
        for (const auto& f : frames) {
            int h = f.rows, w = f.cols;
            Mat kernelX = getGaussianKernel(w, w * intensity, CV_32F);
            Mat kernelY = getGaussianKernel(h, h * intensity, CV_32F);
            Mat kernel = kernelY * kernelX.t();
            double minVal, maxVal;
            minMaxLoc(kernel, &minVal, &maxVal);
            kernel = kernel / maxVal;
            Mat mask;
            cvtColor(kernel, mask, COLOR_GRAY2BGR);
            Mat vignetted;
            f.convertTo(vignetted, CV_32F);
            multiply(vignetted, mask, vignetted);
            vignetted = min(vignetted, 255.0);
            vignetted = max(vignetted, 0.0);
            vignetted.convertTo(vignetted, CV_8U);
            result.push_back(vignetted);
        }
        return result;
    }
    
    static vector<Mat> motionBlur(const vector<Mat>& frames, int size, double angle) {
        vector<Mat> result;
        Mat k = Mat::zeros(size, size, CV_32F);
        int mid = (size - 1) / 2;
        for (int i = 0; i < size; i++) {
            k.at<float>(mid, i) = 1.0f;
        }
        Point2f center(size / 2.0f - 0.5f, size / 2.0f - 0.5f);
        Mat M = getRotationMatrix2D(center, angle, 1.0);
        warpAffine(k, k, M, Size(size, size));
        Scalar sumVal = sum(k);
        k = k / sumVal[0];
        for (const auto& f : frames) {
            Mat blurred;
            filter2D(f, blurred, -1, k);
            result.push_back(blurred);
        }
        return result;
    }
};

// MorphOps class
class MorphOps {
public:
    static vector<Mat> dilate(const vector<Mat>& frames, int kernelSize) {
        vector<Mat> result;
        Mat k = getStructuringElement(MORPH_RECT, Size(kernelSize, kernelSize));
        for (const auto& f : frames) {
            Mat dilated;
            cv::dilate(f, dilated, k);
            result.push_back(dilated);
        }
        return result;
    }
    
    static vector<Mat> erode(const vector<Mat>& frames, int kernelSize) {
        vector<Mat> result;
        Mat k = getStructuringElement(MORPH_RECT, Size(kernelSize, kernelSize));
        for (const auto& f : frames) {
            Mat eroded;
            cv::erode(f, eroded, k);
            result.push_back(eroded);
        }
        return result;
    }
    
    static vector<Mat> open(const vector<Mat>& frames, int kernelSize) {
        vector<Mat> result;
        Mat k = getStructuringElement(MORPH_RECT, Size(kernelSize, kernelSize));
        for (const auto& f : frames) {
            Mat opened;
            morphologyEx(f, opened, MORPH_OPEN, k);
            result.push_back(opened);
        }
        return result;
    }
    
    static vector<Mat> close(const vector<Mat>& frames, int kernelSize) {
        vector<Mat> result;
        Mat k = getStructuringElement(MORPH_RECT, Size(kernelSize, kernelSize));
        for (const auto& f : frames) {
            Mat closed;
            morphologyEx(f, closed, MORPH_CLOSE, k);
            result.push_back(closed);
        }
        return result;
    }
};

// CompositeOps class
class CompositeOps {
public:
    static vector<Mat> stackHorizontal(const vector<Mat>& frames1, const vector<Mat>& frames2) {
        vector<Mat> result;
        size_t minSize = min(frames1.size(), frames2.size());
        for (size_t i = 0; i < minSize; i++) {
            Mat resized;
            resize(frames2[i], resized, Size(frames1[i].cols, frames1[i].rows));
            Mat stacked;
            hconcat(frames1[i], resized, stacked);
            result.push_back(stacked);
        }
        return result;
    }
    
    static vector<Mat> overlay(const vector<Mat>& frames1, const vector<Mat>& frames2, int x, int y) {
        vector<Mat> result;
        for (size_t i = 0; i < frames1.size(); i++) {
            Mat f = frames1[i].clone();
            if (i < frames2.size()) {
                Mat f2 = frames2[i];
                int h = f2.rows, w = f2.cols;
                if (x + w <= f.cols && y + h <= f.rows) {
                    Rect roi(x, y, w, h);
                    f2.copyTo(f(roi));
                }
            }
            result.push_back(f);
        }
        return result;
    }
    
    static vector<Mat> fadeIn(const vector<Mat>& frames, int duration) {
        vector<Mat> result;
        int fadeFrames = min(duration, (int)frames.size());
        for (size_t i = 0; i < frames.size(); i++) {
            double alpha = (fadeFrames > 0) ? min(1.0, (double)i / fadeFrames) : 1.0;
            Mat faded;
            frames[i].convertTo(faded, CV_32F);
            faded = faded * alpha;
            faded = min(faded, 255.0);
            faded = max(faded, 0.0);
            faded.convertTo(faded, CV_8U);
            result.push_back(faded);
        }
        return result;
    }
    
    static vector<Mat> fadeOut(const vector<Mat>& frames, int duration) {
        vector<Mat> result;
        int fadeFrames = min(duration, (int)frames.size());
        int startIdx = frames.size() - fadeFrames;
        for (size_t i = 0; i < frames.size(); i++) {
            double alpha;
            if (i >= (size_t)startIdx) {
                alpha = (fadeFrames > 0) ? 1.0 - (double)(i - startIdx) / fadeFrames : 0.0;
            } else {
                alpha = 1.0;
            }
            Mat faded;
            frames[i].convertTo(faded, CV_32F);
            faded = faded * alpha;
            faded = min(faded, 255.0);
            faded = max(faded, 0.0);
            faded.convertTo(faded, CV_8U);
            result.push_back(faded);
        }
        return result;
    }
    
    static vector<Mat> crossfade(const vector<Mat>& frames1, const vector<Mat>& frames2, int duration) {
        vector<Mat> result;
        int fadeLen = min(duration, (int)min(frames1.size(), frames2.size()));
        for (size_t i = 0; i < frames1.size() - fadeLen; i++) {
            result.push_back(frames1[i].clone());
        }
        for (int i = 0; i < fadeLen; i++) {
            int idx1 = frames1.size() - fadeLen + i;
            int idx2 = i;
            if (idx1 < (int)frames1.size() && idx2 < (int)frames2.size()) {
                double alpha = (double)i / fadeLen;
                Mat f1, f2;
                frames1[idx1].convertTo(f1, CV_32F);
                frames2[idx2].convertTo(f2, CV_32F);
                if (f1.size() != f2.size()) {
                    resize(f2, f2, f1.size());
                }
                Mat blended = f1 * (1 - alpha) + f2 * alpha;
                blended = min(blended, 255.0);
                blended = max(blended, 0.0);
                blended.convertTo(blended, CV_8U);
                result.push_back(blended);
            }
        }
        for (size_t i = fadeLen; i < frames2.size(); i++) {
            result.push_back(frames2[i].clone());
        }
        return result;
    }
    
    static vector<Mat> stackVertical(const vector<Mat>& frames1, const vector<Mat>& frames2) {
        vector<Mat> result;
        size_t minSize = min(frames1.size(), frames2.size());
        for (size_t i = 0; i < minSize; i++) {
            Mat resized;
            resize(frames2[i], resized, Size(frames1[i].cols, frames1[i].rows));
            Mat stacked;
            vconcat(frames1[i], resized, stacked);
            result.push_back(stacked);
        }
        return result;
    }
    
    static vector<Mat> textOverlay(const vector<Mat>& frames, const string& text, int x, int y, 
                                   double scale, Scalar color, int thickness) {
        vector<Mat> result;
        for (const auto& f : frames) {
            Mat frame = f.clone();
            putText(frame, text, Point(x, y), FONT_HERSHEY_SIMPLEX, scale, color, thickness);
            result.push_back(frame);
        }
        return result;
    }
};

// VideoEditor class
class VideoEditor {
public:
    VideoIO* io;
    GeometricOps geom;
    TemporalOps temp;
    ColorOps color;
    FilterOps filt;
    MorphOps morph;
    CompositeOps comp;
    
    VideoEditor(const string& inputPath) {
        io = new VideoIO(inputPath);
    }
    
    ~VideoEditor() {
        delete io;
    }
    
    void execute(const string& cmd, const string& output, const vector<string>& args) {
        if (cmd == "crop") {
            vector<Mat> frames = io->readAll();
            frames = GeometricOps::crop(frames, stoi(args[0]), stoi(args[1]), stoi(args[2]), stoi(args[3]));
            io->write(output, frames);
        }
        else if (cmd == "resize") {
            vector<Mat> frames = io->readAll();
            frames = GeometricOps::resize(frames, stoi(args[0]), stoi(args[1]));
            io->write(output, frames);
        }
        else if (cmd == "rotate") {
            vector<Mat> frames = io->readAll();
            frames = GeometricOps::rotate(frames, stod(args[0]), io->w, io->h);
            io->write(output, frames);
        }
        else if (cmd == "flip") {
            vector<Mat> frames = io->readAll();
            int d = args.empty() ? 1 : stoi(args[0]);
            frames = GeometricOps::flip(frames, d);
            io->write(output, frames);
        }
        else if (cmd == "reverse") {
            vector<Mat> frames = io->readAll();
            frames = TemporalOps::reverse(frames);
            io->write(output, frames);
        }
        else if (cmd == "speed") {
            vector<Mat> frames = io->readAll();
            double s = stod(args[0]);
            if (s >= 1) {
                frames = TemporalOps::speedUp(frames, s);
                io->write(output, frames);
            } else {
                io->writeFps(output, frames, (int)(io->fps * s));
            }
        }
        else if (cmd == "trim") {
            vector<Mat> frames = io->readRange(stoi(args[0]), stoi(args[1]));
            io->write(output, frames);
        }
        else if (cmd == "gray") {
            vector<Mat> frames = io->readAll();
            frames = ColorOps::gray(frames);
            io->write(output, frames);
        }
        else if (cmd == "invert") {
            vector<Mat> frames = io->readAll();
            frames = ColorOps::invert(frames);
            io->write(output, frames);
        }
        else if (cmd == "bright") {
            vector<Mat> frames = io->readAll();
            int v = args.empty() ? 50 : stoi(args[0]);
            frames = ColorOps::brightness(frames, v);
            io->write(output, frames);
        }
        else if (cmd == "contrast") {
            vector<Mat> frames = io->readAll();
            double v = args.empty() ? 1.5 : stod(args[0]);
            frames = ColorOps::contrast(frames, v);
            io->write(output, frames);
        }
        else if (cmd == "hue") {
            vector<Mat> frames = io->readAll();
            frames = ColorOps::hueShift(frames, stoi(args[0]));
            io->write(output, frames);
        }
        else if (cmd == "sat") {
            vector<Mat> frames = io->readAll();
            frames = ColorOps::saturation(frames, stod(args[0]));
            io->write(output, frames);
        }
        else if (cmd == "blur") {
            vector<Mat> frames = io->readAll();
            int k = args.empty() ? 15 : stoi(args[0]);
            frames = FilterOps::blur(frames, k);
            io->write(output, frames);
        }
        else if (cmd == "sharpen") {
            vector<Mat> frames = io->readAll();
            frames = FilterOps::sharpen(frames);
            io->write(output, frames);
        }
        else if (cmd == "edge") {
            vector<Mat> frames = io->readAll();
            frames = FilterOps::edge(frames);
            io->write(output, frames);
        }
        else if (cmd == "denoise") {
            vector<Mat> frames = io->readAll();
            frames = FilterOps::denoise(frames);
            io->write(output, frames);
        }
        else if (cmd == "threshold") {
            vector<Mat> frames = io->readAll();
            int t = args.empty() ? 127 : stoi(args[0]);
            frames = FilterOps::threshold(frames, t);
            io->write(output, frames);
        }
        else if (cmd == "dilate") {
            vector<Mat> frames = io->readAll();
            frames = MorphOps::dilate(frames, stoi(args[0]));
            io->write(output, frames);
        }
        else if (cmd == "erode") {
            vector<Mat> frames = io->readAll();
            frames = MorphOps::erode(frames, stoi(args[0]));
            io->write(output, frames);
        }
        else if (cmd == "open") {
            vector<Mat> frames = io->readAll();
            frames = MorphOps::open(frames, stoi(args[0]));
            io->write(output, frames);
        }
        else if (cmd == "close") {
            vector<Mat> frames = io->readAll();
            frames = MorphOps::close(frames, stoi(args[0]));
            io->write(output, frames);
        }
        else if (cmd == "stack") {
            vector<Mat> frames1 = io->readAll();
            VideoIO io2(args[0]);
            vector<Mat> frames2 = io2.readAll();
            frames1 = CompositeOps::stackHorizontal(frames1, frames2);
            io->write(output, frames1);
        }
        else if (cmd == "overlay") {
            vector<Mat> frames1 = io->readAll();
            VideoIO io2(args[0]);
            vector<Mat> frames2 = io2.readAll();
            int x = args.size() > 1 ? stoi(args[1]) : 0;
            int y = args.size() > 2 ? stoi(args[2]) : 0;
            frames1 = CompositeOps::overlay(frames1, frames2, x, y);
            io->write(output, frames1);
        }
        else if (cmd == "zoom") {
            vector<Mat> frames = io->readAll();
            double factor = args.empty() ? 1.5 : stod(args[0]);
            int cx = args.size() > 1 ? stoi(args[1]) : -1;
            int cy = args.size() > 2 ? stoi(args[2]) : -1;
            frames = GeometricOps::zoom(frames, factor, cx, cy);
            io->write(output, frames);
        }
        else if (cmd == "pan") {
            vector<Mat> frames = io->readAll();
            int dx = args.empty() ? 0 : stoi(args[0]);
            int dy = args.size() > 1 ? stoi(args[1]) : 0;
            frames = GeometricOps::pan(frames, dx, dy);
            io->write(output, frames);
        }
        else if (cmd == "freeze") {
            vector<Mat> frames = io->readAll();
            int frameIdx = args.empty() ? 0 : stoi(args[0]);
            int duration = args.size() > 1 ? stoi(args[1]) : 30;
            frames = TemporalOps::freeze(frames, frameIdx, duration);
            io->write(output, frames);
        }
        else if (cmd == "loop") {
            vector<Mat> frames = io->readAll();
            int count = args.empty() ? 2 : stoi(args[0]);
            frames = TemporalOps::loop(frames, count);
            io->write(output, frames);
        }
        else if (cmd == "duplicate") {
            vector<Mat> frames = io->readAll();
            int factor = args.empty() ? 2 : stoi(args[0]);
            frames = TemporalOps::duplicate(frames, factor);
            io->write(output, frames);
        }
        else if (cmd == "sepia") {
            vector<Mat> frames = io->readAll();
            frames = ColorOps::sepia(frames);
            io->write(output, frames);
        }
        else if (cmd == "warm") {
            vector<Mat> frames = io->readAll();
            int intensity = args.empty() ? 20 : stoi(args[0]);
            frames = ColorOps::warm(frames, intensity);
            io->write(output, frames);
        }
        else if (cmd == "cool") {
            vector<Mat> frames = io->readAll();
            int intensity = args.empty() ? 20 : stoi(args[0]);
            frames = ColorOps::cool(frames, intensity);
            io->write(output, frames);
        }
        else if (cmd == "posterize") {
            vector<Mat> frames = io->readAll();
            int levels = args.empty() ? 4 : stoi(args[0]);
            frames = ColorOps::posterize(frames, levels);
            io->write(output, frames);
        }
        else if (cmd == "emboss") {
            vector<Mat> frames = io->readAll();
            frames = FilterOps::emboss(frames);
            io->write(output, frames);
        }
        else if (cmd == "cartoon") {
            vector<Mat> frames = io->readAll();
            frames = FilterOps::cartoon(frames);
            io->write(output, frames);
        }
        else if (cmd == "vignette") {
            vector<Mat> frames = io->readAll();
            double intensity = args.empty() ? 0.3 : stod(args[0]);
            frames = FilterOps::vignette(frames, intensity);
            io->write(output, frames);
        }
        else if (cmd == "motion_blur") {
            vector<Mat> frames = io->readAll();
            int size = args.empty() ? 15 : stoi(args[0]);
            double angle = args.size() > 1 ? stod(args[1]) : 0.0;
            frames = FilterOps::motionBlur(frames, size, angle);
            io->write(output, frames);
        }
        else if (cmd == "fade_in") {
            vector<Mat> frames = io->readAll();
            int duration = args.empty() ? 30 : stoi(args[0]);
            frames = CompositeOps::fadeIn(frames, duration);
            io->write(output, frames);
        }
        else if (cmd == "fade_out") {
            vector<Mat> frames = io->readAll();
            int duration = args.empty() ? 30 : stoi(args[0]);
            frames = CompositeOps::fadeOut(frames, duration);
            io->write(output, frames);
        }
        else if (cmd == "crossfade") {
            vector<Mat> frames1 = io->readAll();
            VideoIO io2(args[0]);
            vector<Mat> frames2 = io2.readAll();
            int duration = args.size() > 1 ? stoi(args[1]) : 30;
            frames1 = CompositeOps::crossfade(frames1, frames2, duration);
            io->write(output, frames1);
        }
        else if (cmd == "stack_v") {
            vector<Mat> frames1 = io->readAll();
            VideoIO io2(args[0]);
            vector<Mat> frames2 = io2.readAll();
            frames1 = CompositeOps::stackVertical(frames1, frames2);
            io->write(output, frames1);
        }
        else if (cmd == "text") {
            vector<Mat> frames = io->readAll();
            string text = args.empty() ? "Text" : args[0];
            int x = args.size() > 1 ? stoi(args[1]) : 10;
            int y = args.size() > 2 ? stoi(args[2]) : 30;
            double scale = args.size() > 3 ? stod(args[3]) : 1.0;
            Scalar color = args.size() > 6 ? 
                Scalar(stoi(args[6]), stoi(args[5]), stoi(args[4])) : Scalar(255, 255, 255);
            int thickness = args.size() > 7 ? stoi(args[7]) : 2;
            frames = CompositeOps::textOverlay(frames, text, x, y, scale, color, thickness);
            io->write(output, frames);
        }
        else if (cmd == "extract") {
            int frameIdx = args.empty() ? 0 : stoi(args[0]);
            io->extractFrame(frameIdx, output);
            cout << "Done: " << output << endl;
            return;
        }
        else {
            cerr << "Unknown command: " << cmd << endl;
            return;
        }
        cout << "Done: " << output << endl;
    }
};

int main(int argc, char* argv[]) {
    if (argc < 4) {
        cerr << "Usage: " << argv[0] << " <input> <command> <output> [args...]" << endl;
        return 1;
    }
    
    string input = argv[1];
    string cmd = argv[2];
    string output = argv[3];
    vector<string> args;
    for (int i = 4; i < argc; i++) {
        args.push_back(string(argv[i]));
    }
    
    VideoEditor editor(input);
    editor.execute(cmd, output, args);
    
    return 0;
}

