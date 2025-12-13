TARGET = iioalsadaptor

HEADERS += iioalsadaptor.h \
           iioalsadaptorplugin.h

SOURCES += iioalsadaptor.cpp \
           iioalsadaptorplugin.cpp

CONFIG += qt debug warn_on link_prl link_pkgconfig plugin

include( ../adaptor-config.pri )
