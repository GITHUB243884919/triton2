#include "proxy_comm.h"
#include "tcp_conn.h"

CFlag g_byRunFlag;

int main(int argc, char* argv[])
{
	CTCPSocket<RECVBUFLENGTH, POSTBUFLENGTH> client;

	if (argc != 3)
	{
		printf("usage: %s server_addr port\n", argv[0]);
		exit(0);
	}

	CProxyHead msg;

	msg.m_nTotalLength = sizeof(msg);

	msg.m_nSrcFE = cet_gamesvr;
	msg.m_nSrcID = 2;
	msg.m_nDstFE = cet_proxy;
	msg.m_byTransferType = 0;     //p to p

	msg.m_stTransferInfo.nDstID = 1;

	msg.m_byOpFlag = 2;          //regist
	msg.m_nOptLength = 1;          //regist
	msg.m_abyOpt[0] = 3;          //regist
	msg.m_nBodyLength = 12;


	char buffer[1024];
	short len;

	msg.Encode(buffer, len);

	client.CreateClient();
	client.ConnectTo(argv[1], atoi(argv[2]));

	client.SendOneCode(len, (BYTE*) buffer);

	while(1)
	{
		/* 从标准输入设备取得字符串*/
		read(STDIN_FILENO,buffer,sizeof(buffer));

		bzero(buffer,sizeof(buffer));
		msg.Encode(buffer, len);

		/* 将字符串传给server端*/
		if(client.SendOneCode(len,(BYTE*) buffer)<0)
		{
			perror("send");
			exit(1);
		}
	}

	return 1;
}
