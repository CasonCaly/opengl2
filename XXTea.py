# coding:utf-8
"""
XXTea的python包装类
"""
from ctypes import *


if __name__ == "__main__":
    xxtea = CDLL("libxxtea.dylib")
    raw_data = "helloworldsssss"
    encry_len = len(raw_data)
    key = "wsss"
    key_len = len(key)

    data = (c_ubyte * encry_len).from_buffer_copy(raw_data)
    data_len = c_uint(encry_len)
    key = (c_ubyte * key_len).from_buffer_copy(key)
    key_len = c_uint(key_len)
    ret_length = c_uint(0)
    ret_length_pt = pointer(ret_length)

    xxtea.xxtea_encrypt.argtypes = [POINTER(c_ubyte), c_uint, POINTER(c_ubyte), c_uint, POINTER(c_uint)]
    xxtea.xxtea_encrypt.restype = POINTER(c_ubyte)

    encry_data = xxtea.xxtea_encrypt(data, data_len, key, key_len, ret_length_pt)
    decry_len = c_uint(0)
    decry_len_pt = pointer(decry_len)

    xxtea.xxtea_decrypt.argtypes = [POINTER(c_ubyte), c_uint, POINTER(c_ubyte), c_uint, POINTER(c_uint)]
    xxtea.xxtea_decrypt.restype = POINTER(c_ubyte)
    decry_data = xxtea.xxtea_decrypt(encry_data, ret_length, key, key_len, decry_len_pt)
    print  cast(decry_data, c_char_p).value

