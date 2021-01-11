from ctypes import *
import sys 
import os 

def generate_test_program():
	script = '#!/bin/bash\n'
	script += 'VAR=42\n'
	script +='while [[ $VAR -eq 42 ]]; do sleep 10; done\n'
	script += '#EOF'
	open('target.sh', 'w').write(script)



if not os.path.isfile('peek.so'):
	os.system('gcc -shared -fPIC -o peek.so curious.c')
lib = cdll.LoadLibrary("./peek.so")
# Start a simple program and see if we can modify it's memory from
# this program
generate_test_program()
os.system('bash target.sh &')
# get the pointer to the variable in script running
lib.get_env_address.restype = c_char_p
a = lib.get_env_address('VAR', 'target.sh')
print(c_char_p(a).value)

os.system('rm peek.so')