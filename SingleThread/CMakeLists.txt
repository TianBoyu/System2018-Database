cmake_minimum_required(VERSION 3.10)
project(SingleThread)

set(CMAKE_CXX_STANDARD 11)

include_directories(.)
include_directories(BpTree)
include_directories(Utils)

add_executable(SingleThread
        BpTree/bpt.cc
        BpTree/bpt.h
        BpTree/BpTree.hpp
        BpTree/predefined.h
        Utils/Error.hpp
        Utils/Logger.hpp
        Utils/Parser.hpp
        Database.hpp
        main.cpp Utils/Tools.cpp)
