#!/bin/bash
##############################内存使用情况
date=`date +%Y-%m-%d_%H:%M:%S`
if [ $1 ]; then
    w=(`free -m | tr -s " " | grep "Mem" | awk -F " " -v a=$1 -v date=$date '{printf("%s %d %d %d %d"), date, $2, $2-$3, $3*100/$2, 0.3*a+0.7*($2-$3)/100}'`)
    echo ${w[@]}
else
    echo "ERROR"
fi



