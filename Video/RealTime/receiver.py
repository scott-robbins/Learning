import numpy as np
import socket
import pickle
import struct
import utils
import time
import cv2
import sys
import os

class Receiver():
	PORT = 8080
	ENDPT = ''
	PKT_SZ = struct.calcsize("Q")

	def __init__(self, rmt_ip, rmt_port,record):
		self.PORT = rmt_port
		self.ENDPT = rmt_ip
		self.backups = 0
		self.running = True
		self.recording = record
		self.kill_feed()
		self.cleanup_frames()
		self.start_feed()
		time.sleep(5)

	def set_recording(self):
		print('[+] Recording Live Feed')
		self.cleanup_frames()
		self.recording = True

	def cleanup_frames(self):
		if os.path.isdir('frames'):
			print('[-] Cleaning up old frames')
			bkups = utils.cmd('ls *ImgBackups*.zip',False)
			N = 0
			for i in bkups:
				n = int(i.split('.')[0].split('Backups')[1])
				if n > N: N = n
			bk = 'ImgBackups%d.zip' % (N + 1)
			os.system('zip %s -r frames/ -9 >> /dev/null' % bk)
			os.system('rm -rf frames')
		os.mkdir('frames')


	def run(self):
		# data feed stream
		data = b""
		s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		s.connect((self.ENDPT, self.PORT))
		prev_time = time.time()
		iteration = 1
		backup_limit = 250
		while self.running:
			ld, lt = utils.create_timestamp()
			try:
				# Receive frame of video 
				frame, data = self.acquire_frame(data, s)
				# # Do image processing 
				# # calculate frames/second
				# current_time = time.time()
				# fps = 1/(current_time-prev_time)
				# prev_time = current_time
				# # # putting the FPS count on the frame
				stamp = ld + ' ' + lt #+ ' ' +'FPS: '+str(int(fps))
				font = cv2.FONT_HERSHEY_SIMPLEX
				cv2.imshow('Live Feed', frame)
				iteration += 1


				if iteration >= backup_limit and iteration%backup_limit==0:
					print('Backing up images %s %s' % (ld, lt))
					self.backup_frames()
					
				# Allow user to end stream 
				key = cv2.waitKey(1) & 0xFF
				if key  == ord('q'):
					break
			except socket.error:
				sellf.running = False
				pass
		s.close()
		self.kill_feed()

	def backup_frames(self):
		bkup = 'ImgBackups%d.zip' % self.backups
		c1 = 'zip %s -r frames/ -9 >> /dev/null;' % bkup
		c2 = 'rm -rf frames/'
		os.system(c1+c2)
		os.mkdir('frames')
		self.backups += 1


	def shutdown(self, s):
		s.close()
		print('[-] Ending Stream')
		return

	def kill_feed(self):
		c = f"ssh pi@{self.ENDPT} ./killstream.sh"
		return utils.cmd(c,False)

	def start_feed(self):
		print('[+] Starting Remote Feed')
		c = f"ssh pi@{self.ENDPT} python3 /home/pi/streamer.py &\n"
		return utils.cmd(c,False)		

	def acquire_frame(self, data, sock):
		# wait and acquire enough packets to construct frame of header info
		while len(data) < self.PKT_SZ:
			try:
				pkt = sock.recv(4*1024) # get 4Kb at a time
				if not pkt: break
				data += pkt
			except socket.error:
				print('[x] Connection to %s broken' % self.ENDPT)
				self.running = False

		# unpack raw packet data to get incoming image dimensions
		packed_msg = data[:self.PKT_SZ]
		data = data[self.PKT_SZ:]
		msg_size = struct.unpack("Q", packed_msg)[0]

		# acquire image data
		while len(data) < msg_size:
			data += sock.recv(4 * 1024)
		raw_frame = data[:msg_size]
		data = data[msg_size:]
		frame = np.rot90(np.rot90(np.array(pickle.loads(raw_frame))))
		return frame, data

def main():
	ip = '192.168.1.170'
	rec = False
	
	if '-ip' in sys.argv:
		addr = sys.argv[-1]

	if '--record' in sys.argv or '-r' in sys.argv:
		rec = True	

	viewer = Receiver(ip,8080, rec)
	viewer.run()

if __name__ == '__main__':
	main()
