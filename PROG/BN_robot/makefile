MAIN: main.o RS232_CPP/rs232.o Robot/Usb.o Joystick/js.o Robot/Arm.o Robot/Lidar.o Robot/Bluetooth.o Algorithms/ICP.o Control/adder.o Control/block.o Control/demultiplexer.o Control/differentiator.o Control/functionblock.o Control/generator.o Control/generator_project.o Control/integrator.o Control/multiplexer.o Control/system.o Control/system_project.o Robot/asserv.o Robot/MobileBase.o SequenceHandler_STL/movement.o SequenceHandler_STL/utility.o SequenceHandler_STL/sequence.o SequenceHandler_STL/sequencehandler.o SequenceHandler_STL/sequencewriter.o SequenceHandler_STL/sequencereader.o Robot/Sequencer.o
	g++ -O3 -o draw RS232_CPP/rs232.o Robot/Usb.o Joystick/js.o Robot/Arm.o Robot/Lidar.o Robot/Bluetooth.o Algorithms/ICP.o Control/adder.o Control/block.o Control/demultiplexer.o Control/differentiator.o Control/functionblock.o Control/generator.o Control/generator_project.o Control/integrator.o Control/multiplexer.o Control/system.o Control/system_project.o Robot/asserv.o Robot/MobileBase.o SequenceHandler_STL/movement.o SequenceHandler_STL/utility.o SequenceHandler_STL/sequence.o SequenceHandler_STL/sequencehandler.o SequenceHandler_STL/sequencewriter.o SequenceHandler_STL/sequencereader.o Robot/Sequencer.o main.o -Wall -lwiringPi -pthread -std=c++17 -lstdc++fs 

rs232.o: RS232_CPP/rs232.cpp
	g++ -O3 -o RS232_CPP/rs232.o -c RS232_CPP/rs232.cpp -Wall

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

ICP.o : Algorithms/ICP.cpp
	g++ -O3 -o Algorithms/ICP.o -c Algorithms/ICP.cpp -Wall

adder.o : Control/adder.cpp
	g++ -O3 -o Control/adder.o -c Control/adder.cpp -Wall

block.o : Control/block.cpp
	g++ -O3 -o Control/block.o -c Control/block.cpp -Wall

demultiplexer.o : Control/demultiplexer.cpp
	g++ -O3 -o Control/demultiplexer.o -c Control/demultiplexer.cpp -Wall

differentiator.o : Control/differentiator.cpp
	g++ -O3 -o Control/differentiator.o -c Control/differentiator.cpp -Wall

functionblock.o : Control/functionblock.cpp
	g++ -O3 -o Control/functionblock.o -c Control/functionblock.cpp -Wall

generator.o : Control/generator.cpp
	g++ -O3 -o Control/generator.o -c Control/generator.cpp -Wall

generator_project.o : Control/generator_project.cpp
	g++ -O3 -o Control/generator_project.o -c Control/generator_project.cpp -Wall

integrator.o : Control/integrator.cpp
	g++ -O3 -o Control/integrator.o -c Control/integrator.cpp -Wall

multiplexer.o : Control/multiplexer.cpp
	g++ -O3 -o Control/multiplexer.o -c Control/multiplexer.cpp -Wall

system.o : Control/system.cpp
	g++ -O3 -o Control/system.o -c Control/system.cpp -Wall

system_project.o : Control/system_project.cpp
	g++ -O3 -o Control/system_project.o -c Control/system_project.cpp -Wall

asserv.o : Robot/asserv.cpp
	g++ -O3 -o Robot/asserv.o -c Robot/asserv.cpp -Wall

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

main.o: main.cpp RS232_CPP/rs232.hpp Robot/Usb.hpp Joystick/js.hpp Robot/Arm.hpp Robot/Lidar.hpp Robot/Bluetooth.hpp Matrix/Matrix.hpp Algorithms/ICP.hpp Control/Control_global.h Control/sensor.h Control/adder.h Control/block.h Control/demultiplexer.h Control/differentiator.h Control/functionblock.h Control/generator.h Control/generator_project.h Control/integrator.h Control/multiplexer.h Control/system.h Control/system_project.h Robot/asserv.hpp Robot/MobileBase.hpp SequenceHandler_STL/movement.h SequenceHandler_STL/utility.h SequenceHandler_STL/sequence.h SequenceHandler_STL/sequencehandler.h SequenceHandler_STL/sequencewriter.h SequenceHandler_STL/sequencereader.h Robot/Sequencer.hpp
	g++ -O3 -o main.o -c main.cpp -Wall -lwiringPi -pthread

clean:
	@echo "cleaning"
	rm *.o
	rm Algorithms/*.o
	rm Control/*.o
	rm Robot/*.o
	rm RS232_CPP/*.o
	rm SequenceHandler_STL/*.o
	make
