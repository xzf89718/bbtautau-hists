cmake_minimum_required(VERSION 3.2)

project(bbtautau-hists LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wpedantic -Wextra")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
SET(CMAKE_C_ARCHIVE_FINISH   "<CMAKE_RANLIB> -no_warning_for_no_symbols -c <TARGET>")
SET(CMAKE_CXX_ARCHIVE_FINISH "<CMAKE_RANLIB> -no_warning_for_no_symbols -c <TARGET>")

find_package(ROOT REQUIRED COMPONENTS Core Hist Tree RIO Net)
include_directories(SYSTEM ${ROOT_INCLUDE_DIR})
find_package(Boost)
if(Boost_FOUND)
    message(${Boost_INCLUDE_DIRS})
    include_directories(${Boost_INCLUDE_DIRS})
endif()

add_library(UtilsLib SHARED
    Utils.cpp Utils.h)

add_library(ConfigLib SHARED
    Config.cpp Config.h
    Processes.cpp Processes.h
    Regions.cpp Regions.h
    Variables.cpp Variables.h)

add_library(HistToolLib SHARED
    HistTool.cpp HistTool.h
    DrawStackTool.cpp DrawStackTool.h)

target_link_libraries(UtilsLib PUBLIC ${ROOT_LIBRARIES})
target_link_libraries(ConfigLib PUBLIC UtilsLib ${ROOT_LIBRARIES})
target_link_libraries(HistToolLib PUBLIC UtilsLib ConfigLib ${ROOT_LIBRARIES})

add_executable(bbtautau-hists main.cpp)
target_link_libraries(bbtautau-hists ConfigLib HistToolLib UtilsLib)
