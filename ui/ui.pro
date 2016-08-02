TEMPLATE = lib
TARGET = deviceinfo_ui

QT = core  \
     gui

include($$SRC/common.pri)

HEADERS = deviceinfowidget.h      

SOURCES = deviceinfowidget.cpp    

FORMS =   deviceinfowidget.ui
