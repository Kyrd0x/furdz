# Perform a right rotation on a 32-bit integer
def ror(dword, bits):
    return (dword >> bits | dword << (32 - bits)) & 0xFFFFFFFF

# Perform a left rotation on a 32-bit integer
def rol(dword, bits):
    return (dword << bits | dword >> (32 - bits)) & 0xFFFFFFFF

# Convert a string to Unicode with optional uppercase transformation
def unicode(string, uppercase=True):
    result = ""
    if uppercase:
        string = string.upper()
    for char in string:
        result += char + "\x00"  # Add null byte after each character
    return result

# Generate a hash for a module and function name using rotation and addition
def hash(module, function, bits=23, direction="R"):
    module_hash = 0
    function_hash = 0

    # Hash the module name
    for char in unicode(module + "\x00"):
        if direction == "R":
            module_hash = ror(module_hash, bits)
        else:
            module_hash = rol(module_hash, bits)
        module_hash += ord(char)

    # Hash the function name
    for char in str(function + "\x00"):
        if direction == "R":
            function_hash = ror(function_hash, bits)
        else:
            function_hash = rol(function_hash, bits)
        function_hash += ord(char)

    # Combine the two hashes and ensure it fits in 32 bits
    final_hash = module_hash + function_hash & 0xFFFFFFFF
    print(f"RO{direction[0].upper()}{bits} Hash for {module}::{function} is {hex(final_hash)}")
    return final_hash