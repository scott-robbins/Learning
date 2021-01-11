import sys
import os 

def main():
	if len(sys.argv) < 2:
		print('Usage: %s 0x7ffe98595e7a' % sys.argv[0])
	FLIP = False

	IND = -1

	if '-r' in sys.argv:
		FLIP = True

	if '-d' in sys.argv:
		IND = int(sys.argv[-1])*2 - 1

	bytestring = sys.argv[1]
	result = ''
	# Create the lins for iterating through string by two
	bins = range(0,len(bytestring)+1,2)
	# bins = range(len(bytestring)-1,0,2)
	bins.sort()
	if FLIP:
		bins.reverse()
	# print(bins)
	ind = 1
	for i in bins[0:-1]:
		start = bins[ind-1]
		stop = bins[ind]
		if ''.join(bytestring[start:stop]) != '0x' and ''.join(bytestring[stop:start]) != '0x':
			if not FLIP:
				result += '\\x%s' % (''.join(bytestring[start:stop]))
			else:
				result += '\\x%s' % (''.join(bytestring[stop:start]))
		ind += 1
	print(''.join(list(result)[0:IND-1]))

if __name__ == '__main__':
	main()
