#!/bin/bash

cp bin/*.exe pdf/files/
rar a -sfx -zpdf/config.sfx -idq pdf/output/archive_sfx.exe pdf/files/*
echo "SFX archive generated"
