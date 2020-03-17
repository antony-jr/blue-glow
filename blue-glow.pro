INCLUDEPATH += . $(PWD)
TEMPLATE = app
CONFIG += release
TARGET = BlueGlow
QT += core network gui widgets qml quickcontrols2

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += main.cc 
# BlueGlow.cc GithubAPI.cc
# HEADERS += BlueGlow.hpp GithubAPI.hpp

RESOURCES += resources/resources.qrc qml/qml.qrc
