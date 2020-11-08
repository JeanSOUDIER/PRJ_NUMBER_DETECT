#ifndef CAM_H
#define CAM_H

#include <iostream>
#include <string>
#include <vector>
#include <opencv2/opencv.hpp>
#include <pthread.h>
#include "Matrix.hpp"

constexpr int CV_IMWRITE_JPEG_QUALITY = 1;

class Cam {
	public:
		Cam();
		Cam(int width, int height, int frameRate, int flip);
		~Cam();

		cv::Mat TakePhoto();
		ste::Matrix<double> TakePhotoM();
		void TakePhoto(std::string path);

		void ImgShow(cv::Mat img);
		void ImgShow();
		cv::Mat ImgShow(std::string path);
		void Test();

		static void* CamHelper(void *context);
        void StartThread();
	protected:
        void* ThreadRun();
        cv::Mat ArrayToMat();
        void MatToArray(cv::Mat img);
        void Cam::UpdateImShow();

	private:
		std::string gstreamer_pipeline (int capture_width, int capture_height, int display_width, int display_height, int framerate, int flip_method);
		void ImageWrite(const cv::Mat &image, const std::string filename);

		int m_width, m_heigth, m_fr, m_flip;
		cv::Mat m_mat;
		cv::VideoCapture *m_cap;
		std::string m_pipeline;
		bool m_start = false;

		std::array<std::array<std::atomic<int> , 1280>, 720> m_buffImg;
		std::vector<cv::Mat> m_imgShow;
		std::atomic<bool> m_start;
		std::atomic<bool> m_buff;

		pthread_t *inc_x_thread;
};

#endif //CAM_H
