#!/bin/bash
############################################################
# $Name:         check_xyj_service.sh
# $Revision:     1.00
# $Function:     Check xiyouji service startup exit and version information
# $Author:       Tim Wang
# $organization: LineKong
# $Create Date:  2011-05-13 14:00
# $Modify Date:  2011-05-13 17:00
############################################################

#============Setup a default script path =============
ScriptDir="/home/xyjol/command_center/"
ServiceHome="/home/xyjol/server/"
CurrentDate=`date "+%F"`
#=============================================

# Check Service start stop and version Information

check_service() {
    local CheckType SvrLogFile SvrInfo SvrName SvrBin TcpsvrdInfo
    
    if [ 4 -ne $# ]
    then
         echo "$0 ServiceName Servicebin [ start| stop| version ] ServiceLogFile"
         return 1;
    fi

    SvrName=$1
    SvrBin=$2
    CheckType=$3
    SerLogFile=$4

    if [ -d "$ServiceHome/$SvrName" ]
    then
         case $CheckType in
               start)
                   SvrInfo="$SvrName : "`grep "$CurrentDate" $ServiceHome/$SvrName/log/$SerLogFile 2>/dev/null | grep "startup" | grep "success" | tail -1`
                   if [ -f "$ServiceHome/$SvrName/bin/tcpsvrd" ]
                   then
                        TcpsvrdInfo="$SvrName : "`grep "$CurrentDate" $ServiceHome/$SvrName/log/tcpsvrd.log 2>/dev/null | grep "startup" | grep "success" | tail -1`
                   fi
                ;;
                
               stop)
                   SvrInfo="$SvrName : "`grep "$CurrentDate" $ServiceHome/$SvrName/log/$SerLogFile 2>/dev/null | grep "exit" | tail -1`
                   if [ -f "$ServiceHome/$SvrName/bin/tcpsvrd" ]
                   then
                        TcpsvrdInfo="$SvrName : "`grep "$CurrentDate" $ServiceHome/$SvrName/log/tcpsvrd.log 2>/dev/null | grep "exit" | tail -1`
                   fi
                ;;
                
               version)
                   export LD_LIBRARY_PATH=.:./lib:
                   cd $ServiceHome/$SvrName/bin
                   SvrInfo="$SvrName : "`./$SvrBin -v`
                   if [ -f "$ServiceHome/$SvrName/bin/tcpsvrd" ]
                   then
                        TcpsvrdInfo="$SvrName : "`./tcpsvrd -v`
                   fi
                ;;

                *)
                        echo "$0 Servicebin [ start| stop| version ] ServiceLogFile" 
                        return 1
                ;;
         esac
                
         echo "$SvrInfo"
         if [ -f "$ServiceHome/$SvrName/bin/tcpsvrd" ]
         then
              echo "$TcpsvrdInfo"
         fi
         return 0
    fi
}

# Check Service Shared Memory segment

check_ipcs() {
	local ShmKey
        ShmKey=`ipcs -m | grep -w xyjol | awk '($6>2 || $7 == "dest"){printf " "$1}'`
        if [ "WW" != "WW$ShmKey" ]
        then
                echo "$ShmKey"
        else
                echo "no problem"
        fi
        return 0
}

main() {
        
        if [ "WW$1" = "WWipcs" -o "WW$2" = "WWipcs" ]
        then
              check_ipcs
        elif [ 2 -eq $# ]
        then
              case $1 in
                       othen)
                             check_service "proxyserver" "proxyd" "$2" "proxyd.log"
                             check_service "dbserver" "dbsvrd" "$2" "dbsvrd.log"
                             check_service "loginserver" "loginserver" "$2" "loginserver.log"
                             check_service "gateserver" "gateserver" "$2" "gateserver.log"
                             check_service "logserver" "logsvrd" "$2" "logsvrd.log"
                   ;;
                       scene)
                             for gsid in 1 2 3 4 5 6
                             do
                                 check_service "gameserver$gsid" "sceneserver" "$2" "sceneserver.log"
                             done

                             for dungsid in 1 2
                             do
                                 check_service "dungeongameserver$dungsid" "sceneserver" "$2" "sceneserver.log"
                             done

                   ;;
                         all)
                             check_service "proxyserver" "proxyd" "$2" "proxyd.log"
                             check_service "dbserver" "dbsvrd" "$2" "dbsvrd.log"
                             check_service "loginserver" "loginserver" "$2" "loginserver.log"
                             check_service "gateserver" "gateserver" "$2" "gateserver.log"
                             check_service "logserver" "logsvrd" "$2" "logsvrd.log"

                             for gsid in 1 2 3 4 5 6
                             do
                                 check_service "gameserver$gsid" "sceneserver" "$2" "sceneserver.log"
                             done

                             for dungsid in 1 2 
                             do
                                 check_service "dungeongameserver$gsid" "sceneserver" "$2" "sceneserver.log"
                             done

                   ;;
                           *)
                              echo ""
                              echo "$0 [ othen| scene| all ] [ start| stop| version| ipcs ]"
                              echo ""
                              return 1
             esac
        else
             echo ""
             echo "$0 [ othen| scene| all ] [ start| stop| version| ipcs ]"
             echo ""
             return 1
        fi
        
        return 0
}

main $1 $2
