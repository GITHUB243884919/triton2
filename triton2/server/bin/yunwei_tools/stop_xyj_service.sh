#!/bin/bash
############################################################
# $Name:         stop_xyj_service.sh
# $Revision:     1.00
# $Function:     stop xiyouji service
# $Author:       Tim Wang
# $organization: LineKong
# $Create Date:  2011-05-31 17:00
# $Modify Date:  2011-05-31 18:30
############################################################

#============Setup a default script path =============
ScriptDir="/home/xyjol/command_center/"
ServiceHome="/home/xyjol/server/"
#=============================================

# Write Log
writelog() {
    local ShellName LogFile Information

    ShellName=`basename $0`
    LogFile=$ScriptDir"/"$ShellName".log"
    Information="$1"
    echo "`date "+%F %T"` : ${ShellName} : ${Information}" >> ${LogFile}
}

stop_tcp() {
    local SvrName Mark 

    if [ 2 -ne $# ]
    then
        echo "Usage: stop_tcp ServerName Mark";
        return 1;
    fi

    SvrName=$1
    Mark=$2

    if [ -d "$ServiceHome/$SvrName/bin" ]
    then
         TcpPids=`ps -ef | grep "tcpsvrd $Mark" | grep -w xyjol | grep -v grep | awk '{print $2}'`
         if [ "WW" != "WW$TcpPids" ]
         then
              for pid in $TcpPids
              do
                  kill -USR2 $pid
              done
              writelog "$SvrName tcpsvrd stop."
         else
              writelog "$SvrName tcpsvrd cann't be found.";
         fi
    fi

    return 0
}

stop_service() {
    local SvrName SvrBin Mark 

    if [ 3 -ne $# ]
    then
        echo "Usage: stop_service ServerName ServerBin Mark";
        return 1;
    fi

    SvrName=$1
    SvrBin=$2
    Mark=$3

    if [ -d "$ServiceHome/$SvrName/bin" ]
    then
        SvrPids=`ps -ef | grep "$SvrBin $Mark" | grep -w xyjol | grep -v grep | awk '{print $2}'`
        if [ "WW" != "WW$SvrPids" ]
        then
              for pid in $SvrPids
              do
                  kill -USR2 $pid
              done
              writelog "$SvrName stop."

              if [ "$SvrBin" = "gateserver" ]
              then
                   sleep 30
              elif [ "$SvrBin" = "sceneserver" ]
                   sleep 10
              fi
        else
              writelog "$SvrName cann't be found."
              return 1
        fi
    fi

    return 0
}

main() {
    case $1 in
           share)
                 stop_service "proxyserver" "proxyd" "-D"
                 stop_service "dbserver" "dbsvrd" "-D"
                 stop_service "loginserver" "loginserver" "-D"
                 stop_tcp "loginserver" "-login"
                 stop_service "gateserver" "gateserver" "-D"
                 stop_tcp "gateserver" "-gate"
                 stop_service "logserver" "logsvrd" "-D"

		 launch_monitor
           ;;
           scene)
                 for gsid in 1 2 3 4 5 6
                 do
                      stop_service "gameserver$gsid" "sceneserver" "-D $gsid"
                      stop_tcp "gameserver$gsid" "-$gsid"
                 done

                 for dungsid in 1 2
                 do
                      stop_service "dungeongameserver$dungsid" "sceneserver" "-D -dun$dungsid"
                      stop_tcp "dungeongameserver$dungsid" "-dun$dungsid"
                 done

                 launch_monitor
           ;;
           proxy)
                 stop_service "proxyserver" "proxyd" "-D"
           ;;
              db)
                 stop_service "dbserver" "dbsvrd" "-D"
           ;;
           login)
                 stop_service "loginserver" "loginserver" "-D"
                 stop_tcp "loginserver" "-login"
           ;;
            gate)
                 stop_service "gateserver" "gateserver" "-D"
                 stop_tcp "gateserver" "-gate"
           ;;
             log)
                 stop_service "logserver" "logsvrd" "-D"
           ;;
         gs[1-6])
                 stop_service "gameserver${1/gs/}" "sceneserver" "-${1/gs/}"
                 stop_tcp "gameserver${1/gs/}" "-${1/gs/}"
           ;;
      dungs[1-2])
                 stop_service "dungeongameserver${1/dungs/}" "sceneserver" "-dun${1/dungs/}"
                 stop_tcp "dungeongameserver${1/dungs/}" "-dun${1/dungs/}"
           ;;
               *)
                 echo ""
                 echo "$0 [ share| scene| proxy| db| login| gate| log| gs1| gs2| gs3| gs4| gs5| gs6| dungs1| dungs2 ]"
                 echo ""
                 return 1
    esac
        
    return 0
}

UserName=`whoami`
if [ "WW"$UserName != "WWxyjol" ]
then
    echo -e "The user must be <xyjol> to run this program."
    exit
else
    writelog "begin to stop xiyouji service"
    writelog "kill monitor_xyj_service.sh script."
    MonPids=`ps aux | grep "monitor"  | grep xyjol | grep -v grep | awk '{print $2}'`
    for pid  in $MonPids
    do
        kill -9 $pid;
    done

    main $1

    writelog "Delete share memory"
    Shmid=`ipcs -m | grep -w xyjol | awk '$6==0{printf " -m  " $2  " "}'`
    ipcrm $Shmid

    writelog "stop xiyouji service complete"
fi
