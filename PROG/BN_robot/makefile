MAIN: main.o rs232.o rs232P.o Usb.o js.o Arm.o Lidar.o Bluetooth.o Regression.o MobileBase.o movement.o utility.o sequence.o sequencehandler.o sequencewriter.o sequencereader.o Sequencer.o
	g++ -O3 -o draw rs232.o rs232P.o Usb.o js.o Arm.o Lidar.o Bluetooth.o Regression.o MobileBase.o movement.o utility.o sequence.o sequencehandler.o sequencewriter.o sequencereader.o Sequencer.o main.o -Wall -lwiringPi -pthread -std=c++17 -lstdc++fs 

rs232.o: rs232.c
	g++ -O3 -o rs232.o -c rs232.c -Wall

rs232P.o: rs232.cpp
	g++ -O3 -o rs232P.o -c rs232.cpp -Wall

Usb.o: Usb.cpp
	g++ -O3 -o Usb.o -c Usb.cpp -Wall

js.o: js.cpp
	g++ -O -o js.o -c js.cpp -Wall

Arm.o: Arm.cpp
	g++ -O3 -o Arm.o -c Arm.cpp -Wall -lwiringPi

Lidar.o : Lidar.cpp
	g++ -O3 -o Lidar.o -c Lidar.cpp -Wall -lwiringPi -pthread

Bluetooth.o : Bluetooth.cpp
	g++ -O3 -Wall -pthread -std=c++17 -c Bluetooth.cpp -o Bluetooth.o

Regression.o: Regression.cpp
	g++ -O3 -o Regression.o -c Regression.cpp -Wall

MobileBase.o : MobileBase.cpp
	g++ -O3 -o MobileBase.o -c MobileBase.cpp -Wall

movement.o : movement.cpp
	g++ -O3 -o movement.o -c movement.cpp -Wall

utility.o : utility.cpp
	g++ -Wall -std=c++17 -lstdc++fs -c utility.cpp -o utility.o

sequence.o : sequence.cpp
	g++ -O3 -o sequence.o -c sequence.cpp -Wall

sequencehandler.o : sequencehandler.cpp
	g++ -O3 -o sequencehandler.o -c sequencehandler.cpp -Wall

sequencewriter.o : sequencewriter.cpp
	g++ -O3 -o sequencewriter.o -c sequencewriter.cpp -Wall

sequencereader.o : sequencereader.cpp
	g++ -O3 -o sequencereader.o -c sequencereader.cpp -Wall

Sequencer.o : Sequencer.cpp
	g++ -O3 -o Sequencer.o -c Sequencer.cpp -Wall -I/usr/include/qt5/QtCore -I/usr/include/qt5/QtGui -I/usr/include/qt5 -lQtCore -lQtGui

main.o: main.cpp rs232.h rs232.hpp Usb.hpp js.hpp Arm.hpp Lidar.hpp Bluetooth.hpp Regression.hpp MobileBase.hpp movement.h utility.h sequence.h sequencehandler.h sequencewriter.h sequencereader.h Sequencer.hpp
	g++ -O3 -o main.o -c main.cpp -Wall -lwiringPi -pthread

clean:
	@echo "cleaning"
	rm *.o
	make
