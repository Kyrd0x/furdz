# FUD Malware

School project, deadline 1st of April.

Harmfull (meterpreter TCP) payload **working** on up-to-date Windows 10 & 11.\
VirusTotal flagged 8/76

Harmless (MessageBox) payload working on up-to-date Windows 10 & 11.\
Spotted a bit on VT for now, aiming for 0


## TODO
 - Anti sandbox checks (hostname, disk size, debug_flag) and ability to activate them or not (individually)
 - Custom ROR for ntdll and WinAPI clear hashes
 - Make a proper Python
 - Improve loader (how to shellcode properly) :
        - grep and use a legit PPID maybe
        - xxx
 - add word encryption and AES encryption + possiblity to choose encr system


