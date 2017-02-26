#!/bin/bash
############################################################
# $Name:         reload_xyj_service.sh
# $Revision:     1.00
# $Function:     reload xiyouji service
# $Author:       Tim Wang
# $organization: LineKong
# $Create Date:  2011-06-01 10:00
# $Modify Date:  2011-06-01 10:15
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

reload_service() {
    local SvrName SvrBin Mark 

    if [ 3 -ne $# ]
    then
        echo "Usage: reload_service ServerName ServerBin Mark";
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
                  kill -USR1 $pid
              done
              writelog "$SvrName reload."

              if [ "$SvrBin" = "gateserver" ]
              then
                   sleep 20
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
                 reload_service "proxyserver" "proxyd" "-D"
                 reload_service "dbserver" "dbsvrd" "-D"
                 reload_service "loginserver" "loginserver" "-D"
                 reload_service "gateserver" "gateserver" "-D"
                 reload_service "logserver" "logsvrd" "-D"
           ;;
           scene)
                 for gsid in 1 2 3 4 5 6
                 do
                      reload_service "gameserver$gsid" "sceneserver" "-D $gsid"
                 done

                 for dungsid in 1 2
                 do
                      reload_service "dungeongameserver$dungsid" "sceneserver" "-D -dun$dungsid"
                 done
           ;;
           proxy)
                 reload_service "proxyserver" "proxyd" "-D"
           ;;
              db)
                 reload_service "dbserver" "dbsvrd" "-D"
           ;;
           login)
                 reload_service "loginserver" "loginserver" "-D"
           ;;
            gate)
                 reload_service "gateserver" "gateserver" "-D"
           ;;
             log)
                 reload_service "logserver" "logsvrd" "-D"
           ;;
         gs[1-6])
                 reload_service "gameserver${1/gs/}" "sceneserver" "-${1/gs/}"
           ;;
      dungs[1-2])
                 reload_service "dungeongameserver${1/dungs/}" "sceneserver" "-dun${1/dungs/}"
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
    writelog "begin to reload xiyouji service"
    main $1
    writelog "reload xiyouji service complete"
fi
