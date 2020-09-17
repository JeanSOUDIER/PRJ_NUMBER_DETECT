MAIN: main.o rs232.o Usb.o Arm.o
	g++ -O3 -o draw rs232.o Usb.o Arm.o main.o -Wall -lwiringPi -pthread

rs232.o: rs232.c
	g++ -O3 -o rs232.o -c rs232.c -Wall

Usb.o: Usb.cpp
	g++ -O3 -o Usb.o -c Usb.cpp -Wall

Arm.o: Arm.cpp
	g++ -O3 -o Arm.o -c Arm.cpp -Wall -lwiringPi

main.o: main.cpp rs232.h Usb.hpp Arm.hpp
	g++ -O3 -o main.o -c main.cpp -Wall -lwiringPi -pthread

clean:
	@echo "cleanning"
	rm *.o
	make
