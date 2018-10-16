#!/bin/bash
###################################用户信息统计

date=`date +%Y-%m-%d_%H:%M:%S`
usernum=`cat /etc/passwd | awk -F ":" '{if($3 >= 1000 && $3 != 65534){printf("%s\n", $1)}}' | wc -l`
###################近期活跃用户(3个)
useract=`last | grep -v "wtmp" | cut -d " " -f 1 | sort | uniq -c | sort -n -r | awk -F " " 'BEGIN {i=0} { if(i++<3 && $2!=""){ if (i > 1) {printf(",")} printf("%s", $2) } }'`
###################具有root权限的用户
admi=`cat /etc/group | grep "sudo" | cut -d : -f 4`
id=`w | tail -n +3 | awk -F " " 'BEGIN {i=0} {if (i > 0) {printf(",")} i++; printf("%s_%s_%s",$1 ,$3 ,$2)}'`
echo $date $usernum [$useract] [$admi] [$id]

