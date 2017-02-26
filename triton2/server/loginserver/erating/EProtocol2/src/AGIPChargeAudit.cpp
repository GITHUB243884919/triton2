/**
 * @file:		AGIPChargeAudit.cpp
 * @date:		01/13/2010
 * @author:		Gaoguodong
 * @email:
 *
 * modify
 *
 * @version:	0.1.0
 *
**/

#include "AGIPChargeAudit.h"

// AGIPChargeAudit
AGIPChargeAudit::AGIPChargeAudit() : SysProtocol(AGIP_CURRENT_VERSION,
		CMD_CHARGE_AUDIT, sizeof(SAGIPChargeAudit)) {
}

uint64_t	AGIPChargeAudit::getBeginID(int32_t nflag) const {
	return ntohll(*((uint64_t*)&((PSAGIPChargeAudit)m_pucPDU)->un_Begin_ID), nflag); 
}

void		AGIPChargeAudit::setBeginID(uint64_t Begin_ID, int32_t nflag) {
	*(uint64_t*)(&((PSAGIPChargeAudit)m_pucPDU)->un_Begin_ID) = htonll(Begin_ID, nflag); 
}

uint64_t	AGIPChargeAudit::getEndID(int32_t nflag) const {
	return ntohll(*((uint64_t*)&((PSAGIPChargeAudit)m_pucPDU)->un_End_ID), nflag); 
}

void		AGIPChargeAudit::setEndID(uint64_t End_ID, int32_t nflag) {
	*(uint64_t*)(&((PSAGIPChargeAudit)m_pucPDU)->un_End_ID) = htonll(End_ID, nflag);
}

int16_t		AGIPChargeAudit::getSubjectID() const {
	return ntohs(((PSAGIPChargeAudit)m_pucPDU)->s_Subject_ID); 
}

void		AGIPChargeAudit::setSubjectID(int16_t Subject_ID) {
	((PSAGIPChargeAudit)m_pucPDU)->s_Subject_ID = htons(Subject_ID);
}

int			AGIPChargeAudit::showInfo() {
	SysProtocol::showInfo();

	
	printf( \
			"----------------------------------------------AGIPChargeAudit\n"
			"Game_ID:            %u\n"
			"Gateway_ID:         %u\n"
			"Begin_ID:           %llu\n"
			"End_ID:             %llu\n"
			"Subject_ID:         %d\n"
			"----------------------------------------------AGIPChargeAudit\n",
			getGameID(),
			getGatewayID(),
			getBeginID(),
			getEndID(),
			getSubjectID()
		  ); 

	return S_SUCCESS;
}

// AGIPChargeAuditRes
AGIPChargeAuditRes::AGIPChargeAuditRes() : SysProtocol(AGIP_CURRENT_VERSION, CMD_CHARGE_AUDIT_RES, sizeof(SAGIPChargeAuditRes)) {

} 

int32_t		AGIPChargeAuditRes::getResultCode() const {
	return ntohl(((PSAGIPChargeAuditRes)m_pucPDU)->n_Result_Code);
}
void		AGIPChargeAuditRes::setResultCode(int32_t Result_Code) {
	((PSAGIPChargeAuditRes)m_pucPDU)->n_Result_Code = htonl(Result_Code);
}

uint64_t	AGIPChargeAuditRes::getBeginID(int32_t nflag) const {
	return ntohll(*((uint64_t*)&((PSAGIPChargeAuditRes)m_pucPDU)->un_Begin_ID), nflag);
}

void		AGIPChargeAuditRes::setBeginID(uint64_t Begin_ID, int32_t nflag) {
	*(uint64_t*)(&((PSAGIPChargeAuditRes)m_pucPDU)->un_Begin_ID) = htonll(Begin_ID, nflag);
}

uint64_t	AGIPChargeAuditRes::getEndID(int32_t nflag) const {
	return ntohll(*((uint64_t*)&((PSAGIPChargeAuditRes)m_pucPDU)->un_End_ID), nflag);
}

void		AGIPChargeAuditRes::setEndID(uint64_t End_ID, int32_t nflag) {
	*(uint64_t*)(&((PSAGIPChargeAuditRes)m_pucPDU)->un_End_ID) = htonll(End_ID, nflag);
}

int16_t		AGIPChargeAuditRes::getSubjectID() const {
	return ntohs(((PSAGIPChargeAuditRes)m_pucPDU)->s_Subject_ID);
}

void		AGIPChargeAuditRes::setSubjectID(int16_t Subject_ID) {
	((PSAGIPChargeAuditRes)m_pucPDU)->s_Subject_ID = htons(Subject_ID);
}

int16_t		AGIPChargeAuditRes::getIDCount() const {
	return ntohs(((PSAGIPChargeAuditRes)m_pucPDU)->s_ID_Count);
}
void		AGIPChargeAuditRes::setIDCount(int16_t ID_Count) {
	((PSAGIPChargeAuditRes)m_pucPDU)->s_ID_Count = htons(ID_Count);
}

int32_t		AGIPChargeAuditRes::getAmount() const {
	return ntohl(((PSAGIPChargeAuditRes)m_pucPDU)->n_Amount);
}

void		AGIPChargeAuditRes::setAmount(int32_t Amount) {
	((PSAGIPChargeAuditRes)m_pucPDU)->n_Amount = htonl(Amount);
}

int			AGIPChargeAuditRes::showInfo() {
	SysProtocol::showInfo();


	printf( \
			"----------------------------------------------AGIPChargeAuditRes\n"
			"Result_Code:		 %d\n"
			"Game_ID:            %u\n"
			"Gateway_ID:         %u\n"
			"Begin_ID:           %llu\n"
			"End_ID:             %llu\n"
			"Subject_ID:         %d\n"
			"ID_Count:           %d\n"
			"Amount:             %d\n"
			"----------------------------------------------AGIPChargeAuditRes\n",
			getResultCode(),
			getGameID(),
			getGatewayID(),
			getBeginID(),
			getEndID(),
			getSubjectID(),
			getIDCount(),
			getAmount()
		  );

	return S_SUCCESS;
}
