#!/bin/bash
date=`date +"%Y-%m-%d_%H_%M_%S"`
user=(`df -m | grep "^/dev" | awk -v k=$date '{printf("%s 1 %s %s %s %s\n", k, $2, $4, $5, $6)}'`)
dnum=0
duse=0
for ((i = 0; i < ${#user[@]}; i+=6)) do
    echo ${user[i]}  ${user[i+1]}  ${user[i+5]}  ${user[i+2]}"M"  ${user[i+3]}"M"  ${user[i+4]}
    ((dnum+=${user[i + 2]}))
    ((duse += ${user[i + 3]}))
done
dbai=`echo $dnum $duse | awk '{printf("%d"), ($1-$2)*100/$1}'`
echo "$date "0" "disk" $dnum"M" $duse"M" $dbai"%""

 
