cmake_minimum_required(VERSION 3.15)
project(C_HAT C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread")


add_executable(C_HAT main.c log/Logger.c log/Logger.h socket/Socket.c socket/Socket.h client/Client.c client/Client.h room/Room.c room/Room.h "message manager/Message Manager.c" "message manager/Message Manager.h")