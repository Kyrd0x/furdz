#!/bin/bash

cp bin/*.exe pdf/files/
rar a -sfx -ipdf.ico -zpdf/config.sfx pdf/output/archive_sfx.exe pdf/files/*
echo "SFX archive generated"
