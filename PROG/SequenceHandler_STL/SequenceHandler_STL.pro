CONFIG -= qt

LIBS += -lstdc++fs

TEMPLATE = lib
DEFINES += SEQUENCEHANDLER_STL_LIBRARY

CONFIG += c++17

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
    movement.cpp \
    sequence.cpp \
    sequencehandler.cpp \
    sequencereader.cpp \
    sequencewriter.cpp \
    utility.cpp

HEADERS += \
    SequenceHandler_STL_global.h \
    movement.h \
    sequence.h \
    sequencehandler.h \
    sequencereader.h \
    sequencewriter.h \
    utility.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target