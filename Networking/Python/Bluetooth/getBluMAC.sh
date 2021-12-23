#!/bin/bash
echo $(bluetoothctl list | cut -d ' ' -f 2)
#EOF