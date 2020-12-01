#ifndef CAMERAHANDLER_H
#define CAMERAHANDLER_H

#include "utilityocv.h"

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

class CameraHandler {

public:

    CameraHandler(const uint64_t &width = 1280, const uint64_t &height = 720, const uint64_t &framerate = 60, const int &flip_method = 0);
    ~CameraHandler();

    bool isCameraStarted() const;

    cv::Mat takePhoto();

private:

    cv::VideoCapture *video_capture;
    std::string pipeline;

    bool _camera_started;

};

#endif // CAMERAHANDLER_H
