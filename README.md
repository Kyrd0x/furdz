# FUD Malware

School project, deadline 1st of April.

Harmless (MessageBox) payload working on up-to-date Windows 10 & 11.\
Spotted a bit on VT for now

Harmfull (meterpreter TCP) payload working on up-to-date Windows 10 with simple XOR.\
Windows 11 flagged. Shellcode recoding in progress


## TODO
 - Improve loader (how to shellcode properly) :
        - grep and use a legit PPID maybe
        - xxx
 - Anti sandbox checks (hostname, disk size, debug_flag) and ability to activate them or not (individually)
 - add word encryption and AES encryption + possiblity to choose encr system

## Notes about VirusTotal:
       - before drastic small comparison it was a 5/76, now spotted as 'Brutel' (harmless)

