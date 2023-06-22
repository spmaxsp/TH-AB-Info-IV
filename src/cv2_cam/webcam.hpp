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
    
    int pos_mh_x;
    int pos_mh_y;

    Webcam(int x, int y);

    void initDevice(int id);
    void closeDevice();
    void getImage();
    void locateMovingHead();

    bool isStreamOpen();  
};
