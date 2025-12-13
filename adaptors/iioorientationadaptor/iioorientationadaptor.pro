TARGET = iioorientationadaptor

HEADERS += iioorientationadaptor.h \
           iioorientationadaptorplugin.h

SOURCES += iioorientationadaptor.cpp \
           iioorientationadaptorplugin.cpp

CONFIG += qt debug warn_on link_prl link_pkgconfig plugin

include( ../adaptor-config.pri )
