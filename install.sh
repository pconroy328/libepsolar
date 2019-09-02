#!/bin/bash
sudo cp libepsolar.h /usr/local/include
sudo mkdir /usr/local/include/epsolar
sudo cp tracerseries.h /usr/local/include/epsolar/.
sudo cp dist/Debug/GNU-Linux*/liblibepsolar.a /usr/local/lib/libepsolar.a
sudo chmod 755 /usr/local/include/libepsolar.h
sudo chmod 755 /usr/local/include/epsolar/*
sudo ldconfig -v 
