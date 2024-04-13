cmake_minimum_required(VERSION 3.5)

project(Ather VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)



set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)
set( REQUIRED_LIBS Core Gui Widgets Network WebSockets Sql )
set( REQUIRED_LIBS_QUALIFIED Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Network Qt5::WebSockets Qt5::Sql )
find_package( Qt${QT_VERSION_MAJOR} COMPONENTS ${REQUIRED_LIBS} REQUIRED )
find_package( spdlog REQUIRED )
set(CMAKE_CPP_COMPILER /usr/bin/x86_64-w64-mingw32-g++)
set(CMAKE_C_COMPILER /usr/bin/x86_64-w64-mingw32-gcc)
set(CMAKE_CXX_FLAGS "-fpermissive")

include_directories( Include )


set(Ather_INCLUDE
   Include/global.hpp
   Include/athermainwindow.hpp
   Include/listener.hpp
   Include/helper/colortext.hpp
   Include/helper/wmessagebox.hpp
   Include/transport/connector.hpp
   Include/transport/packager.hpp
   Include/helper/macro.hpp
   Include/login.hpp
   Include/interacttab.hpp
   Include/command/command.h
   Include/timecell.h
   Include/helper/base.hpp
   Include/addlistener.hpp
   Include/generatepayload.h
   Include/setlistener.h
   Include/c2config.h
   Include/generatewaiting.h
   Include/about.h
    )

set(Ather_UI

    UserInterface/athermainwindow.ui
    UserInterface/login.ui
    UserInterface/interacttab.ui
    UserInterface/listener.ui
    UserInterface/addlistener.ui
    UserInterface/generatepayload.ui
    UserInterface/setlistener.ui
    UserInterface/c2config.ui
    UserInterface/about.ui
    UserInterface/generatewaiting.ui

    )


set(Ather_SOURCE
        Source/global.cpp
        Source/main.cpp
        Source/athermainwindow.cpp
        Source/login.cpp
        Source/listener.cpp
        Source/helper/colortext.cpp
        Source/helper/wmessagebox.cpp
        Source/transport/connector.cpp
        Source/transport/packager.cpp
        Source/interacttab.cpp
        Source/command/command.cpp
        Source/command/commandlist.cpp
        Source/command/commandexecute.cpp
        Source/timecell.cpp
        Source/helper/base.cpp
        Source/addlistener.cpp
        Source/generatepayload.cpp
        Source/setlistener.cpp
        Source/c2config.cpp
        Source/about.cpp
        Source/generatewaiting.cpp
        rc.qrc
    )


set(CMAKE_AUTOUIC OFF)
qt5_wrap_ui(UI_HEADERS ${Ather_UI})
set(CMAKE_AUTOUIC_SEARCH_PATHS "/UserInterface/")
add_executable(Ather ${Ather_INCLUDE}  ${Ather_SOURCE} ${Ather_UI})


target_link_libraries(Ather PRIVATE Qt${QT_VERSION_MAJOR}::Widgets ${REQUIRED_LIBS_QUALIFIED} spdlog::spdlog spdlog::spdlog_header_only)


set_target_properties(Ather PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Ather)
endif()
