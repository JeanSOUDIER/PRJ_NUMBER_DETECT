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

        QImage QToGray(QImage img);
        QImage QToHistogram(QImage img);
        double GetThreshold();
        QImage QToThreshold(QImage img, double thres);
        std::vector<QImage> QToRegionprops(QImage imgGray, QImage imgThres, bool test);

        QImage TakePhoto();

	private:
        cv::Mat ToGray(cv::Mat img);
        cv::Mat ToHistogram(cv::Mat imgGray);
        cv::Mat ToThreshold(cv::Mat imgGray, double thres);
        std::vector<cv::Mat> ToRegionprops(cv::Mat imgGray, cv::Mat imgThres, bool test);

        cv::Mat show_histogram(cv::Mat const& image);
        QImage ToQImage(cv::Mat img, bool format);
        std::vector<QImage> ToQImageVect(std::vector<cv::Mat> img, bool format);

        cv::Mat ToCVMat(QImage img, bool format);
        std::vector<cv::Mat> ToCVMatVect(std::vector<QImage> img, bool format);

		double m_moy;
		bool m_print;
		Cam CPI2;

};

#endif //TF_H
