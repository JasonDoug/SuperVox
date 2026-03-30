cmake_minimum_required(VERSION 3.10.2)
project("supervox")

include(FetchContent)
FetchContent_Declare(
    oboe
    GIT_REPOSITORY https://github.com/google/oboe
    GIT_TAG main
)
FetchContent_MakeAvailable(oboe)

# Define shared C++ directories
set(SHARED_CPP_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../../../cpp)

# Include directories
include_directories(
    ${SHARED_CPP_DIR}/audio
    ${SHARED_CPP_DIR}/ml
    ${SHARED_CPP_DIR}/dsp
)

add_library(
    supervox
    SHARED
    native-lib.cpp
    AndroidAudioEngine.cpp
    ${SHARED_CPP_DIR}/ml/TFLiteMLEngine.cpp
)

find_library(
    log-lib
    log
)

target_link_libraries(
    supervox
    oboe
    ${log-lib}
)
