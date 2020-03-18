INCLUDEPATH += . $(PWD)
TEMPLATE = app
CONFIG += release
TARGET = BlueGlow
QT += core network gui widgets qml quickcontrols2 multimedia

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += main.cc SettingsManager.cc Backend.cc GithubAPI.cc
HEADERS += SettingsManager.hpp Backend.hpp GithubAPI.hpp

RESOURCES += resources/resources.qrc qml/qml.qrc
