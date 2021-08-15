import struct
import pickle
import socket
import cv2
import sys

def start_listening(port):
	try:
		serve = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
		serve.bind(('0.0.0.0', port))
		serve.listen(5)
		print('Serving on 0.0.0.0:%d' % port)
	except socket.error:
		print('[!] Unable to create socket')
		assert socket.error
	return serve


def serve_video(sock):
	serving = True

	while serving:
		client, addr = sock.accept()
		print('[+] Connection accepted from %s:%d' % (addr[0], addr[1]))
		if client:
			# Start Streaming video
			vid = cv2.VideoCapture(0)
			vid.set(cv2.CAP_PROP_FRAME_WIDTH, 720)
			vid.set(cv2.CAP_PROP_FRAME_HEIGHT, 680)
			# serve the video feed to client 
			while(vid.isOpened()):
				im, frame = vid.read()
				data = pickle.dumps(frame)
				pkt = struct.pack("Q", len(data))+data
				try:
					client.sendall(pkt)
					key = cv2.waitKey(1) & 0xFF
					if key ==ord('q'):
						client_socket.close()
						serving = False
				except socket.error:
					client.close()
					pass
	return sock
		
def main():
	PORT = 8080
	# create socket for serving video 
	server = start_listening(PORT)
	# Sit and wait for clients to watch video feed
	try:
		server = serve_video(server)
	except KeyboardInterrupt:
		print('[!] Killing Feed')
		pass
	# Clean up when finished
	server.close()


if __name__ == '__main__':
	main()
