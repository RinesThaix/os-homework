#!/usr/bin/env bash
#rm a* ?
#ищем файлы в данной директории по шаблону a*, инвокируем вызов rm на них
find . -name "a*" -exec rm {} \;