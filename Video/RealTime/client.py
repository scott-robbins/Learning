import numpy as np
import socket
import pickle
import struct
import cv2
import sys

# Define Constants 
PORT = 8080
packet_size = struct.calcsize("Q")
k1 = np.ones(((3,3)),np.uint8)

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

def null_filter(frame):
    return frame

def edge_detect(frame):
    A = np.array(frame).mean()
    # B = np.array(frame).max()
    edge_detect = cv2.Canny(frame, A, 2*A)
    return edge_detect

def segmentation(frame):
    # convert to grayscale image and threshold
    gray = cv2.cvtColor(frame,cv2.COLOR_BGR2GRAY)
    ret, thresh = cv2.threshold(gray,0,255,cv2.THRESH_BINARY_INV+cv2.THRESH_OTSU)
    # noise subtraction
    return cv2.morphologyEx(thresh,cv2.MORPH_OPEN,k1, iterations = 1)
    

def video_feed(client, camera_modes, mode):
    data = b""
    # Receive Video
    while True:
        # Receive a frame of video 
        frame, client, data = acquire_frames(client,data)
        # Do image processing based on mode selected
        frame = camera_modes[mode](frame)
        # show frame
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
    camera_modes = {'None': null_filter,
                    'Edges': edge_detect,
                    'Segments': segmentation}
    modes = list(camera_modes.keys())
    if len(sys.argv) > 2:
        mode = sys.argv[-1].split('--')[-1]
        if mode not in modes:
            print('[!] Unknown mode. Please use one of the following:')
            print(modes)
            exit()
    else:
        mode = 'None'
    client_socket = connect_to_camera()
    video_feed(client_socket, camera_modes, mode)


if __name__ == '__main__':
    main()
