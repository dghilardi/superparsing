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
    -ljsoncpp \
    -lboost_thread \
    -lboost_system
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
    Utils/visualutils.cpp \
    mrf.cpp \
    Utils/threadsafestringset.cpp \
    globalSettings.cpp \
    Video/video.cpp \
    Video/queryvideo.cpp \
    Video/supervoxel.cpp \
    Video/supervoxellikelihood.cpp

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
    Utils/visualutils.h \
    mrf.h \
    Utils/threadsafestringset.h \
    globalSettings.h \
    Video/video.h \
    Video/queryvideo.h \
    Video/supervoxel.h \
    Video/supervoxellikelihood.h

OTHER_FILES += instances/instance.json \
    instances/whole-training.json
