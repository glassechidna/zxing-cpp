#!/bin/bash

set -ex

cd "$(dirname "$0")";

for file in $(ls *.png); do
  #zxing has a --test-mode but it's not working for binary files
  zxing "$file" | diff $(ls "${file%.*}".{txt,bin} 2>/dev/null) -
done
