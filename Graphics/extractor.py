import matplotlib.pyplot as plt
import scipy.ndimage as ndi
import numpy as np
import sys,os

def extract_frames(video, save):
	if os.path.isdir(os.path.join(os.getcwd(),'frames')):
		print('[+] Removing old images')
		os.system('rm -rf frames/')
	os.mkdir('frames')
	print(f'[+] Converting {video} to images')
	ext = video.split('.')[-1]
	if ext == 'gif':
		cmd = f'ffmpeg -v 0 -i {video} frames/out%d.png'
	else:
		cmd = f'ffmpeg -v 0 -i {video} -vf fps={rate} frames/out%d.png'
	os.system(cmd)
	# Load images 
	print(f'[+] {video} converted to {len(os.listdir("frames/"))} images')
	images = {}
	for frame in os.listdir('frames'):
		images[frame] = np.array(plt.imread(os.path.join('frames/',frame)))
	if not save:
		os.system('rm -rf frames')
	return images


def extract_alpha(frame):
	dims = frame.shape
	avg = frame.mean()
	if avg > 0:
		sigma = 1
	else:
		sigma = avg/10
	rslope = ndi.gaussian_laplace(frame[:,:,0],sigma)
	gslope = ndi.gaussian_laplace(frame[:,:,1],sigma)
	bslope = ndi.gaussian_laplace(frame[:,:,2],sigma)
	# Vectorized is faster B-)
	[rx,ry] = np.where(rslope == 0)
	[gx,gy] = np.where(gslope == 0)
	[bx,by] = np.where(bslope == 0)
	# these are all then points we wanna make transparent
	frame[rx,ry,3] = 0
	frame[gx,gy,3] = 0
	frame[bx,by,3] = 0
	return frame


def make_transparent_gif(filename):
	if not os.path.isfile(filename):
		print(f'[!] Cannof find {filename}')
	
	frames = extract_frames(filename,False)
	os.mkdir('frames')
	print(f'[+] Processing {filename} [{len(frames.keys())} images]')
	for name,array in frames.items():
		# make the frame transparent
		plt.imsave(f'frames/{name}', extract_alpha(array))
		print(f'- frames/{name}')
	if os.name == 'nt':
		fout = filename.split("\\")[-1]
	else:
		fout = filename.split("/")[-1]
	# make a pallette 
	os.system('rm palette.png')
	os.system(f'ffmpeg -i frames/out%d.png -vf palettegen palette.png')
	os.system(f'ffmpeg -i frames/out%d.png  -i palette.png -lavfi paletteuse=alpha_threshold=128 transparent_{fout}')

def main():
	if len(sys.argv)> 1:
		if 'gif' ==  sys.argv[1].split('.')[-1]:
			make_transparent_gif(sys.argv[1])


if __name__ == '__main__':
	main()