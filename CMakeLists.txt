#
# WARNING: this is only provided for those who absolutely do not have access
# to Qbs. this build script is intentionally barebones and does not have
# all the utilities of the Qbs one, in particular, full installation.
#

cmake_minimum_required(VERSION 3.5)

project(Tok LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC OFF)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(GNUInstallDirs)

set(REQUIRED_QT_VERSION 5.15.0)

find_package(Qt5 COMPONENTS Core Quick QuickTest Widgets Concurrent REQUIRED)
find_package(KF5Kirigami2 REQUIRED)
find_package(KF5I18n REQUIRED)
find_package(KF5Notifications REQUIRED)
find_package(Td 1.7.4 REQUIRED)
find_package(ICU 61.0 COMPONENTS uc REQUIRED)

add_executable(org.kde.Tok
  src/main.cpp

  src/client.cpp
  src/client_p.cpp

  src/userdata.cpp

  src/util.cpp
  src/utilities.cpp

  src/chatsmodel.cpp
  src/chatsort.cpp

  src/messagesmodel.cpp
  src/messagesstore.cpp
  src/chatsstore.cpp

  src/extractinator.cpp
  src/notificationmanager.cpp

  src/tgimageprovider.cpp

  src/setup.cpp

  src/internallib/qabstractrelationalmodel.cpp
  src/internallib/qquickrelationallistener.cpp

  data/main.qrc
)

target_link_libraries(org.kde.Tok
  PUBLIC
    Qt5::Core Qt5::Widgets Qt5::Quick Qt5::Concurrent
    KF5::Kirigami2 KF5::I18n KF5::Notifications
    Td::TdStatic ICU::uc
)

install(TARGETS org.kde.Tok DESTINATION ${CMAKE_INSTALL_BINDIR})
