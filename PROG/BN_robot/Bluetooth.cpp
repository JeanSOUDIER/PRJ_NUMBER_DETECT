#include "Bluetooth.hpp"

Bluetooth::Bluetooth(const int nb_usb, const int bdrate) {
	m_usb = new Usb(nb_usb, bdrate);
	m_port_nr = nb_usb;
	m_bdrate = bdrate;

	//for(auto &item:m_rx){std::atomic_init(&item,0);}
	//for(auto &item:m_tx){std::atomic_init(&item,0);}
	m_start.store(true, std::memory_order_release);
	m_rec.store(false, std::memory_order_release);
	m_sen.store(true, std::memory_order_release);

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
    while(m_start.load(std::memory_order_acquire)) {
    	UpdateThread();
    	ReadThread();
    	WriteThread();
    }

    pthread_exit(NULL);
    return 0;
}

void Bluetooth::ReadThread() {
	std::vector<unsigned char> r(1);
	r = m_usb->ReadBytes(1);
	if(r.at(0) && m_start.load(std::memory_order_acquire)) {
		DEBUG_BLUETOOTH_PRINT("read");
		DEBUG_BLUETOOTH_PRINT(static_cast<int>(r.at(0));
		switch(m_stateR) {
    		case 0:{
    			if(r.at(0) == 255) {m_stateR = 1;}
    			break;
    		}
    		case 1:{
    			m_length = r.at(0);
    			m_ccR = m_length;
    			m_len = 0;
    			m_stateR = 2;
    			break;
    		}
    		case 2:{
    			m_msg.push_back(r.at(0));
    			m_ccR += r.at(0);
    			m_len++;
    			if(m_len == m_length-1) {m_stateR = 3;}
    			break;
    		}
    		case 3:{
    			if(r.at(0) == m_ccR%256) {
    				m_buff_rx.push_back(m_msg);
				DEBUG_BLUETOOTH_PRINT("endR :");
				DEBUG_BLUETOOTH_PRINT(static_cast<int>(m_buff_rx));
    			}
    			m_stateR = 0;
    			break;
    		}
    		default:{
    			m_stateR = 0;
    		}
    	}
	}
}

void Bluetooth::WriteThread() {
	if(m_buff_tx.size() && m_start.load(std::memory_order_acquire)) {
		DEBUG_BLUETOOTH_PRINT("send");
		switch(m_stateS) {
    		case 0:{
    			m_usb->SendBytes(static_cast<std::vector<char>>(255));
			DEBUG_BLUETOOTH_PRINT(static_cast<int>(255));
    			m_stateS = 1;
    			break;
    		}
    		case 1:{
    			m_usb->SendBytes(static_cast<std::vector<char>>(m_buff_tx.at(0).size()));
			DEBUG_BLUETOOTH_PRINT(static_cast<int>(m_buff_tx.at(0).size()));
    			m_ccS = m_buff_tx.at(0).size();
    			m_cpt = 0;
    			m_stateS = 2;
    			break;
    		}
    		case 2:{
    			m_usb->SendBytes(static_cast<std::vector<char>>(m_buff_tx.at(0).at(m_cpt)));
			DEBUG_BLUETOOTH_PRINT(static_cast<int>(m_buff_tx.at(0).at(m_cpt)));
    			m_ccS += m_buff_tx.at(0).at(m_cpt);
    			m_cpt++;
    			if(static_cast<unsigned int>(m_cpt) > m_buff_tx.at(0).size()-1) {
    				m_buff_tx.erase(m_buff_tx.begin());
    				m_stateS = 3;
    			}
    			break;
    		}
    		case 3:{
    			m_usb->SendBytes(static_cast<std::vector<char>>(m_ccS));
			DEBUG_BLUETOOTH_PRINT(static_cast<int>(m_ccS));
			DEBUG_BLUETOOTH_PRINT("endS :");
			DEBUG_BLUETOOTH_PRINT(static_cast<int>(m_buff_tx.size()));
    			m_stateR = 0;
    			break;
    		}
    		default:{
    			m_stateR = 0;
    		}
    	}
	}
}

void Bluetooth::UpdateThread() {
	if(m_start.load(std::memory_order_acquire) && !m_sen.load(std::memory_order_acquire)) {
		DEBUG_BLUETOOTH_PRINT("update TX");
		m_buff_tx.push_back(m_tx.load(std::memory_order_acquire));
		DEBUG_BLUETOOTH_PRINT(static_cast<int>(m_buff_tx.size()));
		m_sen.store(true,std::memory_order_release);
	}
	if(m_start.load(std::memory_order_acquire) && m_buff_rx.size()) {
		DEBUG_BLUETOOTH_PRINT("update RX");
		for(unsigned int i=0;i<m_buff_rx.at(0).size();i++) {
			m_rx.at(i).store(m_buff_rx.at(0).at(i),std::memory_order_release);
		}
		m_buff_rx.erase(m_buff_rx.begin());
		m_rec.store(true,std::memory_order_release);
	}
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
	DEBUG_BLUETOOTH_PRINT("read main thread");
	while(!m_rec.load(std::memory_order_acquire)) {}
	DEBUG_BLUETOOTH_PRINT("read op");
	std::vector<unsigned char> returnValue(m_rx.size());
	std::generate(returnValue.begin() , returnValue.end() , [this]{
        static unsigned int index = 0;
        const unsigned char currentItem = m_rx.at(index).load();
        index++;
        return currentItem;
	});
	m_rec.store(false,std::memory_order_release);
	DEBUG_BLUETOOTH_PRINT("read end");
    return returnValue;
}

void Bluetooth::SetTX(std::vector<char> txt) {
	DEBUG_BLUETOOTH_PRINT("send main thread");
	while(!m_sen.load(std::memory_order_acquire)) {}
	DEBUG_BLUETOOTH_PRINT("send op");
	if(txt.size() < 256) {
		for(unsigned int i=0;i<txt.size();i++) {
			m_tx.at(i).store(txt.at(i),std::memory_order_release);
		}
		m_sen.store(false,std::memory_order_release);
	} else {
		std::cout << "error length too big : " << txt.size() << std::endl;
	}
	DEBUG_BLUETOOTH_PRINT("send end");
}

void Bluetooth::SetTX(std::string txt) {
	std::vector<char> a(txt.begin(), txt.end());
	SetTX(a);
}
