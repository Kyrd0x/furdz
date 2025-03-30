import sys

def ror(dword,bits):
    return (dword >> bits | dword << (32 - bits)) & 0xFFFFFFFF

def rol(dword,bits):
    return (dword << bits | dword >> (32 - bits)) & 0xFFFFFFFF
    
def unicode(string, uppercase=True):
    result = ""
    if uppercase:
        string = string.upper()
    for char in string:
        result += char+"\x00"
    return result

def hash(module, function, bits=23, direction="R"):
    module_hash = 0
    function_hash = 0
    for char in unicode(module+"\x00"):
        if direction == "R":
            module_hash = ror(module_hash, bits)
        else:
            module_hash = rol(module_hash, bits)
        module_hash += ord(char)
        print(f"char: {char} | module_hash: {hex(module_hash)}")
    for char in str(function+"\x00"):
        if direction == "R":
            function_hash = ror(function_hash, bits)
        else:
            function_hash = rol(function_hash, bits)
        function_hash += ord(char)
        print(f"char: {char} | function_hash: {hex(function_hash)}")
    print(f"module_hash: {hex(module_hash)}")
    print(f"function_hash: {hex(function_hash)}")
    final_hash = module_hash + function_hash & 0xFFFFFFFF
    print(f"RO{direction[0].upper()}{bits} Hash for {module}::{function} is {hex(final_hash)}")
    return final_hash

# default
module_name = ""
function_name = ""
value=13
direction="R"

if len(sys.argv) > 1:
    module_name = sys.argv[1]
if len(sys.argv) > 2:
    function_name = sys.argv[2]
if len(sys.argv) > 3:
    value = int(sys.argv[3])
if len(sys.argv) > 4:
    direction = sys.argv[4]
    if direction != "R" and direction != "L":
        print("Invalid direction. Use 'R' or 'L'")
        sys.exit(1)

res = hash(module_name, function_name, value, direction)