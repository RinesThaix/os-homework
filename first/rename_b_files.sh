#!/usr/bin/env bash
#для всех файлов по шаблону b* перемещаем
for file in b*; do mv "$file" "_$file"; done