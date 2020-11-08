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

	m_start.store(true, std::memory_order_release);
	m_buff.store(false, std::memory_order_release);

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
	/*cv::namedWindow("CSI Camera", cv::WINDOW_AUTOSIZE);
	cv::imshow("CSI Camera",img);
	while(1) {
		int keycode = cv::waitKey(30) & 0xff;
		if (keycode == 27) break ;
	}*/
	MatToArray(img);
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

cv::Mat Cam::ArrayToMat() {
	cv::Mat res(1280, 720,int);
	for(unsigned int i=0;i<img.size();i++) {
		for(unsigned int j=0;j<img.at(0).size();j++) {
			res.at<int>(i,j) = m_buffImg[j][i];
		}
	}
	return res;
}

void Cam::MatToArray(cv::Mat img) {
	std::array<std::array<int, 1280>, 720> res;
	for(unsigned int i=0;i<img.size();i++) {
		for(unsigned int j=0;j<img.at(0).size();j++) {
			m_buffImg[j][i].store(img.at<int>(j,i),std::memory_order_release);
		}
	}
	m_buff.store(true, std::memory_order_release);
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

void* Cam::CamHelper(void *context) { 
    return static_cast<Cam*>(context)->ThreadRun();
}

void* Cam::ThreadRun() {
    while(m_start.load(std::memory_order_acquire)) {
		UpdateImShow();
    	if(m_imgShow.size()) {
    		cv::namedWindow("CSI Camera", cv::WINDOW_AUTOSIZE);
			cv::imshow("CSI Camera",m_imgShow.at(0));
			while(1) {
				int keycode = cv::waitKey(30) & 0xff;
				if (keycode == 27) break;
				UpdateImShow();
			}
			m_imgShow.erase(m_imgShow.begin());
    	}
    }

    pthread_exit(NULL);
    return 0;
}

void Cam::StartThread() {
    inc_x_thread = new pthread_t();
    const int rcL = pthread_create(inc_x_thread, NULL, &Cam::CamHelper, this);
    if (rcL) {
        std::cout << "Error:unable to create thread Cam," << rcL << std::endl;
    }
}

void Cam::UpdateImShow() {
	if(m_buff.load(std::memory_order_acquire)) {
		m_imgShow.push_back(ArrayToMat());
		m_buff.store(false, std::memory_order_release);
	}
}
