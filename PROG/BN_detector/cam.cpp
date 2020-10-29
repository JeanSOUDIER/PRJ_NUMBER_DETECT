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

ste::Matrix<double> Cam::TakePhotoM() {
	cv::Mat img = TakePhoto();
	img = TakePhoto();
	//ImgShow(img);
	ste::Matrix<double> res(img.cols,img.rows);
	for(unsigned int i=0;i<img.cols;i++) {
		for(unsigned int j=0;j<img.rows;j++) {
			res.at(i,j) = img.at<int>(j,i);
		}
	}
	return res;
}

void Cam::TakePhoto(std::string path) {
	cv::Mat img = TakePhoto();
	path += ".jpg";
	ImageWrite(img,path);
}

void Cam::ImgShow(cv::Mat img) {
	cv::namedWindow("CSI Camera", cv::WINDOW_AUTOSIZE);
	cv::imshow("CSI Camera",img);
	while(1) {
		int keycode = cv::waitKey(30) & 0xff;
		if (keycode == 27) break ;
	}
}
void Cam::ImgShow() {
	ImgShow(TakePhoto());
}

cv::Mat Cam::ImgShow(std::string path) {
	cv::FileStorage file(path+".jpg", cv::FileStorage::WRITE);
	//cv::Mat img;
	//file >> img;
	//ImgShow(img);
}

void Cam::Test() {
	cv::namedWindow("CSI Camera", cv::WINDOW_AUTOSIZE);
    cv::Mat img;
	std::cout << "Hit ESC to exit" << "\n" ;
    while(true)
    {
    	if (!m_cap->read(img)) {
		std::cout<<"Capture read error"<<std::endl;
		break;
	}
	
	cv::imshow("CSI Camera",img);
	int keycode = cv::waitKey(30) & 0xff ; 
        if (keycode == 27) break ;
    }
}

std::string Cam::gstreamer_pipeline (int capture_width, int capture_height, int display_width, int display_height, int framerate, int flip_method) {
    return "nvarguscamerasrc ! video/x-raw(memory:NVMM), width=(int)" + std::to_string(capture_width) + ", height=(int)" +
           std::to_string(capture_height) + ", format=(string)NV12, framerate=(fraction)" + std::to_string(framerate) +
           "/1 ! nvvidconv flip-method=" + std::to_string(flip_method) + " ! video/x-raw, width=(int)" + std::to_string(display_width) + ", height=(int)" +
           std::to_string(display_height) + ", format=(string)BGRx ! videoconvert ! video/x-raw, format=(string)BGR ! appsink";
}

void Cam::ImageWrite(const cv::Mat &image, const std::string filename) {
    // Support for writing JPG
    std::vector<int> compression_params;
    compression_params.push_back(CV_IMWRITE_JPEG_QUALITY);
    compression_params.push_back(100);

    // This writes to the specified path
    std::string path = "/path/you/provide/" + filename + ".jpg";

    //cv::imwrite(path, image, compression_params);
}

