cmake_minimum_required(VERSION 3.5)

project(OASISView VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if(APPLE)
    find_library(FFTW3_LIB NAMES fftw3 PATHS /opt/homebrew/lib)
    find_path(FFTW3_INCLUDE_DIR NAMES fftw3.h PATHS /opt/homebrew/include)
endif()

if(UNIX AND NOT APPLE)
    find_library(FFTW3_LIB NAMES fftw3 PATHS /usr/lib/aarch64-linux-gnu)
    find_path(FFTW3_INCLUDE_DIR NAMES fftw3.h PATHS /usr/include)
endif()

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

set(PROJECT_SOURCES
        main.cpp
        MainWindow.cpp
        MainWindow.h
        OASISView.h
        OASISView.cpp
        OASISParser/OASISElement.h
        OASISParser/OASISParserException.h
        OASISParser/OASISElementParser.h
        OASISParser/OASISElementParser.cpp
        OASISParser/OASISCells.h
        OASISParser/OASISCells.cpp
        OASISParser/OASISData.h
        OASISParser/OASISData.cpp
        OASISParser/OASISCellRecords.h
        OASISParser/OASISCellRecords.cpp
)
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(OASISView
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        Simulator/Simulator.cpp
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET OASISView APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(OASISView SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(OASISView
            ${PROJECT_SOURCES}
        )
    endif()
endif()

if(FFTW3_LIB AND FFTW3_INCLUDE_DIR)
    include_directories(${FFTW3_INCLUDE_DIR})
    target_link_libraries(${PROJECT_NAME} PRIVATE ${FFTW3_LIB})
else()
    message(FATAL_ERROR "FFTW3 not found")
endif()

target_link_libraries(OASISView PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(OASISView PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(OASISView)
endif()
