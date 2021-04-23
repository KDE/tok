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
find_package(Td 1.7.3 REQUIRED)

add_library(tokInternal STATIC

  src/client.cpp
  src/client_p.cpp

  src/userdata.cpp

  src/util.cpp

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

)
target_link_libraries(tokInternal
  PUBLIC
    Qt5::Core Qt5::Widgets Qt5::Quick Qt5::Concurrent
    KF5::Kirigami2 KF5::I18n KF5::Notifications
    Td::TdStatic
)

add_executable(org.kde.Tok
  src/main.cpp
  data/main.qrc
)

add_executable(test
  src/test_main.cpp
  src/test_event_feeder.cpp
  data/main.qrc
)

add_definitions(-Wall -Werror)

target_compile_definitions(org.kde.Tok
  PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>
)
target_link_libraries(org.kde.Tok
  PRIVATE
    tokInternal
)
target_link_libraries(test
  PRIVATE
    tokInternal Qt5::QuickTest
)
configure_file(data/test/tst_main.qml tst_main.qml COPYONLY)

install(TARGETS org.kde.Tok DESTINATION ${CMAKE_INSTALL_BINDIR})
install(FILES src/org.kde.Tok.notifyrc DESTINATION ${CMAKE_INSTALL_DATADIR}/knotifications5)
install(FILES src/org.kde.Tok.desktop DESTINATION ${CMAKE_INSTALL_DATADIR}/applications)
