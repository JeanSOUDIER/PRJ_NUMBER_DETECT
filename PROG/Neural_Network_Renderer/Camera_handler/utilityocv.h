#ifndef UTILITYOCV_H
#define UTILITYOCV_H

#include "../NeuralNetwork/Matrix/Matrix.hpp"
#include "../NeuralNetwork/MatrixNN_global.h"

#include "../NeuralNetwork/SequenceHandler_STL/utility.h"

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>

#include <opencv2/core.hpp>
#include <opencv2/core/utility.hpp>

#include <QImage>
#include <QPixmap>

namespace UtilityOCV {

    enum class SOURCE{
        ORIGINAL ,
        GRAY ,
        BW ,
        INV_BW
    };

    enum class SORT{
        X,
        Y
    };


    QImage toQImage(const cv::Mat &image , const QImage::Format &format);
    QPixmap toQPixmap(const cv::Mat &image , const QImage::Format &format);

    ///NEEDS DEBUG
    QImage histogramImage(const cv::Mat &grayscale_image);
    QPixmap histogramPixmap(const cv::Mat &grayscale_image);

    cv::Mat resize(const cv::Mat &image ,
                   const uint64_t &width , const uint64_t &height,
                   const cv::InterpolationFlags &interpolation_flag  = cv::InterpolationFlags::INTER_LINEAR);

    cv::Mat invert(const cv::Mat &grayscale_image);

    cv::Mat BGRtoGray(const cv::Mat &coloured_image);
    cv::Mat RGBtoGray(const cv::Mat &coloured_image);

    cv::Mat threshold(const cv::Mat &grayscale_image , const double &thr = 127., const int &type = 3);

    std::vector<cv::Mat> regionprops(const cv::Mat &original_image , const cv::Mat &gray_image,
                                     const cv::Mat &bw_image ,
                                     cv::Mat &output_image ,
                                     const uint64_t &min_surface = 1, const uint64_t &max_surface = 1280*720,
                                     const SOURCE &source_output_image = SOURCE::ORIGINAL ,
                                     const SOURCE &source_vector = SOURCE::ORIGINAL,
                                     const SORT &sort = SORT::X ,
                                     const bool &allow_containment = false);

    ste::Matrix<uchar> toSteMatrix_UC(const cv::Mat &cvMat);
    ste::Matrix<float> toSteMatrix_F(const cv::Mat &cvMat);
    ste::Matrix<double> toSteMatrix_D(const cv::Mat &cvMat);
/*
    template <typename T>
    ste::Matrix<T> toSteMatrix(const cv::Mat &cvMat){
        std::vector<std::vector<T>> result;
        result.reserve(cvMat.rows);

        for(int row = 0 ; row < cvMat.rows ; row++){
            const T* row_data = cvMat.ptr<T>(row);
            result.push_back(std::vector<T>(row_data , row_data + cvMat.cols));
        }

        return ste::Matrix<T>(result);

    }
*/
}

#endif // UTILITYOCV_H
