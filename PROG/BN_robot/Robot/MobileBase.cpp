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
	m_posX.store(posX, std::memory_order_release);
	m_posY.store(posY, std::memory_order_release);
	m_angle.store(angle, std::memory_order_release);
	m_RPLidar = RPLidar;
	m_speedNorm = 0;
	m_posEmul = {0, 0, -M_PI/2};
	//thread
	if(m_lidar_start) {
		m_RPLidar->StartThread();
		delay(1000);
		StartPlacing();
	}
	delay(1000);
	m_start.store(true,std::memory_order_release);
	//StartThread();
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
	GoPos(0,0,GetAngleStart());
	double pos = GetPosStart(true,0,700);
	GoPos(0,pos-START_DIST,-M_PI*Utility::sign(pos-START_DIST));
}

double MobileBase::GetAngleStart() {
	bool c = false;
	double angle, dist_85, dist_board;
	double N1 = 0;
	GetLidarPoints(false);
	do{
		GetLidarPoints(false);
		dist_85 = std::sqrt(m_posXlid.at(85)*m_posXlid.at(85)+m_posYlid.at(85)*m_posYlid.at(85));
		dist_board = std::sqrt(m_posYlid.at(90)*m_posYlid.at(90)+dist_85*dist_85-2*m_posYlid.at(90)*dist_85*std::cos(5*M_PI/180));
		angle = std::acos((dist_85*dist_85-m_posYlid.at(90)*m_posYlid.at(90)-dist_board*dist_board)/(-2*m_posYlid.at(90)*dist_board))-M_PI/2;
		std::cout << 2*angle << std::endl;
		if(static_cast<int>(2000*angle) == static_cast<int>(2000*N1) && !std::isnan(angle) && !std::isinf(angle) && std::fabs(angle) < 1) {c = true;}
		N1 = angle;
	}while(!c); //while 2 points are not the same, no inf, no nan and between 0 and 1
	return N1;
}

double MobileBase::GetPosStart(bool delai, int angle, double dist_max) {
	angle = std::fmod(angle+90,359);
	bool c = false;
	if(delai) {delay(500);}
	GetLidarPoints(false);
	int N1 = 0;
	do{
		GetLidarPoints(false);
		std::cout << m_posYlid.at(angle) << std::endl;
		if(N1 == static_cast<int>(m_posYlid.at(angle)) && !std::isinf(m_posYlid.at(angle)) && !std::isnan(m_posYlid.at(angle)) && m_posYlid.at(angle) >= 0 && m_posYlid.at(angle) < dist_max) {c = true;}
		N1 = m_posYlid.at(angle);
	}while(!c); //while 2 points are not the same, no inf, no nan and between 0 and dist_max
	return N1;
}

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
    	if(gamma != 0) { //go to the angle
    		SetSpeed(-SPEED_CST*Utility::sign(gamma),SPEED_CST*Utility::sign(gamma));
		delay(std::abs(gamma)*2*SPEED_ANGLE);
    	}
	if(r != 0) { //go to the distance
		SetSpeed(SPEED_CST*Utility::sign(r),SPEED_CST*Utility::sign(r));
    	std::cout << r*SPEED_NORM << std::endl;
		delay(r*SPEED_NORM);
	}
	if(a != 0) { //go to the end angle
		SetSpeed(-SPEED_CST*Utility::sign(a),SPEED_CST*Utility::sign(a));
		delay(std::abs(a)*SPEED_ANGLE);
	}
	SetSpeed(0, 0);
}

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
	}
}

void MobileBase::SetSpeed(std::vector<int> speed) {
	SetSpeed(speed.at(0), speed.at(1));
}

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
	std::cout << "x=" << m_posX.load() << " y=" << m_posY.load() << " a=" << m_angle.load() << std::endl;
}

std::vector<double> MobileBase::GetCurrentPos() {
	return std::vector<double> ({m_posX.load(), m_posY.load(), m_angle.load()});
}

std::valarray<double> MobileBase::GetCurrentPosi() {
	return m_posEmul;
}

void MobileBase::SetCurrentPosi(std::valarray<double> p) {
	m_posEmul = p;
}

std::valarray<double> MobileBase::currentPos_helper(void* context){return static_cast<MobileBase*>(context)->GetCurrentPosi();}
std::valarray<double> MobileBase::currentPos_helper_meter(void* context){
    
    std::valarray<double> temp = static_cast<MobileBase*>(context)->GetCurrentPosi();
    
    return std::valarray<double>({(1./1000.)*temp[0] , (1./1000.)*temp[1] , temp[2]});
    
}


void* MobileBase::ThreadRun() {
	ICP myICP;
	GetLidarPoints(false);
	GetLidarPoints(true);
	m_startTime = std::clock();
    while(m_start.load(std::memory_order_acquire)) {
		GetLidarPoints(true);
		std::vector<std::vector<double>> posi = {m_posXlid, m_posYlid};
		std::vector<std::vector<double>> posiN1 = {m_posXN1lid, m_posYN1lid};
		std::vector<double> res = myICP.GetPos(posi, posiN1); //compute the ICP
		m_endTime = std::clock();
		double delta = (m_endTime-m_startTime)/(CLOCKS_PER_SEC/1000); //time for integration
		m_startTime = std::clock();
		delta /= 1000;
		//std::cout << "				x=" << res.at(0) << " y=" << res.at(1) << " a=" << res.at(2) << " d=" << delta << std::endl;
		double cst = 5;
		if(res.at(0) < cst) { //saturation
			m_posX.store(m_posX.load()+res.at(0)*delta, std::memory_order_release);
		} else {
			m_posX.store(m_posX.load()+cst*delta, std::memory_order_release);
		}
		if(res.at(1) < cst) {
			m_posY.store(m_posY.load()+res.at(1)*delta, std::memory_order_release);
		} else {
			m_posY.store(m_posY.load()+cst*delta, std::memory_order_release);
		}
		if(res.at(0) < 0.2) {
			m_angle.store(m_angle.load()+res.at(2)*delta, std::memory_order_release);
		} else {
			m_angle.store(m_angle.load()+0.2*delta, std::memory_order_release);
		}
		/*m_posX.store(m_posX.load()+res.at(0)*delta, std::memory_order_release);
		m_posY.store(m_posY.load()+res.at(1)*delta, std::memory_order_release);
		m_angle.store(m_angle.load()+res.at(2)*delta, std::memory_order_release);*/
		m_posXN1 = m_posX.load();
		m_posYN1 = m_posY.load();
		m_angleN1 = m_angle.load();
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
