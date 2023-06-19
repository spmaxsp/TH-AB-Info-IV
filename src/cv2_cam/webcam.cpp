#include "webcam.hpp"

Webcam::Webcam(int x, int y){
    this->size_x = x;
    this->size_y = y;
}

void Webcam::initDevice(int id){
    LOG_INIT_CERR();

    this->cap.open(id);
    if (!this->cap.isOpened()) {
        log(LOG_ERROR) << "Failed to open webcam\n";
    }
    this->stream_open = true;
}

void Webcam::closeDevice(){
    this->cap.release();
    this->stream_open = false;
}

void Webcam::loadDummyImage(){
    //generate blue/purple diagonal gradient
    this->webcamImage = cv::Mat(size_y, size_x, CV_8UC3);

    for (int y = 0; y < this->webcamImage.rows; y++) {
        for (int x = 0; x < this->webcamImage.cols; x++) {
            this->webcamImage.at<cv::Vec3b>(y, x)[0] = (x + y) % 256; //B
            this->webcamImage.at<cv::Vec3b>(y, x)[1] = 0; //G
            this->webcamImage.at<cv::Vec3b>(y, x)[2] = (x + y) % 256; //R
        }
    }

    //Convert to RGBA
    cv::cvtColor(this->webcamImage, this->webcamImage, cv::COLOR_RGB2RGBA);
}

void Webcam::captureImage(){
    this->cap.read(this->webcamImage);

    //Resize image
    cv::resize(this->webcamImage, this->webcamImage, cv::Size(size_x, size_y));

    //Convert to RGBA
    cv::cvtColor(this->webcamImage, this->webcamImage, cv::COLOR_BGR2RGBA);
}

void Webcam::getImage(){
    if (!this->stream_open){
        this->loadDummyImage();
    } else {
        this->captureImage();
    }
}

bool Webcam::isStreamOpen(){
    return this->stream_open;
}