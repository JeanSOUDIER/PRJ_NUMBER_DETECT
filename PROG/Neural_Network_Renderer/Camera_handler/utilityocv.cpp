#include "utilityocv.h"

namespace UtilityOCV {


QImage toQImage(const cv::Mat &image , const QImage::Format &format){

//  QImage::Format_Grayscale8 &&  QImage::Format_RGB888

    return QImage(image.data, image.cols, image.rows, image.step, format);
}


QPixmap toQPixmap(const cv::Mat &image , const QImage::Format &format){
    return QPixmap::fromImage(toQImage(image , format));
}

///NEEDS DEBUG
QImage histogramImage(const cv::Mat &grayscale_image){

    constexpr int hist_height = 256;

    cv::Mat hist;
    cv::Mat3b hist_image = cv::Mat3b::zeros(hist_height, 256);

    const std::vector<int> channels = {0};
    const std::vector<int> histSize = std::vector<int>({256});
    const std::vector<float> ranges = std::vector<float>({0.f , 256.f});

    cv::calcHist(grayscale_image , channels , cv::Mat() ,
                 hist ,  histSize , ranges ,
                 false);

    double max_val=0;
    cv::minMaxLoc(hist, 0, &max_val);


    unsigned int maxi = 0;
    for(int b = 0; b < 256; b++) {
        const unsigned int height = cvRound(hist.at<float>(b)*hist_height/max_val);
        if(maxi < height) {maxi = height;}
        cv::line(
                 hist_image,
                 cv::Point(b, hist_height-height),
                 cv::Point(b, hist_height),
                 cv::Scalar::all(255)
                );
    }

    return toQImage(hist_image , QImage::Format::Format_Grayscale8);
}

QPixmap histogramPixmap(const cv::Mat &grayscale_image){
    return QPixmap::fromImage(histogramImage(grayscale_image));
}


cv::Mat resize(const cv::Mat &image ,
               const uint64_t &width , const uint64_t &height ,
               const cv::InterpolationFlags &interpolation_flag){

    cv::Mat result;

    cv::resize(image , result , cv::Size(),
               static_cast<double>(width)/image.cols , static_cast<double>(height)/image.rows ,
               interpolation_flag);

    return result;
}

cv::Mat invert(const cv::Mat &grayscale_image){
    cv::Mat result;
    cv::bitwise_not(grayscale_image , result);
    return result;
}

cv::Mat BGRtoGray(const cv::Mat &coloured_image){
    cv::Mat result;
    cv::cvtColor(coloured_image, result, cv::COLOR_BGR2GRAY);
    return result;
}

cv::Mat RGBtoGray(const cv::Mat &coloured_image){
    cv::Mat result;
    cv::cvtColor(coloured_image, result, cv::COLOR_RGB2GRAY);
    return result;
}


cv::Mat threshold(const cv::Mat &grayscale_image , const double &thr , const int &type){

    cv::Mat result;
    cv::threshold (grayscale_image , result , thr , 255 , type);
    return result;

}


std::vector<cv::Mat> regionprops(const cv::Mat &original_image , const cv::Mat &gray_image , const cv::Mat &bw_image ,
                                 cv::Mat &output_image,
                                 const uint64_t &min_surface, const uint64_t &max_surface,
                                 const SOURCE &source_output_image, const SOURCE &source_vector,
                                 const SORT &sort,
                                 const bool &allow_containment){

    switch(source_output_image){

        case(SOURCE::ORIGINAL):{output_image = original_image.clone(); break;}
        case(SOURCE::GRAY):{output_image = gray_image.clone(); break;}
        case(SOURCE::BW):{output_image = bw_image.clone(); break;}
        case(SOURCE::INV_BW):{output_image = UtilityOCV::invert(bw_image); break;}
        default:{output_image = original_image.clone(); break;}

    }

    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(bw_image , contours , cv::RETR_TREE , cv::CHAIN_APPROX_SIMPLE);

    std::vector<cv::Rect> rectangles;
    rectangles.reserve(contours.size());

    for(const std::vector<cv::Point> &area : contours){

        const cv::Rect surface = cv::boundingRect(area);

        if((static_cast<unsigned>(surface.area()) > min_surface) && (static_cast<unsigned>(surface.area()) < max_surface)){
            rectangles.push_back(cv::boundingRect(area));
        }
    }




    if(rectangles.empty()){return std::vector<cv::Mat>();}




    ///If this flag is set, rectangles inside other ones are not taken in count
    if(!allow_containment){

        rectangles.erase(
                    std::remove_if(rectangles.begin() , rectangles.end() , [rectangles](const cv::Rect &rectangle){

            bool isContained = false;

            for(auto iterator = rectangles.begin() ; (iterator != rectangles.end()) && !isContained ; ++iterator){

                if(*iterator == rectangle){continue;}
                if((rectangle & *iterator) == rectangle){
                    isContained = true;
                }

            }
            return isContained;

        })
                    , rectangles.end()
                    );

    }

    switch(sort){

    case(SORT::X):{
        std::sort(rectangles.begin() , rectangles.end() ,
                  [](const cv::Rect &a , const cv::Rect &b){return (a.x < b.x);}
        );
        break;
    }

    case(SORT::Y):{
        std::sort(rectangles.begin() , rectangles.end() ,
                  [](const cv::Rect &a , const cv::Rect &b){return (a.y < b.y);}
        );
        break;
    }

    default:{

        std::cerr << "UtilityOCV::regionprops\nInvalid UtilityOCV::SORT argument." << std::endl;
        return std::vector<cv::Mat>();
    };

    }


    std::vector<cv::Mat> result;
    result.reserve(rectangles.size());

    switch(source_vector){

        case(SOURCE::ORIGINAL):{

            for(const cv::Rect &rectangle : rectangles){
                result.push_back(original_image(rectangle));
                cv::rectangle(output_image , rectangle , {168 , 38 , 131} , 4); //<-- RGB
            }
            break;
        }

        case(SOURCE::GRAY):{

            for(const cv::Rect &rectangle : rectangles){
                result.push_back(gray_image(rectangle));
                cv::rectangle(output_image , rectangle , {168 , 38 , 131} , 4); //<-- RGB
            }
            break;
        }

        case(SOURCE::BW):{

            for(const cv::Rect &rectangle : rectangles){
                result.push_back(bw_image(rectangle));
                cv::rectangle(output_image , rectangle , {168 , 38 , 131} , 4); //<-- RGB
            }
            break;
        }


        case(SOURCE::INV_BW):{

            const cv::Mat inv_bw_image = UtilityOCV::invert(bw_image);

            for(const cv::Rect &rectangle : rectangles){
                result.push_back(inv_bw_image(rectangle));
                cv::rectangle(output_image , rectangle , {168 , 38 , 131} , 4); //<-- RGB
            }
            break;
        }

        default:{
            for(const cv::Rect &rectangle : rectangles){
                result.push_back(original_image(rectangle));
                cv::rectangle(output_image , rectangle , {168 , 38 , 131} , 4); //<-- RGB
            }
            break;
        }

    }


    return result;

}


ste::Matrix<uchar> toSteMatrix_UC(const cv::Mat &cvMat){

    std::vector<std::vector<uchar>> result;
    result.reserve(cvMat.rows);

    for(int row = 0 ; row < cvMat.rows ; row++){
        const uchar* row_data = cvMat.ptr<uchar>(row);
        result.push_back(std::vector<uchar>(row_data , row_data + cvMat.cols));
    }

    return ste::Matrix<uchar>(result);
}


ste::Matrix<float> toSteMatrix_F(const cv::Mat &cvMat){

    const ste::Matrix<uchar> uchar_mat = UtilityOCV::toSteMatrix_UC(cvMat);
    std::vector<std::vector<float>> result;
    result.reserve(uchar_mat.rows());


    for(const std::vector<uchar> &row : uchar_mat.toVector()){
        result.push_back(std::vector<float>(row.begin() , row.end()));
    }

    return ste::Matrix<float>(result);

}

ste::Matrix<double> toSteMatrix_D(const cv::Mat &cvMat){

    const ste::Matrix<uchar> uchar_mat = UtilityOCV::toSteMatrix_UC(cvMat);
    std::vector<std::vector<double>> result;
    result.reserve(uchar_mat.rows());

    for(const std::vector<uchar> &row : uchar_mat.toVector()){
        result.push_back(std::vector<double>(row.begin() , row.end()));
    }

    return ste::Matrix<double>(result);

}


} //namespace UtilityOCV

