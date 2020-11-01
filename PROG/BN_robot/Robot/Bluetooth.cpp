#include "Bluetooth.hpp"

Bluetooth::Bluetooth(const int nb_usb, const int bdrate) {
	m_usb = new Usb(nb_usb, bdrate);
	m_port_nr = nb_usb;
	m_bdrate = bdrate;

	m_start.store(true, std::memory_order_release);
	m_rec.store(false, std::memory_order_release);
	m_sen.store(true, std::memory_order_release);

	m_stateR = 0;
	m_stateS = 0;

	StartThread();
}

Bluetooth::~Bluetooth() {
	m_start.store(false, std::memory_order_release);
	delete inc_x_thread;
	delete m_usb;
}

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
	std::vector<char> r(1);
	r = m_usb->ReadBytes(1);
	if(r.at(0) && m_start.load(std::memory_order_acquire)) {
		DEBUG_BLUETOOTH_PRINT("read");
		DEBUG_BLUETOOTH_PRINT(static_cast<int>(r.at(0)));
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
    			m_msg.clear();
    			break;
    		}
    		case 2:{
    			m_msg.push_back(r.at(0));
    			m_ccR += r.at(0);
    			m_len++;
    			if(m_len > m_length-1) {m_stateR = 3;}
    			break;
    		}
    		case 3:{
    			if(r.at(0) == m_ccR%256) {
    				m_msg.push_back('\0');
    				m_buff_rx.push_back(m_msg);
					DEBUG_BLUETOOTH_PRINT("endR :");
					DEBUG_BLUETOOTH_PRINT(static_cast<int>(m_buff_rx.size()));
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
		std::vector<char> s(1);
		switch(m_stateS) {
    		case 0:{
    			s.at(0) = static_cast<char>(255);
    			m_usb->SendBytes(s);
    			DEBUG_BLUETOOTH_PRINT("b");
				DEBUG_BLUETOOTH_PRINT(static_cast<int>(255));
    			m_stateS = 1;
    			break;
    		}
    		case 1:{
    			s.at(0) = static_cast<char>(m_buff_tx.at(0).size());
    			m_usb->SendBytes(s);
    			DEBUG_BLUETOOTH_PRINT("s");
				DEBUG_BLUETOOTH_PRINT(static_cast<int>(m_buff_tx.at(0).size()));
    			m_ccS = m_buff_tx.at(0).size();
    			m_cpt = 0;
    			m_stateS = 2;
    			break;
    		}
    		case 2:{
    			s.at(0) = static_cast<char>(m_buff_tx.at(0).at(m_cpt));
    			m_usb->SendBytes(s);
    			DEBUG_BLUETOOTH_PRINT("m");
				DEBUG_BLUETOOTH_PRINT(static_cast<int>(m_buff_tx.at(0).at(m_cpt)));
    			m_ccS += m_buff_tx.at(0).at(m_cpt);
    			m_cpt++;
    			if(static_cast<unsigned int>(m_cpt) > m_buff_tx.at(0).size()-1) {
    				m_stateS = 3;
    			}
    			break;
    		}
    		case 3:{
    			char cc = static_cast<char>(m_ccS%256);
    			s.at(0) = cc;
    			m_usb->SendBytes(s);
				DEBUG_BLUETOOTH_PRINT(static_cast<int>(cc));
				DEBUG_BLUETOOTH_PRINT("endS :");
				DEBUG_BLUETOOTH_PRINT(static_cast<int>(m_buff_tx.size()));
    			m_buff_tx.erase(m_buff_tx.begin());
    			m_stateS = 0;
    			break;
    		}
    		default:{
    			m_stateS = 0;
    		}
    	}
	}
}

void Bluetooth::UpdateThread() {
	if(m_start.load(std::memory_order_acquire) && !m_sen.load(std::memory_order_acquire)) {
		DEBUG_BLUETOOTH_PRINT("update TX");
		std::vector<char> temp(m_tx_size);
		for(int i=0;i<m_tx_size;i++) {
			temp.at(i) = m_tx.at(i).load(std::memory_order_acquire);
		}
		m_buff_tx.push_back(temp);
		DEBUG_BLUETOOTH_PRINT(m_tx_size);
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

std::vector<char> Bluetooth::GetRX(void) {
	DEBUG_BLUETOOTH_PRINT("read main thread");
	while(!m_rec.load(std::memory_order_acquire)) {}
	DEBUG_BLUETOOTH_PRINT("read op");
	int len = GetRXsize();
	std::vector<char> returnValue(len);
	std::generate(returnValue.begin() , returnValue.end() , [this]{
        static unsigned int index = 0;
        const char currentItem = m_rx.at(index).load();
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
		m_tx_size = txt.size();
		for(int i=0;i<m_tx_size;i++) {
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

int Bluetooth::GetRXsize() {
	for(unsigned int i=0;i<256;i++) {
		if(m_rx.at(i).load() == '\0') {return i;}
	}
	return 256;
}
