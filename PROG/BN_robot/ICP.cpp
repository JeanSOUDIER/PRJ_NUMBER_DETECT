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
	std::vector<std::vector<double>> model = {std::vector<double>() , std::vector<double>()};
	std::vector<double> modelLine;
	std::vector<std::vector<double>> data = {std::vector<double>() , std::vector<double>()};
	for(unsigned int i=0;i<StaData.at(0).size();i++) {
		if(!std::isinf(StaData.at(0).at(i))) {
			model.at(0).push_back(StaData.at(0).at(i));
			model.at(1).push_back(StaData.at(1).at(i));
			modelLine.push_back(StaData.at(0).at(i));
			modelLine.push_back(StaData.at(1).at(i));
		}
		if(!std::isinf(DynData.at(0).at(i))) {
			data.at(0).push_back(DynData.at(0).at(i));
			data.at(1).push_back(DynData.at(1).at(i));
		}
	}
	unsigned long long M = model.at(0).size();
	unsigned long long N = data.at(0).size();
	delaunator::Delaunator DT(modelLine);

	/*std::vector<double> test = {0, 1, 1, 3, 5, 6};
	delaunator::Delaunator d(test);
	for(std::size_t i = 0; i < d.triangles.size(); i+=3) {
        printf(
            "Triangle points: [[%f, %f], [%f, %f], [%f, %f]]\n",
            d.coords[2 * d.triangles[i]],        //tx0
            d.coords[2 * d.triangles[i] + 1],    //ty0
            d.coords[2 * d.triangles[i + 1]],    //tx1
            d.coords[2 * d.triangles[i + 1] + 1],//ty1
            d.coords[2 * d.triangles[i + 2]],    //tx2
            d.coords[2 * d.triangles[i + 2] + 1] //ty2
        );
    }
    for(unsigned int i=0;i<d.triangles.size();i++) {
        std::cout << d.triangles[i] << std::endl;
    }*/

	/*std::vector<dt::Vector2<double>> points;
	points.push_back(dt::Vector2<double>{(0), (1)});
	points.push_back(dt::Vector2<double>{(1), (3)});
	points.push_back(dt::Vector2<double>{(5), (6)});

	dt::Delaunay<double> triangulation;
	const std::vector<dt::Triangle<double>> triangles = triangulation.triangulate(points);
	const std::vector<dt::Edge<double>> edges = triangulation.getEdges();

	for(const auto &t : triangles) {
		std::cout << static_cast<double>(t.a->norm2()) << " ";
		std::cout << static_cast<double>(t.b->norm2()) << " ";
		std::cout << static_cast<double>(t.c->norm2()) << std::endl;
	}

	for(const auto &e : edges) {
		std::cout << static_cast<double>(e.v->norm2()) << " ";
		std::cout << static_cast<double>(e.w->norm2()) << std::endl;
	}*/

	std::vector<double> wghs(N);
	std::fill(wghs.begin(), wghs.end(), 1);
	ste::Matrix<double> TR = ste::Matrix<double>({{0, 1},{1, 0}});
	ste::Matrix<double> TT = ste::Matrix<double>({{0, 0}});
	double res=9e99;
	double oldres;
	for(unsigned int iter=0;iter<m_maxIter;iter++) {
		oldres = res;
		std::vector<std::vector<double>> temp = nearestNeighbor(DT.triangles,data);
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
		double res = 0;
		for(unsigned int i=0;i<resid.size();i++) {
            if(resid.at(i) < 1.5*Rob) {
                res += resid.at(i)*resid.at(i);
            }
		}
		res /= resid.size();
		Rob *= 4.3040;
		double suWghs = 0;
		for(int i=0;i<wghs.size();i++) {
			wghs.at(i) = 1/(1+(resid.at(i)/Rob)*(resid.at(i)/Rob));
			suWghs += wghs.at(i);
		}
		std::vector<double> med(2);
		med.at(0) = (data.at(0)*wghs)/suWghs;
		med.at(1) = (data.at(1)*wghs)/suWghs;

		std::vector<double> mem(2) = {0, 0};
		for(unsigned int i=0;i<wghs.size();i++) {
			mem.at(0) += model.at(0).at(vi.at(i))*wghs.at(i);
			mem.at(1) += model.at(1).at(vi.at(i))*wghs.at(i);
		}


		ste::Matrix<double> C({{0, 0}, {0, 0}});
		for(unsigned int i=0;i<wghs.size();i++) {
			C.at(0,0) += data.at(0).at(i)*wghs.at(i);
			C.at(0,1) += data.at(1).at(i)*wghs.at(i);
			C.at(1,0) += data.at(0).at(i)*wghs.at(i);
			C.at(1,1) += data.at(1).at(i)*wghs.at(i);
		}
		C.at(0,0) -= suWghs*med.at(0)*mem.at(0);
		C.at(0,1) -= suWghs*med.at(0)*mem.at(1);
		C.at(1,0) -= suWghs*med.at(1)*mem.at(0);
		C.at(1,1) -= suWghs*med.at(1)*mem.at(1);

		std::vector<ste::Matrix<double>> UV = svd(C);
        ste::Matrix<double> Ri = {UV.at(0)*UV.at(1).transpose()};
        if (det(Ri) < 0) {
        	UV.at(0).at(0,0) = -UV.at(0).at(0,0);
        	UV.at(0).at(0,1) = -UV.at(0).at(0,1);
        	UV.at(0).at(1,0) = -UV.at(0).at(1,0);
        	UV.at(0).at(1,1) = -UV.at(0).at(1,1);
            Ri = UV.at(0)*UV.at(1).transpose();
        }
        std::vector<double> Ti = {mem.at(0)-Ri.at(0,0)*med.at(0)+Ri.at(0,1)*med.at(1), mem.at(1)-Ri.at(1,0)*med.at(0)+Ri.at(1,1)*med.at(1)};
        std::vector<std::vector<double>> dataN1(2,N);
        for(unsigned int i=0;i<data.at(0).size();i++) {
        	dataN1.at(0).at(i) = data.at(0).at(i)*Ri.at(0,0)+data.at(1).at(i)*Ri.at(1,0);
        	dataN1.at(1).at(i) = data.at(0).at(i)*Ri.at(0,1)+data.at(1).at(i)*Ri.at(1,1);
        }
        data.at(0) = dataN1.at(0);
        data.at(1) = dataN1.at(1);
        for(unsigned int i=0;i<data.at(0).size();i++) {
        	data.at(0).at(i) = data.at(0).at(i)+Ti.at(0);
        	data.at(1).at(i) = data.at(1).at(i)+Ti.at(1);
        }
    	TR = Ri*TR;
    	ste::Matrix<double> TiMat = {Ti};
   		TT=Ri*TT+TiMat;
    	if(iter >= 5) {
        	if(std::fabs(oldres-res) < m_error) {
           		break
        	}
    	}
	}
	std::vector<double> result = {TT.at(0), TT.at(1), std::acos(TR.at(0,0))};
	return result;
}

std::vector<ste::Matrix<double>> ICP::svd(ste::Matrix<double> Mat) {
	double a = Mat.at(0,0), b = Mat.at(0,1), c = Mat.at(1,0), d = Mat.at(1,1);
	double tet = 1/2*std::atan2(2*a*b+2*c*d,a*a+b*b-c*c-d*d);
	ste::Matrix<double> U = {{std::cos(tet), -std::sin(tet)}, {std::sin(tet), std::cos(tet)}};
	double tet = 1/2*std::atan2(2*a*b+2*c*d,a*a-b*b+c*c-d*d);
	double s11 = (a*std::cos(tet)+c*std::sin(tet))*std::cos(phi) + (b*std::cos(tet)+d*std::sin(tet))*std::sin(phi);
	double s22 = (a*std::sin(tet)-c*std::cos(tet))*std::sin(phi) + (-b*std::sin(tet)+d*std::cos(tet))*std::cos(phi);
	ste::Matrix<double> V = {{s11/std::abs(s11)*std::cos(phi), -s22/std::abs(s22)*std::sin(phi)}, {s11/std::abs(s11)*std::sin(phi), s22/std::abs(s22)*std::cos(phi)}};
	std::vector<ste::Matrix<double>> res = {U,V};
	return res;
}

double ICP::median(vector<double> vec) {
        typedef vector<double>::size_type vec_sz;
        vec_sz size = vec.size();
        if (size == 0) {
        	throw domain_error("median of an empty vector");
        }
        sort(vec.begin(), vec.end());
        vec_sz mid = size/2;
        return size % 2 == 0 ? (vec[mid] + vec[mid-1]) / 2 : vec[mid];
}

/*std::vector<std::vector<double>> nearestNeighbor(std::vector<std::vector<double>> tri, std::vector<double> data) {
	double min_dist = ...; //Index 0
	for(auto &line:matrix.toVector()){
  		for(auto &item:line){
      		const double distance = dist(item , truc);
       		if(distance < min_dist){min_dist = distance;}
  		}
	}
}*/

bool ICP::SaveImg(ste::Matrix<double> Map, std::string name) {
	return Utility::writeCSV(name,Map.toVector(),";");
}
