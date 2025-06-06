cmake_minimum_required(VERSION 3.27)

project(
  Mail
  VERSION 0.1
  LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Quick Sql)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Quick)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Quick Sql) # 添加 Sql
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Quick Sql)
set(PROJECT_SOURCES main.cpp qml.qrc)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
  qt_add_executable(
    Mail
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
    writemail.cpp
    writemail.h
    mailuser.h
    mailuser.cpp
    MailModel.cpp
    MailModel.h
    DatabaseManager.cpp
    DatabaseManager.h)
  # Define target properties for Android with Qt 6 as: set_property(TARGET Mail
  # APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
  # ${CMAKE_CURRENT_SOURCE_DIR}/android) For more information, see
  # https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
  if(ANDROID)
    add_library(Mail SHARED ${PROJECT_SOURCES})
    # Define properties for Android with Qt 5 after find_package() calls as:
    # set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
  else()
    add_executable(Mail ${PROJECT_SOURCES})
  endif()
endif()

target_link_libraries(
  Mail PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Quick
               Qt${QT_VERSION_MAJOR}::Sql)

# MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1. If you are developing
# for iOS or macOS you should consider setting an explicit, fixed bundle
# identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.Mail)
endif()
set_target_properties(
  Mail
  PROPERTIES ${BUNDLE_ID_OPTION} MACOSX_BUNDLE_BUNDLE_VERSION
             ${PROJECT_VERSION} MACOSX_BUNDLE_SHORT_VERSION_STRING
             ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR} MACOSX_BUNDLE
             TRUE WIN32_EXECUTABLE
             TRUE)

include(GNUInstallDirs)
install(
  TARGETS Mail
  BUNDLE DESTINATION .
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
  qt_import_qml_plugins(Mail)
  qt_finalize_executable(Mail)
endif()
