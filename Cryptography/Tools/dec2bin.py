import sys

def dec2bin(decimal, n_bits):
	binary = b''
	while n_bits >= 0:
		if (decimal - 2**n_bits+1) >= 1:
			binary += b'1'
			decimal -= 2**n_bits
		else:
			binary += b'0'
		n_bits -= 1
	return binary


if __name__ == '__main__':
	if len(sys.argv) > 1:
		N = int(sys.argv[1])
		print(dec2bin(N, 32))