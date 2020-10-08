#include "Regression.hpp"

Regression::Regression() {}

Regression::~Regression() {}

std::vector<double> Regression::RegressionLineaire(std::vector<double> xi, std::vector<double> yi) {

	std::vector<double> res = {0, 0, 0};
	if(xi.size() != yi.size()) {
		std::cout << "erreur taille" << std::endl;
		return res;
	}
	if(xi.size() < 3) {
		std::cout << "trop peu d'elements" << std::endl;
		return res;
	}

	double Sxi = std::for_each(xi.begin(), xi.end(), Sum()).sum;
	double Syi = std::for_each(yi.begin(), yi.end(), Sum()).sum;

	double xii = Sxi/xi.size();
	double yii = Syi/yi.size();
	
	std::vector<double> Pmi(xi.size());
	for(int i=0;i<xi.size();i++) {
		Pmi.at(i) = xi.at(i)*yi.at(i);
	}
	double SPmi = std::for_each(Pmi.begin(), Pmi.end(), Sum()).sum;

	std::vector<double> E_xi(xi.size());
	for(int i=0;i<xi.size();i++) {
		E_xi.at(i) = (xi.at(i)-xii)*(xi.at(i)-xii);
	}

	double SE_xi = std::for_each(E_xi.begin(), E_xi.end(), Sum()).sum;

	//compute a
	m_a = SPmi-((xi.size()*xii*yii))/SE_xi;

	//compute b
	m_b = yii-(m_a*xii);

	//compute r
	std::vector<double> E_yi(yi.size());
	for(int i=0;i<yi.size();i++) {
		E_yi.at(i) = (yi.at(i)-yii)*(yi.at(i)-yii);
	}

	double SE_yi = std::for_each(E_yi.begin(), E_yi.end(), Sum()).sum;

	m_r = sqrt(((m_a*m_a)*SE_xi)/SE_yi)*Utility::sign(m_a);

	res[0] = m_a;
	res[1] = m_b;
	res[2] = m_r;

	return res;
}

