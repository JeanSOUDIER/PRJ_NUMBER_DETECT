MAIN: main.o rs232P.o Usb.o Bluetooth.o cam.o
	g++ -O3 -o nn rs232P.o Usb.o Bluetooth.o cam.o main.o -Wall -pthread -std=c++17 -lstdc++fs -I/usr/lib/opencv -L/usr/lib -lopencv_core -lopencv_highgui -lopencv_videoio

rs232P.o: rs232.cpp
	g++ -O3 -o rs232P.o -c rs232.cpp -Wall

Usb.o: Usb.cpp
	g++ -O3 -o Usb.o -c Usb.cpp -Wall

Bluetooth.o : Bluetooth.cpp
	g++ -O3 -Wall -pthread -std=c++17 -c Bluetooth.cpp -o Bluetooth.o

cam.o : cam.cpp
	g++ -std=c++11 -Wall -I/usr/lib/opencv -c cam.cpp -L/usr/lib -lopencv_core -lopencv_highgui -lopencv_videoio -o cam.o

main.o: main.cpp rs232.hpp Usb.hpp Bluetooth.hpp cam.hpp
	g++ -O3 -o main.o -c main.cpp -Wall -pthread -lstdc++fs -I/usr/lib/opencv -L/usr/lib -lopencv_core -lopencv_highgui -lopencv_videoio

clean:
	@echo "cleaning"
	rm *.o
	make
