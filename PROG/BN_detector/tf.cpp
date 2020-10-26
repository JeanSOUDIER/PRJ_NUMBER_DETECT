#include "tf.hpp"

TF::TF() {}

TF::~TF() {}

ste::Matrix<double> TF::ToNormalize(ste::Matrix<double> img) {
	std::vector<unsigned int> len = {img.size()};
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
			img.at(i,j) = img.at(i,j)/maxi;
		}
	}
	return img;
}

ste::Matrix<bool> TF::ToThreshold(ste::Matrix<double> img, double level) {
	std::vector<unsigned int> len = {img.size()};
	ste::Matrix<bool> res(len.at(0),len.at(1));
	for(unsigned int i=0;i<len.at(0);i++) {
		for(unsigned int j=0;j<len.at(1);j++) {
			if(level < img.at(i,j)) {
				res.at(i,j) = false;
			} else {
				res.at(i,j) = true;
			}
		}
	}
	PrintPPM(res, "Threshold");
	return res;
}

ste::Matrix<bool> TF::ToErosion(ste::Matrix<bool> img) {
	std::vector<unsigned int> len = {img.size()};
	ste::Matrix<bool> res(len.at(0),len.at(1));
	res.fill(len.at(0), len.at(1), false);
	for(unsigned int i=1;i<len.at(0)-1;i++) {
		for(unsigned int j=1;j<len.at(1)-1;j++) {
			if(img.at(i-1,j) && img.at(i,j-1) && img.at(i,j) && img.at(i,j+1) && img.at(i+1,j)) {
				img.at(i,j) = true;
			}
		}
	}
	PrintPPM(res, "Erosion");
	return res;
}

ste::Matrix<bool> TF::ToErosion(ste::Matrix<bool> img, ste::Matrix<bool> Patern) {
	std::vector<unsigned int> len = {img.size()};
	std::vector<unsigned int> lenP = {Patern.size()};
	ste::Matrix<bool> res(len.at(0),len.at(1));
	res.fill(len.at(0), len.at(1), false);
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
				if(temp) {res.at(i,j) = true;}
			}
		}
	}
	PrintPPM(res, "ErosionG");
	return res;
}

std::vector<std::vector<unsigned int>> TF::GetEdges(ste::Matrix<bool> img) {
	std::vector<unsigned int> len = {img.size()};
	std::vector<std::vector<unsigned int> res(2);
	bool temp;
	for(unsigned int i=0;i<len.at(0);i++) {
		temp = false;
		for(unsigned int j=0;j<len.at(1);j++) {
			temp &= img.at(i,j);
		}
		if(temp) {
			res.at(0).push_back(i);
			res.at(1).push_back(j);
			std::cout << i << " " << j << std::endl;
			break;
		}
	}
	for(unsigned int i=len.at(0);i>=0;i--) {
		temp = false;
		for(unsigned int j=0;j<len.at(1);j++) {
			temp &= img.at(i,j);
		}
		if(temp) {
			res.at(0).push_back(i);
			res.at(1).push_back(j);
			std::cout << i << " " << j << std::endl;
			break;
		}
	}
	for(unsigned int i=0;i<len.at(1);i++) {
		temp = false;
		for(unsigned int j=0;j<len.at(0);j++) {
			temp &= img.at(j,i);
		}
		if(temp) {
			res.at(0).push_back(i);
			res.at(1).push_back(j);
			std::cout << i << " " << j << std::endl;
			break;
		}
	}
	for(unsigned int i=len.at(1);i>=0;i--) {
		temp = false;
		for(unsigned int j=0;j<len.at(0);j++) {
			temp &= img.at(j,i);
		}
		if(temp) {
			res.at(0).push_back(i);
			res.at(1).push_back(j);
			std::cout << i << " " << j << std::endl;
			break;
		}
	}
	return res;
}

std::vector<double> TF::GetPassMAt(std::vector<std::vector<unsigned int>> edges) {
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
	std::vector<unsigned int> len = {img.size()};
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
	Xscale = h/len.at(0);
	Yscale = l/len.at(1);
	//Mat TF rigid
	ste::Matrix<double> res(len.at(0),len.at(1));
	for(unsigned int i=0;i<len.at(0);i++) {
		for(unsigned int j=0;j<len.at(1);j++) {
			ste::Matrix<double> coord = {{i, j, 1}};
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

void PrintPPM(ste::Matrix<double> img, std::string path) {
	img = ToNormalize(img);
	path += ".ppm";
	ofstream ofs(path, ios_base::out | ios_base::binary);
	std::vector<unsigned int> len = {img.size()};
	ofs << "P6" << endl << len.at(0) << ' ' << len.at(1) << endl << "255" << endl;
    for(unsigned int i=0;i<len.at(0);i++) {
		for(unsigned int j=0;j<len.at(1);j++) {
        	ofs << static_cast<char>(img.at(i,j)*256) << static_cast<char>(img.at(i,j)*256) << static_cast<char>(img.at(i,j)*256);
        }
	}
    ofs.close();
}

void PrintPPM(ste::Matrix<bool> img, std::string path) {
	path += ".ppm";
	ofstream ofs(path, ios_base::out | ios_base::binary);
	std::vector<unsigned int> len = {img.size()};
	ofs << "P6" << endl << len.at(0) << ' ' << len.at(1) << endl << "255" << endl;
    for(unsigned int i=0;i<len.at(0);i++) {
		for(unsigned int j=0;j<len.at(1);j++) {
        	ofs << static_cast<char>(img.at(i,j) ? 0 : 256) << static_cast<char>(img.at(i,j) ? 0 : 256) << static_cast<char>(img.at(i,j) ? 0 : 256);
        }
	}
    ofs.close();
}
