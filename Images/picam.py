import matplotlib.pyplot as plt
from threading import Thread
import numpy as np
import json 
import time 
import sys
import os 
 
def load_camera_info():
	if os.path.isfile('.picam.json'):
		print('[*] Loading Camera Info from Disk')
		return json.loads(open('.picam.json','r').read())
	else:
		print('[!!] Missing JSON Config File')
		return register_camera()

def register_camera():
	hname = raw_input('Enter Hostname: ')
	ipaddr = raw_input('Enter IP Address: ')
	cam_data = {'hostname': hname, 'ip': ipaddr}
	open('.picam.json','w').write(json.dumps(cam_data))
	return cam_data

def get_image(c):
	name = c['hostname']
	ip = c['ip']
	ssh = 'ssh %s@%s ' % (name, ip)
	cmd = 'raspistill -t 1 -o im.jpeg'
	snap_cmd = ssh + cmd
	get_cmd = 'sftp %s@%s:/home/%s/ <<< $'%(name,ip,name)+"'get im.jpeg'"
	tmp = '#!/bin/bash\n%s\n%s\nrm $0\n#EOF' % (snap_cmd, get_cmd)
	open('tmp.sh','w').write(tmp)
	print('[-] Snapping image')
	os.system('bash tmp.sh >> /dev/null')
	im = np.array(plt.imread('im.jpeg'))
	os.remove('im.jpeg')
	return im

def main():
	camera = load_camera_info()

	if '-snap' in sys.argv:
		arr = get_image(camera)
		plt.imshow(arr); plt.show()

if __name__ == '__main__':
	main()
