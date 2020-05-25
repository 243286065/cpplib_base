 
find_path(ASIO_INCLUDE asio.hpp HINTS "${PROJECT_SOURCE_DIR}/src/base/third_party/asio/asio/include" "/usr/include" "/usr/local/include" "/opt/local/include" )

if (ASIO_INCLUDE)
    set(ASIO_FOUND TRUE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DASIO_STANDALONE=1" )

    message( STATUS "Found ASIO include at: ${ASIO_INCLUDE}" )
else ()
    message( FATAL_ERROR "Failed to locate ASIO dependency." )
endif ()