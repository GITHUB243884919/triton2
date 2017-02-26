/**
 * @file:		AGIPChargeAudit.h
 * @date:		01/12/2010
 * @author:		Gaoguodong
 * @email:		gaoguodong@linekong.com
 *
 * modify
 *
 * @version:	0.1.0
 *
**/

#ifndef __AGIPCHARGEAUDIT_H__
#define __AGIPCHARGEAUDIT_H__

#include "SysProtocol.h"

#define CMD_CHARGE_AUDIT					0x10004407
#define CMD_CHARGE_AUDIT_RES				0x20004407

typedef struct _AGIP_CHARGE_AUDIT_ { 
	_AGIP_HEADER_		header;

	uint64_t			un_Begin_ID;
	uint64_t			un_End_ID;

	int16_t				s_Subject_ID;
	int16_t				pad;

	_AGIP_CHECKSUM_ 	checksum;
}SAGIPChargeAudit, *PSAGIPChargeAudit;

typedef struct _AGIP_CHARGE_AUDIT_RES_ {
	_AGIP_HEADER_		header;

	int32_t				n_Result_Code;

	uint64_t			un_Begin_ID;
	uint64_t			un_End_ID;
	int16_t				s_Subject_ID;
	int16_t				s_ID_Count;
	int32_t				n_Amount;

	_AGIP_CHECKSUM_ 	checksum;
}SAGIPChargeAuditRes, *PSAGIPChargeAuditRes;

class Engine_Export AGIPChargeAudit : public SysProtocol {
public:
	AGIPChargeAudit();

	uint64_t		getBeginID(int32_t nflag = 1) const;
	void			setBeginID(uint64_t Begin_ID, int32_t nflag = 1);
	uint64_t		getEndID(int32_t nflag = 1) const;
	void			setEndID(uint64_t End_ID, int32_t nflag = 1);

	int16_t			getSubjectID() const;
	void			setSubjectID(int16_t Subject_ID);

	int 			showInfo();
};

class Engine_Export AGIPChargeAuditRes : public SysProtocol {
public:
	AGIPChargeAuditRes();

	int32_t			getResultCode() const;
	void			setResultCode(int32_t Result_Code);

	uint64_t		getBeginID(int32_t nflag = 1) const;
	void			setBeginID(uint64_t Begin_ID, int32_t nflag = 1);

	uint64_t		getEndID(int32_t nflag = 1) const;
	void			setEndID(uint64_t End_ID, int32_t nflag = 1);

	int16_t			getSubjectID() const;
	void			setSubjectID(int16_t Subject_ID);

	int16_t			getIDCount() const;
	void			setIDCount(int16_t ID_Count);

	int32_t			getAmount() const;
	void			setAmount(int32_t Amount);

	int 			showInfo();
};

#endif

