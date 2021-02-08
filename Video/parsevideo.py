import scipy.ndimage as ndi
import numpy as np
import ctypes
import shutil
import cv2
import sys 
import os 


blur5 = [[1,1,1,1,1],
		 [1,1,1,1,1],
	     [1,1,0,1,1],
		 [1,1,1,1,1],
		 [1,1,1,1,1]]

blur3 = [[1,1,1],[1,0,1],[1,1,1]]

# Function for importing a custom C library 
def build_lib(src_file):
	if os.path.isfile(src_file):
		if os.name == 'posix': # on linux
			buildcmd = 'gcc -shared -fPIC -o lib.so %s' % src_file
			os.system(buildcmd)
			lib = cdll.LoadLibrary("./lol.so")
		elif os.name == 'nt': # on windows
			if os.path.isfile('lol.dll'):
				lib = cdll.LoadLibrary("lol.dll")
	return lib



# Just starting with some python to get started...
def main():
	display = False
	n_frames = 0
	
	# check args 
	if len(sys.argv) < 2:
		print('Usage: %s video.ext')
		exit()
	if '--watch' in sys.argv or '--display' in sys.argv:
		display = True

	# Get the name of the video
	video_file = sys.argv[1]
	ext = video_file.split('.')[1]
	video_name = 'video_in.%s' % ext
	print('[*] Decoding %s [%s File]' % (video_file, ext))
	shutil.copyfile(video_file,video_name)

	# Open the local copy of the video
	vid = cv2.VideoCapture(video_name)
	if (vid.isOpened()== False): 
		print("Error opening video stream or file")

	frame_buffer = list()

	try:
		# Read until video is completed
		while(vid.isOpened()):
			# Capture frame-by-frame
			ret, frame = vid.read()
			n_frames += 1

			if ret == True :
				# Image Processing Code here, we have a valid frame ready
				im = np.array(frame).astype(np.uint8)
				frame_buffer.append(im)
				
				if n_frames > 3:
					del frame_buffer[0]

				# Pass image to C Library
				if display:
					# Display Image in window
					if len(frame_buffer) > 2:
						# Very Flashy but captures edges of important object pretty well!!
						vision = (frame_buffer[1]-frame_buffer[0]) - frame_buffer[2]
						h1 = cv2.resize(frame, (0, 0), fx=0.3, fy=0.3)
						h2 = cv2.resize(vision, (0, 0), fx=0.3, fy=0.3)
						combo = np.concatenate((h1, h2),axis=1)
						cv2.imshow('Frame',combo)

				if cv2.waitKey(25) & 0xFF == ord('q'):
					break
			else: 
				break
	except KeyboardInterrupt:
		pass
	vid.release()
	print('[*] %d Frames read from %s' % (n_frames,video_file))

	if display:
		cv2.destroyAllWindows()
	os.remove(video_name)

if __name__ == '__main__':
	main()
