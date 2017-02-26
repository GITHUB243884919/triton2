usr=root
pwd=game

if $(test $# -ne 4 ) 
then
echo "wrong param"
exit
fi
/usr/local/mysql/bin/mysql  -u$2  -p$3 $4<< !
drop table if exists $1_LOG_ROLE_TASK_01;
drop table if exists $1_LOG_ROLE_KILLED_01;
drop table if exists $1_LOG_ROLE_LOGOUT_01;
drop table if exists $1_LOG_ROLE_EXP_01;
drop table if exists $1_LOG_ROLE_EXP_02;
drop table if exists $1_LOG_ROLE_EXP_03;
drop table if exists $1_LOG_ROLE_EXP_04;
drop table if exists $1_LOG_ROLE_EXP_05;
drop table if exists $1_LOG_ROLE_EXP_06;
drop table if exists $1_LOG_ROLE_EXP_07;
drop table if exists $1_LOG_ROLE_EXP_08;
drop table if exists $1_LOG_ROLE_EXP_09;
drop table if exists $1_LOG_ROLE_EXP_10;
drop table if exists $1_LOG_ROLE_EXP_11;
drop table if exists $1_LOG_ROLE_EXP_12;
drop table if exists $1_LOG_ROLE_EXP_13;
drop table if exists $1_LOG_ROLE_EXP_14;
drop table if exists $1_LOG_ROLE_EXP_15;
drop table if exists $1_LOG_ROLE_MONEY_01;
drop table if exists $1_LOG_ROLE_ITEM_01;
drop table if exists $1_LOG_ROLE_GHOST_01;
drop table if exists $1_LOG_ITEM_UPGRADE_01;
drop table if exists $1_LOG_ORGE_DROPED_01;
drop table if exists $1_LOG_ITEM_DISPEAR_01;
drop table if exists $1_ID_S2S_EXECUTESQL_REQUEST_01;
drop table if exists $1_LOG_FAMILY_01;
drop table if exists $1_LOG_CORPS_01;
drop table if exists $1_LOG_ROLE_MAIL_01;
drop table if exists $1_LOG_SERVER_STAT_01;
drop table if exists $1_LOG_ROLE_CHAT_01;
drop table if exists $1_LOG_EXCHANGE_ACCOUNT_01;
drop table if exists $1_LOG_REPETION_01;
drop table if exists $1_LOG_ITEM_GHOST_01;
drop table if exists $1_LOG_LIFESKILL_01;
drop table if exists $1_LOG_BOSSDEATH_01;
drop table if exists $1_LOG_ROLE_UPGRADE_01;
drop table if exists $1_LOG_ACTION_MONIT_01;
drop table if exists $1_LOG_EXP_PRODUCE_01;
drop table if exists $1_LOG_ITEM_PRODUCE_01; 
drop table if exists $1_LOG_ITEM_RECYCLE_01; 
drop table if exists $1_LOG_MONEY_PRODUCE_01;
drop table if exists $1_LOG_MONEY_RECYCLE_01;
drop table if exists $1_LOG_ROLE_EXP_16;
!

