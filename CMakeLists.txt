cmake_minimum_required(VERSION 3.26)
project(Qt_Template)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_PREFIX_PATH "D:/Qt6.6/6.6.0/mingw_64")
set(CMAKE_AUTOUIC_SEARCH_PATHS
        ${CMAKE_CURRENT_SOURCE_DIR}/Forms
)


find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

add_executable(Qt_Template WIN32 main.cpp
        Sources/Login.cpp
        Sources/Login.h
        Forms/widget.ui
        Resources/widget.qrc
        Sources/register.cpp
        Sources/register.h
        Forms/register.ui
        Sources/mainwindow.cpp
        Sources/mainwindow.h
        Forms/mainwindow.ui
        Sources/course.cpp
        Sources/course.h
        Sources/course.cpp
        Sources/Interface.cpp
        Sources/Interface.h
        Sources/raisePlan.cpp
        Sources/raisePlan.h
        Sources/childThread.cpp
        Sources/childThread.h
        Sources/a_mainwindow.cpp
        Sources/a_mainwindow.h
        Forms/a_mainwindow.ui
        Sources/a_mainwindow.h
        Sources/examineraiseplan.cpp
        Sources/examineraiseplan.h
        Forms/examineraiseplan.ui
        Sources/a_childThread.cpp
        Sources/a_childThread.h
        Sources/utils.cpp
        Sources/utils.h
        Sources/courseTable.cpp
        Sources/courseTable.h
        Forms/classtable.ui
        Sources/apply.cpp
        Sources/apply.h
        Sources/examineapply.cpp
        Sources/examineapply.h
        Forms/examineapply.ui
        Sources/editapply.cpp
        Sources/editapply.h
        Forms/editapply.ui
        Sources/userapply.cpp
        Sources/userapply.h
        Forms/userapply.ui
)
target_link_libraries(Qt_Template
        Qt::Core
        Qt::Gui
        Qt::Widgets
)


qt_add_resources(Qt_Template "Resources"
    PREFIX "/"
    FILES
        images/set.png
)


if (WIN32 AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(DEBUG_SUFFIX)
    if (MSVC AND CMAKE_BUILD_TYPE MATCHES "Debug")
        set(DEBUG_SUFFIX "d")
    endif ()
    set(QT_INSTALL_PATH "${CMAKE_PREFIX_PATH}")
    if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
        set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
            set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        endif ()
    endif ()
    if (EXISTS "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
    endif ()
    foreach (QT_LIB Core Gui Widgets)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/bin/Qt6${QT_LIB}${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
    endforeach (QT_LIB)
endif ()
