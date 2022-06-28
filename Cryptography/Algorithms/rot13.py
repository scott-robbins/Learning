import string

def encode(plaintext):
	chars = list(string.printable)
	lut = dict(zip(list(range(len(chars))), chars))
	alphas = dict(zip(chars, list(range(len(chars)))))
	result = ''
	for letter in list(plaintext):
		if letter in chars:
			N = alphas[letter] + 13
			print(f'{letter}:{alphas[letter]}')
			if N >= len(chars):
				N = N - len(chars)
			result += lut[N]
		else:
			print(f'[!] unknown character {letter}')
	return result

def decode(ciphertext):
	chars = list(string.printable)
	lut = dict(zip(list(range(len(chars))), chars))
	alphas = dict(zip(chars, list(range(len(chars)))))
	result = ''
	for letter in list(ciphertext):
		if letter in chars:
			N = alphas[letter] - 13
			if N < 0:
				N += len(chars)
		result += lut[N]
	return result