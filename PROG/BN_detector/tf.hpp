#ifndef TF_H
#define TF_H

#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <cstdio>
#include <cstring>
//#include <QImage>

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core/utility.hpp>

#include "region.h"
#include "regionprops.h"
#include "Cam.hpp"

class TF {
	public:
		TF();
		~TF();

		void ToProcessed();
		void ToProcessed(cv::Mat img);

	private:

};

#endif //TF_H
