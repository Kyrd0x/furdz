import sys

def ror(dword,bits):
    return (dword >> bits | dword << (32 - bits)) & 0xFFFFFFFF
    
def unicode(string, uppercase=True):
    result = ""
    if uppercase:
        string = string.upper()
    for char in string:
        result += char+"\x00"
    return result

def hash(module, function, bits=23):
    module_hash = 0
    function_hash = 0
    for char in unicode(module+"\x00"):
        module_hash = ror(module_hash, bits)
        module_hash += ord(char)
    for char in str(function+"\x00"):
        function_hash = ror(function_hash, bits)
        function_hash += ord(char)
    final_hash = module_hash + function_hash & 0xFFFFFFFF
    return final_hash