#!/usr/bin/env bash

COMPILATION=1

if [ $COMPILATION -eq 0 ]; then
    echo 'Start compilation executable...'
    g++ -std=c++11 -lpthread ../main.cpp ../SocketCAN.cpp -o SocketCAN

    echo 'Launch application...'
    ./SocketCAN
else
    echo 'Start compilation shared library...'
    g++ -shared -std=c++11 -lpthread ../SocketCAN.cpp -o SocketCAN.so
    echo 'Generated SocketCAN.so...'
fi

