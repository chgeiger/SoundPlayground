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

# Try to find PortAudio (preferred: pkg-config, fallback: system paths)
set(SOUNDPLAYGROUND_HAVE_PORTAUDIO 0)
set(PORTAUDIO_INCLUDE_DIRS "")
set(PORTAUDIO_LIBRARIES "")

find_package(PkgConfig QUIET)
if(PkgConfig_FOUND)
	pkg_check_modules(PORTAUDIO_PC QUIET portaudio-2.0)
	if(PORTAUDIO_PC_FOUND)
		set(SOUNDPLAYGROUND_HAVE_PORTAUDIO 1)
		set(PORTAUDIO_INCLUDE_DIRS ${PORTAUDIO_PC_INCLUDE_DIRS})
		set(PORTAUDIO_LIBRARIES ${PORTAUDIO_PC_LINK_LIBRARIES})
	endif()
endif()

if(NOT SOUNDPLAYGROUND_HAVE_PORTAUDIO)
	find_path(PORTAUDIO_INCLUDE_DIR portaudio.h)
	find_library(PORTAUDIO_LIBRARY NAMES portaudio)

	if(PORTAUDIO_INCLUDE_DIR AND PORTAUDIO_LIBRARY)
		set(SOUNDPLAYGROUND_HAVE_PORTAUDIO 1)
		set(PORTAUDIO_INCLUDE_DIRS ${PORTAUDIO_INCLUDE_DIR})
		set(PORTAUDIO_LIBRARIES ${PORTAUDIO_LIBRARY})
	endif()
endif()

if(SOUNDPLAYGROUND_HAVE_PORTAUDIO)
	message(STATUS "PortAudio gefunden und wird eingebunden")
else()
	message(WARNING "PortAudio nicht gefunden. Audio-Out-Funktionen bleiben deaktiviert, bis PortAudio installiert ist.")
endif()

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
	AudioConnection.cpp
	SinusGeneratorModule.h
	SinusGeneratorModule.cpp
	AudioOutModule.h
	AudioOutModule.cpp
	CustomGraphicsScene.h
	CustomGraphicsScene.cpp)

# Link Qt6 libraries
target_link_libraries(${PROJECT_NAME} PUBLIC
	Qt6::Core
	Qt6::Gui
	Qt6::Widgets)

if(SOUNDPLAYGROUND_HAVE_PORTAUDIO)
	target_include_directories(${PROJECT_NAME} PRIVATE ${PORTAUDIO_INCLUDE_DIRS})
	target_link_libraries(${PROJECT_NAME} PRIVATE ${PORTAUDIO_LIBRARIES})
endif()

# Include the configuration header in the build
target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_BINARY_DIR}")
