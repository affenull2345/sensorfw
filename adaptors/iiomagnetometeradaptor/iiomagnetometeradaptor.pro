TARGET = iiomagnetometeradaptor

HEADERS += iiomagnetometeradaptor.h \
           iiomagnetometeradaptorplugin.h

SOURCES += iiomagnetometeradaptor.cpp \
           iiomagnetometeradaptorplugin.cpp

CONFIG += qt debug warn_on link_prl link_pkgconfig plugin

include( ../adaptor-config.pri )
