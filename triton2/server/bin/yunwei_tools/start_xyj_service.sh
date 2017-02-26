#!/bin/bash
############################################################
# $Name:         start_xyj_service.sh
# $Revision:     1.00
# $Function:     Launch xiyouji service
# $Author:       Tim Wang
# $organization: LineKong
# $Create Date:  2011-05-13 17:00
# $Modify Date:  2011-05-31 17:30
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

launch_tcp() {
    local SvrName Mark 

    if [ 2 -ne $# ]
    then
        echo "Usage: launch_tcp ServerName Mark";
        return 1;
    fi

    SvrName=$1
    Mark=$2

    if [ -d "$ServiceHome/$SvrName/bin" ]
    then
         TcpPids=`ps -ef | grep "tcpsvrd $Mark" | grep -w xyjol | grep -v grep | awk '{print $2}'`
         if [ "WW" != "WW$TcpPids" ]
         then
              writelog "$SvrName tcpsvrd is running."
         else
              ulimit -c unlimited
              export LD_LIBRARY_PATH=.:./lib:
              cd $ServiceHome/$SvrName/bin
              ./tcpsvrd "$Mark"
         fi
    fi

    return 0
}

launch_service() {
    local SvrName SvrBin Mark 

    if [ 3 -ne $# ]
    then
        echo "Usage: launch_service ServerName ServerBin Mark";
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
              writelog "$SvrName is running."
              return 1;
        else
              if [ "$SvrBin" = "gateserver" ]
              then
                   sleep 10
              else
                   sleep 1
              fi

              ulimit -c unlimited ;
              export LD_LIBRARY_PATH=.:./lib:
              cd $ServiceHome/$SvrName/bin
              ./$SvrBin "$Mark" 
              writelog "launch $SvrName"
        fi
    fi

    return 0
}

launch_monitor(){

     LogPids=`ps -ef | egrep "logsvrd -D|" | grep -w xyjol | grep -v grep | awk '{print $2}'`
     if [ "WW" = "WW$Pids" ]
     then
          MonPids=`ps aux | grep "monitor_xyj_service"  | grep xyjol | grep -v grep | awk '{print $2}'`
          if [ "WW" != "WW$Pids" ]
          then
               writelog "monitor_xyj_service.sh is running."
          else
               cd $ScriptDir
               if [ -x monitor_xyj_service.sh ]
               then
                    ./monitor_xyj_service.sh &
               else
                     echo "No such monitor_xyj_service.sh"
               fi
          fi
     fi
}


main() {
    case $1 in
           share)
                 launch_service "proxyserver" "proxyd" "-D"
                 launch_service "dbserver" "dbsvrd" "-D"
                 launch_tcp "loginserver" "-login"
                 launch_service "loginserver" "loginserver" "-D"
                 launch_tcp "gateserver" "-gate"
                 launch_service "gateserver" "gateserver" "-D"
                 launch_service "logserver" "logsvrd" "-D"

		 launch_monitor
           ;;
           scene)
                 for gsid in 1 2 3 4 5 6
                 do
                      launch_tcp "gameserver$gsid" "-$gsid"
                      launch_service "gameserver$gsid" "sceneserver" "-D $gsid"
                 done

                 for dungsid in 1 2
                 do
                      launch_tcp "dungeongameserver$dungsid" "-dun$dungsid"
                      launch_service "dungeongameserver$dungsid" "sceneserver" "-D -dun$dungsid"
                 done

		 launch_monitor
           ;;
           proxy)
                 launch_service "proxyserver" "proxyd" "-D"
           ;;
              db)
                 launch_service "dbserver" "dbsvrd" "-D"
           ;;
           login)
                 launch_tcp "loginserver" "-login"
                 launch_service "loginserver" "loginserver" "-D"
           ;;
            gate)
                 launch_tcp "gateserver" "-gate"
                 launch_service "gateserver" "gateserver" "-D"
           ;;
             log)
                 launch_service "logserver" "logsvrd" "-D"
           ;;
         gs[1-6])
                 launch_tcp "gameserver${1/gs/}" "-${1/gs/}"
                 launch_service "gameserver${1/gs/}" "sceneserver" "-${1/gs/}"
           ;;
      dungs[1-2])
                 launch_tcp "dungeongameserver${1/dungs/}" "-dun${1/dungs/}"
                 launch_service "dungeongameserver${1/dungs/}" "sceneserver" "-dun${1/dungs/}"
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
    writelog "Delete share memory"
    Shmid=`ipcs -m | grep -w xyjol | awk '$6==0{printf " -m  " $2  " "}'`
    ipcrm $Shmid

    main $1
    writelog "launch xiyouji service complete"
fi
