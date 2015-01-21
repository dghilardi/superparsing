INCLUDEPATH += "/usr/include/qt4"
CONFIG+=qtAndOpenGL
qtAndOpenGL {
    QT += core gui opengl
    CONFIG += console
    TARGET = superparsing
    DEFINES += QT_AND_OPENGL

    SOURCES += Utils3d/geometryengine.cpp \
        Utils3d/openglwidget.cpp \
        Utils3d/openglwindow.cpp \
        Utils3d/tridimensionalobject.cpp

    HEADERS += Utils3d/geometryengine.h \
        Utils3d/openglwidget.h \
        Utils3d/openglwindow.h \
        Utils3d/tridimensionalobject.h

    RESOURCES += Utils3d/shaders.qrc

    FORMS += Utils3d/openglwindow.ui

}else{
    CONFIG += console
    CONFIG -= app_bundle
    CONFIG -= qt
}

TEMPLATE = app

unix {
   CONFIG += link_pkgconfig
   PKGCONFIG += opencv \
#    jsoncpp
}

LIBS += \
    -L/usr/local/lib -ldai \
    -lgmpxx -lgmp \
    -lmatio \
    -lhdf5 -lz \
    -ljsoncpp \
    -lboost_thread \
    -lboost_system \
#    -lopencv_core\
#    -lopencv_highgui \
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
    Utils/printutils.cpp \
    mrf.cpp \
    Utils/threadsafestringset.cpp \
    globalSettings.cpp \
    Video/video.cpp \
    Video/queryvideo.cpp \
    Video/supervoxel.cpp \
    Video/supervoxellikelihood.cpp \
    quantizedsift.cpp \
    dictionarytraining.cpp \
    GCO/maxflow.cpp \
    GCO/LinkedBlockList.cpp \
    GCO/graph.cpp \
    GCO/GCoptimization.cpp \
    Utils3d/tridimensionalvoxel.cpp \
    Utils3d/poly2tri/common/shapes.cc \
    Utils3d/poly2tri/sweep/advancing_front.cc \
    Utils3d/poly2tri/sweep/cdt.cc \
    Utils3d/poly2tri/sweep/sweep_context.cc \
    Utils3d/poly2tri/sweep/sweep.cc

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
    Utils/printutils.h \
    mrf.h \
    mrf.tcc \
    Utils/threadsafestringset.h \
    globalSettings.h \
    Video/video.h \
    Video/queryvideo.h \
    Video/supervoxel.h \
    Video/supervoxellikelihood.h \
    mrfnode.h \
    quantizedsift.h \
    dictionarytraining.h \
    GCO/LinkedBlockList.h \
    GCO/graph.h \
    GCO/GCoptimization.h \
    GCO/energy.h \
    GCO/block.h \
    Utils3d/tridimensionalvoxel.h \
    Utils3d/poly2tri/poly2tri.h \
    Utils3d/poly2tri/common/shapes.h \
    Utils3d/poly2tri/common/utils.h \
    Utils3d/poly2tri/sweep/advancing_front.h \
    Utils3d/poly2tri/sweep/cdt.h \
    Utils3d/poly2tri/sweep/sweep_context.h \
    Utils3d/poly2tri/sweep/sweep.h

OTHER_FILES += instances/*

