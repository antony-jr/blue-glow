CMAKE_MINIMUM_REQUIRED(VERSION 3.2)
project(BlueGlow)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

find_package(Qt5Widgets)
find_package(Qt5Gui)
find_package(Qt5Qml)
find_package(Qt5QuickControls2)
find_package(Qt5Multimedia)
find_package(Qt5Network)

# Include Directories.
include_directories(${CMAKE_BINARY_DIR})

add_executable(BlueGlow main.cc
   			SettingsManager.cc
			Backend.cc
			GithubAPI.cc
			SettingsManager.hpp
			Backend.hpp
			GithubAPI.hpp
			resources/resources.qrc 
			qml/qml.qrc)

target_link_libraries(BlueGlow PUBLIC 
   			       Qt5::Widgets
			       Qt5::Gui 
			       Qt5::Qml 
			       Qt5::QuickControls2
			       Qt5::Multimedia
			       Qt5::Network)
