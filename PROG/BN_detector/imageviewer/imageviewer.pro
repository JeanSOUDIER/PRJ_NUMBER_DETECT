QT += widgets
qtHaveModule(printsupport): QT += printsupport

HEADERS       = imageviewer.h \
    Camera/cam.hpp \
    Camera/tf.hpp \
    Camera/Bluetooth.hpp \
    Camera/rs232.hpp \
    Camera/Usb.hpp
SOURCES       = imageviewer.cpp \
                main.cpp \
    Camera/cam.cpp \
    Camera/tf.cpp \
    Camera/Bluetooth.cpp \
    Camera/rs232.cpp \
    Camera/Usb.cpp

LIBS += -I/usr/include/opencv
LIBS += -L/usr/lib -lopencv_core -lopencv_highgui -lopencv_videoio -lopencv_imgcodecs -lopencv_imgproc

# install
target.path = $$[QT_INSTALL_EXAMPLES]/widgets/widgets/imageviewer
INSTALLS += target
