#ifndef CAM_H
#define CAM_H

#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>

class Cam {
	public:
		Cam();
		Cam(int width, int height, int frameRate, int flip);
		~Cam();

		cv::Mat TakePhoto();
		void TakePhoto(std::string path);

		void ImgShow(cv::Mat img);
		void ImgShow(std::string path);
		void ImgShow();
	private:
		std::string gstreamer_pipeline (int capture_width, int capture_height, int display_width, int display_height, int framerate, int flip_method);

		int m_width, m_heigth, m_fr, m_flip;
		cv::Mat m_mat;
		cv::VideoCapture *m_cap;
		std::string m_pipeline;
		bool m_start = false;

};

#endif //CAM_H
