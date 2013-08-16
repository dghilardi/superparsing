unix {
   CONFIG += link_pkgconfig
   PKGCONFIG += opencv \
#    jsoncpp
}
TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

LIBS += \
    -L/usr/local/lib -ldai \
    -lgmpxx -lgmp \
    -lmatio \
    -lhdf5 -lz \
    -ljsoncpp
#    -lopencv_core\
#    -lopencv_highgui\
#    -lopencv_imgproc\
#    -lopencv_video\
#    -lopencv_nonfree\
#    -lopencv_features2d

SOURCES += main.cpp \
    geolabel.cpp \
    image.cpp \
    retrimage.cpp \
    queryimage.cpp \
    OpenCVGraphSegmentation/Segmentation.cpp \
    OpenCVGraphSegmentation/disjoint-set.cpp \
    superpixel.cpp \
    retrievalset.cpp \
    classlikelihood.cpp \
    globlikelihood.cpp \
    neighbourstat.cpp \
    visualutils.cpp

HEADERS += \
    geolabel.h \
    image.h \
    retrimage.h \
    queryimage.h \
    superpixel.h \
    retrievalset.h \
    classlikelihood.h \
    globlikelihood.h \
    debugHeader.h \
    neighbourstat.h \
    visualutils.h

