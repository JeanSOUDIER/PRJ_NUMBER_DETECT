#include "Bluetooth.hpp"

Bluetooth::Bluetooth(const int nb_usb, const int bdrate) {
	m_usb = new Usb(nb_usb, bdrate);
	m_port_nr = nb_usb;
	m_bdrate = bdrate;

	for(auto &item:m_rx){std::atomic_init(&item,0);}
	for(auto &item:m_tx){std::atomic_init(&item,0);}
	m_start.store(true, std::memory_order_release);
	m_rec.store(false, std::memory_order_release);
	m_sen.store(false, std::memory_order_release);

	StartThread();
}

Bluetooth::~Bluetooth() {
	m_start.store(false, std::memory_order_release);
	delete inc_x_thread;
	delete m_usb;
}

/*unsigned char Bluetooth::Read() {
	std::vector<unsigned char> reading(1);
	reading = m_usb->ReadBytes(1);
	return reading.at(0);
}

void Bluetooth::WriteWord(std::string txt) {
	std::vector<char> a(txt.begin(), txt.end());
	WriteWord(a);
}

void Bluetooth::WriteWord(std::vector<char> txt) {
	txt.insert(txt.begin(),txt.size());
	txt.insert(txt.begin(),255);
	m_usb->SendBytes(txt);
}

bool Bluetooth::IsAddrValid(std::string addr) {
	if(addr.size() == 17) {
		std::replace(addr.begin(), addr.end(), ':', ' ');  // replace ':' by ' '
		std::stringstream ss(addr);
		int temp;
		bool test = true;
		while (ss >> std::hex >> temp) {
		    if(temp < 0 || temp > 255) {
		    	test = false;
		    	break;
		    }
		}
		return test;
	} else {
		return false;
	}
}*/

void* Bluetooth::ThreadRun() {
    
    int stateR = 0, stateS = 0, len = 0, ccR, ccW, cpt = 0;
    std::vector<unsigned char> r(1);
    while(m_start.load(std::memory_order_acquire)) {
    	//read
    	r = m_usb->ReadBytes(1);
    	if(r.at(0)) {
    		switch(stateR) {
	    		case 0:{
	    			if(r.at(0) == 255) {stateR = 1;}
	    			break;
	    		}
	    		case 1:{
	    			m_length = r.at(0);
	    			ccR = m_length;
	    			stateR = 2;
	    			break;
	    		}
	    		case 2:{
	    			m_msg.push_back(r.at(0));
	    			ccR += r.at(0);
	    			len++;
	    			if(len == m_length) {stateR = 3;}
	    			break;
	    		}
	    		case 3:{
	    			if(r.at(0) == ccR%256) {
	    				m_rx.emplace_back(m_msg);
						m_rx.pop_back();
	    			}
	    			stateR = 0;
	    			break;
	    		}
	    		default:{
	    			stateR = 0;
	    		}
	    	}
    	}

    	//send
    	if(m_tx.size()) {
    		switch(stateS) {
	    		case 0:{
	    			m_usb->SendBytes(255);
	    			stateS = 1;
	    			break;
	    		}
	    		case 1:{
	    			m_usb->SendBytes(m_tx.size());
	    			ccW = m_tx.size();
	    			cpt = 0;
	    			stateS = 2;
	    			break;
	    		}
	    		case 2:{
	    			m_usb->SendBytes(m_tx.at(0).at(cpt).load());
	    			ccW += m_tx.at(0).at(cpt).load();
	    			cpt++;
	    			if(cpt > m_tx.at(0).size()-1) {
	    				stateS = 3;
	    			}
	    			
	    			break;
	    		}
	    		case 3:{
	    			m_usb->SendBytes(ccW);
	    			m_tx.pop_front();
	    			stateR = 0;
	    			break;
	    		}
	    		default:{
	    			stateR = 0;
	    		}
	    	}
    	}
    }

    pthread_exit(NULL);
    return 0;
}

void* Bluetooth::BluetoothHelper(void *context) {
    return static_cast<Bluetooth*>(context)->ThreadRun();
}

void Bluetooth::StartThread() {
    inc_x_thread = new pthread_t();
    const int rcL = pthread_create(inc_x_thread, NULL, &Bluetooth::BluetoothHelper, this);
    if (rcL) {
        std::cout << "Error:unable to create thread Bluetooth," << rcL << std::endl;
    }
}

std::vector<unsigned char> Bluetooth::GetRX(void) {
    std::vector<unsigned char> returnValue;
    if(m_rx.size()) {
    	returnValue = static_cast<std::vector<unsigned char>>(m_rx.at(0).load());
    	m_rx.pop_front();
    } else {
    	returnValue = {-1};
    }
    return returnValue;
}

void Bluetooth::SetTX(std::vector<char> txt) {
	std::array<char> s = {static_cast<std::array<char>>(txt)};
	m_tx.emplace_back(s);
}

void Bluetooth::SetTX(std::string txt) {
	std::vector<char> a(txt.begin(), txt.end());
	SetTX(a);
}
