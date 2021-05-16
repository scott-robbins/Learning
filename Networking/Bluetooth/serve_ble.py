"""
A simple Python script to receive messages from a client over
Bluetooth using Python sockets (with Python 3.3 or above).
"""
import socket
import sys


if len(sys.argv) < 2:
	print('Usage: python %s [LOCAL_BLE_MAC]')
	exit()

hostMACAddress = sys.argv[1]
port = 3 s = socket.socket(socket.AF_BLUETOOTH, socket.SOCK_STREAM, socket.BTPROTO_RFCOMM)
backlog = 1
size = 1024

s = socket.socket(socket.AF_BLUETOOTH, socket.SOCK_STREAM, socket.BTPROTO_RFCOMM)
s.bind((hostMACAddress,port))
s.listen(backlog)

try:
    client, address = s.accept()
    print('%s connected' % address[0]) 
    client.send('HEEEY MAN :D') 
    data = client.recv(size)
    print(data)
    client.close()
except:	
    print("Closing socket")	
    s.close()
