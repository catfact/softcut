cmake_minimum_required(VERSION 3.12)
project(softcut)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

#------------------------
# -- main audio program

add_executable(softcut softcut-main.cpp SoftCutHeadLogic.cpp SubHead.cpp PaWrapper.cpp PaWrapper.h SoftCut.cpp SoftCut.h Commands.cpp Commands.h OscInterface.cpp OscInterface.h)

find_library(portaudio_lib libportaudio.a REQUIRED)
target_link_libraries(softcut ${portaudio_lib} ${lo_lib})

if(APPLE)
    find_library(coreaudio_lib CoreAudio REQUIRED)
    find_library(audiotoolbox_lib AudioToolbox REQUIRED)
    find_library(audiounit_lib AudioUnit REQUIRED)
    find_library(carbon_lib Carbon REQUIRED)
    target_link_libraries( softcut ${coreaudio_lib} ${audiotoolbox_lib} ${audiounit_lib} ${carbon_lib})
endif()

if(APPLE)
    find_library(lo_lib liblo.dylib REQUIRED)
else()
    find_library(lo_lib liblo.so REQUIRED)
endif()
target_link_libraries(softcut ${lo_lib})

include_directories(softcut /usr/local/include)

set_target_properties(softcut PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS YES
        )

#----------------------
# -- controller (OSC client)

add_executable(softcut-client client/client-main.cpp)
target_link_libraries(softcut-client ${lo_lib})
include_directories(softcut-client /usr/local/include)
