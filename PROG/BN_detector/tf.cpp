#include "tf.hpp"

TF::TF() {}

TF::~TF() {}

ste::Matrix<double> TF::ToProcessed(std::vector<ste::Matrix<char>> img, double level) {
	std::cout << "process RGB" << std::endl;
	return ToProcessed(ToGrayLevel(img), level);
}

ste::Matrix<double> TF::ToProcessed(ste::Matrix<double> img, double level) {
	std::cout << "process" << std::endl;
	return ToTF(img, GetPassMAt(GetEdges(ToErosion(ToInvertion(ToThreshold(ToNormalize(img),level))))));
}

ste::Matrix<double> TF::ToGrayLevel(std::vector<ste::Matrix<char>> img) {
	std::cout << "gray" << std::endl;
	std::vector<unsigned long long> len = img.at(0).size();
	ste::Matrix<double> res(len.at(0),len.at(1));
	for(unsigned int i=0;i<len.at(0);i++) {
		for(unsigned int j=0;j<len.at(1);j++) {
			res.at(i,j) = 0.2989*img.at(0).at(i,j)+0.5870*img.at(1).at(i,j)+0.1140*img.at(2).at(i,j);
		}
	}
	PrintPPM(res, "Gray");
	return res;
}

ste::Matrix<double> TF::ToNormalize(ste::Matrix<double> img) {
	std::cout << "norm" << std::endl;
	std::vector<unsigned long long> len = {img.size()};
	double maxi = 0;
	for(unsigned int i=0;i<len.at(0);i++) {
		for(unsigned int j=0;j<len.at(1);j++) {
			if(maxi < img.at(i,j)) {
				maxi = img.at(i,j);
			}
		}
	}
	std::cout << "m=" << maxi << std::endl;
	for(unsigned int i=0;i<len.at(0);i++) {
		for(unsigned int j=0;j<len.at(1);j++) {
			img.at(i,j) = 256*img.at(i,j)/maxi;
		}
	}
	//PrintPPM(img, "Normalized");
	return img;
}

ste::Matrix<unsigned char> TF::ToThreshold(ste::Matrix<double> img, double level) {
	std::cout << "level" << std::endl;
	std::vector<unsigned long long> len = {img.size()};
	ste::Matrix<unsigned char> res(len.at(0),len.at(1));
	for(unsigned int i=0;i<len.at(0);i++) {
		for(unsigned int j=0;j<len.at(1);j++) {
			if(256*level < img.at(i,j)) {
				res.at(i,j) = 255;
			} else {
				res.at(i,j) = 0;
			}
		}
	}
	PrintPPM(res, "Threshold");
	return res;
}

ste::Matrix<unsigned char> TF::ToInvertion(ste::Matrix<unsigned char> img) {
    std::vector<unsigned long long> len = {img.size()};
	ste::Matrix<unsigned char> res(len.at(0),len.at(1));
	for(unsigned int i=1;i<len.at(0)-1;i++) {
		for(unsigned int j=1;j<len.at(1)-1;j++) {
			if(img.at(i,j) == 255) {
                res.at(i,j) = 0;
			} else {
                res.at(i,j) = 255;
			}
		}
	}
	PrintPPM(res, "Invert");
	return res;
}

ste::Matrix<unsigned char> TF::ToErosion(ste::Matrix<unsigned char> img) {
	std::cout << "erosion" << std::endl;
	std::vector<unsigned long long> len = {img.size()};
	ste::Matrix<unsigned char> res(len.at(0),len.at(1));
	res.fill(len.at(0), len.at(1), 0);
	for(unsigned int i=1;i<len.at(0)-1;i++) {
		for(unsigned int j=1;j<len.at(1)-1;j++) {
			if(img.at(i-1,j) == 255 && img.at(i,j-1) == 255 && img.at(i,j) == 255 && img.at(i,j+1) == 255 && img.at(i+1,j) == 255) {
				res.at(i,j) = 255;
			}
		}
	}
	PrintPPM(res, "Erosion");
	return res;
}

ste::Matrix<unsigned char> TF::ToErosion(ste::Matrix<unsigned char> img, ste::Matrix<unsigned char> Patern) {
	std::cout << "erosion G" << std::endl;
	std::vector<unsigned long long> len = {img.size()};
	std::vector<unsigned long long> lenP = {Patern.size()};
	ste::Matrix<unsigned char> res(len.at(0),len.at(1));
	res.fill(len.at(0), len.at(1), 0);
	if(lenP.at(0) > len.at(0) || lenP.at(1) > len.at(1)) {
		std::cout << "error patern too big" << std::endl;
	} else if(lenP.at(0) != lenP.at(1)) {
		std::cout << "error patern not squared" << std::endl;
	} else if(lenP.at(0)%2 != 1) {
		std::cout << "error patern not odd" << std::endl;
	} else {
		unsigned int l = lenP.at(0)/2;
		bool temp;
		for(unsigned int i=l;i<len.at(0)-l;i++) {
			for(unsigned int j=l;j<len.at(1)-l;j++) {
				temp = false;
				for(unsigned int k=0;k<=lenP.at(0);k++) {
					for(unsigned int m=0;m<=lenP.at(0);m++) {
						if(Patern.at(k,m)) {
							temp &= img.at(i+k-l,j+m-l);
						}
					}
				}
				if(temp) {res.at(i,j) = 255;}
			}
		}
	}
	PrintPPM(res, "ErosionG");
	return res;
}

std::vector<std::vector<unsigned int>> TF::GetEdges(ste::Matrix<unsigned char> img) {
	std::cout << "edge" << std::endl;
	std::vector<unsigned long long> len = {img.size()};
	std::vector<std::vector<unsigned int>> res(2);
	std::cout << "e1" << std::endl;
	for(int i=0;i<len.at(0);i++) {
		for(int j=0;j<len.at(1);j++) {
			if(img.at(i,j)) {
                res.at(0).push_back(i);
                res.at(1).push_back(j);
                std::cout << i << " " << j << std::endl;
                i = len.at(0);
                j = len.at(1);
			}
		}
	}
	std::cout << "e2" << std::endl;
	for(int i=0;i<len.at(0);i++) {
		for(int j=len.at(1)-1;j>=0;j--) {
			if(img.at(i,j)) {
                res.at(0).push_back(i);
                res.at(1).push_back(j);
                std::cout << i << " " << j << std::endl;
                i = len.at(0);
                j = -1;
			}
		}
	}
	std::cout << "e3" << std::endl;
	for(int i=0;i<len.at(1);i++) {
		for(int j=0;j<len.at(0);j++) {
			if(img.at(j,i)) {
                res.at(0).push_back(i);
                res.at(1).push_back(j);
                std::cout << i << " " << j << std::endl;
                i = len.at(0);
                j = len.at(1);
			}
		}
	}
	std::cout << "e4" << std::endl;
	for(int i=len.at(1)-1;i>=0;i--) {
		for(int j=0;j<len.at(0);j++) {
			if(img.at(j,i)) {
                res.at(0).push_back(i);
                res.at(1).push_back(j);
                std::cout << i << " " << j << std::endl;
                i = -1;
                j = len.at(1);
			}
		}
	}
	return res;
}

std::vector<double> TF::GetPassMAt(std::vector<std::vector<unsigned int>> edges) {
	std::cout << "mat" << std::endl;
	double x0 = edges.at(0).at(0);
	double x1 = edges.at(0).at(1);
	double x2 = edges.at(0).at(2);
	double x3 = edges.at(0).at(3);
	double y0 = edges.at(1).at(0);
	double y1 = edges.at(1).at(1);
	double y2 = edges.at(1).at(2);
	double y3 = edges.at(1).at(3);

	double a1 = (y0-y2)/(x0-x2);
	double a2 = (y1-y3)/(x1-x3);
	std::cout << a1 << " " << a2 << std::endl;
	double a = (a1+a2)/2;
	std::cout << a << std::endl;
	double dist1 = std::sqrt((x0-x3)*(x0-x3)+(y0-y3)*(y0*y3));
	double dist2 = std::sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1*y2));
	double dist3 = std::sqrt((x0-x1)*(x0-x1)+(y0-y1)*(y0*y1));
	double dist4 = std::sqrt((x2-x3)*(x2-x3)+(y2-y3)*(y2*y3));
	double dL = (dist1+dist2)/2;
	double dH = (dist3+dist4)/2;
	if(dL > dH) {
		double temp = dH;
		dH = dL;
		dL = temp;
	}
	std::cout << dL << " " << dH << std::endl;
	double dX1 = std::fabs(x0-x1)+std::min(x0,x1);
	double dX2 = std::fabs(x2-x3)+std::min(x2,x3);
	double dY1 = std::fabs(y0-y1)+std::min(y0,y1);
	double dY2 = std::fabs(y2-y3)+std::min(y2,y3);
	double dX = (dX1+dX2)/2;
	double dY = (dY1+dY2)/2;
	std::cout << dX << " " << dY << std::endl;
	std::vector<double> res = {dX, dY, a, dH, dL};
	return res;
}

ste::Matrix<double> TF::ToTF(ste::Matrix<double> img, std::vector<double> VectTF) {
	std::cout << "tf" << std::endl;
	std::vector<unsigned long long> len = {img.size()};
	double x = VectTF.at(0);
	double y = VectTF.at(1);
	double a = VectTF.at(2);
	double h = VectTF.at(3);
	double l = VectTF.at(4);
	//PassMat
	ste::Matrix<double> M(3,3);
	M.at(0,0) = std::cos(a);
	M.at(0,1) = -std::sin(a);
	M.at(0,2) = y*std::sin(a)-x*std::cos(a);
	M.at(1,0) = std::sin(a);
	M.at(1,1) = std::cos(a);
	M.at(1,2) = -x*std::sin(a)-y*std::cos(a);
	M.at(2,0) = 0;
	M.at(2,1) = 0;
	M.at(2,2) = 1;
	double Xscale = h/len.at(0);
	double Yscale = l/len.at(1);
	//Mat TF rigid
	ste::Matrix<double> res(len.at(0),len.at(1));
	for(unsigned int i=0;i<len.at(0);i++) {
		for(unsigned int j=0;j<len.at(1);j++) {
			ste::Matrix<double> coord({{i, j, 1}});
			coord = M*coord.transpose();
			double x3 = coord.at(0,0)*Xscale;
			double y3 = coord.at(1,0)*Yscale;
			double x1 = std::round(x3);
			double x2 = x1+1;
			double y1 = std::round(y3);
			double y2 = y1+1;
			double alpha = (x3-x1)/(x2-x1);
			double beta = (y3-y1)/(y2-y1);
			res.at(i,j) = (1-alpha)*(1-beta)*img.at(x1,y1)+alpha*(1-beta)*img.at(x2,y1)+(1-alpha)*beta*img.at(x1,y2)+alpha*beta*img.at(x2,y2);
		}
	}
	PrintPPM(res, "TF");
	return res;
}

void TF::PrintPPM(std::vector<ste::Matrix<char>> img, std::string path) {
	std::cout << "print " << path << std::endl;
	path += ".ppm";
	std::ofstream ofs(path, std::ios_base::out | std::ios_base::binary);
	std::vector<unsigned long long> len = {img.at(0).size()};
	ofs << "P6" << std::endl << len.at(0) << ' ' << len.at(1) << std::endl << "255" << std::endl;
    for(unsigned int i=0;i<len.at(0);i++) {
		for(unsigned int j=0;j<len.at(1);j++) {
        	ofs << img.at(0).at(i,j) << img.at(1).at(i,j) << img.at(2).at(i,j);
        }
	}
    ofs.close();
}

void TF::PrintPPM(ste::Matrix<double> img, std::string path) {
	std::cout << "print " << path << std::endl;
	img = ToNormalize(img);
	path += ".ppm";
	std::ofstream ofs(path, std::ios_base::out | std::ios_base::binary);
	std::vector<unsigned long long> len = {img.size()};
	ofs << "P6" << std::endl << len.at(0) << ' ' << len.at(1) << std::endl << "255" << std::endl;
    for(unsigned int i=0;i<len.at(0);i++) {
		for(unsigned int j=0;j<len.at(1);j++) {
        	ofs << static_cast<char>(img.at(i,j)) << static_cast<char>(img.at(i,j)) << static_cast<char>(img.at(i,j));
        }
	}
    ofs.close();
}

void TF::PrintPPM(ste::Matrix<unsigned char> img, std::string path) {
	std::cout << "print " << path << std::endl;
	path += ".ppm";
	std::ofstream ofs(path, std::ios_base::out | std::ios_base::binary);
	std::vector<unsigned long long> len = {img.size()};
	ofs << "P6" << std::endl << len.at(0) << ' ' << len.at(1) << std::endl << "255" << std::endl;
    for(unsigned int i=0;i<len.at(0);i++) {
		for(unsigned int j=0;j<len.at(1);j++) {
        	ofs << static_cast<char>(img.at(i,j)) << static_cast<char>(img.at(i,j)) << static_cast<char>(img.at(i,j));
        }
	}
    ofs.close();
}

std::vector<ste::Matrix<char>> TF::ReadPPM(std::string path) {
	std::cout << "read" << std::endl;
	path += ".ppm";
	std::ifstream ifs(path, std::ios_base::in | std::ios_base::binary);
	if(ifs) {
        ifs.seekg (0, ifs.end);
        int length = ifs.tellg();
        ifs.seekg (0, ifs.beg);
        /*char c;
        ifs.get(c); //P
        ifs.get(c); //6
        unsigned long long l1 = 0;
        while(1) {
            ifs.get(c);
            if(c == ' ') {break;}
            l1 = l1<<8+c;
        }
        unsigned long long l2 = 0;
        while(1) {
            ifs.get(c);
            if(c == 255) {break;}
            l1 = l1<<8+c;
        }*/
        unsigned long long l1 = sqrt(length/3), l2 = l1;
        std::cout << l1 << " " << l2 << std::endl;
        ste::Matrix<char> R(l1,l2);
        ste::Matrix<char> G(l1,l2);
        ste::Matrix<char> B(l1,l2);
        std::cout << "3" << std::endl;
        for(unsigned int i=0;i<l1;i++) {
            for(unsigned int j=0;j<l2;j++) {
                ifs.get(R.at(i,j));
                ifs.get(G.at(i,j));
                ifs.get(B.at(i,j));
            }
        }
        ifs.close();
        std::vector<ste::Matrix<char>> res = {R, G, B};
        PrintPPM(res, "RGB");
        return res;
	} else {
	    std::cout << "no file" << std::endl;
	}
}
