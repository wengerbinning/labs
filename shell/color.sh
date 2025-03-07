#!/bin/sh

srt=${1:-0}
end=${10:-99}

for i in $(seq $srt $end); do
echo -en "\033[${i}m"
printf "%02d" $i
echo -e "\033[0m"
done


COLORTERM=truecolor