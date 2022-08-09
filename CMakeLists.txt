cmake_minimum_required(VERSION 3.21)

# Suppresses a warning in cmake 3.x concerning a currently unused feature (setting VERSION in project() command)
if (POLICY CMP0048)
    cmake_policy(SET CMP0048 NEW)
endif (POLICY CMP0048)

project(qtomm VERSION 0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

#MSVC编译UTF8文件
add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")


option(BUILD_TRANSLATION "Build translations of the application." ON)
option(WERROR "Error on compiler warnings. Not available for MSVC." ON)

#QT路径
set(CMAKE_PREFIX_PATH "D:/Qt/Qt5.12.10/5.12.10/msvc2017_64/lib/cmake")

find_package(Qt5 COMPONENTS
        Core
        Gui
        Widgets
        svg
        LinguistTools
        REQUIRED)

set(Skia_FOUND TRUE)
if (WIN32)
    if (CMAKE_BUILD_TYPE MATCHES "Debug")
        set(IMPORTED_LOCATION_TYPE "debug")
        include_directories(${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/skia/Debug-x64/)
        link_directories(${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/skia/Debug-x64/out/Debug-windows-x64)
        message("${CMAKE_BUILD_TYPE} mode: " "path: ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/skia/Debug-x64/")
    else ()
        set(IMPORTED_LOCATION_TYPE "optimized")
        include_directories(${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/skia/Release-x64/)
        link_directories(${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/skia/Release-x64/out/Release-windows-x64)
        message("${CMAKE_BUILD_TYPE} mode: " "path: ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/skia/Release-x64/")
    endif ()
endif ()

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/" ${CMAKE_MODULE_PATH})
include(GetGitRevisionDescription)
git_describe(GIT_DESCRIBE "--tags")

#qrc资源
file(GLOB_RECURSE QRC_SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.qrc)
set(QRC_SOURCE_FILES resources.qrc)
qt5_add_resources(QRC_FILES ${QRC_SOURCE_FILES})

#编译
add_executable(qtomm src/main/main.cpp ${QRC_FILES})
set_target_properties(qtomm PROPERTIES WIN32_EXCUTABLE ON)
add_library(libqtomm STATIC)

macro(set_warning_level target)
    if (MSVC)
        target_compile_options(${target} PRIVATE /W4 /WX-)
    else ()
        target_compile_options(${target} PRIVATE -Wall -Wextra -pedantic)
        if (WERROR)
            target_compile_options(${target} PRIVATE -Werror)
        endif ()
    endif ()
endmacro()
set_warning_level(qtomm)
set_warning_level(libqtomm)

target_link_libraries(libqtomm
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        Qt5::Svg
        )
target_link_libraries(qtomm libqtomm)

#skia
if (WIN32)
    target_link_libraries(libqtomm ${IMPORTED_LOCATION_TYPE}
            d3d12allocator.lib
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
            DXGI.lib
            D3D12.lib
            Opengl32.lib
            d3dcompiler.lib
            )

    if (CMAKE_BUILD_TYPE MATCHES "Debug")
        target_link_libraries(libqtomm ${IMPORTED_LOCATION_TYPE}
                spvtools.lib
                spvtools_val.lib
                )
    endif ()

endif () #skia

message("bin path: " ${CMAKE_BINARY_DIR})
message("current src path: " ${CMAKE_SOURCE_DIR})

set(cfg_files
        ${CMAKE_SOURCE_DIR}/uicolors/ui-colors-light.cfg
        ${CMAKE_SOURCE_DIR}/uicolors/ui-colors-dark.cfg
        ${CMAKE_SOURCE_DIR}/keybindings/default_keybindings.cfg
        )

target_include_directories(libqtomm PUBLIC
        "${CMAKE_SOURCE_DIR}/src"
        "${PROJECT_BINARY_DIR}"
        )

add_subdirectory(src)
#添加额外的 源文件 例如资源文件等
LIST(APPEND SRCS resources.qrc)

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