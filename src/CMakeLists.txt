target_sources(libqtomm PRIVATE
        ${CMAKE_BINARY_DIR}/config.cpp
        ${CMAKE_BINARY_DIR}/config.h
        )


configure_file(" ${CMAKE_CURRENT_SOURCE_DIR}/config.h.in" "${CMAKE_BINARY_DIR}/config.h")
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/config.cpp.in" "${CMAKE_BINARY_DIR}/config.cpp ")
