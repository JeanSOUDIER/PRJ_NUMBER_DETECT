MAIN: main.o rs232.o UsbArm.o CtrlArm.o
	g++ -O3 -o draw rs232.o UsbArm.o CtrlArm.o main.o -Wall -lwiringPi -pthread

rs232.o: rs232.c
	g++ -O3 -o rs232.o -c rs232.c -Wall

UsbArm.o: UsbArm.cpp
	g++ -O3 -o UsbArm.o -c UsbArm.cpp -Wall

CtrlArm.o: CtrlArm.cpp
	g++ -O3 -o CtrlArm.o -c CtrlArm.cpp -Wall -lwiringPi

main.o: main.cpp rs232.h UsbArm.hpp CtrlArm.hpp
	g++ -O3 -o main.o -c main.cpp -Wall -lwiringPi -pthread

clean:
	@echo "cleanning"
	rm *.o
