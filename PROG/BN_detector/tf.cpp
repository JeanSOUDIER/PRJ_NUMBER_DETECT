#include "tf.hpp"

TF::TF() {}

TF::~TF() {}

cv::Mat TF::ToGrayScale(cv::Mat img) {

}

cv::Mat TF::ToThreshold(cv::Mat img, double level) {
	
}

cv::Mat TF::ToErosion(cv::Mat img) {
	
}

cv::Mat TF::ToErosion(cv::Mat img, cv::Mat Patern) {
	
}

std::vector<int> TF::GetEdges(cv::Mat img) {
	
}

std::vector<double> TF::GetPassMAt(std::vector<double> edges) {

}

cv::Mat TF::toTFrigid(cv::Mat img, std::vector<double> PassMat) {
	
}
