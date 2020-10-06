#include "cam.hpp"

Cam::Cam()
	: Cam(1280,720,60,0) {}

Cam::Cam(int width, int height, int frameRate, int flip) {
	m_width = width;
	m_heigth = height;
	m_fr = frameRate;
	m_flip = flip;

	m_pipeline = gstreamer_pipeline(m_width,
	m_heigth,
	m_width,
	m_heigth,
	m_fr,
	m_flip);

	m_cap = new cv::VideoCapture(m_pipeline, cv::CAP_GSTREAMER);
	if(!m_cap->isOpened()) {
	std::cout<<"Failed to open camera."<<std::endl;
	m_start = false;
	} else {
		m_start = true;
	}
	std::cout << "Cam Start" << std::endl;
}

Cam::~Cam() {
	m_cap->release();
	delete m_cap;
    cv::destroyAllWindows();
}

cv::Mat Cam::TakePhoto() {
	cv::Mat img;
	if(m_start) {
		if(!m_cap->read(img)) {std::cout << "error capture cam" << std::endl;}
	}
	return img;
}

void Cam::TakePhoto(std::string path) {
	cv::Mat img = TakePhoto();
	path += ".jpg";
	//cv::imwrite(path,img);
}

void Cam::ImgShow(cv::Mat img) {
	cv::imshow("CSI Camera",img);
}

void Cam::ImgShow(std::string path) {
	cv::FileStorage file(path+".jpg", cv::FileStorage::WRITE);
	//cv::Mat img;
	//file >> img;
	//ImgShow(img);
}

void Cam::ImgShow() {
	cv::Mat img = TakePhoto();
	ImgShow(img);
}

std::string Cam::gstreamer_pipeline (int capture_width, int capture_height, int display_width, int display_height, int framerate, int flip_method) {
    return "nvarguscamerasrc ! video/x-raw(memory:NVMM), width=(int)" + std::to_string(capture_width) + ", height=(int)" +
           std::to_string(capture_height) + ", format=(string)NV12, framerate=(fraction)" + std::to_string(framerate) +
           "/1 ! nvvidconv flip-method=" + std::to_string(flip_method) + " ! video/x-raw, width=(int)" + std::to_string(display_width) + ", height=(int)" +
           std::to_string(display_height) + ", format=(string)BGRx ! videoconvert ! video/x-raw, format=(string)BGR ! appsink";
}


