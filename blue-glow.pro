INCLUDEPATH += . $(PWD)
TEMPLATE = app
LIBS += -lutil
CONFIG += release
TARGET = BlueGlow
QT += core network gui widgets

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += main.cc BlueGlow.cc
HEADERS += BlueGlow.hpp

RESOURCES += resources/resources.qrc
FORMS += mainwindow.ui
