QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

#QMAKE_LFLAGS_RELEASE = /NODEFAULTLIB:msvcrt.lib
#QMAKE_LFLAGS_DEBUG   = /NODEFAULTLIB:msvcrtd.lib
#QMAKE_LFLAGS_DEBUG   = /NODEFAULTLIB:libcmt.lib

## Avoid conflicts between CUDA and MSVC
#QMAKE_CFLAGS_DEBUG      += /MTd
#QMAKE_CFLAGS_RELEASE    += /MT
#QMAKE_CXXFLAGS_DEBUG    += /MTd
#QMAKE_CXXFLAGS_RELEASE  += /MT


CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    aboutwindow.cpp \
    mainwindow.cpp \
    camerawidget.cpp \
    numberscroll.cpp \
    numberwidget.cpp \
    resultswidget.cpp \
    settingswindow.cpp \
    Camera_handler/camerahandler.cpp \
    Camera_handler/utilityocv.cpp \
    Bluetooth/Bluetooth.cpp \
    Bluetooth/Usb.cpp \
    Bluetooth/rs232.cpp \
    MatrixNN/MNIST_Handler/mnist_handler.cpp \
    MatrixNN/PROJECT_FILES/project_network.cpp \
    MatrixNN/SequenceHandler_STL/utility.cpp \
    MatrixNN/dataset_handler.cpp \
    MatrixNN/layer.cpp \
    MatrixNN/MatrixNN_global.cpp \
    MatrixNN/neuralnetwork.cpp \


HEADERS += \
    Camera_handler/camerahandler.h \
    Camera_handler/utilityocv.h \
    aboutwindow.h \
    camerawidget.h \
    mainwindow.h \
    numberscroll.h \
    numberwidget.h \
    resultswidget.h \
    settingswindow.h \
    Bluetooth/Bluetooth.hpp \
    Bluetooth/Usb.hpp \
    Bluetooth/rs232.hpp \
    MatrixNN/Matrix/Matrix.hpp \
    MatrixNN/MNIST_Handler/mnist_handler.h \
    MatrixNN/PROJECT_FILES/project_network.h \
    MatrixNN/SequenceHandler_STL/utility.h \
    MatrixNN/dataset_handler.h \
    MatrixNN/layer.h \
    MatrixNN/MatrixNN_global.h \
    MatrixNN/neuralnetwork.h \


LIBS += -I/usr/include/opencv
LIBS += -L/usr/lib -lopencv_core -lopencv_highgui -lopencv_videoio -lopencv_imgcodecs -lopencv_imgproc


#CUDA_SOURCES += MatrixNN/Matrix/CUDA_src/CUDA_global.cu \
#                MatrixNN/Matrix/CUDA_src/CUDA_setup.cu \
#                MatrixNN/Matrix/CUDA_src/CUDA_matrix_operators.cu


#HEADERS += MatrixNN/Matrix/CUDA_src/CUDA_global.h \
#           MatrixNN/Matrix/CUDA_src/CUDA_setup.h \
#           MatrixNN/Matrix/CUDA_src/CUDA_matrix_operators.h


#CUDA_DIR = "C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v11.1"
#SYSTEM_NAME = x64              #NB: SYSTEM DEPENDENT
#SYSTEM_TYPE = 64               #SAME HERE
#CUDA_ARCH = sm_61              #Compute capability of the GPU (here GTX Geforce 1050 - Compute capability 6.1 so sm_61)
#NVCC_OPTIONS = --use_fast_math #Cuda compiler options

##Enable CUDA headers
#INCLUDEPATH += $$CUDA_DIR/include

##CUDA librairies headers
#QMAKE_LIBDIR += $$CUDA_DIR/lib/$$SYSTEM_NAME
##Required libraires added here
#LIBS += -lcuda -lcudart -lcublas

## The following makes sure all path names (which often include spaces) are put between quotation marks
#CUDA_INC = $$join(INCLUDEPATH,'" -I"','-I"','"')

##Configuration of the CUDA compiler
#CONFIG(debug, debug|release) {
#       # Debug mode
#       cuda_d.input = CUDA_SOURCES
#       cuda_d.output = $$CUDA_OBJECTS_DIR/${QMAKE_FILE_BASE}_cuda.o
#       cuda_d.commands = $$CUDA_DIR/bin/nvcc.exe -D_DEBUG $$NVCC_OPTIONS $$CUDA_INC $$LIBS --machine $$SYSTEM_TYPE -arch=$$CUDA_ARCH -c -o ${QMAKE_FILE_OUT} ${QMAKE_FILE_NAME}
#       cuda_d.dependency_type = TYPE_C
#       QMAKE_EXTRA_COMPILERS += cuda_d
#}
#else {
#       # Release mode
#       cuda.input = CUDA_SOURCES
#       cuda.output = $$CUDA_OBJECTS_DIR/${QMAKE_FILE_BASE}_cuda.o
#       cuda.commands = $$CUDA_DIR/bin/nvcc.exe $$NVCC_OPTIONS $$CUDA_INC $$LIBS --machine $$SYSTEM_TYPE -arch=$$CUDA_ARCH -c -o ${QMAKE_FILE_OUT} ${QMAKE_FILE_NAME}
#       cuda.dependency_type = TYPE_C
#       QMAKE_EXTRA_COMPILERS += cuda
#}

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    rsc.qrc
