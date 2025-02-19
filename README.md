# FUD Malware

School project, deadline 1st of April.

MessageBox shellcode Working, spotted 15 on VT for now

todo :
 - Improve loader (how to shellcode properly) :
        - grep and use a legit PPID
        - recode IAT stuff in order to avoir functions imports in clear
        - change XOR system (either 1 byte or 1:1 clear word encoded)
 - Anti sandbox checks
 - Lighter compilation