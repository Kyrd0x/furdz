# FUD Malware

School project, deadline 1st of April.

Harmless payload working on up-to-date Windows 10 & 11 (MessageBox)
Spotted 7/76 on VT for now

Harmfull later


## TODO
 - harmfull payload, and ability to choose
 - Improve loader (how to shellcode properly) :
        - grep and use a legit PPID
        - xxx
 - Anti sandbox checks (hostname, disk size, debug_flag) and ability to activate them or not (individually)
 - add word encryption and AES encryption + possiblity to choose encr system

## Notes about VirusTotal (7/76):
       - PEB->ldr_data spotted
       - PE header parsed
       - XOR encoded section
