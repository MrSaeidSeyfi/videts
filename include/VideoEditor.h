#pragma once

#include <string>
#include <vector>

class VideoEditor {
public:
    VideoEditor(const std::string& inputPath);
    ~VideoEditor();

    void execute(const std::string& cmd, const std::string& output, const std::vector<std::string>& args);

private:
    // opaque impl pointer
    class Impl;
    Impl* impl;
};
