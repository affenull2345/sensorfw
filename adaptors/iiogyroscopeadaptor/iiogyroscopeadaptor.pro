TARGET = iiogyroscopeadaptor

HEADERS += iiogyroscopeadaptor.h \
           iiogyroscopeadaptorplugin.h

SOURCES += iiogyroscopeadaptor.cpp \
           iiogyroscopeadaptorplugin.cpp

CONFIG += qt debug warn_on link_prl link_pkgconfig plugin

include( ../adaptor-config.pri )
