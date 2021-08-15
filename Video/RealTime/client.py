import socket
import pickle
import struct
import cv2
import sys

# Define Constants 
PORT = 8080
packet_size = struct.calcsize("Q")

def get_args():
    if len(sys.argv)> 1:
        return sys.argv[1]
    else:
        print('Usage: %s [camera-ip]' % sys.argv[0])
        exit()

def connect_to_camera():
    # create socket
    try:
        sock = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
        host_ip = get_args()
        sock.connect((host_ip,PORT))
    except socket.error:
        print('[!] Error Connecting to camera')
        assert socket.error
    return sock


def video_feed(client):
    data = b""
    # Receive Video
    while True:
        # Receive a frame of video 
        frame, client, data = acquire_frames(client,data)
        cv2.imshow("Live Feed",frame) 
        # Allow user to end stream 
        key = cv2.waitKey(1) & 0xFF
        if key  == ord('q'):
            break
    # Close Connection 
    client.close()


def acquire_frames(client,data):
    # Wait while acquiring enough packets to construct a frame
    while len(data) < packet_size:
        # get 4kb of header data about image size
        packet = client.recv(4 * 1024)
        if not packet: break
        data += packet 
    # unpack raw packet data to determine incoming image size
    packed_msg = data[:packet_size]
    data = data[packet_size:]
    msg_size = struct.unpack("Q", packed_msg)[0]

    # acquire image data
    while len(data) < msg_size:
        data += client.recv(4 * 1024)
    raw_frame = data[:msg_size]
    data = data[msg_size:]
    frame = pickle.loads(raw_frame)
    return frame, client, data



def main():
    client_socket = connect_to_camera()
    video_feed(client_socket)


if __name__ == '__main__':
    main()
