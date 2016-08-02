TEMPLATE = app
TARGET = mans

QT = core  \
     gui

include($$SRC/common.pri)

SOURCES = main.cpp

LIBS += \
        -ldeviceinfo \
        -ldeviceinfo_ui
