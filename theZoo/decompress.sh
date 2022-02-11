#!/usr/bin/env bash

# malware/Source/Original/*.zip
P="./**/*zip"
pass="infected"
for file in $P
do
    dir=`dirname "${file}"`
    7z x -y -o"${dir}" -p"${pass}" "${file}"
done
