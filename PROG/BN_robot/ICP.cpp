#include "ICP.hpp"

ICP::ICP()
	: ICP(1e-8,1000,3500,50,50,2*M_PI) {}

ICP::ICP(const double error, const unsigned int MaxIter, const int mapLen)
	: ICP(error,MaxIter,mapLen,50,50,2*M_PI) {}

ICP::ICP(const double error, const unsigned int MaxIter, const int mapLen, const double MaxX, const double MaxY, const double MaxA) {
	m_max.push_back(MaxX);
	m_max.push_back(MaxY);
	m_max.push_back(MaxA);
	m_error = error;
	m_maxIter = MaxIter;
	m_mapLen = mapLen;
}

ICP::~ICP() {}

std::vector<double> ICP::GetPos(const std::vector<std::vector<double>> DynData, const std::vector<std::vector<double>> StaData) {
	// ---- Init ---- //
	std::vector<std::vector<double>> model = {std::vector<double>() , std::vector<double>()};
	std::vector<std::vector<double>> data = {std::vector<double>() , std::vector<double>()};
	unsigned int len1 = 0, len2 = 0;
	for(unsigned int i=0;i<StaData.at(0).size();i++) {
        if(!std::isinf(StaData.at(0).at(i))) {len1++;}
        if(!std::isinf(DynData.at(0).at(i))) {len2++;}
	}
	if(len1 > len2) {
	    len1 = len2;
	}
	unsigned int cpt = 0, cpt2 = 0;
    while(cpt < len1) {
        if(!std::isinf(StaData.at(0).at(cpt2))) {
            model.at(0).push_back(StaData.at(0).at(cpt2));
            model.at(1).push_back(StaData.at(1).at(cpt2));
            cpt++;
        }
        cpt2++;
    }
    cpt = 0, cpt2 = 0;
    while(cpt < len1) {
        if(!std::isinf(DynData.at(0).at(cpt2))) {
            data.at(0).push_back(DynData.at(0).at(cpt2));
            data.at(1).push_back(DynData.at(1).at(cpt2));
            cpt++;
        }
        cpt2++;
    }
	std::vector<double> wghs(data.at(0).size());
	//std::fill(wghs.begin(), wghs.end(), 1);
	ste::Matrix<double> TR = ste::Matrix<double>({{0, 1},{1, 0}});
	ste::Matrix<double> TT(2,1);
	TT.at(0,0) = 0;
	TT.at(1,0) = 0;
	long double res=9e99;
	long double oldres;
	// ---- ICP ---- //
	for(unsigned int iter=0;iter<m_maxIter;iter++) {
		// ---- Find points ---- //
		std::cout << iter << " " << std::fabs(oldres-res) << std::endl;
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
		for(unsigned int i=0;i<resid.size();i++) {
            if(resid.at(i) < 1.5*Rob) {
                res += resid.at(i)*resid.at(i);
            }
		}
		res /= resid.size();
		// ---- Cauchy ---- //
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
			mem.at(0) += model.at(0).at(vi.at(i))*wghs.at(i); //PB
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
		for(unsigned int i=0;i<model.size();i++) {
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
        if (Ri.det() < 0) {
        	UV.at(1).at(0,1) = -UV.at(1).at(0,1);
        	UV.at(1).at(1,1) = -UV.at(1).at(1,1);
            Ri = UV.at(0)*UV.at(1).transpose();
        }
        std::vector<double> Ti = {mem.at(0)-Ri.at(0,0)*med.at(0)+Ri.at(0,1)*med.at(1),
                                  mem.at(1)-Ri.at(1,0)*med.at(0)+Ri.at(1,1)*med.at(1)};
        std::vector<std::vector<double>> dataN1(2);
        for(unsigned int i=0;i<data.at(0).size();i++) {
        	dataN1.at(0).push_back(data.at(0).at(i)*Ri.at(0,0)+data.at(1).at(i)*Ri.at(1,0));
        	dataN1.at(1).push_back(data.at(0).at(i)*Ri.at(0,1)+data.at(1).at(i)*Ri.at(1,1));
        }
        data.at(0) = dataN1.at(0);
        data.at(1) = dataN1.at(1);
        for(unsigned int i=0;i<data.at(0).size();i++) {
        	data.at(0).at(i) = data.at(0).at(i)+Ti.at(0);
        	data.at(1).at(i) = data.at(1).at(i)+Ti.at(1);
        }
        // ---- Up TF ---- //
    	TR = Ri*TR;
   		TT = Ri*TT;
   		TT.at(0,0) += Ti.at(0);
   		TT.at(1,0) += Ti.at(1);
    	if(iter >= 5) {
        	if(std::fabs(oldres-res) < m_error) {
           		break;
        	}
    	}
	}
	std::vector<double> result = {TT.at(0,0), TT.at(1,0), std::acos(TR.at(0,0))};
	return result;
}

std::vector<ste::Matrix<double>> ICP::svd(ste::Matrix<double> Mat) {
	double a = Mat.at(0,0), b = Mat.at(0,1), c = Mat.at(1,0), d = Mat.at(1,1);
	double tet = std::atan2(2*a*b+2*c*d,a*a+b*b-c*c-d*d)/2;
	ste::Matrix<double> U({{std::cos(tet), -std::sin(tet)}, {std::sin(tet), std::cos(tet)}});
	double phi = std::atan2(2*a*b+2*c*d,a*a-b*b+c*c-d*d)/2;
	double s11 = (a*std::cos(tet)+c*std::sin(tet))*std::cos(phi) + (b*std::cos(tet)+d*std::sin(tet))*std::sin(phi);
	double s22 = (a*std::sin(tet)-c*std::cos(tet))*std::sin(phi) + (-b*std::sin(tet)+d*std::cos(tet))*std::cos(phi);
	ste::Matrix<double> V({{s11/std::abs(s11)*std::cos(phi), -s22/std::abs(s22)*std::sin(phi)}, {s11/std::abs(s11)*std::sin(phi), s22/std::abs(s22)*std::cos(phi)}});
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

std::vector<std::vector<double>> ICP::nearestNeighbor(std::vector<std::vector<double>> model, std::vector<std::vector<double>> data) {
	std::vector<double> id(data.at(0).size());
	std::vector<double> dist(data.at(0).size());
	double d;
	for(unsigned int i=0;i<data.at(0).size();i++) {
		dist.at(i) = distEucl(model.at(0).at(0), model.at(1).at(0), data.at(0).at(i), data.at(1).at(i));
		id.at(i) = 0;
		for(unsigned int j=1;j<model.at(0).size();j++) {
            d = distEucl(model.at(0).at(j), model.at(1).at(j), data.at(0).at(i), data.at(1).at(i));
			if(d < dist.at(i)) {
				dist.at(i) = d;
				id.at(i) = j;
			}
		}
	}
	std::vector<std::vector<double>> res = {id, dist};
	return res;
}

double ICP::distEucl(double a1, double a2, double b1, double b2) {
    double res = sqrt((a1-b1)*(a1-b1)+(a2-b2)*(a2-b2));
	return res;
}

double ICP::pdt(std::vector<double> a, std::vector<double> b) {
    double res = 0;
    unsigned int len = std::min(a.size(),b.size());
    for(unsigned int i=0;i<len;i++) {
        res += a.at(i)*b.at(i);
    }
    return res;
}

bool ICP::SaveImg(ste::Matrix<double> Map, std::string name) {
	return Utility::writeCSV(name,Map.toVector(),";");
}
