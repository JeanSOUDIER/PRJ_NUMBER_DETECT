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
	unsigned int cpt = 0;
	double cost = m_error+1;
	double costN1 = 1000000;
	ste::Matrix<double> Pass(3,3);
	std::vector<double> pos(3);
	ste::Matrix<double> p = ste::Matrix<double>({{1},{1},{1}});
	ste::Matrix<double> DynMap(m_mapLen,m_mapLen);
	ste::Matrix<double> StaMap(m_mapLen,m_mapLen);
	ste::Matrix<double> CpyMap(m_mapLen,m_mapLen);
	DynMap = FullMap(DynData);
	StaMap = FullMap(StaData);
	CpyMap = FullMap({{}});
	//SaveImg(DynMap,"DMapStart");
	//SaveImg(StaMap,"SMapStart");
	//SaveImg(CpyMap,"CMapStart");
	std::random_device rd{};
	std::mt19937 gen{rd()};
	std::uniform_int_distribution<int> dist_int(0,2);
	std::uniform_real_distribution<double> dist_dbl(0,1);
	std::cout << "init" << std::endl;
	while(cpt < m_maxIter && m_error < cost) {
		int rd = dist_int(gen);
		//pos.at(rd) = rr*(2*m_max.at(rd))-m_max.at(rd);
		pos.at(0) = dist_dbl(gen)*(2*m_max.at(0))-m_max.at(0);
		Pass = PasMat(pos);
		cost = 0;
		for(unsigned int x3=0;x3<m_mapLen;x3++) {
			for(unsigned int y3=0;y3<m_mapLen;y3++) {
				p.at(0,0) = x3;
				p.at(1,0) = y3;
				ste::Matrix<double> pp = Pass*p;
				int x1 = std::abs(pp.at(0,0));
				int x2 = x1+1;
				int y1 = std::abs(pp.at(1,0));
				int y2 = y1+1;
				if(x1 >= 0 && x2 < m_mapLen && y1 >= 0 && y2 < m_mapLen) {
					double alpha = (x3-x1)/(x2-x1);
					double beta = (y3-y1)/(y2-y1);
					CpyMap.at(x3,y3) = (1-alpha)*DynMap.at(x1,y1)+alpha*(1-beta)*DynMap.at(x2,y1)+beta*(1-alpha)*DynMap.at(x1,y2)+alpha*beta*DynMap.at(x2,y2);
					cost += std::pow(StaMap.at(x3,y3)-CpyMap.at(x3,y3),2);
				}
			}
		}
		//SaveImg(CpyMap,"CMap"+std::to_string(cpt));
		std::cout << pos.at(0) << " " << pos.at(1) << " " << pos.at(2) << std::endl;
		std::cout << cpt << " " << cost << std::endl;
		cpt++;
		if(cost < costN1) {
			costN1 = cost;
		}
	}
	return pos;
}

std::vector<double> ICP::GetPosMatlab(const std::vector<std::vector<double>> DynData, const std::vector<std::vector<double>> StaData) {
	std::vector<std::vector<double>> model;
	std::vector<std::vector<double>> data;
	for(unsigned int i=0;i<StaData.at(0).size();i++) {
		if(!isinf(StaData.at(0).at(i))) {
			model.at(0).push_back(StaData.at(0).at(i));
			model.at(1).push_back(StaData.at(1).at(i));
			data.at(0).push_back(DynData.at(0).at(i));
			data.at(1).push_back(DynData.at(1).at(i));
		}
	}
	int M = StaData.at(0).size();
	delaunator::Delaunator DT(data);
	std::vetor<double> wghs(M);
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
            if(resid.at(i) > 1.5*Rob) {
                res += resid.at(i)*resid.at(i);
            }
		}
		res /= resid.size();
		Rob = 4.3040*Rob;
		double suWghs = 0;
		for(int j=0;j<wghs.size();j++) {
			wghs.at(j) = 1/(1+std::pow((resid.at(j)/Rob),2));
			suWghs += wghs.at(j);
		}
		//med = (data*wghs)/suWghs;
		//mem = (model(:,vi)*wghs)/suWghs;
		//C = (data.*repmat(wghs',m,1))*model(:,vi)'-(suWghs*med)*mem';
        //[U,~,V] = svd(C);
        //std::vector<double> Ri=V*U';
        if (et(Ri) < 0) {
        	//V(:,end)=-V(:,end);
            //Ri=V*U';
        }
        //Ti=mem-Ri*med;
        //data=Ri*data;
        //data(1,:)=data(1,:)+Ti(i);
        //data(2,:)=data(2,:)+Ti(i);
    	TR=Ri*TR;
   		TT=Ri*TT+Ti;
    	if(iter >= 5) {
        	if(std::fabs(oldres-res) < m_error) {
           		break
        	}
    	}
	}
	std::vector<double> result = {TT.at(0), TT.at(1), std::acos(TR.at(0,0))};
	return result;
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

std::vector<std::vector<double>> nearestNeighbor(std::vector<std::vector<double>> tri, std::vector<double> data) {
	double min_dist = ...; //Index 0
	for(auto &line:matrix.toVector()){
  		for(auto &item:line){
      		const double distance = dist(item , truc);
       		if(distance < min_dist){min_dist = distance;}
  		}
	}
}

ste::Matrix<double> ICP::PasMat(const double x, const double y, const double a) {
	ste::Matrix<double> Pas(3,3);
	Pas.at(0,0) = std::cos(a);
	Pas.at(1,0) = std::sin(a);
	Pas.at(2,0) = -std::cos(a)*x-std::sin(a)*y;
	Pas.at(0,1) = -std::sin(a);
	Pas.at(1,1) = std::cos(a);
	Pas.at(2,1) = std::sin(a)*x-std::cos(a)*y;
	Pas.at(0,2) = 0;
	Pas.at(1,2) = 0;
	Pas.at(2,2) = 1;
	return Pas;
}

ste::Matrix<double> ICP::PasMat(const std::vector<double> pos) {
	return PasMat(pos.at(0), pos.at(1), pos.at(2));
}

ste::Matrix<double> ICP::FullMap(std::vector<std::vector<double>> Points) {
	ste::Matrix<double> res(m_mapLen,m_mapLen);
	res.fill(static_cast<unsigned long long>(m_mapLen),
             static_cast<unsigned long long>(m_mapLen),
             0);
	for(unsigned int i=0;i<Points.at(0).size();i++) {
        double x = Points.at(0).at(i);
        double y = Points.at(1).at(i);
        if(!std::isinf(x) && !std::isinf(y)) {
            int x1 = std::fabs(x);
            int x2 = x1+1;
            int y1 = std::fabs(y);
            int y2 = y1+1;
            if(x1 < m_mapLen && y1 < m_mapLen) {res.at(x1,y1) = (x-x1)*(y-y1);}
            if(x2 < m_mapLen && y1 < m_mapLen) {res.at(x2,y1) = (x2-x)*(y-y1);}
            if(x1 < m_mapLen && y2 < m_mapLen) {res.at(x1,y2) = (x-x1)*(y2-y);}
            if(x2 < m_mapLen && y2 < m_mapLen) {res.at(x2,y2) = (x2-x)*(y2-y);}
        }
	}
	return res;
}

/*std::vector<double> ICP::GetPosBrutForce(const std::vector<std::vector<double>> DynData, const std::vector<std::vector<double>> StaData) {
	int len = DynData.at(0).size();
	std::vector<double> res(3);
	double mini = 1000000;
	for(int i=0;i<len;i++) {
		std::vector<double> x(len-1),y(len-1),a(len-1);
		for(int j=0;j<len;j++) {
			if(i != j && (i+1)%len != j) {

				//compute M
				//déduit x,y,a
			}
		}
		// ecart type x,y,z
		// moyenne
		/*if(moy < mini) {
			mini = moy;
			res = pos;
		}
	}
}*/

bool ICP::SaveImg(ste::Matrix<double> Map, std::string name) {
	return Utility::writeCSV(name,Map.toVector(),";");
}
