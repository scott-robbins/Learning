import os, sys, string, random,json

lets = list(string.printable)
nums = list(range(len(lets)))
mapping = dict(zip(lets,nums))
alphlut = dict(zip(nums,lets))

def caesar_encode(filename):
	II = nums
	random.shuffle(II)
	ralpha = dict(zip(II,lets))
	rawdata = open(filename,'rb').read()
	output = b''
	print(ralpha)
	for i in range(len(rawdata)):
		output += bytes(ralpha[mapping[chr(rawdata[i])]],'ascii')
	return output, II

def caesar_decode(filename, II):
	alpha = dict(zip(list(range(len(lets))),lets))
	ralpha = dict(zip(lets, II))
	cdata = open(filename,'rb').read()
	output = ''
	print(ralpha)
	for i in range(len(cdata)):
		output += alpha[ralpha[chr(cdata[i])]]
	return output	


def load_key(keyfile):
	vector = []
	nums = open(keyfile,'r').read().replace('[','').replace(']','').replace('\n','').replace(',','').split(' ')
	for element in nums: vector.append(int(element))
	return vector 


def main():
	if '-in' in sys.argv:
		filename = sys.argv[2]
		result, iv = caesar_encode(filename)
		print('%s' % iv)
		fout = filename.split('.')[0] + '.caesar'
		open(fout,'wb').write(result)

	if '-out' in sys.argv:
		filename = sys.argv[2]
		# Need to check for key
		if len(sys.argv) < 10:
			# did they provide a keyfile?
			if len(sys.argv) == 4:
				kf = sys.argv[-1]
				print('[+] Loading Key from %s' % kf)
				k = load_key(kf)
				print(caesar_decode(filename,k))			

if __name__ == '__main__':
	main()