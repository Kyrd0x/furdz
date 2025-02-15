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

def hash(module, function, bits=13):
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

module = "kernel32.dll"
function = "LoadLibraryA"
rotation = 13

if (len(sys.argv)>1):
    module=str(sys.argv[1])
if (len(sys.argv)>2):
    function=str(sys.argv[2])
if (len(sys.argv)>3):
    rotation=int(sys.argv[3])    

print(f"== ROR{rotation} hash ===\n")

print("Module:\t\t\t", module)
print("Function:\t\t", function)
print(f"Module ROR{rotation} "+'hash:\t0x%X' % hash(module,"",rotation))
print(f"Function ROR{rotation} "+'hash:\t0x%X' % hash("",function,rotation))
print(f"ROR{rotation} "+'hash:\t\t0x%X' % hash(module,function,rotation))