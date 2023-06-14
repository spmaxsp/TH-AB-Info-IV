#pragma once

#include <opencv2/opencv.hpp>
#include <vector>

#include <BSlogger.hpp>

class Webcam {
private:
    cv::VideoCapture cap;

    bool stream_open = false;

    int size_x;
    int size_y;

    void loadDummyImage();
    void captureImage();
    
public:
    cv::Mat webcamImage;

    Webcam(int x, int y);

    std::vector<std::pair<int, std::string>> getAvailableWebcams();

    void initDevice(int id);
    void closeDevice();

    void getImage();
    bool isStreamOpen();    
};
