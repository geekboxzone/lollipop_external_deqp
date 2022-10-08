#!/bin/sh

echo "Removing old dEQP Ondevice Package..."
adb $* uninstall com.drawelements.deqp

echo "Installing dEQP Ondevice Package..."
adb $* install -r dEQP.apk
