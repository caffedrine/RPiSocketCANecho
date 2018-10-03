#!/usr/bin/env bash

COMPILATION=0

if [ $COMPILATION -eq 0 ]; then
    echo 'Start compilation executable...'
    gcc ../main.c ../C_SocketCAN.c -o CSocketCAN

    echo 'Launch application...'
    ./CSocketCAN
else
    echo 'Start compilation shared library...'
    gcc -shared ../C_SocketCAN.c -o CSocketCAN.so
    echo 'Generated SocketCAN.so...'
fi

