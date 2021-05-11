QT += core gui widgets

CONFIG += c++11
CONFIG -= app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

DEPENDPATH += . ../../cnc_lib
INCLUDEPATH += ../../cnc_lib
LIBS += -L../../debug -lcnc_lib

SOURCES += \
    cncmainwindow.cpp \
    main.cpp

HEADERS += \
    cncmainwindow.h

FORMS +=

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
