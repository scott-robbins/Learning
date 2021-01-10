from ctypes import *
import os

if os.name == 'posix': # on linux
    lib = cdll.LoadLibrary("./lib.so")
elif os.name == 'nt': # on windows
    lib = cdll.LoadLibrary("lib.dll")


def show_bytes(buffer):
    bytestr = ''
    data_size = lib.get_size(buffer_b)
    for i in range(data_size):
        bytestr += str(lib.bit_get(buffer, i))
    print(bytestr)
    return bytestr


lib.test_print()
lib.cprint("Hey C, It's Python!")
print(lib.add(1, 2))


# Calling function with a structure
# fields are defined by tuple with (fieldname, c_type)
class PyStruct(Structure):
    _fields_ = [("str",c_char_p),  ("num",c_int)]


p = PyStruct()
p.str = "This is a test"
p.num = 42
# This isnt printing right.
lib.show_struct(byref(p))

# Now try the bit operations
buffer_a = chr(41)
buffer_b = chr(42)
print('Buffer A and Buffer B:')
show_bytes(buffer_a)
show_bytes(buffer_b)
# test trying to manually adjust bits
# lib.bit_set(buffer_b, 0, 1)
# lib.bit_set(buffer_b, 1, 1)
# lib.bit_set(buffer_b, 3, 1)
# lib.bit_set(buffer_b, 5, 1)
# show_bytes(buffer_b)

# what about XOR?
buffer_c = chr(0)
print('Attempting low level XOR:')
lib.bit_xor(buffer_a, buffer_b, buffer_c, 8)
show_bytes(buffer_c)

print('Attempting to bit-shift result to left by 3')
lib.bit_rot_left(buffer_c, 8, 3)
show_bytes(buffer_c)