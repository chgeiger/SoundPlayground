cmake_minimum_required(VERSION 3.16)

# Set some basic project attributes
project (SoundPlayground
	VERSION 0.1
	DESCRIPTION "A Sound Playground with Qt6 Graphics")

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Qt6 configuration
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Find Qt6
find_package(Qt6 COMPONENTS Core Gui Widgets REQUIRED)

# Create a simple configuration header
configure_file(config.h.in config.h)

# This project will output an executable file
add_executable(${PROJECT_NAME}
	SoundPlayground.cpp
	MainWindow.h
	MainWindow.cpp
	AudioModule.h
	AudioModule.cpp
	AudioPort.h
	AudioPort.cpp
	AudioConnection.h
	AudioConnection.cpp)

# Link Qt6 libraries
target_link_libraries(${PROJECT_NAME} PUBLIC
	Qt6::Core
	Qt6::Gui
	Qt6::Widgets)

# Include the configuration header in the build
target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_BINARY_DIR}")
