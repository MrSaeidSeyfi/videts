#include "../include/VideoEditor_structured.h"
#include "VideoIO.h"
#include "GeometricOps.h"
#include "TemporalOps.h"
#include "ColorOps.h"
#include "FilterOps.h"
#include "MorphOps.h"
#include "CompositeOps.h"

#include <iostream>

using namespace std;

// Impl is implemented here and uses the free classes above
class VideoEditor::Impl {
public:
    VideoIO* io;
    Impl(const string& inputPath) { io = new VideoIO(inputPath); }
    ~Impl() { delete io; }

    void execute(const string& cmd, const string& output, const vector<string>& args) {
        if (cmd == "crop") {
            auto frames = io->readAll();
            frames = GeometricOps::crop(frames, stoi(args[0]), stoi(args[1]), stoi(args[2]), stoi(args[3]));
            io->write(output, frames);
        }
        else if (cmd == "resize") {
            auto frames = io->readAll();
            frames = GeometricOps::resize(frames, stoi(args[0]), stoi(args[1]));
            io->write(output, frames);
        }
        else if (cmd == "rotate") {
            auto frames = io->readAll();
            frames = GeometricOps::rotate(frames, stod(args[0]), io->w, io->h);
            io->write(output, frames);
        }
        else if (cmd == "flip") {
            auto frames = io->readAll();
            int d = args.empty() ? 1 : stoi(args[0]);
            frames = GeometricOps::flip(frames, d);
            io->write(output, frames);
        }
        else if (cmd == "reverse") {
            auto frames = io->readAll();
            frames = TemporalOps::reverse(frames);
            io->write(output, frames);
        }
        else if (cmd == "speed") {
            auto frames = io->readAll();
            double s = stod(args[0]);
            if (s >= 1) { frames = TemporalOps::speedUp(frames, s); io->write(output, frames); }
            else io->writeFps(output, frames, (int)(io->fps * s));
        }
        else if (cmd == "trim") { auto frames = io->readRange(stoi(args[0]), stoi(args[1])); io->write(output, frames); }
        else if (cmd == "gray") { auto frames = io->readAll(); frames = ColorOps::gray(frames); io->write(output, frames); }
        else if (cmd == "invert") { auto frames = io->readAll(); frames = ColorOps::invert(frames); io->write(output, frames); }
        else if (cmd == "bright") { auto frames = io->readAll(); int v = args.empty() ? 50 : stoi(args[0]); frames = ColorOps::brightness(frames, v); io->write(output, frames); }
        else if (cmd == "contrast") { auto frames = io->readAll(); double v = args.empty() ? 1.5 : stod(args[0]); frames = ColorOps::contrast(frames, v); io->write(output, frames); }
        else if (cmd == "hue") { auto frames = io->readAll(); frames = ColorOps::hueShift(frames, stoi(args[0])); io->write(output, frames); }
        else if (cmd == "sat") { auto frames = io->readAll(); frames = ColorOps::saturation(frames, stod(args[0])); io->write(output, frames); }
        else if (cmd == "blur") { auto frames = io->readAll(); int k = args.empty() ? 15 : stoi(args[0]); frames = FilterOps::blur(frames, k); io->write(output, frames); }
        else if (cmd == "sharpen") { auto frames = io->readAll(); frames = FilterOps::sharpen(frames); io->write(output, frames); }
        else if (cmd == "edge") { auto frames = io->readAll(); frames = FilterOps::edge(frames); io->write(output, frames); }
        else if (cmd == "denoise") { auto frames = io->readAll(); frames = FilterOps::denoise(frames); io->write(output, frames); }
        else if (cmd == "threshold") { auto frames = io->readAll(); int t = args.empty() ? 127 : stoi(args[0]); frames = FilterOps::threshold(frames, t); io->write(output, frames); }
        else if (cmd == "dilate") { auto frames = io->readAll(); frames = MorphOps::dilate(frames, stoi(args[0])); io->write(output, frames); }
        else if (cmd == "erode") { auto frames = io->readAll(); frames = MorphOps::erode(frames, stoi(args[0])); io->write(output, frames); }
        else if (cmd == "open") { auto frames = io->readAll(); frames = MorphOps::open(frames, stoi(args[0])); io->write(output, frames); }
        else if (cmd == "close") { auto frames = io->readAll(); frames = MorphOps::close(frames, stoi(args[0])); io->write(output, frames); }
        else if (cmd == "stack") { auto frames1 = io->readAll(); VideoIO io2(args[0]); auto frames2 = io2.readAll(); auto outFrames = CompositeOps::stackHorizontal(frames1, frames2); io->write(output, outFrames); }
        else if (cmd == "overlay") { auto frames1 = io->readAll(); VideoIO io2(args[0]); auto frames2 = io2.readAll(); int x = args.size() > 1 ? stoi(args[1]) : 0; int y = args.size() > 2 ? stoi(args[2]) : 0; auto outFrames = CompositeOps::overlay(frames1, frames2, x, y); io->write(output, outFrames); }
        else if (cmd == "zoom") { auto frames = io->readAll(); double factor = args.empty() ? 1.5 : stod(args[0]); int cx = args.size() > 1 ? stoi(args[1]) : -1; int cy = args.size() > 2 ? stoi(args[2]) : -1; frames = GeometricOps::zoom(frames, factor, cx, cy); io->write(output, frames); }
        else if (cmd == "pan") { auto frames = io->readAll(); int dx = args.empty() ? 0 : stoi(args[0]); int dy = args.size() > 1 ? stoi(args[1]) : 0; frames = GeometricOps::pan(frames, dx, dy); io->write(output, frames); }
        else if (cmd == "freeze") { auto frames = io->readAll(); int frameIdx = args.empty() ? 0 : stoi(args[0]); int duration = args.size() > 1 ? stoi(args[1]) : 30; frames = TemporalOps::freeze(frames, frameIdx, duration); io->write(output, frames); }
        else if (cmd == "loop") { auto frames = io->readAll(); int count = args.empty() ? 2 : stoi(args[0]); frames = TemporalOps::loop(frames, count); io->write(output, frames); }
        else if (cmd == "duplicate") { auto frames = io->readAll(); int factor = args.empty() ? 2 : stoi(args[0]); frames = TemporalOps::duplicate(frames, factor); io->write(output, frames); }
        else if (cmd == "sepia") { auto frames = io->readAll(); frames = ColorOps::sepia(frames); io->write(output, frames); }
        else if (cmd == "warm") { auto frames = io->readAll(); int intensity = args.empty() ? 20 : stoi(args[0]); frames = ColorOps::warm(frames, intensity); io->write(output, frames); }
        else if (cmd == "cool") { auto frames = io->readAll(); int intensity = args.empty() ? 20 : stoi(args[0]); frames = ColorOps::cool(frames, intensity); io->write(output, frames); }
        else if (cmd == "posterize") { auto frames = io->readAll(); int levels = args.empty() ? 4 : stoi(args[0]); frames = ColorOps::posterize(frames, levels); io->write(output, frames); }
        else if (cmd == "emboss") { auto frames = io->readAll(); frames = FilterOps::emboss(frames); io->write(output, frames); }
        else if (cmd == "cartoon") { auto frames = io->readAll(); frames = FilterOps::cartoon(frames); io->write(output, frames); }
        else if (cmd == "vignette") { auto frames = io->readAll(); double intensity = args.empty() ? 0.3 : stod(args[0]); frames = FilterOps::vignette(frames, intensity); io->write(output, frames); }
        else if (cmd == "motion_blur") { auto frames = io->readAll(); int size = args.empty() ? 15 : stoi(args[0]); double angle = args.size() > 1 ? stod(args[1]) : 0.0; frames = FilterOps::motionBlur(frames, size, angle); io->write(output, frames); }
        else if (cmd == "fade_in") { auto frames = io->readAll(); int duration = args.empty() ? 30 : stoi(args[0]); frames = CompositeOps::fadeIn(frames, duration); io->write(output, frames); }
        else if (cmd == "fade_out") { auto frames = io->readAll(); int duration = args.empty() ? 30 : stoi(args[0]); frames = CompositeOps::fadeOut(frames, duration); io->write(output, frames); }
        else if (cmd == "crossfade") { auto frames1 = io->readAll(); VideoIO io2(args[0]); auto frames2 = io2.readAll(); int duration = args.size() > 1 ? stoi(args[1]) : 30; auto outF = CompositeOps::crossfade(frames1, frames2, duration); io->write(output, outF); }
        else if (cmd == "stack_v") { auto frames1 = io->readAll(); VideoIO io2(args[0]); auto frames2 = io2.readAll(); auto outF = CompositeOps::stackVertical(frames1, frames2); io->write(output, outF); }
        else if (cmd == "text") { auto frames = io->readAll(); string text = args.empty() ? "Text" : args[0]; int x = args.size() > 1 ? stoi(args[1]) : 10; int y = args.size() > 2 ? stoi(args[2]) : 30; double scale = args.size() > 3 ? stod(args[3]) : 1.0; Scalar color = args.size() > 6 ? Scalar(stoi(args[6]), stoi(args[5]), stoi(args[4])) : Scalar(255,255,255); int thickness = args.size() > 7 ? stoi(args[7]) : 2; auto outF = CompositeOps::textOverlay(frames, text, x, y, scale, color, thickness); io->write(output, outF); }
        else if (cmd == "extract") { int frameIdx = args.empty() ? 0 : stoi(args[0]); io->extractFrame(frameIdx, output); cout << "Done: " << output << endl; return; }
        else { cerr << "Unknown command: " << cmd << endl; return; }
        cout << "Done: " << output << endl;
    }
};

VideoEditor::VideoEditor(const std::string& inputPath) { impl = new Impl(inputPath); }
VideoEditor::~VideoEditor() { delete impl; }
void VideoEditor::execute(const std::string& cmd, const std::string& output, const std::vector<std::string>& args) { impl->execute(cmd, output, args); }
