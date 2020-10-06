MAIN: main.o rs232.o Usb.o Bluetooth.o cam.o
	g++ -O3 -o draw rs232.o Usb.o Bluetooth.o cam.o main.o -Wall -pthread -std=c++17 -lstdc++fs -I/usr/lib/opencv -L/usr/lib -lopencv_core -lopencv_highgui -lopencv_videoio

rs232.o: rs232.c
	g++ -O3 -o rs232.o -c rs232.c -Wall

Usb.o: Usb.cpp
	g++ -O3 -o Usb.o -c Usb.cpp -Wall

Bluetooth.o : Bluetooth.cpp
	g++ -O3 -Wall -pthread -std=c++17 -c Bluetooth.cpp -o Bluetooth.o

cam.o : cam.cpp
	g++ -std=c++11 -Wall -I/usr/lib/opencv -c cam.cpp -L/usr/lib -lopencv_core -lopencv_highgui -lopencv_videoio -o cam.o

main.o: main.cpp rs232.h Usb.hpp Bluetooth.hpp cam.hpp
	g++ -O3 -o main.o -c main.cpp -Wall -pthread -lstdc++fs -I/usr/lib/opencv -L/usr/lib -lopencv_core -lopencv_highgui -lopencv_videoio

clean:
	@echo "cleaning"
	rm *.o
	make
