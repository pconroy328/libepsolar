#!/bin/bash
sudo cp libepsolar.h /usr/local/include
if [ ! -d /usr/local/include/epsolar ]; then 
   sudo mkdir /usr/local/include/epsolar
fi
sudo cp tracerseries.h /usr/local/include/epsolar/.
sudo cp dist/Debug/GNU-Linux*/liblibepsolar.a /usr/local/lib/libepsolar.a
sudo chmod 755 /usr/local/include/libepsolar.h
sudo chmod 755 /usr/local/include/epsolar/*
sudo ldconfig
