import os, sys

def generate_crc32_table():
    # initial implementation in C taken from http://www.mrob.com/pub/comp/crc-all.html
    crc_shift_reg = 0   #crc shift register
    poly_pattern = 0    #polynomial exclusive-or pattern
    terms = [0,1,2,4,5,7,8,10,11,12,16,22,23,26]
    # Make Exclusive or pattern from polynomial (0xedb88320)
    for polynomial in terms:
        poly_pattern |= (0x1<< (31 - polynomial))
    # Computer table of CRC's
    crc32_table = {}
    for i in range(1,256):
        c = i
        for j in range(8,0,-1):
            a = (c >> 1) ^ poly_pattern
            if (c & 1):
                c = (c >> 1) ^ poly_pattern
            else:
                c = c>> 1
        crc32_table[i] = c
        crc32_table[0] = 0
    return crc32_table

def crc32(crc_table, bytestr):
    # initial implementation in C taken from http://www.mrob.com/pub/comp/crc-all.html
    crc = 2**32 - 1
    # pass in as argument to avoid including here, will make this faster during loops
    #crc_table = generate_crc32_table() 
    for k in bytestr:
        crc = crc_table[(crc & 0xff) ^ k] ^ (crc >> 8)
    return crc ^ 0xffffffff

if __name__ == '__main__':
    if len(sys.argv) > 1:
        ctable = generate_crc32_table() 
        for word in sys.argv[1:]:
            print(hex(crc32(ctable, bytes(word,'utf-8'))))
