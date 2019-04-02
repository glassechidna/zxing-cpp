#!/bin/bash


cd "$(dirname "$0")";

find . -name "*.png" | while read file; do
  #zxing has a --test-mode but it's not working for binary files
  # perl is here because in textmode, zxing appends a newline, but a lot of test files don't have that
  zxing "$file" | perl -pe 'chomp if eof' | diff -u "$(ls "${file%.*}".{txt,bin} 2>/dev/null)" -
done
