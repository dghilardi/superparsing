unix {
   CONFIG += link_pkgconfig
   PKGCONFIG += opencv
}
TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp

