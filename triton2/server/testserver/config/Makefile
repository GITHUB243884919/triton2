

include ../../bin/Makefile


config:
	$(call make_config, data/ip.xml, ip.xml)
	$(call make_config, data/tcpsvrd.cfg, tcpsvrd.cfg)

#	../../make_config.sh $@ data/ip.xml ip.xml
#	../../make_config.sh  $@ data/tcpsvrd.cfg tcpsvrd.cfg
#	sed -e 's/_GM_SERVER_PORT_/$(_GM_SERVER_PORT_)/g' -e 's/_PROXY_SERVER_IP_/$(_PROXY_SERVER_IP_)/g'  \
		-e 's/_PROXY_SERVER_PORT_/$(_PROXY_SERVER_PORT_)/g' -e 's/_ERATING_SERVER_IP_/$(_ERATING_SERVER_IP_)/g' \
		-e 's/_ERATING_SERVER_PORT_/$(_ERATING_SERVER_PORT_)/g' -e 's/_VALIDATE_SERVER_IP_/$(_VALIDATE_SERVER_IP_)/g' \
		-e 's/_VALIDATE_SERVER_PORT_/$(_VALIDATE_SERVER_PORT_)/g' -e 's/_CHARGE_SERVER_PORT_/$(_CHARGE_SERVER_PORT_)/g'\
		-e 's/_LOGIN_SERVER_PORT_/$(_LOGIN_SERVER_PORT_)/g' data/ip.xml		> ip.xml
#	sed -e 's/_GM_SERVER_PORT_/$(_GM_SERVER_PORT_)/g' -e 's/_PROXY_SERVER_IP_/$(_PROXY_SERVER_IP_)/g'  \
		-e 's/_PROXY_SERVER_PORT_/$(_PROXY_SERVER_PORT_)/g' -e 's/_ERATING_SERVER_IP_/$(_ERATING_SERVER_IP_)/g' \
		-e 's/_ERATING_SERVER_PORT_/$(_ERATING_SERVER_PORT_)/g' -e 's/_VALIDATE_SERVER_IP_/$(_VALIDATE_SERVER_IP_)/g' \
		-e 's/_VALIDATE_SERVER_PORT_/$(_VALIDATE_SERVER_PORT_)/g' -e 's/_CHARGE_SERVER_PORT_/$(_CHARGE_SERVER_PORT_)/g'\
		-e 's/_LOGIN_SERVER_PORT_/$(_LOGIN_SERVER_PORT_)/g' data/tcpsvrd.cfg		> tcpsvrd.cfg
