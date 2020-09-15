#include <iostream> 
#include <boost/gil/extension/io/jpeg_io.hpp> 
namespace gil = boost::gil; 
int main() 
{ 
 gil::rgb8_image_t img; 
 gil::jpeg_read_image("demo.jpg", img); 
 std::cout << "Read complete, got an image " << img.width() 
           << " by " << img.height() << " pixels\n"; 
 gil::rgb8_pixel_t px = *const_view(img).at(5, 10); 
 std::cout << "The pixel at 5,10 is " 
           << (int)px[0] << ',' 
           << (int)px[1] << ',' 
           << (int)px[2] << '\n'; 
} 





AdjustImg::AdjustImg(std::string name) {
	gil::jpeg_read_image("demo.jpg", m_img);
	std::cout << "Read complete, got an image " << m_img.width() << " by " << m_img.height() << " pixels\n";
}

Rectangle DetectEsges(void) {
	bool state = false;
	Rectangle rect;
	for(int i=0;i<m_img.width();i++) {
		int Sum = 0;
		for(int j=0;j<m_img.height();j++) {
			gil::rgb8_pixel_t px = *const_view(img).at(i, j);
			Sum += px[0]+px[1]+px[2];
		}
		if(Sum > SEUIL_H && !state) {
			rect.Start.px = i;
			state = true;
		}
		if(Sum < SEUIL_L && state) {
			rect.Start.py = i;
			break;
		}
	}
	for(int i=0;i<m_img.height();i++) {
		int Sum = 0;
		for(int j=0;j<m_img.width();j++) {
			gil::rgb8_pixel_t px = *const_view(img).at(j, i);
			Sum += px[0]+px[1]+px[2];
		}
		if(Sum > SEUIL_H && !state) {
			rect.End.px = i;
			state = true;
		}
		if(Sum < SEUIL_L && state) {
			rect.End.py = i;
			return rect;
		}
	}
}

gil::rgb8_image_t Adjust(Rectangle rect) {

}
