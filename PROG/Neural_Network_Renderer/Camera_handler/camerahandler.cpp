#include "camerahandler.h"

CameraHandler::CameraHandler(const uint64_t &width , const uint64_t &height, const uint64_t &framerate, const int &flip_method){

   pipeline = "nvarguscamerasrc ! video/x-raw(memory:NVMM), width=(int)" + std::to_string(width) + ", height=(int)" +
               std::to_string(height) + ", format=(string)NV12, framerate=(fraction)" + std::to_string(framerate) +
               "/1 ! nvvidconv flip-method=" + std::to_string(flip_method) + " ! video/x-raw, width=(int)" + std::to_string(width) + ", height=(int)" +
               std::to_string(height) + ", format=(string)BGRx ! videoconvert ! video/x-raw, format=(string)BGR ! appsink";

   video_capture = new cv::VideoCapture(pipeline, cv::CAP_GSTREAMER);

}

CameraHandler::~CameraHandler(){

    video_capture->release();
    delete video_capture;

}


cv::Mat CameraHandler::takePhoto(){

    _camera_started = video_capture->isOpened();

    cv::Mat photo;

    if(isCameraStarted()){
        do{
            video_capture->read(photo);
        }while(photo.empty());
    }

   cv::cvtColor(photo , photo , cv::COLOR_BGR2RGB);

   return photo;
}

bool CameraHandler::isCameraStarted() const{return _camera_started;}
