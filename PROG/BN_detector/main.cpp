#include <iostream>
#include <cmath>
//#include "Arm.hpp"
//#include "MobileBase.hpp"
#include "Bluetooth.hpp"
//#include "Lidar.hpp"
//#include "Sequencer.hpp"
#include "cam.hpp"
#include "tf.hpp"
#include "Matrix.hpp"

int main() {
    std::cout <<"----SETUP----" << std::endl;
    constexpr int ttyUSB_ARBO = 38;
    constexpr int ttyUSB_LDS = 39;
    constexpr int rfcomm0 = 26;
    constexpr int rfcomm1 = 27;
    constexpr int ttyACM0 = 24;
    constexpr int ttyS0 = 0;
    constexpr int ttyTHS1 = 40;

    /*Sequencer Prgm(
        new Arm(6, ttyUSB_ARBO, 115200),
        nullptr,//new Bluetooth(rfcomm0, 115200),
        new MobileBase(ttyACM0, 115200, new Lidar(ttyUSB_LDS, 230400))
    );

    std::cout <<"----LOOP----" << std::endl;
    while(Prgm.Execute()) {}

    Arm WidowXL(6, ttyUSB_ARBO, 115200);
    WidowXL.ToKeyboard();*/


    //MobileBase OpenCR(ttyACM0, 115200, nullptr);
    //OpenCR.GoPos(0,0,M_PI);
	//Cam CPI2;
    //ste::Matrix<double> img = CPI2.TakePhoto();
    TF myTF;
    ste::Matrix<double> img = myTF.ToProcessed(myTF.ReadPPM("test"), 0.5);
    myTF.PrintPPM(img, "cam");

	//CPI2.ImgShow();

    //Bluetooth BLE(ttyTHS1, 9600);
    /*unsigned char temp = '\0';
    while(temp != 'o') {
	temp = BLE.Read();
	//if(temp != 0 && temp != 4 && temp != 32 && temp != 118) {std::cout << temp << std::endl;}
	if(temp > 'A' && temp < 'z') {std::cout << temp << std::endl;}
	else if(temp) {std::cout << static_cast<int>(temp) << std::endl;}
    }*/
    //BLE.SetTX("hola");
    sleep(1);

    //MobileBase OpenCR(ttyACM0, 115200, new Lidar(ttyUSB_LDS, 230400));
    //delay(10000);
    //Lidar RPLidar(true, ttyUSB_LDS, 230400);
    //std::vector<int> range = RPLidar.GetRange();
    //std::vector<int> intensity = RPLidar.GetIntensity();

    //RPLidar.display(true);
    //RPLidar.DisplayGraph();

    return 0;
}
