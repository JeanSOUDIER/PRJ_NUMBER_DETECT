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
    StartLidar();
}

Lidar::~Lidar() {
    /*const std::vector<char> data = {'e'};
    m_usb->SendBytes(data);
    delay(1000);*/
    delete m_usb;
}

void Lidar::SetStart(const bool state) {
    m_start.store(state, std::memory_order_release);
    StartLidar();
}

void Lidar::StartLidar(void) {
    if(m_start) {
        std::cout << "Starting lidar" << std::endl;
        const std::vector<char> data = {'a'};
        m_usb->SendBytes(data);
    }
}

void Lidar::Poll(void) {
    bool got_scan = false;
    std::vector<unsigned char> raw_bytes;
    uint8_t good_sets = 0;
    uint32_t m_motor_speed = 0;
    int index;

    while (m_start && !got_scan)
    {
        // Wait until first data sync of frame: 0xFA, 0xA0
        //boost::asio::read(serial_, boost::asio::buffer(&raw_bytes[0], 1));
        raw_bytes = m_usb->ReadBytes(1);

        if(raw_bytes[0] == 0xFA)
        {
            // Now that entire start sequence has been found, read in the rest of the message
            got_scan = true;
            //boost::asio::read(serial_,boost::asio::buffer(&raw_bytes[1], 41));
            raw_bytes.clear();
            raw_bytes = m_usb->ReadBytes(40);


            if(raw_bytes[0] >= 0xA0  && raw_bytes[0] <= 0xDB)
            {
                int degree_count_num = 0;
                index = (raw_bytes[0] - 0xA0) * 6;
                good_sets++;

                m_motor_speed += (raw_bytes[2] << 8) + raw_bytes[1]; //accumulate count for avg. time increment

                //read data in sets of 6
                for(uint16_t j = 3; j < 39; j = j + 6)
                {
                    uint8_t byte0 = raw_bytes[j];
                    uint8_t byte1 = raw_bytes[j+1];
                    uint8_t byte2 = raw_bytes[j+2];
                    uint8_t byte3 = raw_bytes[j+3];

                    uint16_t intensity = (byte1 << 8) + byte0;
                    uint16_t range     = (byte3 << 8) + byte2;

                    m_range.at(359 - index - degree_count_num).store(range/1000.0,std::memory_order_release);
                    m_intensity.at(359 - index - degree_count_num).store(intensity,std::memory_order_release);

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
bool Lidar::GetStart(void) {return m_start.load(std::memory_order_acquire);}

void* Lidar::LidarHelper(void *context) {return static_cast<Lidar*>(context)->ThreadLidar();}

void* Lidar::ThreadLidar() {


    while(m_start.load(std::memory_order_acquire)){Poll();}
    //while(1) {Poll();}

    pthread_exit(NULL);
    return 0;
}

void Lidar::display(const bool isXY){

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

