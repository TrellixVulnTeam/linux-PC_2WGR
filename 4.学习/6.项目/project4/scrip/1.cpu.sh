#!/bin/bash
function Usage() {
        echo "Usage: $0"

}

if [[ $# -ge 1  ]]; then
        Usage
            exit 1
        fi

        #source ./PiHealth.conf

        if [[ $? -ne 0  ]]; then
                exit 1
            fi

            NowTime=`date +"%Y-%m-%d__%H:%M:%S"`

            loadAvg=`cut -d " " -f 1-3 /proc/loadavg`
            CpuTemp=`cat /sys/class/thermal/thermal_zone0/temp`
            CpuTemp=`echo "scale=2;$CpuTemp/1000" | bc`

            eval $(head -n 1 /proc/stat | awk -v sum1=0 -v idle1=0 '{for (i=2;i<=8;i++) {sum1=sum1+$i} printf("sum1=%d;idle1=%d;",sum1,$5)}' )
            sleep 0.5

            eval $(head -n 1 /proc/stat | awk -v sum2=0 -v idle2=0 '{for (i=2;i<=8;i++) {sum2=sum2+$i} printf("sum2=%d;idle2=%d;",sum2,$5)}' )

            CpuUsedPerc=`echo "scale=4;(1-($idle2-$idle1)/($sum2-$sum1))*100" | bc`
            CpuUsedPerc=`printf "%.2f\n" "$CpuUsedPerc"`

            Warnlevel="normal"

            if [[ `echo $CpuTemp '>=' 70 | bc -l` = 1  ]]; then
                    Warnlevel="warning"
                elif [[ `echo $CpuTemp '>=' 50 | bc -l` = 1  ]]; then
                        Warnlevel="note"
                    fi

                    echo "$NowTime $loadAvg $CpuUsedPerc ${CpuTemp}9°C $Warnlevel"

