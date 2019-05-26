INCLUDEPATH += . $(PWD)
TEMPLATE = app
CONFIG += release
TARGET = BlueGlow
QT += core network gui widgets multimedia

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += main.cc BlueGlow.cc GithubAPI.cc
HEADERS += BlueGlow.hpp GithubAPI.hpp

RESOURCES += resources/resources.qrc
FORMS += mainwindow.ui
