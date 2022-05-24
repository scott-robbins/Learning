import time
import sys

def dec2bin(decimal, n_bits):
	binary = b''
	for bits in range(n_bits,0,-1):
		if (decimal - 2**bits+1) >= 1:
			binary += b'1'
			decimal -= 2**bits
		else:
			binary += b'0'
	return binary


if __name__ == '__main__':
	t0 = time.time()
	if len(sys.argv) > 1:
		N = int(sys.argv[1])
		dt = time.time() - t0
		print(f'{dec2bin(N, 32)}\t[{dt}s elapsed]')
