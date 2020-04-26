cmake_minimum_required(VERSION 3.15)
project(projetChat C)

set(CMAKE_C_STANDARD 99)

add_executable(projetChat main.c sockets/old/socket.c sockets/creation/creation.c sockets/creation/creation.h sockets/Parameters/bindParam.c sockets/Parameters/bindParam.h sockets/Adress-Ports/bindAdressPort.c sockets/Adress-Ports/bindAdressPort.h sockets/listening/listening.c sockets/listening/listening.h sockets/connexion/connexion.c sockets/connexion/connexion.h)