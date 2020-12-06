#include "ICP.hpp"

ICP::ICP()
	: ICP(1e-8,400) {} //Defaults values

ICP::ICP(const double error, const unsigned int MaxIter) {
	m_error = error;
	m_maxIter = MaxIter;
}

ICP::~ICP() {}

std::vector<double> ICP::GetPos(const std::vector<std::vector<double>> &DynData, const std::vector<std::vector<double>> &StaData) {
	bool m_satur = false;
	// ---- Init ---- //
	std::vector<std::vector<double>> model = {std::vector<double>() , std::vector<double>()};
	std::vector<std::vector<double>> data = {std::vector<double>() , std::vector<double>()};
	unsigned int len1 = 0, len2 = 0;
	for(unsigned int i=0;i<StaData.at(0).size();i++) { // Assume that the clouds come with "std::inf" when the point is out of range of the lidar
        if(!std::isinf(StaData.at(0).at(i))) {len1++;} // Computing the number of "std::inf" in the 2 clouds
        if(!std::isinf(DynData.at(0).at(i))) {len2++;}
	}
	if(len1 > len2) { // Taking the higher value so if the clouds come with size(360x2) and have for exemple 6 and 10 "std::inf" the resize both clouds to size(350x2)
	    len1 = len2;
	}
	unsigned int cpt = 0, cpt2 = 0;
    while(cpt < len1) {
        if(!std::isinf(StaData.at(0).at(cpt2))) { //resizing cloud 1
            model.at(0).push_back(StaData.at(0).at(cpt2));
            model.at(1).push_back(StaData.at(1).at(cpt2));
            cpt++;
        }
        cpt2++;
    }
    cpt = 0, cpt2 = 0;
    while(cpt < len1) {
        if(!std::isinf(DynData.at(0).at(cpt2))) { //resizing cloud 2
            data.at(0).push_back(DynData.at(0).at(cpt2));
            data.at(1).push_back(DynData.at(1).at(cpt2));
            cpt++;
        }
        cpt2++;
    }
	std::vector<double> wghs(data.at(0).size()); // Vector of weights
	ste::Matrix<double> TR = ste::Matrix<double>({{0, 1},{1, 0}}); // Rotation matrix
	ste::Matrix<double> TT(2,1); // Translation matrix
	TT.at(0,0) = 0;
	TT.at(1,0) = 0;
	long double res=9e99;
	long double oldres;
	// ---- ICP loop ---- //
	for(unsigned int iter=0;iter<m_maxIter;iter++) {
		// ---- Find points ---- //
		//std::cout << iter << " " << std::fabs(oldres-res) << std::endl;
		oldres = res;
		std::vector<std::vector<double>> temp = nearestNeighbor(data,model);
		// ---- Find TF ---- //
		std::vector<double> vi = temp.at(0);
		std::vector<double> resid = temp.at(1);
		// ---- Compute error ---- //
		double Rob = 1.9*median(resid);
		double maxResid = *max_element(resid.begin(), resid.end());
		if(Rob < 1e-6*maxResid) {
			Rob = 0.3*maxResid;
		} else {
			if(!maxResid) {Rob = 1;}
		}
		res = 0;
		unsigned int ResPt = 0;
		for(unsigned int i=0;i<resid.size();i++) {
            if(resid.at(i) < 1.5*Rob) {
                res += resid.at(i)*resid.at(i);
                ResPt++;
            }
		}
		res /= ResPt;
		// ---- Cauchy error ---- //
		Rob *= 4.3040;
		double suWghs = 0;
		for(unsigned int i=0;i<wghs.size();i++) {
			wghs.at(i) = 1/(1+(resid.at(i)/Rob)*(resid.at(i)/Rob));
			suWghs += wghs.at(i);
		}
		// ---- App TF ---- //
		std::vector<double> med(2);
		med.at(0) = pdt(data.at(0),wghs)/suWghs;
		med.at(1) = pdt(data.at(1),wghs)/suWghs;
		std::vector<double> mem = {0, 0};
		for(unsigned int i=0;i<wghs.size();i++) {
			mem.at(0) += model.at(0).at(vi.at(i))*wghs.at(i);
			mem.at(1) += model.at(1).at(vi.at(i))*wghs.at(i);
		}
		mem.at(0) /= suWghs;
		mem.at(1) /= suWghs;
		ste::Matrix<double> C({{0, 0}, {0, 0}});
		std::vector<std::vector<double>> Ctemp(2);
		for(unsigned int i=0;i<wghs.size();i++) {
            Ctemp.at(0).push_back(data.at(0).at(i)*wghs.at(i));
            Ctemp.at(1).push_back(data.at(1).at(i)*wghs.at(i));
		}
		for(unsigned int i=0;i<Ctemp.at(0).size();i++) {
			C.at(0,0) += Ctemp.at(0).at(i)*model.at(0).at(vi.at(i));
			C.at(0,1) += Ctemp.at(0).at(i)*model.at(1).at(vi.at(i));
			C.at(1,0) += Ctemp.at(1).at(i)*model.at(0).at(vi.at(i));
			C.at(1,1) += Ctemp.at(1).at(i)*model.at(1).at(vi.at(i));
		}
		C.at(0,0) -= suWghs*med.at(0)*mem.at(0);
		C.at(0,1) -= suWghs*med.at(0)*mem.at(1);
		C.at(1,0) -= suWghs*med.at(1)*mem.at(0);
		C.at(1,1) -= suWghs*med.at(1)*mem.at(1);
		std::vector<ste::Matrix<double>> UV = svd(C);
        ste::Matrix<double> Ri = {UV.at(0)*UV.at(1).transpose()};
        Ri = Ri.transpose();
        if (Ri.det() < 0) {
        	UV.at(1).at(0,1) = -UV.at(1).at(0,1);
        	UV.at(1).at(1,1) = -UV.at(1).at(1,1);
            Ri = UV.at(0)*UV.at(1).transpose();
        }
        std::vector<double> Ti(2);
        Ti.at(0) = mem.at(0)-(Ri.at(0,0)*med.at(0)+Ri.at(0,1)*med.at(1));
        Ti.at(1) = mem.at(1)-(Ri.at(1,0)*med.at(0)+Ri.at(1,1)*med.at(1));
        std::vector<std::vector<double>> dataN1(2);
        ste::Matrix<double> Dat(data);
        Dat = Ri*Dat;
        data = Dat.toVector();

        for(unsigned int i=0;i<data.at(0).size();i++) {
        	data.at(0).at(i) = data.at(0).at(i)+Ti.at(0);
        	data.at(1).at(i) = data.at(1).at(i)+Ti.at(1);
        }
        // ---- Update passage matrix ---- //
    	TR = Ri*TR;
   		TT = Ri*TT;
   		TT.at(0,0) += Ti.at(0);
   		TT.at(1,0) += Ti.at(1);
   		// ---- Compute the new error ---- //
    	if(iter >= 5) {
        	if(std::fabs(oldres-res) < m_error) {
           		break;
        	}
    	}
    	if(iter == m_maxIter-1) {
    		std::cout << "sat ICP" << std::endl; // Print that we arrive to maxIter and haven't found a satisfaying error
			m_satur = true;
    	}
	}
	std::vector<double> result(3);
	if(m_satur) {
		result.at(0) = 0; // Don't use this points
		result.at(1) = 0;
		result.at(2) = 0;
	} else {
		result.at(0) = TT.at(0,0);
		result.at(1) = TT.at(1,0);
		result.at(2) = std::acos(TR.at(0,1));
	}
	return result;
}

std::vector<ste::Matrix<double>> ICP::svd(const ste::Matrix<double> &Mat) {
    // https://lucidar.me/fr/mathematics/singular-value-decomposition-of-a-2x2-matrix/
	const double a = Mat.at(0,0);
    const double b = Mat.at(0,1);
    const double c = Mat.at(1,0);
    const double d = Mat.at(1,1);
	const double tet = std::atan2(2*a*b+2*c*d,a*a+b*b-c*c-d*d)/2;
	// Problem of sign
	const ste::Matrix<double> U({{-std::cos(tet), -std::sin(tet)}, {-std::sin(tet), std::cos(tet)}});
	const double phi = std::atan2(2*a*b+2*c*d,a*a-b*b+c*c-d*d)/2;
	const double s11 = (a*std::cos(tet)+c*std::sin(tet))*std::cos(phi) + (b*std::cos(tet)+d*std::sin(tet))*std::sin(phi);
	const double s22 = (a*std::sin(tet)-c*std::cos(tet))*std::sin(phi) + (-b*std::sin(tet)+d*std::cos(tet))*std::cos(phi);
	const ste::Matrix<double> V({{-s11/std::abs(s11)*std::cos(phi), -s22/std::abs(s22)*std::sin(phi)}, {-s11/std::abs(s11)*std::sin(phi), s22/std::abs(s22)*std::cos(phi)}});
	std::vector<ste::Matrix<double>> res = {U,V};
	return res;
}

double ICP::median(std::vector<double> vec) {
        typedef std::vector<double>::size_type vec_sz;
        vec_sz size = vec.size();
        if(size == 0) {
        	throw std::domain_error("median of an empty vector");
        }
        std::sort(vec.begin(), vec.end());
        vec_sz mid = size/2;
        return size % 2 == 0 ? (vec[mid] + vec[mid-1]) / 2 : vec[mid];
}

std::vector<std::vector<double>> ICP::nearestNeighbor(const std::vector<std::vector<double>> &model, const std::vector<std::vector<double>> &data) {
	std::vector<double> id(model.at(0).size());
	std::vector<double> dist(model.at(0).size());
	double d;
	for(unsigned int i=0;i<model.at(0).size();i++) {
		dist.at(i) = distEucl(data.at(0).at(0), data.at(1).at(0), model.at(0).at(i), model.at(1).at(i));
		id.at(i) = 0;
		for(unsigned int j=1;j<data.at(0).size();j++) {
            d = distEucl(data.at(0).at(j), data.at(1).at(j), model.at(0).at(i), model.at(1).at(i));
			if(d < dist.at(i)) {
				dist.at(i) = d;
				id.at(i) = j;
			}
		}
	}
	std::vector<std::vector<double>> res = {id, dist};
	return res;
}

double ICP::distEucl(const double &a1, const double &a2, double const &b1, const double &b2) {
    return std::sqrt((a1-b1)*(a1-b1)+(a2-b2)*(a2-b2));
}

double ICP::pdt(const std::vector<double> &a, const std::vector<double> &b) {
    double res = 0;
    for(unsigned int i=0;i<a.size();i++) {
        res += a.at(i)*b.at(i);
    }
    return res;
}

bool ICP::SaveImg(const ste::Matrix<double> &Map, const std::string &name) {
	return Utility::writeCSV(name,Map.toVector(),";");
}
