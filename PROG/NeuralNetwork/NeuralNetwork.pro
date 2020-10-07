QT -= gui
QT += core

TEMPLATE = lib
DEFINES += NEURALNETWORK_LIBRARY

CONFIG += c++14

#QMAKE_LFLAGS_RELEASE =/NODEFAULTLIB:msvcrt.lib
#QMAKE_LFLAGS_DEBUG =/NODEFAULTLIB:msvcrtd.lib
#QMAKE_LFLAGS_RELEASE = /NODEFAULTLIB:msvcrt.lib
#QMAKE_LFLAGS_DEBUG   = /NODEFAULTLIB:msvcrtd.lib
#QMAKE_LFLAGS_DEBUG   = /NODEFAULTLIB:libcmt.lib


#QMAKE_CFLAGS_DEBUG      += /MTd
#QMAKE_CFLAGS_RELEASE    += /MT
#QMAKE_CXXFLAGS_DEBUG    += /MTd
#QMAKE_CXXFLAGS_RELEASE  += /MT

#DESTDIR = release
#OBJECTS_DIR = release/obj
#CUDA_OBJECTS_DIR = release/cuda

#debug {
#    NVCCOPTIONS += -g -G
#}

# CUDA settings
#CUDA_SOURCES += cuda_code.cu
#OTHER_FILES += CUDA_SOURCES

#CUDA_DIR = "C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v11.1"
#SYSTEM_NAME     = x64
#CUDA_INC += $$CUDA_DIR\include

# Path to header and libs files
#INCLUDEPATH  += $$CUDA_DIR/include
#QMAKE_LIBDIR += $$CUDA_DIR/lib/Win32
#QMAKE_LIBDIR += $$CUDA_DIR/lib/$$SYSTEM_NAME \
#LIBS += -lcudart -lcuda

## GPU architecture
#CUDA_ARCH     = sm_60  #Compute capability

## Prepare the extra compiler configuration
#CUDA_INC = $$join(INCLUDEPATH,' -I','-I',' ')

#INCLUDEPATH += $$CUDA_INC


#cuda.commands = $${CUDA_DIR}/bin/nvcc -m32 --gpu-architecture=$$CUDA_ARCH -c \
#                $$CUDA_INC $$LIBS  ${QMAKE_FILE_NAME} -o ${QMAKE_FILE_OUT}

#cuda.dependency_type = TYPE_C
#cuda.depend_command = $$(CUDA_DIR)/bin/nvcc $$CUDA_INC ${QMAKE_FILE_NAME}

#cuda.input = CUDA_SOURCES
#cuda.output = ../../$${OBJECTS_DIR}/${QMAKE_FILE_BASE}_cuda.obj


#QMAKE_EXTRA_COMPILERS += cuda

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    convolutionalnn.cpp \
    layer.cpp \
    neuralnetwork.cpp \
    neuron.cpp

HEADERS += \
    NeuralNetwork_global.h \
    convolutionalnn.h \
    layer.h \
    neuralnetwork.h \
    neuron.h


# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target
