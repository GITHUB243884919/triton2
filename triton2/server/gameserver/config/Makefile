
include ../../bin/Makefile

config:
	$(call make_config, sceneserver.xml.base, sceneserver.xml)
	$(call make_config, tcpsvrd.cfg.base, tcpsvrd.cfg)

#	../../make_config.sh $(USER) ./data/sceneserver.xml sceneserver.xml
#	../../make_config.sh ./data/tcpsvrd.cfg tcpsvrd.cfg

#	sed -e 's/_GAME_SERVER_IP_/$(_GAME_SERVER_IP_)/g' -e 's/_GAME_SERVER_PORT_/$(_GAME_SERVER_PORT_)/g' \
        -e 's/_PROXY_SERVER_IP_/$(_PROXY_SERVER_IP_)/g' -e 's/_PROXY_SERVER_PORT_/$(_PROXY_SERVER_PORT_)/g' \
        ./data/sceneserver.xml > sceneserver.xml
#	sed -e 's/_GAME_SERVER_PORT_/$(_GAME_SERVER_PORT_)/g' ./data/tcpsvrd.cfg > tcpsvrd.cfg
