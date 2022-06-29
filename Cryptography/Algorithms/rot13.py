import string

def encode(plaintext):
	return ''.join([chr(ord(e)+13) for e in list(plaintext)])

def decode(ciphertext):
	return ''.join([chr(ord(e)-13) for e in list(ciphertext)])
