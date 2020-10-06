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

	m_cap(m_pipeline, cv::CAP_GSTREAMER);
    if(!m_cap.isOpened()) {
		std::cout<<"Failed to open camera."<<std::endl;
		m_start = false;
    } else {
    	m_start = true;
    }
    std::cout << "Cam Start" << std::endl;
}

Cam::~Cam() {
	m_cap.release();
    cv::destroyAllWindows();
}

cv::Mat Cam::TakePhoto() {
	cv::Mat img;
	if(m_start) {
		bool test = m_cap.read(img);
		if(!test) {std::cout << "error capture cam" << std::endl;}
	}
	return img;
}

void Cam::TakePhoto(std::string path) {
	cv::Mat img = TakePhoto();
	cv::FileStorage file(path+".jpg", cv::FileStorage::WRITE);
	file << "Picture" << img;
}

void Cam::ImgShow(cv::Mat img) {
	cv::imshow("CSI Camera",img);
}

void Cam::ImgShow(std::string path) {
	cv::FileStorage file(path+".jpg", cv::FileStorage::WRITE);
	file >> img;
	ImgShow(img);
}

void Cam::ImgShow() {
	cv::Mat img = TakePhoto();
	ImgShow(img);
}
