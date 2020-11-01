#include "MobileBase.hpp"

MobileBase::MobileBase(const int nb_usb, const int bdrate)
	: MobileBase(0, 0, 0, nb_usb, bdrate, nullptr) {}

MobileBase::MobileBase(const double posX, const double posY, const double angle, const int nb_usb, const int bdrate)
	: MobileBase(posX, posY, angle, nb_usb, bdrate, nullptr) {}

MobileBase::MobileBase(const int nb_usb, const int bdrate, Lidar *RPLidar)
    : MobileBase(0, 0, 0, nb_usb, bdrate, RPLidar) {}

MobileBase::MobileBase(const double posX, const double posY, const double angle, const int nb_usb, const int bdrate, Lidar *RPLidar) {
	m_usb = new Usb(nb_usb, bdrate);
	if(RPLidar) {
		m_lidar_start = true;
	} else {
		m_lidar_start = false;
	}
	m_port_nr = nb_usb;
	m_bdrate = bdrate;
	m_posX = posX;
	m_posY = posY;
	m_angle = angle;
	m_RPLidar = RPLidar;
	m_speedNorm = 0;
	//thread
	if(m_lidar_start) {
		m_RPLidar->StartThread();
		delay(1000);
		StartPlacing();
	}
	delay(1000);
	m_start.store(true,std::memory_order_release);
	StartThread();
    std::cout << "MobileBase start" << std::endl;
}

MobileBase::~MobileBase() {
	if(m_lidar_start) {
		m_RPLidar->SetStart(false);
		delete m_RPLidar;
	}
	m_start.store(false,std::memory_order_release);
	delete inc_x_thread;
	delete m_usb;
}

void MobileBase::StartPlacing() {
	char c = '\0';
	double angle, dist_85, dist_board;
	do{
		GetLidarPoints(false);
		dist_85 = std::sqrt(m_posXlid.at(85)*m_posXlid.at(85)+m_posYlid.at(85)*m_posYlid.at(85));
		dist_board = std::sqrt(m_posYlid.at(90)*m_posYlid.at(90)+dist_85*dist_85-2*m_posYlid.at(90)*dist_85*std::cos(5*M_PI/180));
		angle = std::acos((dist_85*dist_85-m_posYlid.at(90)*m_posYlid.at(90)-dist_board*dist_board)/(-2*m_posYlid.at(90)*dist_board))-M_PI/2;
		std::cout << 2*angle << std::endl;
		std::cin >> c;
	}while(c != '1');
	GoPos(0,0,2*angle);
	c = '\0';
	do{
		GetLidarPoints(false);
		std::cout << m_posYlid.at(90) << std::endl;
		std::cin >> c;
	}while(c != '1');
	GoPos(0,m_posYlid.at(90)-START_DIST,-M_PI*Utility::sign(m_posYlid.at(90)-START_DIST));
	std::cout << angle << std::endl;
}

/*void MobileBase::Go(const double x, const double y, const double a) {
	m_posXgoal = x;
	m_posYgoal = y;
	m_angle_goal = a;
}*/

void MobileBase::GoPos(const double x, const double y, const double a) {
	const double r = sqrt(x*x+y*y);
    	double gamma;
    	if(x == 0 && y == 0) {
    		gamma = 0;
    	} else if(y == 0 && x < 0) {
    		gamma = M_PI;
    	} else {
    		gamma = 2*atan(y/(x+r));
    	}
    	gamma += M_PI;
    	gamma = std::fmod(gamma,2*M_PI);
    	gamma -= M_PI;
    	if(gamma != 0) {
    		SetSpeed(-SPEED_CST*Utility::sign(gamma),SPEED_CST*Utility::sign(gamma));
		delay(std::abs(gamma)*2*SPEED_ANGLE);
    	}
	if(r != 0) {
		SetSpeed(SPEED_CST*Utility::sign(r),SPEED_CST*Utility::sign(r));
    	std::cout << r*SPEED_NORM << std::endl;
		delay(r*SPEED_NORM);
	}
	if(a != 0) {
		SetSpeed(-SPEED_CST*Utility::sign(a),SPEED_CST*Utility::sign(a));
		delay(std::abs(a)*SPEED_ANGLE);
	}
	SetSpeed(0, 0);
}

/*double MobileBase::getDistBoard() {
	return m_dist_board;
}*/

void MobileBase::GetLidarPoints(bool nb) {
	if(m_lidar_start) {
		if(nb) {
			m_posXN1lid.clear();
			m_posYN1lid.clear();
			for(unsigned int i=0;i<m_posXlid.size();i++) {
				m_posXN1lid.push_back(m_posXlid.at(i));
				m_posYN1lid.push_back(m_posYlid.at(i));
			}
		}
		std::vector<double> xP = m_RPLidar->GetXPos();
		std::vector<double> yP = m_RPLidar->GetYPos();
		m_posXlid.clear();
		m_posYlid.clear();
		for(unsigned int i=0;i<xP.size();i++) {
			m_posXlid.push_back(xP.at(i));
			m_posYlid.push_back(yP.at(i));
		}
		//m_RPLidar->Display(false);
		//std::cout << "out : " << m_RPLidar->SaveLidarPoints() << std::endl;
	}
}

/*void MobileBase::GetPosBase() {
	std::vector<double> res = FindSegment(12,27);
	std::cout << res[0] << " " << res[1] << " " << res[2] << std::endl;
	/*res = FindSegment(910,179);
	std::cout << res[0] << " " << res[1] << " " << res[2] << std::endl;
	res = FindSegment(179,269);
	std::cout << res[0] << " " << res[1] << " " << res[2] << std::endl;
	res = FindSegment(269,359);
	std::cout << res[0] << " " << res[1] << " " << res[2] << std::endl;*/
	//points cardinaux
	//find a*x+b => a,b x4 + dist board
	//set m_posX, m_posY, m_angle, m_dist_board
/*}

std::vector<double> MobileBase::FindSegment(int start, int stop) {
	std::vector<double> subvectorX;
	std::vector<double> subvectorY;
	if(start < stop) {
		for(int i=start;i<stop;i++) {
			subvectorX.push_back(m_x.at(i));
			subvectorY.push_back(m_y.at(i));
		}
	} else {
		for(unsigned int i=start;i<m_x.size();i++) {
			subvectorX.push_back(m_x.at(i));
			subvectorY.push_back(m_y.at(i));
		}
		for(int i=0;i<stop;i++) {
			subvectorX.push_back(m_x.at(i));
			subvectorY.push_back(m_y.at(i));
		}
	}
	for(unsigned int i=0;i<subvectorX.size();i++) {
		if(std::isinf(subvectorX.at(i))) {
			subvectorX.erase(subvectorX.begin()+i);
			subvectorY.erase(subvectorY.begin()+i);
		}
	}
	std::vector<std::vector<double>> v = {std::vector<double>() , std::vector<double>()};

    for(unsigned int i=0;i<subvectorX.size();i++) {
      v.at(0).push_back(subvectorX.at(i));
      v.at(1).push_back(subvectorY.at(i));
    }

    Utility::writeCSV("graphXYreduit",v,";");
	Regression reg;
	std::vector<double> res = reg.RegressionLineaire(subvectorX,subvectorY);
	return res;
}*/

/*void MobileBase::SetMotBalance(const double rho, const double theta) {
	const int facteur = 4;
	SetSpeed(rho+facteur*theta, rho-facteur*theta);
}*/

void MobileBase::SetSpeed(int L, int R) {
	if(L > 330) {L = 330;std::cout << "speed sat 0" << std::endl;}
	if(L < -330) {L = -330;std::cout << "speed sat 1" << std::endl;}
	if(R > 330) {R = 330;std::cout << "speed sat 2" << std::endl;}
	if(R < -330) {R = -330;std::cout << "speed sat 3" << std::endl;}
	L += 330;
	R += 330;
	unsigned char Lc = static_cast<unsigned char>(L>>8);
	unsigned char Ld = static_cast<unsigned char>(L%256);
	unsigned char Rc = static_cast<unsigned char>(R>>8);
	unsigned char Rd = static_cast<unsigned char>(R%256);
	int cc = Lc+Ld+Rc+Rd;
	unsigned char c = static_cast<unsigned char>(cc%256);
	std::vector<char> sending{static_cast<unsigned char>(255), static_cast<unsigned char>(Rc), static_cast<unsigned char>(Rd), static_cast<unsigned char>(Lc), static_cast<unsigned char>(Ld), static_cast<unsigned char>(c)};
	m_usb->SendBytes(sending);
}

void MobileBase::SetSpeedCons(double speed) {
	m_speedNorm = speed;
}

double MobileBase::GetSpeedCons() {
	return m_speedNorm;
}

void MobileBase::PrintPos() {
	std::cout << "x=" << m_posX << " y=" << m_posY << " a=" << m_angle << std::endl;
}

void* MobileBase::ThreadRun() {
	ICP myICP;
	GetLidarPoints(false);
	GetLidarPoints(true);
	m_startTime = std::clock();
    while(m_start.load(std::memory_order_acquire)) {
    	//TODO asserv lidar + deplacement
		GetLidarPoints(true);
		std::vector<std::vector<double>> posi = {m_posXlid, m_posYlid};
		std::vector<std::vector<double>> posiN1 = {m_posXN1lid, m_posYN1lid};
		std::vector<double> res = myICP.GetPos(posi, posiN1);
		m_endTime = std::clock();
		double delta = (m_endTime-m_startTime)/(CLOCKS_PER_SEC/1000);
		delta /= 1000;
		//std::cout << "x=" << res.at(0) << " y=" << res.at(1) << " a=" << res.at(2) << " d=" << delta << std::endl;
		m_posX += res.at(0)*delta;
		m_posY += res.at(1)*delta;
		m_angle += res.at(2)*delta;
		m_startTime = std::clock();
    }
    pthread_exit(NULL);
    return 0;
}

void* MobileBase::MobileBaseHelper(void *context) {
    return static_cast<MobileBase*>(context)->ThreadRun();
}

void MobileBase::StartThread() {
    inc_x_thread = new pthread_t();
    const int rcL = pthread_create(inc_x_thread, NULL, &MobileBase::MobileBaseHelper, this);
    if (rcL) {
        std::cout << "Error:unable to create thread MobileBase," << rcL << std::endl;
    }
}
