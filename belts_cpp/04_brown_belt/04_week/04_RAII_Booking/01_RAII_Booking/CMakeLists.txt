cmake_minimum_required(VERSION 3.28)
project(01_RAII_Booking)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wunused-variable -Wconversion -Wextra -pedantic-errors -pthread -O3")
set(CMAKE_BUILD_TYPE Debug)
#set(CMAKE_BUILD_TYPE Release)

set(SOURCES
        test.cpp)

set(HEADERS
        profile.h
        test_runner.h
        new_booking_providers.h
        new_trip_manager.h
        old_booking_providers.h
        old_trip_manager.h)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

