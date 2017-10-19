#!/usr/bin/env bash
#для всех файлов по шаблону c* копируем
for file in c*; do mv "$file" "copy_$file"; done