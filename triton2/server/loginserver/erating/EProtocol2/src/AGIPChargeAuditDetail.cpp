/**
 * @file:		AGIPChargeAuditDetail.cpp
 * @date:		01/13/2010
 * @author:		Gaoguodong
 * @email:
 *
 * modify
 *
 * @version:	0.1.0
 *
**/

#include "AGIPChargeAuditDetail.h"

#undef offsetof
#define offsetof(TYPE,MEMBER) ((size_t) &((TYPE *)0)->MEMBER)

// AGIPChargeAuditDetail
AGIPChargeAuditDetail::AGIPChargeAuditDetail() : SysProtocol(AGIP_CURRENT_VERSION, CMD_CHARGE_AUDIT_DETAIL, sizeof(SAGIPChargeAuditDetail)) {

}

uint64_t	AGIPChargeAuditDetail::getBeginID(int32_t nflag) const {
	return ntohll(*((uint64_t*)&((PSAGIPChargeAuditDetail)m_pucPDU)->un_Begin_ID), nflag);
}

void		AGIPChargeAuditDetail::setBeginID(uint64_t Begin_ID, int32_t nflag) {
	*(uint64_t*)(&((PSAGIPChargeAuditDetail)m_pucPDU)->un_Begin_ID) = htonll(Begin_ID, nflag);
}

uint64_t	AGIPChargeAuditDetail::getEndID(int32_t nflag) const {
	return ntohll(*((uint64_t*)&((PSAGIPChargeAuditDetail)m_pucPDU)->un_End_ID), nflag);
}

void		AGIPChargeAuditDetail::setEndID(uint64_t End_ID, int32_t nflag) {
	*(uint64_t*)(&((PSAGIPChargeAuditDetail)m_pucPDU)->un_End_ID) = htonll(End_ID, nflag);
}

int16_t		AGIPChargeAuditDetail::getSubjectID() const {
	return ntohs(((PSAGIPChargeAuditDetail)m_pucPDU)->s_Subject_ID);
}

void		AGIPChargeAuditDetail::setSubjectID(int16_t Subject_ID) {
	((PSAGIPChargeAuditDetail)m_pucPDU)->s_Subject_ID = htons(Subject_ID);
}

int16_t		AGIPChargeAuditDetail::getIDCount() const {
	return ntohs(((PSAGIPChargeAuditDetail)m_pucPDU)->s_ID_Count);
}

void		AGIPChargeAuditDetail::setIDCount(int16_t ID_Count) {
	((PSAGIPChargeAuditDetail)m_pucPDU)->s_ID_Count = htons(ID_Count);
}

int			AGIPChargeAuditDetail::showInfo() {
	SysProtocol::showInfo();

	printf( \
			"----------------------------------------AGIPChargeAuditDetail\n"
			"Game_ID:            %u\n"
			"Gateway_ID:         %u\n"
			"Begin_ID:           %llu\n"
			"End_ID:             %llu\n"
			"Subject_ID:         %d\n"
			"ID_Count:           %d\n"
			"----------------------------------------AGIPChargeAuditDetail\n",
			getGameID(),
			getGatewayID(),
			getBeginID(),
			getEndID(),
			getSubjectID(),
			getIDCount()
		  );

	return S_SUCCESS;
}

// AGIPChargeAuditDetailRes
AGIPChargeAuditDetailRes::AGIPChargeAuditDetailRes() : SysProtocol(AGIP_CURRENT_VERSION,
		CMD_CHARGE_AUDIT_DETAIL_RES, (sizeof(SAGIPChargeAuditDetailRes) - DETAIL_LEN)) {
}

int32_t	AGIPChargeAuditDetailRes::getResultCode() const {
	return ntohl(((PSAGIPChargeAuditDetailRes)m_pucPDU)->n_Result_Code);
} 

void		AGIPChargeAuditDetailRes::setResultCode(int32_t Result_Code) {
	((PSAGIPChargeAuditDetailRes)m_pucPDU)->n_Result_Code = htonl(Result_Code);
}

int16_t		AGIPChargeAuditDetailRes::getIDCount() const {
	return ntohs(((PSAGIPChargeAuditDetailRes)m_pucPDU)->s_ID_Count);
}

void		AGIPChargeAuditDetailRes::setIDCount(int16_t ID_Count) {
	((PSAGIPChargeAuditDetailRes)m_pucPDU)->s_ID_Count = htons(ID_Count);
}

int			AGIPChargeAuditDetailRes::addChargeDetail(uint64_t Detail_ID, 
				uint32_t User_ID, int32_t Amount, int32_t nflag) { 
	if (sizeof(SAGIPChargeAuditDetailRes) + DETAIL_LEN * getIDCount() > AGIP_MAX_BUFFER_SIZE /*1024*/) {
		return E_ERROR;
	}

	int16_t index = getIDCount();

	*(uint64_t*)(m_pucPDU + offsetof(SAGIPChargeAuditDetailRes,un_Detail_ID) + DETAIL_LEN * index)
		= htonll(Detail_ID, nflag);
	*(uint32_t *)(m_pucPDU + offsetof(SAGIPChargeAuditDetailRes,un_User_ID) + DETAIL_LEN * index)
		= (uint32_t)htonl(User_ID);
	*(int32_t *)(m_pucPDU + offsetof(SAGIPChargeAuditDetailRes,n_Amount) + DETAIL_LEN * index)
		= htonl(Amount);

	setIDCount(index + 1);
	m_usTotalLength += DETAIL_LEN;
	setTotalLength(m_usTotalLength);

	return S_SUCCESS;
}

int			AGIPChargeAuditDetailRes::getChargeDetail(int16_t index, uint64_t &Detail_ID,
				uint32_t &User_ID, int32_t &Amount, int32_t nflag) {
	if (index >= getIDCount()) return E_ERROR;

	Detail_ID = ntohll(*(uint64_t*)(m_pucPDU + offsetof(SAGIPChargeAuditDetailRes,un_Detail_ID) + DETAIL_LEN * index), nflag);
	User_ID = (uint32_t)ntohl(*(uint32_t *)(m_pucPDU + offsetof(SAGIPChargeAuditDetailRes,un_User_ID) + DETAIL_LEN * index));
	Amount = ntohl(*(int32_t *)(m_pucPDU + offsetof(SAGIPChargeAuditDetailRes,n_Amount) + DETAIL_LEN * index));
	return S_SUCCESS;
}

uint64_t	AGIPChargeAuditDetailRes::getDetailID(int16_t index, int32_t nflag) const {
	return ntohll(*(uint64_t*)(m_pucPDU + offsetof(SAGIPChargeAuditDetailRes,un_Detail_ID) + DETAIL_LEN * index), nflag);
}

uint32_t	AGIPChargeAuditDetailRes::getUserID(int16_t index) const {
	return (uint32_t)ntohl(*(uint32_t *)(m_pucPDU + offsetof(SAGIPChargeAuditDetailRes,un_User_ID) + DETAIL_LEN * index));
}
int32_t		AGIPChargeAuditDetailRes::getAmount(int16_t index) const {
	return ntohl(*(int32_t *)(m_pucPDU + offsetof(SAGIPChargeAuditDetailRes,n_Amount) + DETAIL_LEN * index));
}

int			AGIPChargeAuditDetailRes::showInfo() {
	SysProtocol::showInfo();

	printf( \
			"-------------------------------------AGIPChargeAuditDetailRes\n"
			"Result_Code:		 %d\n"
			"Game_ID:            %u\n"
			"Gateway_ID:         %u\n"
			"ID_Count:           %d\n",
			getResultCode(),
			getGameID(),
			getGatewayID(),
			getIDCount()
		  );

	int16_t		ID_Count = getIDCount();
	uint64_t	Detail_ID;
	uint32_t	User_ID;
	int32_t		Amount;
	
	for (int16_t i = 0; i < ID_Count; ++i)
    {
        getChargeDetail(i, Detail_ID, User_ID, Amount);   
        printf("[%03u]: Detail_ID = %llu, User_ID = %u, Amount = %d\n",
                i, Detail_ID, User_ID, Amount);
    }
    
    printf("-------------------------------------AGIPChargeAuditDetailRes\n");
		  
	return S_SUCCESS;
}

