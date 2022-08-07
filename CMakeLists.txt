cmake_minimum_required(VERSION 3.21)

# Suppresses a warning in cmake 3.x concerning a currently unused feature (setting VERSION in project() command)
if (POLICY CMP0048)
    cmake_policy(SET CMP0048 NEW)
endif (POLICY CMP0048)

project(qtomm VERSION 0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(mode "Debug")

option(BUILD_TRANSLATION "Build translations of the application." ON)
option(USE_QT_5_12 "Allow to use Qt 5.12. Set this option to true for static analyzing only.
                    Builds with this configuration are not supposed to be run."
        OFF
        )
option(WERROR "Error on compiler warnings. Not available for MSVC." ON)

set(CMAKE_PREFIX_PATH "D:/Qt/Qt5.12.10/5.12.10/msvc2017_64/lib/cmake")

find_package(Qt5 COMPONENTS
        Core
        Gui
        Widgets
        svg
        REQUIRED)

set(Skia_FOUND TRUE)

if (${mode} AND ${mode} STREQUAL "Debug")
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/skia/Debug-x64/)
    link_directories(${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/skia/Debug-x64/out/Debug-x64)
    message("${mode} mode: " "path: ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/skia/Debug-x64/")
else ()
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/skia/Release-x64/)
    link_directories(${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/skia/Release-x64/out/Release-x64)
    message("${mode} mode: " "path: ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/skia/Release-x64/")
endif ()

add_executable(qtomm src/main/main.cpp)
set_target_properties(qtomm PROPERTIES WIN32_EXCUTABLE ON)
add_library(libqtomm STATIC)
target_include_directories(qtomm PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/skia/Release-x64/include)
target_link_libraries(qtomm
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        Qt5::Svg
        )
target_link_libraries(qtomm libqtomm)

#skia
target_link_libraries(qtomm d3d12allocator.lib
        expat.lib
        freetype2.lib
        harfbuzz.lib
        icu.lib
        libpng.lib
        particles.lib
        skia.lib
        skottie.lib
        skparagraph.lib
        skresources.lib
        sksg.lib
        skshaper.lib
        spirv_cross.lib
        svg.lib
        zlib.lib
        )

if (${mode} AND ${mode} STREQUAL "Debug")
    target_link_libraries(qtomm spvtools.lib
            spvtools_val.lib
            )
endif ()

message("bin path: " ${CMAKE_BINARY_DIR})
message("current src path: " ${CMAKE_SOURCE_DIR})

if (WIN32)
    set(DEBUG_SUFFIX)
    if (CMAKE_BUILD_TYPE MATCHES "Debug")
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
                "${QT_INSTALL_PATH}/bin/Qt5${QT_LIB}${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
    endforeach (QT_LIB)
endif ()
