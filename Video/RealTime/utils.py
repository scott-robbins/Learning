import random
import string 
import time
import os

charpool = ['a','b','c','d','e','f','g','h','i','j','k','l','m',
			'n','o','p','q','r','s','t','u','v','w','x','y','z']

def swap(filename, destroy):
	data = []
	for line in open(filename, 'r').readlines():
		data.append(line.replace('\n', ''))
	if destroy:
		os.remove(filename)
	return data

def create_random_filename(ext):
	
	basename = ''.join(random.sample(charpool, 6))
	random_file = basename +ext
	return random_file

def cmd(command, verbose):
	tmp = create_random_filename('.sh')
	tmp2 = create_random_filename('.txt')
	data = '#!/bin/bash\n%s\n#EOF' % command
	open(tmp, 'w').write(data)
	os.system('bash %s >> %s' % (tmp,tmp2))
	os.remove(tmp)
	if verbose:	
		os.system('cat %s' % tmp2)
	return swap(tmp2, True)

def arr2str(content):
	result = ''
	for element in content:
		result += element + '\n'
	return result

def arr2chstr(content):
	result = ''
	for element in content:
		result += element + ' '
	return result


def create_timestamp():
    date = time.localtime(time.time())
    mo = str(date.tm_mon)
    day = str(date.tm_mday)
    yr = str(date.tm_year)

    hr = str(date.tm_hour)
    min = str(date.tm_min)
    sec = str(date.tm_sec)

    date = mo + '/' + day + '/' + yr
    timestamp = hr + ':' + min + ':' + sec
    return date, timestamp
