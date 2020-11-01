MAIN: main.o RS232_CPP/rs232P.o Robot/Usb.o Joystick/js.o Robot/Arm.o Robot/Lidar.o Robot/Bluetooth.o Algorithms/Regression.o Algorithms/ICP.o Robot/MobileBase.o SequenceHandler_STL/movement.o SequenceHandler_STL/utility.o SequenceHandler_STL/sequence.o SequenceHandler_STL/sequencehandler.o SequenceHandler_STL/sequencewriter.o SequenceHandler_STL/sequencereader.o Robot/Sequencer.o
	g++ -O3 -o draw RS232_CPP/rs232P.o Robot/Usb.o Joystick/js.o Robot/Arm.o Robot/Lidar.o Robot/Bluetooth.o Algorithms/Regression.o Algorithms/ICP.o Robot/MobileBase.o SequenceHandler_STL/movement.o SequenceHandler_STL/utility.o SequenceHandler_STL/sequence.o SequenceHandler_STL/sequencehandler.o SequenceHandler_STL/sequencewriter.o SequenceHandler_STL/sequencereader.o Robot/Sequencer.o main.o -Wall -lwiringPi -pthread -std=c++17 -lstdc++fs 

rs232P.o: RS232_CPP/rs232.cpp
	g++ -O3 -o RS232_CPP/rs232P.o -c RS232_CPP/rs232.cpp -Wall

Usb.o: Robot/Usb.cpp
	g++ -O3 -o Robot/Usb.o -c Robot/Usb.cpp -Wall

js.o: Joystick/js.cpp
	g++ -O -o Joystick/js.o -c Joystick/js.cpp -Wall

Arm.o: Robot/Arm.cpp
	g++ -O3 -o Robot/Arm.o -c Robot/Arm.cpp -Wall -lwiringPi

Lidar.o : Robot/Lidar.cpp
	g++ -O3 -o Robot/Lidar.o -c Robot/Lidar.cpp -Wall -lwiringPi -pthread

Bluetooth.o : Robot/Bluetooth.cpp
	g++ -O3 -Wall -pthread -std=c++17 -c Robot/Bluetooth.cpp -o Robot/Bluetooth.o -pthread

Regression.o: Algorithms/Regression.cpp
	g++ -O3 -o Algorithms/Regression.o -c Algorithms/Regression.cpp -Wall

ICP.o : Algorithms/ICP.cpp
	g++ -O3 -o Algorithms/ICP.o -c Algorithms/ICP.cpp -Wall

MobileBase.o : Robot/MobileBase.cpp
	g++ -O3 -o Robot/MobileBase.o -c Robot/MobileBase.cpp -Wall -pthread

movement.o : SequenceHandler_STL/movement.cpp
	g++ -O3 -o SequenceHandler_STL/movement.o -c SequenceHandler_STL/movement.cpp -Wall

utility.o : SequenceHandler_STL/utility.cpp
	g++ -Wall -std=c++17 -lstdc++fs -c SequenceHandler_STL/utility.cpp -o SequenceHandler_STL/utility.o

sequence.o : SequenceHandler_STL/sequence.cpp
	g++ -O3 -o SequenceHandler_STL/sequence.o -c SequenceHandler_STL/sequence.cpp -Wall

sequencehandler.o : SequenceHandler_STL/sequencehandler.cpp
	g++ -O3 -o SequenceHandler_STL/sequencehandler.o -c SequenceHandler_STL/sequencehandler.cpp -Wall

sequencewriter.o : SequenceHandler_STL/sequencewriter.cpp
	g++ -O3 -o sequencewriter.o -c sequencewriter.cpp -Wall

sequencereader.o : SequenceHandler_STL/sequencereader.cpp
	g++ -O3 -o SequenceHandler_STL/sequencereader.o -c SequenceHandler_STL/sequencereader.cpp -Wall

Sequencer.o : Robot/Sequencer.cpp
	g++ -O3 -o Robot/Sequencer.o -c Robot/Sequencer.cpp -Wall -I/usr/include/qt5/QtCore -I/usr/include/qt5/QtGui -I/usr/include/qt5 -lQtCore -lQtGui

main.o: main.cpp RS232_CPP/rs232.hpp Robot/Usb.hpp js.hpp Robot/Arm.hpp Robot/Lidar.hpp Robot/Bluetooth.hpp Algorithms/Regression.hpp Matrix/Matrix.hpp Algorithms/ICP.hpp Robot/MobileBase.hpp SequenceHandler_STL/movement.h SequenceHandler_STL/utility.h SequenceHandler_STL/sequence.h SequenceHandler_STL/sequencehandler.h SequenceHandler_STL/sequencewriter.h SequenceHandler_STL/sequencereader.h Robot/Sequencer.hpp
	g++ -O3 -o main.o -c main.cpp -Wall -lwiringPi -pthread

clean:
	@echo "cleaning"
	rm *.o
	make
