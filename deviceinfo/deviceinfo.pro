TEMPLATE = lib
TARGET = deviceinfo

QT = core

include($$SRC/common.pri)

HEADERS = \
          deviceinfo.h    \
          parser.h

SOURCES = \
          deviceinfo.cpp  \
          parser.cpp
