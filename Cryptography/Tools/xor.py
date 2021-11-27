import os, sys

def xor(filename, key):
	result = b''
	if not os.path.isfile(filename):
		print(f'[!] Cannot find {filename}')
		return result

	raw_data = list(open(filename,'rb').read())
	for i in range(len(raw_data)):
		result += bytes([raw_data[i] ^ key[i%len(key)]])
	return result

def arg2bytes(bstr):
	bs = []
	for element in bstr.split('x')[1:]:
		bs.append(ord(bytes.fromhex(element)))
	return bs



def main():
	if '-fx' in sys.argv:
		# check for key
		if len(sys.argv) > 3:
			k = arg2bytes(sys.argv[3])
		else:
			k = os.urandom(8)
		fname = sys.argv[2]
		print(f'[+] Running {fname} through XOR using Key:{k}')
		open(fname.split('.')[0]+'.xor','wb').write(xor(fname, k))

	if '-dx' in sys.argv:
		fname = sys.argv[2]
		# check for key
		if len(sys.argv) > 3:
			k = arg2bytes(sys.argv[3])
		else:
			k = os.urandom(8)
		print(xor(fname, k))

if __name__ == '__main__':
	main()