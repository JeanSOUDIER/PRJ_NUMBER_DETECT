MAIN: main.o Camera/rs232P.o Camera/Usb.o Camera/Bluetooth.o Camera/cam.o Camera/tf.o
	g++ -O3 -o nn Camera/rs232P.o Camera/Usb.o Camera/Bluetooth.o Camera/cam.o Camera/tf.o main.o -Wall -pthread -std=c++17 -lstdc++fs -I/usr/include/opencv -L/usr/lib -lopencv_core -lopencv_highgui -lopencv_videoio -lopencv_imgcodecs -lopencv_imgproc

rs232P.o: Camera/rs232.cpp
	g++ -O3 -o Camera/rs232P.o -c Camera/rs232.cpp -Wall

Usb.o: Camera/Usb.cpp
	g++ -O3 -o Camera/Usb.o -c Camera/Usb.cpp -Wall

Bluetooth.o : Camera/Bluetooth.cpp
	g++ -O3 -o Camera/Bluetooth.o -c Camera/Bluetooth.cpp -Wall -pthread -std=c++17

cam.o : cam.cpp
	g++ -O3 -o Camera/cam.o -c Camera/cam.cpp -std=c++14 -Wall -I/usr/include/opencv -L/usr/lib -lopencv_core -lopencv_highgui -lopencv_videoio -lopencv_imgcodecs -lopencv_imgproc

tf.o : Camera/tf.cpp
	g++ -O3 -o Camera/tf.o -c Camera/tf.cpp -Wall -I/usr/lib/opencv

main.o: main.cpp Camera/rs232.hpp Camera/Usb.hpp Camera/Bluetooth.hpp Camera/cam.hpp Camera/tf.hpp
	g++ -O3 -o main.o -c main.cpp -Wall -pthread -lstdc++fs -I/usr/include/opencv -L/usr/lib/opencv -lopencv_core -lopencv_highgui -lopencv_videoio -lopencv_imgcodecs -lopencv_imgproc

clean:
	@echo "cleaning"
	rm *.o
	rm Camera/*.o
	make
