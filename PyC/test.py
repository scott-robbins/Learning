from ctypes import *
import os

if os.name == 'posix': # on linux
    if not os.path.isfile('lib.so') and os.path.isfile('test.c'):
        os.system('gcc -shared -fPIC -o lib.so test.c')
    lib = cdll.LoadLibrary("./lib.so")
elif os.name == 'nt': # on windows
    lib = cdll.LoadLibrary("lib.dll")


def show_bytes(buffer, label):
    bytestr = ''
    data_size = lib.get_size(buffer_b)
    for i in range(data_size):
        bytestr += str(lib.bit_get(buffer, i))
    print('%s\t%s' % (bytestr, label))
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
buffer_a = chr(128)
buffer_b = chr(42)

show_bytes(buffer_a, 'A')
show_bytes(buffer_b, 'B')
print('='*20)

# what about XOR?
buffer_c = chr(0)
lib.bit_xor(buffer_a, buffer_b, buffer_c, 8)
show_bytes(buffer_c, 'A ^ B')


# trying AND
buffer_c = chr(0)
lib.bit_and(buffer_a, buffer_b, buffer_c, 8)
show_bytes(buffer_c, 'A & B')

# testing OR
buffer_c = chr(0)
lib.bit_or(buffer_a, buffer_b, buffer_c, 8)
show_bytes(buffer_c, 'A || B')

# Test Bit-Shift Left
lib.bit_rot_left(buffer_c, 8, 3)
show_bytes(buffer_c, '(A ^ B) << 3')
