TEMPLATE = app
TARGET = mans

QT = core  \
     gui

include(common.pro)

HEADERS = \
          deviceinfo/deviceinfo.h    \
          deviceinfo/parser.h        \
          ui/deviceinfowidget.h      \

SOURCES = \
          deviceinfo/deviceinfo.cpp  \
          deviceinfo/parser.cpp      \
          ui/deviceinfowidget.cpp    \
          app/main.cpp

FORMS =   ui/deviceinfowidget.ui
