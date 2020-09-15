#ifndef ADJUST_IMG_H
#define ADJUST_IMG_H

#include <iostream> 
#include <boost/gil/extension/io/jpeg_io.hpp> 
#include <string.h>

#define SEUIL_H 1000
#define SEUIL_B 800

namespace gil = boost::gil; 

struct Point {
	int px, py;
}

struct Rectangle{
	Point Start, End;
};

class AdjustImg {
	public:
		AdjustImg(std::string name);
		Rectangle DetectEsges(void);
		gil::rgb8_image_t Adjust(Rectangle rect);
	private:
		std::string m_name;
		gil::rgb8_image_t m_img;
};

#endif //ADJUST_IMG_H