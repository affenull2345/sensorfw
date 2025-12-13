TARGET = iioaccelerometeradaptor

HEADERS += iioaccelerometeradaptor.h \
           iioaccelerometeradaptorplugin.h

SOURCES += iioaccelerometeradaptor.cpp \
           iioaccelerometeradaptorplugin.cpp

CONFIG += qt debug warn_on link_prl link_pkgconfig plugin

include( ../adaptor-config.pri )
