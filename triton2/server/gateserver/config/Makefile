
include ../../bin/Makefile

config:
	$(call make_config, data/ip.xml, ip.xml)
	$(call make_config, data/tcpsvrd.cfg, tcpsvrd.cfg)

#	../../make_config.sh data/ip.xml ip.xml
#	../../make_config.sh data/tcpsvrd.cfg tcpsvrd.cfg

#	sed -e 's/_GATE_SERVER_IP_/$(_GATE_SERVER_IP_)/g' -e 's/_PROXY_SERVER_IP_/$(_PROXY_SERVER_IP_)/g' \
#		-e 's/_GATE_SERVER_PORT_/$(_GATE_SERVER_PORT_)/g' -e 's/_PROXY_SERVER_PORT_/$(_PROXY_SERVER_PORT_)/g' \
#		-e 's/_MYSQL_CONN_/$(_MYSQL_CONN_)/g' data/ip.xml > ip.xml
#	sed -e 's/_GATE_SERVER_PORT_/$(_GATE_SERVER_PORT_)/g' data/tcpsvrd.cfg > tcpsvrd.cfg
