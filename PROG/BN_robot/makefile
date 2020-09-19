MAIN: main.o rs232.o mutex.o Usb.o Arm.o Lidar.o Bluetooth.o MobileBase.o Sequencer.o
	g++ -O3 -o draw rs232.o mutex.o Usb.o Arm.o Lidar.o Bluetooth.o MobileBase.o Sequencer.o main.o -Wall -lwiringPi -pthread

rs232.o: rs232.c
	g++ -O3 -o rs232.o -c rs232.c -Wall

mutex.o : mutex.cpp
	g++ -O3 -o mutex.o -c mutex.cpp -Wall

Usb.o: Usb.cpp
	g++ -O3 -o Usb.o -c Usb.cpp -Wall

Arm.o: Arm.cpp
	g++ -O3 -o Arm.o -c Arm.cpp -Wall -lwiringPi

Lidar.o : Lidar.cpp
	g++ -O3 -o Lidar.o -c Lidar.cpp -Wall -lwiringPi -pthread

Bluetooth.o : Bluetooth.cpp
	g++ -O3 -o Bluetooth.o -c Bluetooth.cpp -Wall -pthread

MobileBase.o : MobileBase.cpp
	g++ -O3 -o MobileBase.o -c MobileBase.cpp -Wall

Sequencer.o : Sequencer.cpp
	g++ -O3 -o Sequencer.o -c Sequencer.cpp -Wall

main.o: main.cpp rs232.h mutex.hpp Usb.hpp Arm.hpp Lidar.hpp Bluetooth.hpp MobileBase.hpp Sequencer.hpp
	g++ -O3 -o main.o -c main.cpp -Wall -lwiringPi -pthread

clean:
	@echo "cleaning"
	rm *.o
	make
