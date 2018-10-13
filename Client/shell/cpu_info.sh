#!/bin/bash

date=`date +%Y-%m-%d_%H:%M:%S`
#####平均负载
adv=`cat /proc/loadavg | cut -d " " -f 1-3`

cpu1=`cat /proc/stat | grep -w "cpu" | awk -F " " '{printf("%d %d "), $2 + $3 + $4 + $5 + $6 + $7 + $8 , $5}'`
cpuidle1=`echo $cpu1 | cut -d " " -f 2`
cpu1=`echo $cpu1 | cut -d " " -f 1`

sleep 0.5s

cpu2=`cat /proc/stat | grep -w "cpu" | awk -F " " '{printf("%d %d "), $2 + $3 + $4 + $5 + $6 + $7 + $8, $5}'`
cpuidle2=`echo $cpu2 | cut -d " " -f 2`
cpu2=`echo $cpu2 | cut -d " " -f 1`
########　cpu占用率
cpu=`echo $cpu1 $cpuidle1 $cpu2 $cpuidle2 | awk '{printf("%.2f",(1 - ($4 - $2) / ($3 - $1)) * 100)}'`


####
wendu=`cat /sys/class/thermal/thermal_zone0/temp | awk '{printf("%.2f", $1/1000)}'`
warning=`echo $wendu | awk '{if ($1 <= 50){printf("normal")}else if($1 <= 70){printf("note ")}else { printf("warning")} }'`
echo "$date $adv $cpu"%" $wendu"°C" $warning"
