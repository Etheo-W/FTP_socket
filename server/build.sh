#-----------SocketServer.sh---------
#! /bin/bash

mkdir -p ./SocketServerFiles

make

make clean

./socketServer
