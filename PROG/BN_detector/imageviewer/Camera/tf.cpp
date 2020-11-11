#include "tf.hpp"

TF::TF() {
	m_print = false;
}

TF::TF(bool print) {
	m_print = print;
}

TF::~TF() {}

std::vector<QImage> TF::ToProcessed() {
	std::cout << "take photo" << std::endl;
	return ToProcessed(CPI2.TakePhoto());
}

std::vector<QImage> TF::ToProcessed(std::string path) {
	std::cout << "read photo" << std::endl;
	return ToProcessed(CPI2.ImgRead(path));
}

std::vector<QImage> TF::ToProcessed(cv::Mat img) {
	std::cout << "image in" << std::endl;
	if(m_print) {CPI2.ImgShow(img);}

	std::cout << "to gray" << std::endl;
	cv::Mat imgGray;
	cv::cvtColor(img, imgGray, cv::COLOR_BGR2GRAY);
	if(m_print) {CPI2.ImgShow(imgGray);}

	std::cout << "histogram" << std::endl;
	cv::Mat histo;
	cv::Mat imgHisto = show_histogram(imgGray);
	if(m_print) {CPI2.ImgShow(imgHisto);}
	int thres = m_moy;
	std::cout << "thres " << thres << std::endl;

	std::cout << "threshold" << std::endl;
	cv::Mat imgThres;
	threshold(imgGray, imgThres, thres, 255, 3);
	if(m_print) {CPI2.ImgShow(imgThres);}

	std::cout << "to regioprops" << std::endl;
	std::vector<std::vector<cv::Point>> cnt;
	std::vector<cv::Vec4i> hier;
	cv::findContours(imgThres, cnt, hier, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);

	cv::Mat imgDraw = imgGray;
	std::vector<cv::Rect> rectan(cnt.size());
	std::vector<cv::Mat> rez;
	std::vector<cv::Mat> rez2;
	std::vector<double> places;
    std::vector<QImage> res;
	for(size_t i=0;i<cnt.size();i++) {
		if(cv::contourArea(cnt.at(i)) > 5000 && cv::contourArea(cnt.at(i)) < 900000) {
			std::cout << (int)(i) << " " << cv::contourArea(cnt.at(i)) << std::endl;
			rectan.at(i) = cv::boundingRect(cnt.at(i));
			cv::rectangle(imgDraw, rectan.at(i), {0, 255, 0}, 2);

			rez.push_back(imgGray(rectan.at(i)));
            double l = 28/static_cast<double>(rectan.at(i).height);
            double h = 28/static_cast<double>(rectan.at(i).width);
			places.push_back(rectan.at(i).x);
			std::cout << l << " " << h << std::endl;
            cv::resize(rez.at(rez.size()-1), rez.at(rez.size()-1), cv::Size(), h, l);
		}
	}
	
	std::cout << "sorting" << std::endl;
	std::cout << places.size() << std::endl;
	if(places.size() > 1) {
		do {
			double mini = places.at(0);
			int id = 0;
			for(unsigned int j=0;j<places.size();j++) {
				if(mini > places.at(j)) {
					mini = places.at(j);
					id = j;
				}
			}
			rez2.push_back(rez.at(id));
            res.push_back(ToQImage(rez.at(id), true));
			if(m_print) {CPI2.ImgShow(rez2.at(rez2.size()-1));}
			rez.erase(rez.begin()+id);
			places.erase(places.begin()+id);
		} while(places.size() > 0);
	} else if(places.size() == 1) {
		rez2.push_back(rez.at(0));
        res.push_back(ToQImage(rez.at(0), true));
		if(m_print) {CPI2.ImgShow(rez2.at(0));}
	} else {
		std::cout << "no pattern found" << std::endl;
	}	
	if(m_print) {CPI2.ImgShow(imgDraw);}

    std::cout << "done !" << std::endl;
    return res;
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

QImage TF::ToQImage(cv::Mat img, bool format) {
    if(format) {
        QImage imgIn = QImage((uchar*) img.data, img.cols, img.rows, img.step, QImage::Format_Grayscale8);
        return imgIn;
    } else {
        QImage imgIn = QImage((uchar*) img.data, img.cols, img.rows, img.step, QImage::Format_RGB888);
        return imgIn;
    }
}

QImage TF::TakePhoto() {
    return ToQImage(CPI2.TakePhoto(), false);
}
