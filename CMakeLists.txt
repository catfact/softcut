cmake_minimum_required(VERSION 3.7)
project(softcut)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

# hmm...

message("CMAKE_C_FLAGS_DEBUG is ${CMAKE_C_FLAGS_DEBUG}")
message("CMAKE_C_FLAGS_RELEASE is ${CMAKE_C_FLAGS_RELEASE}")
message("CMAKE_C_FLAGS_RELWITHDEBINFO is ${CMAKE_C_FLAGS_RELWITHDEBINFO}")
message("CMAKE_C_FLAGS_MINSIZEREL is ${CMAKE_C_FLAGS_MINSIZEREL}")

set (SRC
        softcut-main.cpp
        SoftCutHeadLogic.cpp
        SubHead.cpp
        SoftCut.cpp
        Commands.cpp
        OscInterface.cpp 
        Resampler.cpp )

if(UNIX)
    if(APPLE)
        list (APPEND SRC PaWrapper.cpp)
    else()
        list (APPEND SRC JackWrapper.cpp)
    endif()
else()
    # nope
endif()

add_executable(softcut ${SRC})

if(UNIX)
    if(APPLE)
        find_library(coreaudio_lib CoreAudio REQUIRED)
        find_library(audiotoolbox_lib AudioToolbox REQUIRED)
        find_library(audiounit_lib AudioUnit REQUIRED)
        find_library(carbon_lib Carbon REQUIRED)

        find_library(lo_lib liblo.dylib REQUIRED)
        find_library(portaudio_lib portaudio.a REQUIRED)

        include_directories(softcut /usr/local/include)
        target_link_libraries( softcut ${coreaudio_lib} ${audiotoolbox_lib} ${audiounit_lib} ${carbon_lib} ${portaudio_lib})
    else()
        find_library(lo_lib liblo.so REQUIRED)
        target_link_libraries(softcut jack)
        target_link_libraries(softcut pthread)
        target_link_libraries(softcut asound)
    endif()
else()
    # nope
endif()

target_link_libraries(softcut ${lo_lib})

set_target_properties(softcut PROPERTIES
        CXX_STANDARD 14
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
        )
