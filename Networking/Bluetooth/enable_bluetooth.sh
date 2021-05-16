#!/bin/bash
echo '[+] Starting Bluetooth'
sudo systemctl status bluetooth
sudo systemctl enable bluetooth
sudo systemctl start bluetooth

bluetoothctl scan on
bluetoothctl discoverable on
#EOF
