MAIN: main.o rs232.o Usb.o Bluetooth.o
	g++ -O3 -o draw rs232.o Usb.o Bluetooth.o main.o -Wall -pthread -std=c++17 -lstdc++fs 

rs232.o: rs232.c
	g++ -O3 -o rs232.o -c rs232.c -Wall

Usb.o: Usb.cpp
	g++ -O3 -o Usb.o -c Usb.cpp -Wall

Bluetooth.o : Bluetooth.cpp
	g++ -O3 -Wall -pthread -std=c++17 -c Bluetooth.cpp -o Bluetooth.o

main.o: main.cpp rs232.h Usb.hpp Bluetooth.hpp
	g++ -O3 -o main.o -c main.cpp -Wall -pthread

clean:
	@echo "cleaning"
	rm *.o
	make
