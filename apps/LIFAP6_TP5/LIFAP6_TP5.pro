QT += gui core widgets network

CONFIG += c++11

DEFINES += PRECISION FLOAT
DEFINES += CNC_OPENGL

DEPENDPATH += . ../../cnc_lib
INCLUDEPATH += ../../cnc_lib
LIBS += -L../../debug -L../../cnc_lib -lcnc_lib

SOURCES += \
        hash_table.cpp \
        main.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    hash_table.h
