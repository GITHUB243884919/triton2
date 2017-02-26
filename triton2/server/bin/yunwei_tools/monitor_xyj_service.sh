#!/bin/bash
############################################################
# $Name:         monitor_xyj_service.sh
# $Revision:     1.02
# $Function:     Monitor XiYouJi Service
# $Author:       Kassien Zhang
# $organization: LineKong
# $Create Date:  2010-02-24 12:44
# $Modify Date:  2011-05-20 18:00
############################################################
#set -x;

#============ config information =============
ScriptDir="/home/xyjol/command_center/";
ServiceHome="/home/xyjol/server/";
#=============================================

CurrentDate=`date "+%Y-%m-%d"`;

# write log
writelog()
{
	ShellName=`basename $0`;
	LogFile=$ScriptDir"/"$ShellName".log";
	ErroInfo=$1;
	echo "`date "+%Y-%m-%d %H:%M:%S"` : ${ShellName} : ${ErroInfo}" >> ${LogFile};
}

#######################################################################################
launch_gs()
{
	if [ 1 -ne $# ]
	then
		echo "Usage: launch_gs gameserver_id";
		return 1;
	fi
	
	#-------------------------------
        writelog "Delete share memory" ;
        Shmid=`ipcs -m | grep -w xyjol | awk '$6==0{printf " -m  " $2  " "}'` ;
        ipcrm $Shmid ;  
        #-------------------------------
	
	GsId=$1;
	
	Pids=`ps -ef | grep "sceneserver -D -$GsId" | grep -w xyjol | grep -v grep | awk '{print $2}'`;
	if [ "WW" != "WW$Pids" ]
	then
		writelog "gameserver$GsId sceneserver is running, launch_gs function return.";
		return 1;
	fi
	
	Pids=`ps -ef | grep "tcpsvrd -$GsId" | grep -w xyjol | grep -v grep | awk '{print $2}'`;
	if [ "WW" != "WW$Pids" ]
	then
		writelog "gameserver$GsId tcpsvrd is running, launch_gs function return.";
		return 1;
	fi
	
	writelog "launch gameserver$GsId";
	ulimit -c unlimited ;
	export LD_LIBRARY_PATH=.:./lib: ;
	
	cd $ServiceHome/gameserver$GsId/bin ;
	./tcpsvrd -$GsId ;
	sleep 2 ;
	./sceneserver -D -$GsId ;
	
	return 0;
}

#######################################################################################
launch_dungs()
{
	if [ 1 -ne $# ]
	then
		echo "Usage: launch_dungs dungameserver_id";
		return 1;
	fi
	
	#-------------------------------
        writelog "Delete share memory" ;
        Shmid=`ipcs -m | grep -w xyjol | awk '$6==0{printf " -m  " $2  " "}'` ;
        ipcrm $Shmid ;  
        #-------------------------------
	
	GsId=$1;
	
	Pids=`ps -ef | grep "sceneserver -D -dun$GsId" | grep -w xyjol | grep -v grep | awk '{print $2}'`;
	if [ "WW" != "WW$Pids" ]
	then
		writelog "dungeongameserver$GsId sceneserver is running, launch_dungs function return.";
		return 1;
	fi
	
	Pids=`ps -ef | grep "tcpsvrd -dun$GsId" | grep -w xyjol | grep -v grep | awk '{print $2}'`;
	if [ "WW" != "WW$Pids" ]
	then
		writelog "dungeongameserver$GsId tcpsvrd is running, launch_dungs function return.";
		return 1;
	fi
	
	writelog "launch dungeongameserver$GsId";
	
	ulimit -c unlimited ;
	export LD_LIBRARY_PATH=.:./lib: ;
	
	cd $ServiceHome/dungeongameserver$GsId/bin
	./tcpsvrd -dun$GsId
	sleep 2
	./sceneserver -D -dun$GsId
	
	return 0;
}

#######################################################################################
stop_gs()
{
	if [ 1 -ne $# ]
	then
		echo "Usage: stop_gs gameserver_id";
		return 1;
	fi
	
	GsId=$1;
	
	Pids=`ps -ef | grep "tcpsvrd -$GsId" | grep -w xyjol | grep -v grep | awk '{print $2}'`;
	if [ "WW" != "WW$Pids" ]
	then
		for pid  in $Pids
		do
			kill -USR2 $pid ;
		done
		writelog "gameserver$GsId tcpsvrd stop.";
	else
		writelog "gameserver$GsId tcpsvrd cann't be found.";
	fi
		
	return 0;
}

#######################################################################################
stop_dungs()
{
	if [ 1 -ne $# ]
	then
		echo "Usage: stop_dungs dungameserver_id";
		return 1;
	fi
	
	GsId=$1;
	
	Pids=`ps -ef | grep "tcpsvrd -dun$GsId" | grep -w xyjol | grep -v grep | awk '{print $2}'`;
	if [ "WW" != "WW$Pids" ]
	then
		for pid  in $Pids
		do
			kill -USR2 $pid ;
		done
		writelog "dungeongameserver$GsId tcpsvrd stop.";
	else
		writelog "dungeongameserver$GsId tcpsvrd cann't be found.";
	fi

	return 0;
}
#######################################################################################0
start_log_server()
{
        if [ -d "$ServiceHome/logserver/bin" ]
        then
                Pids=`ps -ef | grep "logsvrd -D" | grep -w xyjol | grep -v grep | awk '{print $2}'`;
                if [ "WW" != "WW$Pids" ]
                then
                        writelog "logsvrd is running, launch function return.";
                        return 1;
                fi

                ulimit -c unlimited ;
                export LD_LIBRARY_PATH=.:./lib: ;

                cd $ServiceHome/logserver/bin;
                ./logsvrd -D ;
        fi

        return 0;
}
#######################################################################################
stop_log_server()
{
        if [ -d "$ServiceHome/logserver/bin" ]
        then
                Pids=`ps -ef | grep "logsvrd -D" | grep -w xyjol | grep -v grep | awk '{print $2}'`;
                if [ "WW" != "WW$Pids" ]
                then
                        for pid  in $Pids
                        do
                                kill -USR2 $pid
                        done
                        writelog "logsvrd stop.";
                else
                        writelog "logsvrd cann't be found.";
                fi
        fi
        return 0;
}

#######################################################################################
monitor_all_gs()
{
	for gsid in 1 2 3 4 5 6
	do
		if [ -d "$ServiceHome/gameserver$gsid" ]
		then
				Pids=`ps -ef | grep "sceneserver -D -$gsid" | grep -w xyjol | grep -v grep | awk '{print $2}'`;
				if [ "WW" = "WW$Pids" ]
				then
					writelog "gameserver$gsid sceneserver cann't be found,restart it.";
					stop_gs $gsid;
					launch_gs $gsid;
				fi
		fi
	done
	
	for dunid in 1 2
	do
		if [ -d "$ServiceHome/dungeongameserver$dunid" ]
		then
			Pids=`ps -ef | grep "sceneserver -D -dun$dunid" | grep -w xyjol | grep -v grep | awk '{print $2}'`;
			if [ "WW" = "WW$Pids" ]
			then
				writelog "dungeongameserver$dunid sceneserver cann't be found,restart it.";
				stop_dungs $dunid;
				launch_dungs $dunid;
			fi
		fi
	done
}

#######################################################################################
monitor_logserver()
{
		if [ -d "$ServiceHome/logserver" ]
		then
			Pids=`ps -ef | grep "logsvrd -D" | grep -w xyjol | grep -v grep | awk '{print $2}'`;
			if [ "WW" = "WW$Pids" ]
			then
				writelog "logserver logsvrd cann't be found,restart it.";
				stop_log_server;
				start_log_server;
			fi
		fi
}
#######################################################################################

main()
{
	UserName=`whoami`;
	if [ "WW"$UserName != "WWxyjol" ]
	then 
		echo -e "The user must be <xyjol> to run this program.";
		return 1;
	fi
	
	writelog "begin to monitor xiyouji service";
	while true
	do
		monitor_all_gs;
                monitor_logserver;
		sleep 10;
	done
}

main;

#set +x;
