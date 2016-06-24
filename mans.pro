TEMPLATE = app
TARGET = mans

QT = core  \
     gui

include(common.pro)

HEADERS = \
          deviceinfo_parser/deviceinfo.h    \
          deviceinfo_parser/parser.h        \
          ui/deviceinfowidget.h             \

SOURCES = \
          deviceinfo_parser/deviceinfo.cpp  \
          deviceinfo_parser/parser.cpp      \
          ui/deviceinfowidget.cpp           \
          app/main.cpp

FORMS =   ui/deviceinfowidget.ui
