#include "Regression.hpp"

Regression::Regression() {}

Regression::~Regression() {}

std::vector<double> Regression::RegressionLineaire(std::vector<double> xi, std::vector<double> yi) {
	m_x = xi;
	m_y = yi;

	std::vector<double> res = {0, 0, 0};
	if(m_x.size() != m_y.size()) {
		std::cout << "erreur taille" << std::endl;
		return res;
	}
	if(m_x.size() < 3) {
		std::cout << "trop peu d'elements" << std::endl;
		return res;
	}

	double Sxi = 0;
	std::for_each(m_x.begin(), m_x.end(), [&] (int n) {Sxi += m_x[n];});
	double Syi = 0;
	std::for_each(m_y.begin(), m_y.end(), [&] (int n) {Syi += m_y[n];});

	double Mxi = Sxi/m_x.size();
	double Myi = Syi/m_y.size();
	
	std::vector<double> Pmi(m_x.size());
	std::for_each(m_x.begin(), m_x.end(), [&] (int n) {Pmi.at(n) += m_x[n]*m_y[n];});
	double SPmi = 0;
	std::for_each(Pmi.begin(), Pmi.end(), [&] (int n) {SPmi += Pmi[n];});

	std::vector<double> E_Mx(m_x.size());
	std::for_each(m_x.begin(), m_x.end(), [&] (int n) {E_Mx.at(n) += (m_x[n]-Mxi)*(m_x[n]-Mxi);});

	double SE_Mx = 0;
	std::for_each(E_Mx.begin(), E_Mx.end(), [&] (int n) {SE_Mx += E_Mx[n];});

	//compute a
	m_a = SPmi-((m_x.size()*Mxi*Myi))/SE_Mx;

	//compute b
	m_b = Myi-(m_a*Mxi);

	//compute r
	std::vector<double> E_My(m_y.size());
	std::for_each(m_y.begin(), m_y.end(), [&] (int n) {E_My.at(n) += (m_y[n]-Myi)*(m_y[n]-Myi);});

	double SE_My = 0;
	std::for_each(E_My.begin(), E_My.end(), [&] (int n) {SE_My += E_My[n];});

	m_r = sqrt(((m_a*m_a)*SE_Mx)/SE_My)*sign(m_a);

	res[0] = m_a;
	res[1] = m_b;
	res[2] = m_r;

	return res;
}

int Regression::sign(const double test) {
	if(test < 0) {
		return -1;
	} else {
		return 1;
	}
}
