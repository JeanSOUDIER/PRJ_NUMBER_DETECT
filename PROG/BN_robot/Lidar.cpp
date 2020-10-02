#include "Lidar.hpp"

Lidar::Lidar(const int nb_usb, const int bdrate)
    : Lidar(true, nb_usb, bdrate) {}

Lidar::Lidar(const bool start, const int nb_usb, const int bdrate) {

    for(auto &item:m_range){std::atomic_init(&item,0);}
    for(auto &item:m_intensity){std::atomic_init(&item,0);}

    m_usb = new Usb(nb_usb, bdrate);
    m_start.store(start, std::memory_order_release);
    std::cout << "start = " << m_start.load() << std::endl;
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
    delete m_usb;
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

    //std::cout << m_start.load() << std::endl;

    while (!got_scan) //m_start.load() && 
    {
        // Wait until first data sync of frame: 0xFA, 0xA0
        //boost::asio::read(serial_, boost::asio::buffer(&raw_bytes[0], 1));
        raw_bytes = m_usb->ReadBytes(1);

        if(raw_bytes[0] == 0xFA)
        {
            // Now that entire start sequence has been found, read in the rest of the message
            //got_scan = true;
            //boost::asio::read(serial_,boost::asio::buffer(&raw_bytes[1], 41));
            raw_bytes.clear();
            raw_bytes = m_usb->ReadBytes(41);
            raw_bytes.insert(raw_bytes.begin(),0xFA);

            if(raw_bytes[1] >= 0xA0  && raw_bytes[1] <= 0xDB)
            {
              got_scan = true;
              int degree_count_num = 0;
              index = (raw_bytes[1] - 0xA0) * 6;
              good_sets++;

              motor_speed += (raw_bytes[3] << 8) + raw_bytes[2]; //accumulate count for avg. time increment
              rpms=(raw_bytes[3]<<8|raw_bytes[2])/10;

              //read data in sets of 6
              for(uint16_t j = 4; j < 40; j = j + 6)
              {
                uint8_t byte0 = raw_bytes[j];
                uint8_t byte1 = raw_bytes[j+1];
                uint8_t byte2 = raw_bytes[j+2];
                uint8_t byte3 = raw_bytes[j+3];

                uint16_t intensity = (byte1 << 8) + byte0;
                uint16_t range     = (byte3 << 8) + byte2;

                int temp = range;//1000.0;
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
/*void Lidar::Poll(void) {
  uint8_t start_count = 0;
  bool got_scan = false;
  //boost::array<uint8_t, 2520> raw_bytes;
  std::vector<unsigned char> raw_bytes(2520);
  uint8_t good_sets = 0;
  uint32_t motor_speed = 0;
  rpms=0;
  int index;
  std::cout << "Poll" << std::endl;

  while (m_start.load(std::memory_order_acquire) && !got_scan)
  {
    // Wait until first data sync of frame: 0xFA, 0xA0
    //boost::asio::read(serial_, boost::asio::buffer(&raw_bytes[start_count],1));
    raw_bytes = m_usb->ReadBytes(1);

    if(start_count == 0)
    {
      if(raw_bytes[0] == 0xFA)
      {
        start_count = 1;
      }
    }
    else if(start_count == 1)
    {
      if(raw_bytes[0] == 0xA0)
      {
        start_count = 0;

        // Now that entire start sequence has been found, read in the rest of the message
        got_scan = true;

        //boost::asio::read(serial_,boost::asio::buffer(&raw_bytes[2], 2518));
        raw_bytes = m_usb->ReadBytes(2518);
        raw_bytes.insert(raw_bytes.begin(),0xA0);
        raw_bytes.insert(raw_bytes.begin(),0xFA);
        for(unsigned int k=0;k<raw_bytes.size();k++) {
          std::cout << static_cast<int>(raw_bytes[k]) << std::endl;
        }

        //scan->angle_increment = (2.0*M_PI/360.0);
        //scan->angle_min = 0.0;
        //scan->angle_max = 2.0*M_PI-scan->angle_increment;
        //scan->range_min = 0.12;
        //scan->range_max = 3.5;
        //scan->ranges.resize(360);
        //scan->intensities.resize(360);

        //read data in sets of 6
        for(uint16_t i = 0; i < raw_bytes.size(); i=i+42)
        {
          if(raw_bytes[i] == 0xFA && raw_bytes[i+1] == (0xA0 + i / 42)) //&& CRC check
          {
            good_sets++;
            motor_speed += (raw_bytes[i+3] << 8) + raw_bytes[i+2]; //accumulate count for avg. time increment
            rpms=(raw_bytes[i+3]<<8|raw_bytes[i+2])/10;

            for(uint16_t j = i+4; j < i+40; j=j+6)
            {
              index = 6*(i/42) + (j-4-i)/6;

              // Four bytes per reading
              uint8_t byte0 = raw_bytes[j];
              uint8_t byte1 = raw_bytes[j+1];
              uint8_t byte2 = raw_bytes[j+2];
              uint8_t byte3 = raw_bytes[j+3];

              // Remaining bits are the range in mm
              uint16_t intensity = (byte1 << 8) + byte0;

              // Last two bytes represent the uncertanty or intensity, might also be pixel area of target...
              // uint16_t intensity = (byte3 << 8) + byte2;
              uint16_t range = (byte3 << 8) + byte2;

              //scan->ranges[359-index] = range / 1000.0;
              //scan->intensities[359-index] = intensity;
              int temp = range/1000.0;
              if(temp > 200 && m_sat) {temp = 3500;}
              if(temp > 3500) {temp = 3500;}
              m_intensity.at(359 - index).store(temp,std::memory_order_release);
              temp = intensity;
              if(temp > 200 && m_sat) {temp = 3500;}
              if(temp > 3500) {temp = 3500;}
              m_range.at(359 - index).store(temp,std::memory_order_release);
            }
          } else {
            //std::cout << "CRC NOT OK" << std::endl;
          }
        }

        //scan->time_increment = motor_speed/good_sets/1e8;
        m_time_increment = m_motor_speed/good_sets/1e8;
      }
      else
      {
        start_count = 0;
      }
    }
  }
}*/


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
    
    while(m_start.load(std::memory_order_acquire)) {
        Poll();
        //DisplayGraph();
    }

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
