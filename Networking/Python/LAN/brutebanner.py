import multiprocessing
import random
import socket 
import sys 
import os

BOLD = '\033[1m'
END = '\033[0m'

def connect(host, port):
	ans = ''
	try:
		s = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
		s.connect((host, port))
		ans = s.recv(1024)
		s.close()
	except socket.error:
		pass
	return ans


def scan(host, verbose):
	common = {'ftp':	21,
			  'ssh':	22,
			  'telnet':	23,
			  'smtp':	25,
			  'http':	80,
			  'https':	443}
	result= {'ftp':		False,
			  'ssh':	False,
			  'telnet':	False,
			  'smtp':	False,
			  'http':	False,
			  'https':	False}
	if os.name == 'posix':
		print(BOLD+'='*40 )
		print('||\tScanning: %s \t||\n'%host+'='*42)
		print('SERVICE\tPORT\tSTATE\tBANNER\n'+END)
	else:
		print('='*40)
		print('||\tScanning: %s \t||\n'%host+'='*42)
		print('SERVICE\tPORT\tSTATE\tBANNER\n')

	
	pool = multiprocessing.Pool(10)
	for service in common.keys():
		port = common[service]
		isOpen = False
		try:
			probe = pool.apply_async(connect, (host, port))
			banner = probe.get(timeout=3)
			isOpen = len(banner) > 1
		except multiprocessing.TimeoutError:
			pass
		if isOpen and verbose:
			print('%s\t%s\tOPEN\t%s' % (service, port,banner.decode()))
			result[service] = True
	return result

def usage():
	print(BOLD+'[x] Incorrect Usage!')
	print('usage:\n\t $ -h %s [hosts] or also:' % sys.argv[0])
	print('\t $ %s -f [hosts-file]' % sys.argv[0])
	print(END)
	exit()


def main():	
	# check arguments
	if '-h' in sys.argv:
		hosts = sys.argv[2:]
		for test in hosts:
			scan(test,True)
	elif '-f' in sys.argv:
		# Read hosts in from file
		# scan hosts 
		for ln in open(sys.argv[2],'r'):
			host = ln.replace('\n','')
			scan(host, True)
	else:
		usage()


if __name__ == '__main__':
	main()
