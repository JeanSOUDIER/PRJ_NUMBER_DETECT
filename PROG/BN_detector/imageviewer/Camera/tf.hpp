#ifndef TF_H
#define TF_H

#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <cstdio>
#include <cstring>
#include <QImage>

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/core/utility.hpp>

#include "cam.hpp"

class TF {
	public:
		TF();
		TF(bool print);
        ~TF();

        std::vector<QImage> ToProcessed();
        std::vector<QImage> ToProcessed(std::string path);
        std::vector<QImage> ToProcessed(cv::Mat img);

        QImage ToHistogram();

        double GetThreshold();

        QImage TakePhoto();

	private:
        cv::Mat ToGray(cv::Mat img);
        cv::Mat ToHistogram(cv::Mat imgGray);
        cv::Mat ToThreshold(cv::Mat imgGray, double thres);
        std::vector<cv::Mat> ToRegionprops(cv::Mat imgGray, cv::Mat imgThres, bool test);
        cv::Mat ToInvert(cv::Mat &img);

        cv::Mat show_histogram(cv::Mat const& image);
        QImage ToQImage(cv::Mat img, bool format);
        std::vector<QImage> ToQImageVect(std::vector<cv::Mat> img, bool format, bool inverted);

		double m_moy;
		bool m_print;
		Cam CPI2;
		cv::Mat m_img;
};

#endif //TF_H
