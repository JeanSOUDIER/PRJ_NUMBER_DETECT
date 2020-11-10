#include "tf.hpp"

TF::TF() {}

TF::~TF() {}

void TF::ToProcessed() {
	std::cout << "take photo" << std::endl;
	return ToProcessed(CPI2.TakePhoto());
}

void TF::ToProcessed(std::string path) {
	std::cout << "read photo" << std::endl;
	return ToProcessed(CPI2.ImgRead(path));
}

void TF::ToProcessed(cv::Mat img) {
	std::cout << "start process" << std::endl;
	cv::Mat imgGray, imgThres, histo, dst, test;
	//dst.create(img.size(), img.type());
	CPI2.ImgShow(img);

	std::cout << "to gray" << std::endl;
	cv::cvtColor(img, imgGray, cv::COLOR_BGR2GRAY);
	CPI2.ImgShow(imgGray);

	std::cout << "histogram" << std::endl;
	cv::Mat imgHisto = show_histogram(imgGray);
	CPI2.ImgShow(imgHisto);
	int thres = m_moy;
	std::cout << "thres " << thres << std::endl;
	//thres = cv::threshold(imgGray, dst, 0, 255, cv::THRESH_OTSU);

	std::cout << "threshold" << std::endl;
	threshold(imgGray, imgThres, thres, 255, 3);
	CPI2.ImgShow(imgThres);

	std::cout << "to regioprops" << std::endl;
	std::vector<std::vector<cv::Point>> cnt;
	std::vector<cv::Vec4i> hier;
	cv::findContours(imgThres, cnt, hier, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);

	cv::Mat imgDraw = imgGray;
	std::vector<cv::Rect> rectan(cnt.size());
	std::vector<cv::Mat> rez;
	//std::vector<QImage> res;
	for(size_t i=0;i<cnt.size();i++) {
		if(cv::contourArea(cnt.at(i)) > 5000 && cv::contourArea(cnt.at(i)) < 900000) {
			std::cout << (int)(i) << " " << cv::contourArea(cnt.at(i)) << std::endl;
			rectan.at(i) = cv::boundingRect(cnt.at(i));
			cv::rectangle(imgDraw, rectan.at(i), {0, 255, 0}, 2);

			rez.push_back(imgGray(rectan.at(i)));
			double l = 28/static_cast<double>(rectan.at(i).height);
			double h = 28/static_cast<double>(rectan.at(i).width);
			std::cout << l << " " << h << std::endl;
			cv::resize(rez.at(rez.size()-1), rez.at(rez.size()-1), cv::Size(), h, l);
			CPI2.ImgShow(rez.at(rez.size()-1));
			//res.push_back(ToQImage(rez.at(rez.size()-1)));
		}
	}
	CPI2.ImgShow(imgDraw);

	//return res;
}

cv::Mat TF::show_histogram(cv::Mat const& image) {
    // Set histogram bins count
    int bins = 256;
    int histSize[] = {bins};
    // Set ranges for histogram bins
    float lranges[] = {0, 256};
    const float* ranges[] = {lranges};
    // create matrix for histogram
    cv::Mat hist;
    int channels[] = {0};

    // create matrix for histogram visualization
    int const hist_height = 256;
    cv::Mat3b hist_image = cv::Mat3b::zeros(hist_height, bins);

    cv::calcHist(&image, 1, channels, cv::Mat(), hist, 1, histSize, ranges, true, false);

    double max_val=0;
    minMaxLoc(hist, 0, &max_val);

    // visualize each bin
    m_moy = 0;
    int maxi = 0;
    for(int b = 0; b < bins; b++) {
        float const binVal = hist.at<float>(b);
        int   const height = cvRound(binVal*hist_height/max_val);
		if(maxi < height) {
			maxi = height;
			m_moy = b;
		}
        cv::line
            ( hist_image
            , cv::Point(b, hist_height-height), cv::Point(b, hist_height)
            , cv::Scalar::all(255)
            );
    }
    m_moy -= 6;
    return hist_image;
}

/*QImage TF::ToQImage(cv::Mat img) {
	QImage imgIn = QImage((uchar*) img.data, img.cols, img.rows, img.step, QImage::Format_RGB888);
	return imgIn;
}*/
