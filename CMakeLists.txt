cmake_minimum_required(VERSION 3.12)
project(softcut)


add_executable(softcut main.cpp SoftCutHeadLogic.cpp SubHead.cpp PaWrapper.cpp PaWrapper.h SoftCut.cpp SoftCut.h Commands.cpp Commands.h)


find_library(portaudio_lib libportaudio.a REQUIRED)
target_link_libraries(softcut ${portaudio_lib})

if(APPLE)
    find_library(coreaudio_lib CoreAudio REQUIRED)
    find_library(audiotoolbox_lib AudioToolbox REQUIRED)
    find_library(audiounit_lib AudioUnit REQUIRED)
    find_library(carbon_lib Carbon REQUIRED)
    target_link_libraries( softcut ${coreaudio_lib} ${audiotoolbox_lib} ${audiounit_lib} ${carbon_lib})
endif()


include_directories(softcut /usr/local/include)

set_target_properties(softcut PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
        )
