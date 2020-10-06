#ifndef CAM_H
#define CAM_H

#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>

class Cam {
	public:
		Cam();
		Cam(int width, int height, int frameRate, int flip);

		cv::Mat TakePhoto();
		void TakePhoto(std::string path);

		void ImgShow(cv::Mat img);
		void ImgShow(std::string path);
		void ImgShow();
	private:
		int m_width, m_heigth, m_fr, m_flip;
		cv::Mat m_mat;
		cv::VideoCapture m_cap;
		std::string m_pipeline;
		bool m_start = false;

};

#endif //CAM_H
