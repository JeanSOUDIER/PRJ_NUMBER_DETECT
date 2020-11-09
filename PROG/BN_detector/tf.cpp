#include "tf.hpp"

TF::TF() {}

TF::~TF() {}

void TF::ToProcessed() {
	Cam RPCY;
	std::cout << "take photo" << std::endl;
	return ToProcessed(RPCY.TakePhoto());
}

void TF::ToProcessed(cv::Mat img) {
	Cam RPCY;
	cv::Mat imgGray, imgDST;
	RPCY.ImgShow(img);

	std::cout << "to gray" << std::endl;
	cvtColor(img, imgGray, cv::COLOR_RGB2GRAY);
	RPCY.ImgShow(imgGray);

	std::cout << "threshold" << std::endl;
	/* 0: Binary
     1: Binary Inverted
     2: Threshold Truncated
     3: Threshold to Zero
     4: Threshold to Zero Inverted
    */
	threshold(imgGray, imgDST, , 255, 3);
	RPCY.ImgShow(imgDST);

	std::cout << "to label" << std::endl;
    cv::Mat labelImage(img.size(), CV_32S);
    int nLabels = connectedComponents(imgDST, labelImage, 8);
	RPCY.ImgShow(labelImage);

	std::cout << "to regioprops" << std::endl;
	std::vector<cv::Point> parts;
	RegionProps R(parts, imgDST);
	cv::oments a = R.getRegion().Moments();

	// pyrUp( src, src, Size( src.cols*2, src.rows*2 )

	std::cout << "print" << std::endl;
	//QImage imgIn= QImage((uchar*) imgDST.data, imgDST.cols, imgDST.rows, imgDST.step, QImage::Format_RGB888);
	//return imgIn;
}
