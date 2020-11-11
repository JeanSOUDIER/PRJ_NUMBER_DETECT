#ifndef CAM_H
#define CAM_H

#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

constexpr int CV_IMWRITE_JPEG_QUALITY = 1;

class Cam {
	public:
		Cam();
		Cam(int width, int height, int frameRate, int flip);
		~Cam();

		cv::Mat TakePhoto();
		void TakePhoto(std::string path);

		void ImgShow(cv::Mat img);
		void ImgShow();
		void ImgShow(std::string path);

		cv::Mat ImgRead(std::string fileName);
		bool ImgWrite(std::string fileName, cv::Mat img);
	private:
		std::string gstreamer_pipeline (int capture_width, int capture_height, int display_width, int display_height, int framerate, int flip_method);

		int m_width, m_heigth, m_fr, m_flip;
		cv::Mat m_mat;
		cv::VideoCapture *m_cap;
		std::string m_pipeline;
		bool m_start = false;

};

#endif //CAM_H