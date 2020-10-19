#include "ICP.hpp"

ICP::ICP()
	: ICP(0.001,1000,1000,50,50,2*M_PI) {}

ICP::ICP(const double error, const unsigned int MaxIter, const int mapLen)
	: ICP(error,MaxIter,mapLen,50,50,2*M_PI) {}

ICP::ICP(const double error, const unsigned int MaxIter, const int mapLen, const double MaxX, const double MaxY, const double MaxA) {
	m_max.at(0) = MaxX;
	m_max.at(1) = MaxY;
	m_max.at(2) = MaxA;
	m_error = error;
	m_maxIter = MaxIter;
	m_mapLen = mapLen;
}

ICP::~ICP() {}

std::vector<double> ICP::GetPos(const std::vector<std::vector<double>> DynData, const std::vector<std::vector<double>> StaData) {
	unsigned int cpt = 0;
	double cost = m_error+1;
	double costN1 = 1000000;
	srand(time(NULL));
	std::vector<double> Pass(9) = {0,0,0,0,0,0,0,0,0};
	std::vector<double> pos(3);
	std::vector<double> posN1(3) = {0,0,0};
	std::vector<std::vector<double>> DynMap(m_mapLen,m_mapLen);
	std::vector<std::vector<double>> StaMap(m_mapLen,m_mapLen);
	std::vector<std::vector<double>> CpyMap(m_mapLen,m_mapLen);
	DynMap = FullMap(DynData);
	StaMap = FullMap(StaData);
	CpyMap = FullMap({{}});
	SaveImg(DynMap,"DMapStart");
	SaveImg(StaMap,"SMapStart");
	SaveImg(CpyMap,"CMapStart");
	while(cpt < m_maxIter && m_error < cost) {
		int rd = rand()%3;
		pos.at(rd) = rand()%(2*m_max.at(rd))-m_max.at(rd);
		Pass = PasMat(pos);
		cost = 0;
		for(unsigned int x3=0;x3<m_mapLen;x3++) {
			for(unsigned int y3=0;y3<m_mapLen;y3++) {
				std::vector<double> p = {x3,y3,1};
				std::vector<double> pp(3);
				pp = PdtMat(Pass, p);
				int x1 = std::abs(pp.at(0));
				int x2 = x1+1;
				int y1 = std::abs(pp.at(1));
				int y2 = y1+1;
				if(x1 >= 0 && x2 < m_mapLen && y1 >= 0 && y2 < m_mapLen) {
					double alpha = (x3-x1)/(x2-x1);
					double beta = (y3-y1)/(y2-y1);
					CpyMap.at(x3)at(y3) = (1-alpha)*DynMap.at(x1).at(y1)+alpha*(1-beta)*DynMap.at(x2).at(y1)+beta*(1-alpha)*DynMap.at(x1).at(y2)+alpha*beta*DynMap.at(x2).at(y2);
					cost += StaMap.at(x3).at(y3)-CpyMap.at(x3).at(y3);
				}
			}
		}
		SaveImg(CpyMap,"CMap"+str(cpt));
		cpt++;
		if(cost < costN1) {
			posN1.at(0) = pos.at(0);
			posN1.at(1) = pos.at(1);
			posN1.at(2) = pos.at(2);
			costN1 = cost;	
		} else {
			pos.at(0) = posN1.at(0);
			pos.at(1) = posN1.at(1);
			pos.at(2) = posN1.at(2);
			cost = costN1;
		}
	}
}

std::vector<double> ICP::PasMat(const double x, const double y, const double a) {
	std::vector<double> Pas(9) = {std::cos(a), std::sin(a), -std::cos(a)*x-std::sin(a)*y, -std::sin(a), std::cos(a), std::sin(a)*x-std::cos(a)*y, 0, 0, 1};
	return Pas;
}

std::vector<double> ICP::PasMat(const std::vector<double> pos) {
	return PasMat(pos.at(0), pos.at(1), pos.at(2));
}

std::vector<std::vector<double>> ICP::FullMap(std::vector<std::vector<double>> Points) {
	std::vector<std::vector<double> res(m_mapLen,m_mapLen);
	std::fill(res.begin(),res.end(),0);
	for(unsigned int i=0;i<Points.at(0).size();i++) {
		int x1 = std::abs(Points.at(0).at(i));
		int x2 = x1+1;
		int y1 = std::abs(Points.at(1).at(i));
		int y2 = y1+1;
		if(x1 >= 0 && x1 < m_mapLen && y1 >= 0 && y1 < m_mapLen) {res.at(x1).at(y1) = (x-x1)*(y-y1);}
		if(x2 >= 0 && x2 < m_mapLen && y1 >= 0 && y1 < m_mapLen) {res.at(x2).at(y1) = (x2-x)*(y-y1);}
		if(x1 >= 0 && x1 < m_mapLen && y2 >= 0 && y2 < m_mapLen) {res.at(x1).at(y2) = (x-x1)*(y2-y);}
		if(x2 >= 0 && x2 < m_mapLen && y2 >= 0 && y2 < m_mapLen) {res.at(x2).at(y2) = (x2-x)*(y2-y);}
	}
	return res;
}

void ICP::Print(std::vector<std::vector<double>> Map) {
	for(unsigned int i=0;i<m_mapLen;i++) {
		for(unsigned int j=0;j<m_mapLen;j++) {
			std::cout << std::setprecision(5) << Map.at(i).at(j);
		}
		std::cout << std::endl;
	}
	std::cout << std::endl << std::endl;
}

std::vector<double> ICP::PdtMat(std::vector<std::vector<double>> A, std::vector<double> B) {
	std::vector<double> res(B.size());
	for(unsigned int i=0;i<B.size();i++) {
		double temp = 0;
		for(unsigned int j=0;j<Bsize();j++) {
			temp += A.at(i).at(j)*B.at(j);
		}
		res.at(i) = temp;
	}
	return res;
}

bool ICP::SaveImg(std::vector<std::vector<double>> Map, std::string name) {
	return Utility::writeCSV(name,Map,";");
}
