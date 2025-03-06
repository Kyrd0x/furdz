#!/bin/bash

OUTPUT_FILE=archive_sfx.exe
rm pdf/output/$OUTPUT_FILE
cp bin/*.exe pdf/files/
rar a -sfx -ipdf/pdf.ico -zpdf/config.sfx pdf/output/$OUTPUT_FILE pdf/files/
echo "SFX archive generated"
