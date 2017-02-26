#!/bin/sh

TRITON_HOME=~/

_DB_SERVER_IP_="180.148.133.10"
_MYSQL_CONN_="180.148.133.10;3306;1fdddc61f037a5208ff1c4b8a1d5b386;8df4c7bd2e34fc97d268d0da78236dd8;GameDB"
_LOG_SERVER_IP_="180.148.133.10"
_LOGDB_CONN_="180.148.133.12;3306;root;game;GameDB"
_GM_SERVER_PORT_="10001"
_CHARGE_SERVER_PORT_="15159"
_PROXY_SERVER_IP_="180.148.133.19"
_PROXY_SERVER_PORT_="10008"
_LOGIN_SERVER_PORT_="10000"
_LOGIN_SERVER_IP_="180.148.133.19"
_GATE_SERVER_IP_="180.148.133.19"
_GATE_SERVER_PORT_="10002"
_GAME1_SERVER_IP_=" 180.148.133.13"
_GAME1_SERVER_PORT_=" 10000"
_GAME1_SERVER_ID_="1"
_GAME2_SERVER_IP_="180.148.133.14"
_GAME2_SERVER_PORT_="10000"
_GAME2_SERVER_ID_="2"
_GAME3_SERVER_IP_="180.148.133.15"
_GAME3_SERVER_PORT_="10000"
_GAME3_SERVER_ID_="3"
_GAME4_SERVER_IP_="180.148.133.16"
_GAME4_SERVER_PORT_="10000"
_GAME4_SERVER_ID_="4"
_GAME5_SERVER_IP_="180.148.133.17"
_GAME5_SERVER_PORT_="10000"
_GAME5_SERVER_ID_="5"
_FB1_SERVER_IP_="180.148.133.18"
_FB1_SERVER_PORT_="10000"
_FB1_SERVER_ID_="6"
_FB2_SERVER_IP_="180.148.133.19"
_FB2_SERVER_PORT_="10003"
_FB2_SERVER_ID_="7"
_ERATING_GATE_ID="840001"
_ERATING_SERVER_IP_="118.69.199.20"
_ERATING_SERVER_PORT_="9001"
_ERATING_GAMEID_="504"
_ERATING_WORLDID_="801"
_ERATING_GATE_CODE_="gw840001"
_ERATING_GATE_PASSWORD_="5intleo7bpl2664qedvwg5hr5wpz0q43"
_ERATING_GATE_MAC_="000000000000"

replace_scene1 ()
{
sed "{
	s/_GAME1_SERVER_IP_/$_GAME1_SERVER_IP_/g
	s/_GAME1_SERVER_PORT_/$_GAME1_SERVER_PORT_/g
	s/_GAME1_SERVER_ID_/$_GAME1_SERVER_ID_/g
	s/_PROXY_SERVER_IP_/$_PROXY_SERVER_IP_/g
	s/_PROXY_SERVER_PORT_/$_PROXY_SERVER_PORT_/g
	}" $1 > $2
}

replace_scene2 ()
{
sed "{
	s/_GAME1_SERVER_IP_/$_GAME2_SERVER_IP_/g
	s/_GAME1_SERVER_PORT_/$_GAME2_SERVER_PORT_/g
	s/_GAME1_SERVER_ID_/$_GAME2_SERVER_ID_/g
	s/_PROXY_SERVER_IP_/$_PROXY_SERVER_IP_/g
	s/_PROXY_SERVER_PORT_/$_PROXY_SERVER_PORT_/g
	}" $1 > $2
}

replace_scene3 ()
{
sed "{
	s/_GAME1_SERVER_IP_/$_GAME3_SERVER_IP_/g
	s/_GAME1_SERVER_PORT_/$_GAME3_SERVER_PORT_/g
	s/_GAME1_SERVER_ID_/$_GAME3_SERVER_ID_/g
	s/_PROXY_SERVER_IP_/$_PROXY_SERVER_IP_/g
	s/_PROXY_SERVER_PORT_/$_PROXY_SERVER_PORT_/g
	}" $1 > $2
}

replace_scene4 ()
{
sed "{
	s/_GAME1_SERVER_IP_/$_GAME4_SERVER_IP_/g
	s/_GAME1_SERVER_PORT_/$_GAME4_SERVER_PORT_/g
	s/_GAME1_SERVER_ID_/$_GAME4_SERVER_ID_/g
	s/_PROXY_SERVER_IP_/$_PROXY_SERVER_IP_/g
	s/_PROXY_SERVER_PORT_/$_PROXY_SERVER_PORT_/g
	}" $1 > $2
}

replace_scene5 ()
{
sed "{
	s/_GAME1_SERVER_IP_/$_GAME5_SERVER_IP_/g
	s/_GAME1_SERVER_PORT_/$_GAME5_SERVER_PORT_/g
	s/_GAME1_SERVER_ID_/$_GAME5_SERVER_ID_/g
	s/_PROXY_SERVER_IP_/$_PROXY_SERVER_IP_/g
	s/_PROXY_SERVER_PORT_/$_PROXY_SERVER_PORT_/g
	}" $1 > $2
}

replace_scene6 ()
{
sed "{
	s/_GAME1_SERVER_IP_/$_FB1_SERVER_IP_/g
	s/_GAME1_SERVER_PORT_/$_FB1_SERVER_PORT_/g
	s/_GAME1_SERVER_ID_/$_FB1_SERVER_ID_/g
	s/_PROXY_SERVER_IP_/$_PROXY_SERVER_IP_/g
	s/_PROXY_SERVER_PORT_/$_PROXY_SERVER_PORT_/g
	}" $1 > $2
}

replace_scene7 ()
{
sed "{
	s/_GAME1_SERVER_IP_/$_FB2_SERVER_IP_/g
	s/_GAME1_SERVER_PORT_/$_FB2_SERVER_PORT_/g
	s/_GAME1_SERVER_ID_/$_FB2_SERVER_ID_/g
	s/_PROXY_SERVER_IP_/$_PROXY_SERVER_IP_/g
	s/_PROXY_SERVER_PORT_/$_PROXY_SERVER_PORT_/g
	}" $1 > $2
}

replace_param ()
{
sed "{
	s/_DB_SERVER_IP_/$_DB_SERVER_IP_/g
	s/_MYSQL_CONN_/$_MYSQL_CONN_/g
	s/_LOG_SERVER_IP_/$_LOG_SERVER_IP_/g
	s/_LOGDB_CONN_/$_LOGDB_CONN_/g
	s/_GM_SERVER_PORT_/$_GM_SERVER_PORT_/g
	s/_CHARGE_SERVER_PORT_/$_CHARGE_SERVER_PORT_/g
	s/_ECHARGE_ID_/$_ERATING_GATE_ID/g
	s/_ECHARGE_CODE_/$_ERATING_GATE_CODE_/g
	s/_ECHARGE_PWD_/$_ERATING_GATE_PASSWORD_/g
	s/_PROXY_SERVER_IP_/$_PROXY_SERVER_IP_/g
	s/_PROXY_SERVER_PORT_/$_PROXY_SERVER_PORT_/g
	s/_LOGIN_SERVER_PORT_/$_LOGIN_SERVER_PORT_/g
	s/_LOGIN_SERVER_IP_/$_LOGIN_SERVER_IP_/g
	s/_GATE_SERVER_IP_/$_GATE_SERVER_IP_/g
	s/_GATE_SERVER_PORT_/$_GATE_SERVER_PORT_/g
	s/_GAME1_SERVER_IP_/$_GAME1_SERVER_IP_/g
	s/_GAME1_SERVER_PORT_/$_GAME1_SERVER_PORT_/g
	s/_GAME1_SERVER_ID_/$_GAME1_SERVER_ID_/g
	s/_GAME2_SERVER_IP_/$_GAME2_SERVER_IP_/g
	s/_GAME2_SERVER_PORT_/$_GAME2_SERVER_PORT_/g
	s/_GAME2_SERVER_ID_/$_GAME2_SERVER_ID_/g
	s/_GAME3_SERVER_IP_/$_GAME3_SERVER_IP_/g
	s/_GAME3_SERVER_PORT_/$_GAME3_SERVER_PORT_/g
	s/_GAME3_SERVER_ID_/$_GAME3_SERVER_ID_/g
	s/_GAME4_SERVER_IP_/$_GAME4_SERVER_IP_/g
	s/_GAME4_SERVER_PORT_/$_GAME4_SERVER_PORT_/g
	s/_GAME4_SERVER_ID_/$_GAME4_SERVER_ID_/g
	s/_GAME5_SERVER_IP_/$_GAME5_SERVER_IP_/g
	s/_GAME5_SERVER_PORT_/$_GAME5_SERVER_PORT_/g
	s/_GAME5_SERVER_ID_/$_GAME5_SERVER_ID_/g
	s/_FB1_SERVER_IP_/$_FB1_SERVER_IP_/g
	s/_FB1_SERVER_PORT_/$_FB1_SERVER_PORT_/g
	s/_FB1_SERVER_ID_/$_FB1_SERVER_ID_/g
	s/_FB2_SERVER_IP_/$_FB2_SERVER_IP_/g
	s/_FB2_SERVER_PORT_/$_FB2_SERVER_PORT_/g
	s/_FB2_SERVER_ID_/$_FB2_SERVER_ID_/g
	s/_ERATING_SERVER_IP_/$_ERATING_SERVER_IP_/g
	s/_ERATING_SERVER_PORT_/$_ERATING_SERVER_PORT_/g
	s/_ERATING_GAMEID_/$_ERATING_GAMEID_/g
	s/_ERATING_WORLDID_/$_ERATING_WORLDID_/g
	s/_ERATING_GATE_CODE_/$_ERATING_GATE_CODE_/g
	s/_ERATING_GATE_PASSWORD_/$_ERATING_GATE_PASSWORD_/g
	s/_ERATING_GATE_MAC_/$_ERATING_GATE_MAC_/g
	}" $1 > $2
}




#loginserver
LOGINBASE_SRC=$TRITON_HOME/server/loginserver/config/data/ip.xml
LOGINBASE_DST=$TRITON_HOME/server/loginserver/config/ip.xml
echo "configure server/loginserver/config/ip.xml"
replace_param $LOGINBASE_SRC $LOGINBASE_DST

LOGINPORT_SRC=$TRITON_HOME/server/loginserver/config/data/tcpsvrd.cfg
LOGINPORT_DST=$TRITON_HOME/server/loginserver/config/tcpsvrd.cfg
echo "configure server/loginserver/config/tcpsvrd.cfg"
replace_param $LOGINPORT_SRC $LOGINPORT_DST

#gateserver
GATEBASE_SRC=$TRITON_HOME/server/gateserver/config/data/ip.xml
GATEBASE_DST=$TRITON_HOME/server/gateserver/config/ip.xml
echo "configure server/gateserver/config/ip.xml"
replace_param $GATEBASE_SRC $GATEBASE_DST

GATEPORT_SRC=$TRITON_HOME/server/gateserver/config/data/tcpsvrd.cfg
GATEPORT_DST=$TRITON_HOME/server/gateserver/config/tcpsvrd.cfg
echo "configure server/gateserver/config/tcpsvrd.cfg"
replace_param $GATEPORT_SRC $GATEPORT_DST

#proxyserver
PROXYBASE_SRC=$TRITON_HOME/server/proxyserver/config/data/proxysvrd.xml
PROXYBASE_DST=$TRITON_HOME/server/proxyserver/config/proxysvrd.xml
echo "configure server/proxyserver/config/proxysvrd.xml"
replace_param $PROXYBASE_SRC $PROXYBASE_DST

#dbserver
DBBASE_SRC=$TRITON_HOME/server/dbserver/config/data/dbserver.xml
DBBASE_DST=$TRITON_HOME/server/dbserver/config/dbserver.xml
echo "configure server/dbserver/config/dbserver.xml"
replace_param $DBBASE_SRC $DBBASE_DST

#logserver
LOGBASE_SRC=$TRITON_HOME/server/logserver/config/data/logserver.xml
LOGBASE_DST=$TRITON_HOME/server/logserver/config/logserver.xml
echo "configure server/logserver/config/logserver.xml"
replace_param $LOGBASE_SRC $LOGBASE_DST

#gameserver1
GAME1BASE_SRC=$TRITON_HOME/server/gameserver1/config/sceneserver.xml.base
GAME1BASE_DST=$TRITON_HOME/server/gameserver1/config/sceneserver.xml
echo "configure server/gameserver1/config/sceneserver.xml"
replace_scene1 $GAME1BASE_SRC $GAME1BASE_DST

GAME1PORT_SRC=$TRITON_HOME/server/gameserver1/config/tcpsvrd.cfg.base
GAME1PORT_DST=$TRITON_HOME/server/gameserver1/config/tcpsvrd.cfg
echo "configure server/gameserver1/config/tcpsvrd.cfg"
replace_scene1 $GAME1PORT_SRC $GAME1PORT_DST

#gameserver2
GAME2BASE_SRC=$TRITON_HOME/server/gameserver2/config/sceneserver.xml.base
GAME2BASE_DST=$TRITON_HOME/server/gameserver2/config/sceneserver.xml
echo "configure server/gameserver2/config/sceneserver.xml"
replace_scene2 $GAME2BASE_SRC $GAME2BASE_DST

GAME2PORT_SRC=$TRITON_HOME/server/gameserver2/config/tcpsvrd.cfg.base
GAME2PORT_DST=$TRITON_HOME/server/gameserver2/config/tcpsvrd.cfg
echo "configure server/gameserver2/config/tcpsvrd.cfg"
replace_scene2 $GAME2PORT_SRC $GAME2PORT_DST


#gameserver3
GAME3BASE_SRC=$TRITON_HOME/server/gameserver3/config/sceneserver.xml.base
GAME3BASE_DST=$TRITON_HOME/server/gameserver3/config/sceneserver.xml
echo "configure server/gameserver3/config/sceneserver.xml"
replace_scene3 $GAME3BASE_SRC $GAME3BASE_DST

GAME3PORT_SRC=$TRITON_HOME/server/gameserver3/config/tcpsvrd.cfg.base
GAME3PORT_DST=$TRITON_HOME/server/gameserver3/config/tcpsvrd.cfg
echo "configure server/gameserver3/config/tcpsvrd.cfg"
replace_scene3 $GAME3PORT_SRC $GAME3PORT_DST

#gameserver4
GAME4BASE_SRC=$TRITON_HOME/server/gameserver4/config/sceneserver.xml.base
GAME4BASE_DST=$TRITON_HOME/server/gameserver4/config/sceneserver.xml
echo "configure server/gameserver4/config/sceneserver.xml"
replace_scene4 $GAME4BASE_SRC $GAME4BASE_DST

GAME4PORT_SRC=$TRITON_HOME/server/gameserver4/config/tcpsvrd.cfg.base
GAME4PORT_DST=$TRITON_HOME/server/gameserver4/config/tcpsvrd.cfg
echo "configure server/gameserver4/config/tcpsvrd.cfg"
replace_scene4 $GAME4PORT_SRC $GAME4PORT_DST

#gameserver5
GAME5BASE_SRC=$TRITON_HOME/server/gameserver5/config/sceneserver.xml.base
GAME5BASE_DST=$TRITON_HOME/server/gameserver5/config/sceneserver.xml
echo "configure server/gameserver5/config/sceneserver.xml"
replace_scene5 $GAME5BASE_SRC $GAME5BASE_DST

GAME5PORT_SRC=$TRITON_HOME/server/gameserver5/config/tcpsvrd.cfg.base
GAME5PORT_DST=$TRITON_HOME/server/gameserver5/config/tcpsvrd.cfg
echo "configure server/gameserver5/config/tcpsvrd.cfg"
replace_scene5 $GAME5PORT_SRC $GAME5PORT_DST

#dungeongameserver1
FB1BASE_SRC=$TRITON_HOME/server/dungeongameserver1/config/sceneserver.xml.base
FB1BASE_DST=$TRITON_HOME/server/dungeongameserver1/config/sceneserver.xml
echo "configure server/dungeongameserver1/config/sceneserver.xml"
replace_scene6 $FB1BASE_SRC $FB1BASE_DST

FB1PORT_SRC=$TRITON_HOME/server/dungeongameserver1/config/tcpsvrd.cfg.base
FB1PORT_DST=$TRITON_HOME/server/dungeongameserver1/config/tcpsvrd.cfg
echo "configure server/dungeongameserver1/config/tcpsvrd.cfg"
replace_scene6 $FB1PORT_SRC $FB1PORT_DST

#dungeongameserver2
FB2BASE_SRC=$TRITON_HOME/server/dungeongameserver2/config/sceneserver.xml.base
FB2BASE_DST=$TRITON_HOME/server/dungeongameserver2/config/sceneserver.xml
echo "configure server/dungeongameserver2/config/sceneserver.xml"
replace_scene7 $FB2BASE_SRC $FB2BASE_DST

FB2PORT_SRC=$TRITON_HOME/server/dungeongameserver2/config/tcpsvrd.cfg.base
FB2PORT_DST=$TRITON_HOME/server/dungeongameserver2/config/tcpsvrd.cfg
echo "configure server/dungeongameserver2/config/tcpsvrd.cfg"
replace_scene7 $FB2PORT_SRC $FB2PORT_DST
