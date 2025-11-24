#include "../include/VideoIO.h"
#include <opencv2/imgcodecs.hpp>

using namespace cv;
using namespace std;

VideoIO::VideoIO(const std::string& path) {
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

VideoIO::~VideoIO() {
    cap.release();
}

vector<Mat> VideoIO::readAll() {
    vector<Mat> frames;
    Mat frame;
    while (cap.read(frame)) {
        frames.push_back(frame.clone());
    }
    return frames;
}

vector<Mat> VideoIO::readRange(int start, int end) {
    cap.set(CAP_PROP_POS_FRAMES, start);
    vector<Mat> frames;
    Mat frame;
    for (int i = 0; i < (end - start); i++) {
        if (!cap.read(frame)) break;
        frames.push_back(frame.clone());
    }
    return frames;
}

void VideoIO::write(const std::string& path, const vector<Mat>& frames) {
    if (frames.empty()) return;
    int h = frames[0].rows;
    int w = frames[0].cols;
    VideoWriter writer(path, VideoWriter::fourcc('m','p','4','v'), fps, Size(w, h));
    for (const auto& frame : frames) writer.write(frame);
    writer.release();
}

void VideoIO::writeFps(const std::string& path, const vector<Mat>& frames, int newFps) {
    if (frames.empty()) return;
    int h = frames[0].rows;
    int w = frames[0].cols;
    VideoWriter writer(path, VideoWriter::fourcc('m','p','4','v'), newFps, Size(w, h));
    for (const auto& frame : frames) writer.write(frame);
    writer.release();
}

bool VideoIO::extractFrame(int frameIdx, const std::string& outputPath) {
    cap.set(CAP_PROP_POS_FRAMES, frameIdx);
    Mat frame;
    if (cap.read(frame)) {
        imwrite(outputPath, frame);
        return true;
    }
    return false;
}
