#!/usr/bin/env bash
#cat /dev/urandom => поток рандомных данных
#LC_CTYPE=C => На MacOs tr по какой-то причине не готов работать с binary потоком из рандома, поэтому нужно указать ему Locale
# tr -dc "a-z" => оставить из потока данных только буквы
# fold -w => ширина строки (длина названия файлика - 1 в нашем случае)
# head -c => количество данных (в нашем случае кол-во файлов)
# $RANDOM%3 - рандомное число по модулю 3
# tr 012 abc - формат 0 в a, 1 в b и тд
cat /dev/urandom | LC_CTYPE=C tr -dc "a-z" | fold -w 5 | head -n 100 |
while read x
do
   let rand=$RANDOM%3
   echo $rand | tr 012 abc | xargs echo -n #вывели первую букву
   echo "$x" #и все остальное
done | xargs touch #создали файлы по тому, что построчно в потоке

exit 0