cmake_minimum_required(VERSION 3.8)

###################################################################################################
# General project stuff
###################################################################################################
set(PROJECT_NAME low_latency_demo)
project(${PROJECT_NAME})

###################################################################################################
# Include stuff
###################################################################################################
include_directories(source)

###################################################################################################
# Compiler flags
###################################################################################################
if(WIN32)
    # General
    set(CMAKE_CXX_FLAGS "/MP /analyze- /W4 /Zc:wchar_t /Gm- /sdl- /Zc:inline /fp:fast /DWIN32 /D_WINDOWS /D_UNICODE /DUNICODE /DNOMINMAX /D_CRT_SECURE_NO_WARNINGS /errorReport:prompt /WX /Zc:forScope /Gd /Oy- /std:c++17 /openmp /EHsc /nologo /diagnostics:classic /DYNAMICBASE /ERRORREPORT:PROMPT")
    # Release
    set(CMAKE_CXX_FLAGS_RELEASE "/Zi /DNDEBUG /GL /O2 /Ob2 /Oi /Ot /MT /LTCG:incremental /DEBUG /OPT:REF /SAFESEH /OPT:ICF")
    # Debug
    set(CMAKE_CXX_FLAGS_DEBUG "/ZI /Od /RTC1 /MTd /DEBUG /INCREMENTAL")
else()
    # General
    set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror -std=c++1z -fpermissive -lpthread")
    # Release
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
    # Debug
    set(CMAKE_CXX_FLAGS_DEBUG "-g3 -D_DEBUG")
endif()

###################################################################################################
# Executable stuff
###################################################################################################
set(SOURCES source/main.cpp)
add_executable(${PROJECT_NAME} ${SOURCES})
