'''
This program overwrites a variable of another program by finding the location in 
memory using linux /proc filesystem. Based on reading awesome article at:
https://blog.holbertonschool.com/hack-the-virtual-memory-c-strings-proc/
'''
from threading import Thread
import time
import sys 
import os 

search_string = 'EXAMPLE'
write_string  = 'HACKED!'

def start_loop(prog):
	print('[*] Starting Example Process to Corrupt')
	os.system('%s' % prog)


def swap(filename, destroy):
	data = []
	for line in open(filename, 'r').readlines():
		data.append(line.replace('\n',''))
	if destroy:
		os.remove(filename)
	return data

def shutdown():
	# cleanup the daemon on exit
	os.system('sudo kill -9 $(pidof ./loop)')


def main():
	# create the target process
	target = Thread(target=start_loop,args=('./loop',))
	target.setDaemon(True)
	target.start()
	time.sleep(3)

	# Find the process in /proc
	print('[*] Searching for program in /proc')
	os.system("pidof ./loop >> cmd.txt")
	PID = int(swap('cmd.txt', True).pop())
	
	# Look in maps for the address of the head
	mem_file = '/proc/%d/mem' % PID
	map_file = '/proc/%d/maps' % PID
	print('[*] Opening %s and %s' % (mem_file, map_file))

	# find location of heap 
	os.system("sudo cat %s | grep heap | cut -d ' ' -f 1 >> cmd.txt" % map_file)
	heap_range = swap('cmd.txt',True).pop().split('-')
	print('[*] Heap is at: 0x%s-0x%s' % (heap_range[0], heap_range[1]))
	
	# calculate offsets
	heap_start = int(heap_range[0], 16)
	heap_end = int(heap_range[1], 16)

	# Open Mem file and do the fun stuff 
	memf = open(mem_file, 'rb+')
	memf.seek(heap_start)
	# Find the string
	heap_data = memf.read(heap_end - heap_start)
	# find string
	i = heap_data.index(bytes(search_string, "ASCII"))
	print("[*] Found %s at 0x%s[+%x]" % (search_string,heap_range[0], i))
	memf.seek(heap_start + i)
	memf.write(bytes(write_string, "ASCII"))

	# close files
	# maps_file.close()
	memf.close()
	# overwrite it
	time.sleep(10)
	shutdown()

if __name__ == '__main__':
	main()
