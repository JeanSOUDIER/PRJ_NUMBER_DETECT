#include "Lidar.hpp"

Lidar::Lidar(const int nb_usb, const int bdrate)
    : Lidar(true, nb_usb, bdrate) {}

Lidar::Lidar(const bool start, const int nb_usb, const int bdrate) {

    for(auto &item:m_range){std::atomic_init(&item,0);}
    for(auto &item:m_intensity){std::atomic_init(&item,0);}

    m_usb = new Usb(nb_usb, bdrate);
    m_start.store(start, std::memory_order_release);
    m_port_nr = nb_usb;
    m_bdrate = bdrate;
    m_sat = true;
    StartLidar();
    std::cout << "Lidar start" << std::endl;
}

Lidar::~Lidar() {
    /*const std::vector<char> data = {'e'};
    m_usb->SendBytes(data);
    delay(1000);*/
    delete inc_x_thread; //Delete first because otherwise the function called in the thread will have undefined behaviour when executed after calling delete on lidar
    delete m_usb;
}

vois Lidar::StartThread() {
    inc_x_thread = new pthread_t();
    const int rcL = pthread_create(inc_x_thread, NULL, &Lidar::LidarHelper, this);
    if (rcL) {
        std::cout << "Error:unable to create thread Lidar," << rcL << std::endl;
    }
}

void Lidar::SetStart(const bool state) {
    bool test = m_start.load();
    m_start.store(state, std::memory_order_release);
    if(!test) {
        StartLidar();
    }
}

void Lidar::SetSat(bool state) {
    m_sat = state;
}

void Lidar::StartLidar(void) {
    if(m_start.load()) {
        std::cout << "Starting lidar" << std::endl;
        const std::vector<char> data = {'a'};
        m_usb->SendBytes(data);
    }
}

void Lidar::Poll(void) {
    bool got_scan = false;
    std::vector<unsigned char> raw_bytes(42);
    uint8_t good_sets = 0;
    uint32_t m_motor_speed = 0;
    int index;
    uint32_t motor_speed = 0;

    while (m_start.load() && !got_scan) {
        raw_bytes = m_usb->ReadBytes(1);
        if(raw_bytes[0] == 0xFA) {
            raw_bytes.clear();
            raw_bytes = m_usb->ReadBytes(41);
            raw_bytes.insert(raw_bytes.begin(),0xFA);
            if(raw_bytes[1] >= 0xA0  && raw_bytes[1] <= 0xDB) {
              got_scan = true;
              int degree_count_num = 0;
              index = (raw_bytes[1] - 0xA0) * 6;
              good_sets++;

              motor_speed += (raw_bytes[3] << 8) + raw_bytes[2]; //accumulate count for avg. time increment
              rpms=(raw_bytes[3]<<8|raw_bytes[2])/10;

              //read data in sets of 6
              for(uint16_t j = 4; j < 40; j = j + 6) {
                uint8_t byte0 = raw_bytes[j];
                uint8_t byte1 = raw_bytes[j+1];
                uint8_t byte2 = raw_bytes[j+2];
                uint8_t byte3 = raw_bytes[j+3];

                uint16_t intensity = (byte1 << 8) + byte0;
                uint16_t range     = (byte3 << 8) + byte2;

                int temp = range;
                if(temp > 200 && m_sat) {temp = 3500;}
                if(temp > 3500) {temp = 3500;}
                m_intensity.at(359 - index - degree_count_num).store(temp,std::memory_order_release);
                m_range.at(359 - index - degree_count_num).store(intensity,std::memory_order_release);

                degree_count_num++;
              }      

              m_time_increment = m_motor_speed/good_sets/1e8;
            }
        }
    }
}

int Lidar::GetBdRate(void) {return m_usb->GetBdRate();}
int Lidar::GetPortNb(void) {return m_usb->GetPortNb();}
int Lidar::GetMotorSpeed(void) {return m_motor_speed;}
bool Lidar::GetSat() {return m_sat;}
int Lidar::GetTimeIncrement(void) {return m_time_increment;}
std::vector<int> Lidar::GetRange(void) {

    std::vector<int> returnValue(m_range.size());
    std::generate(returnValue.begin() , returnValue.end() , [this]{
            static unsigned int index = 0;
            const int currentItem = m_range.at(index).load();
            index++;
            return currentItem;
    });

    return returnValue;
}
std::vector<int> Lidar::GetIntensity(void) {

    std::vector<int> returnValue(m_intensity.size());
    std::generate(returnValue.begin() , returnValue.end() , [this]{
        static unsigned int index = 0;
        const int currentItem = m_intensity.at(index).load();
        index++;
        return currentItem;
    });

   return returnValue;
}
bool Lidar::GetStart(void) {return m_start.load();}

void* Lidar::LidarHelper(void *context) { 
    return static_cast<Lidar*>(context)->ThreadLidar();
}

void* Lidar::ThreadLidar() {
    
    while(m_start.load(std::memory_order_acquire)) {Poll();}

    pthread_exit(NULL);
    return 0;
}

void Lidar::Display(const bool isXY) {

    std::vector<std::string> disp (m_range.size());

    //Display as "Point N : (x , y)"
    if(isXY){

        std::generate(disp.begin() , disp.end(),
                  [this]{
                    static unsigned index = 0;

                    const std::string currentItem =  (index < m_range.size()) ?
                                "Point " + std::to_string(index) + " : (" +
                                std::to_string(m_intensity.at(index).load()*std::cos(m_range.at(index).load())) +
                                " ; " +
                                std::to_string(m_intensity.at(index).load()*std::sin(m_range.at(index).load())) +
                                ")"
                                : "";
                    index++;
                    return currentItem;
             }
        );

    //Display as "Point N : (range , intensity)"
    }else{
        std::generate(disp.begin() , disp.end(),
                  [this]{
                    static unsigned index = 0;

                    const std::string currentItem =  (index < m_range.size()) ?
                                "Point " + std::to_string(index) + " : (" +
                                std::to_string(m_range.at(index).load()) +
                                " ; " +
                                std::to_string(m_intensity.at(index).load())
                                +")"
                                : "";
                    index++;
                    return currentItem;
             }
        );

    }

    std::copy(disp.begin(), disp.end(),std::ostream_iterator<std::string>(std::cout, "\n"));

}

void Lidar::DisplayGraph() {
    std::cout << "GRAPH" << std::endl;
    std::vector<double> dispX(m_range.size());
    std::vector<double> dispY(m_range.size());
    std::generate(dispX.begin() , dispX.end(),
        [this]{
            static unsigned index = 0;
            const int currentItem =  m_intensity.at(index).load()*std::cos(m_range.at(index).load());
            index++;
            return currentItem;
        }
    );
    std::generate(dispY.begin() , dispY.end(),
        [this]{
            static unsigned index = 0;
            const int currentItem =  m_intensity.at(index).load()*std::sin(m_range.at(index).load());
            index++;
            return currentItem;
        }
    );
    for(int i=-TERMINAL_LENGTH/2;i<TERMINAL_LENGTH/2;i++) {
        for(int j=-TERMINAL_LENGTH/2;j<TERMINAL_LENGTH/2;j++) {
            bool test = false;
            for(unsigned int k=0;k<dispX.size();k++) {
                if(dispX.at(k) > i*TERMINAL_STEP && dispX.at(k) < (i+1)*TERMINAL_STEP && 
                    dispY.at(k) > j*TERMINAL_STEP && dispY.at(k) < (j+1)*TERMINAL_STEP) {
                        test = true;
                        std::cout << "*";
                }
            }
            if(!test) {std::cout << " ";}
        }
        std::cout << std::endl;
    }
}

bool Lidar::SaveLidarPoints() {
  std::vector<std::vector<double>> v = {std::vector<double>() , std::vector<double>()};

  for(int i=0;i<360;i++) {
    v.at(0).push_back(m_intensity.at(i).load()*std::cos(m_range.at(i).load()));
    v.at(1).push_back(m_intensity.at(i).load()*std::sin(m_range.at(i).load()));
  }

  return Utility::writeCSV("graph",v,";");
}
