unix {
   CONFIG += link_pkgconfig
   PKGCONFIG += opencv
}
TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

LIBS += \
    -L/usr/local/lib -ldai \
    -lgmpxx -lgmp \
    -lmatio \
    -lhdf5 -lz

SOURCES += main.cpp \
    geolabel.cpp

HEADERS += \
    geolabel.h

