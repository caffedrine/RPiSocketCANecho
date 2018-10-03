#!/usr/bin/env bash

echo 'Start compilation...'
g++ -std=c++11 -lpthread ../main.cpp ../SocketCAN.cpp -o SocketCAN

echo 'Launch application...'

./SocketCAN