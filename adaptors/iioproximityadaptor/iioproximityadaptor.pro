TARGET = iioproximityadaptor

HEADERS += iioproximityadaptor.h \
           iioproximityadaptorplugin.h

SOURCES += iioproximityadaptor.cpp \
           iioproximityadaptorplugin.cpp

CONFIG += qt debug warn_on link_prl link_pkgconfig plugin

include( ../adaptor-config.pri )
