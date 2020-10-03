#include "Regression.hpp"

Regression::Regression() {}

Regression::~Regression() {}

std::vecot<double> Regression::RegressionLineaire(std::vector<double> xi, std::vector<double> yi) {
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

	double Sxi = std::accumulate(m_x.begin(), m_x.end(), decltype(vector)::value_type(0));
	double Syi = std::accumulate(m_x.begin(), m_x.end(), decltype(vector)::value_type(0));

	double Mxi = Sxi/m_x.size();
	double Myi = Syi/m_y.size();
	
	std::vector<double> Pmi(m_x.size());
	std::for_each(m_x.begin(), m_x.end(), [&] (int n) {
    	Pmi.at(n) += m_x[n]*m_y[n];
	});
	double SPmi = std::accumulate(Pmi.begin(), Pmi.end(), decltype(vector)::value_type(0));

	std::vector<double> E_Mx(m_x.size());
	std::for_each(m_x.begin(), m_x.end(), [&] (int n) {
    	E_Mx.at(n) += (m_x[n]-Mxi)*(m_x[n]-Mxi);
	});

	double SE_Mx = std::accumulate(E_Mx.begin(), E_Mx.end(), decltype(vector)::value_type(0));

	//compute a
	m_a = SPmi-((m_x.size()*Mxi*Myi))/SE_Mx;

	//compute b
	m_b = Myi-(m_a*Mxi);

	//compute r
	std::vector<double> E_My(m_y.size());
	std::for_each(m_y.begin(), m_y.end(), [&] (int n) {
    	E_My.at(n) += (m_y[n]-Myi)*(m_y[n]-Myi);
	});

	double SE_My = std::accumulate(E_My.begin(), E_My.end(), decltype(vector)::value_type(0));

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
