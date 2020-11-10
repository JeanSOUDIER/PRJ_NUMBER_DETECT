MAIN: main.o rs232P.o Usb.o Bluetooth.o cam.o tf.o
	g++ -O3 -o nn rs232P.o Usb.o Bluetooth.o cam.o tf.o main.o -Wall -pthread -std=c++17 -lstdc++fs -I/usr/include/opencv -L/usr/lib -lopencv_core -lopencv_highgui -lopencv_videoio -lopencv_imgcodecs -lopencv_imgproc

rs232P.o: rs232.cpp
	g++ -O3 -o rs232P.o -c rs232.cpp -Wall

Usb.o: Usb.cpp
	g++ -O3 -o Usb.o -c Usb.cpp -Wall

Bluetooth.o : Bluetooth.cpp
	g++ -O3 -o Bluetooth.o -c Bluetooth.cpp -Wall -pthread -std=c++17

cam.o : cam.cpp
	g++ -O3 -o cam.o -c cam.cpp -std=c++14 -Wall -I/usr/include/opencv -L/usr/lib -lopencv_core -lopencv_highgui -lopencv_videoio -lopencv_imgcodecs -lopencv_imgproc

tf.o : tf.cpp
	g++ -O3 -o tf.o -c tf.cpp -Wall -I/usr/lib/opencv

main.o: main.cpp rs232.hpp Usb.hpp Bluetooth.hpp cam.hpp tf.hpp
	g++ -O3 -o main.o -c main.cpp -Wall -pthread -lstdc++fs -I/usr/include/opencv -L/usr/lib/opencv -lopencv_core -lopencv_highgui -lopencv_videoio -lopencv_imgcodecs -lopencv_imgproc

clean:
	@echo "cleaning"
	rm *.o
	make
