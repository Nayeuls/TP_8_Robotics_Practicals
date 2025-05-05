#!/bin/bash
find . -name "*.cc" -o -name "*.c" -o -name "*.h" -o -name "*.ipynb" -o -name "Makefile" -o -name "Makefile.inc" -o -name "*.pdf" -o -name "*.csv" >/tmp/files.txt
echo "Excluding:"
find . -path ./.git -prune -o -type f | grep -vFxf /tmp/files.txt -
rm -f tp8.zip
7z a -tzip -mx=9 -spf tp8.zip @/tmp/files.txt
