#!/usr/bin/env python3
import os, sys, time, string, itertools
import numpy as np

letters = list(string.ascii_letters)
special = list(string.digits + string.punctuation)
counts = dict(zip(letters,list(np.zeros((1,len(letters))).flatten())))

def calculate_letter_frequency(text):
	printable = list(string.printable)
	counts = np.zeros((1,len(list(string.printable)))).flatten()
	items = dict(zip(printable,list(counts)))
	badchars = 0
	print('[+] Analyzing Text \t\t[%d characters]' % len(text))
	for element in list(text):
		try:
			items[element] += 1
		except:
			badchars += 1
			pass
	print('[+] Bad Characters in text \t[%d]' % badchars)
	# recalculate these as percentages instead of counts? 
	N = len(list(text))
	fracs = np.zeros((1,len(list(string.printable)))).flatten()
	ratios = dict(zip(printable,fracs))
	for diagram in printable:
		ratios[diagram] = 100. * (items[diagram]/N)
	return items, ratios

def display_counts(d):
	print('')
	print('\033[3m\t\tLetter Frequencies:\033[0m')
	printout = ''; ii = 1
	for k in d.keys():
		if k in letters:
			if d[k] > 5:
				printout += '\033[1m'
			printout += '%s: %f\t' % (k, d[k])
			if d[k] > 5:
				printout += '\033[0m'
			ii += 1
		if ii % 5 == 0:
			printout += '\n'
			ii = 1
	print(printout)	
	

def main():
	if len(sys.argv) < 2:
		print('[+] Analyzing test file example.txt')
		text = open('example.txt','r').read()
		
	if(len(sys.argv) > 1):
		print('[+] Attempting to Analyze %s' % sys.argv[1])
		text = open(sys.argv[1],'r').read()

	counts, frequencies = calculate_letter_frequency(text)
	display_counts(frequencies)


if __name__ == '__main__':
	main()
