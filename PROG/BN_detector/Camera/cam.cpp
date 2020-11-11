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
		if(!m_cap->read(img)) {std::cout << "error capture cam" << std::endl;} else {
			if(!m_cap->read(img)) {std::cout << "error capture cam" << std::endl;}
		}
	}
	return img;
}

void Cam::TakePhoto(std::string path) {
	cv::Mat img = TakePhoto();
	std::cout << "taken" << std::endl;
	bool test = ImgWrite(path, img);
	std::cout << "test " << static_cast<int>(test) << std::endl;
}

void Cam::ImgShow(cv::Mat img) {
	cv::namedWindow("CSI Camera", cv::WINDOW_AUTOSIZE);
	cv::imshow("CSI Camera",img);
	while(1) {
		int keycode = cv::waitKey(30) & 0xff;
		if (keycode == 27) break ;
	}
	sleep(1);
}

void Cam::ImgShow() {
	ImgShow(TakePhoto());
}

void Cam::ImgShow(std::string path) {
	cv::Mat img = ImgRead(path);
	ImgShow(img);
}

std::string Cam::gstreamer_pipeline (int capture_width, int capture_height, int display_width, int display_height, int framerate, int flip_method) {
    return "nvarguscamerasrc ! video/x-raw(memory:NVMM), width=(int)" + std::to_string(capture_width) + ", height=(int)" +
           std::to_string(capture_height) + ", format=(string)NV12, framerate=(fraction)" + std::to_string(framerate) +
           "/1 ! nvvidconv flip-method=" + std::to_string(flip_method) + " ! video/x-raw, width=(int)" + std::to_string(display_width) + ", height=(int)" +
           std::to_string(display_height) + ", format=(string)BGRx ! videoconvert ! video/x-raw, format=(string)BGR ! appsink";
}

cv::Mat Cam::ImgRead(std::string fileName) {
	fileName = fileName + ".png";
	std::cout << fileName << std::endl;
	cv::Mat img = cv::imread("lena.png");
	std::cout << img.cols << " " << img.rows << std::endl;
	return img;
}

bool Cam::ImgWrite(std::string fileName, cv::Mat img) {
    std::vector<int> compression_params;
    compression_params.push_back(cv::IMWRITE_PNG_COMPRESSION);
    compression_params.push_back(9);
    bool result = false;
    std::cout << "try" << std::endl;
    try {
        result = cv::imwrite(fileName+".png", img, compression_params);
    } catch (const cv::Exception& ex) {
        fprintf(stderr, "Exception converting image to PNG format: %s\n", ex.what());
    }
    return result;
}
