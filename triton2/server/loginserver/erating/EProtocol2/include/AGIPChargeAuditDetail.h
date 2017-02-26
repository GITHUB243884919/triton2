/**
 * @file:		AGIPChargeAuditDetail.h
 * @date:		01/12/2010
 * @author:		Gaoguodong
 * @email:		gaoguodong@linekong.com
 *
 * modify
 * @date:		01/13/2010
 *
 * @version:	0.1.0
 *
**/

#ifndef __AGIPCHARGEAUDITDETAIL_H__
#define __AGIPCHARGEAUDITDETAIL_H__

#include "SysProtocol.h"

#define CMD_CHARGE_AUDIT_DETAIL				0x10004408
#define CMD_CHARGE_AUDIT_DETAIL_RES			0x20004408

typedef struct _AGIP_CHARGE_AUDIT_DETAIL_ { 
	_AGIP_HEADER_		header;

	uint64_t			un_Begin_ID;
	uint64_t			un_End_ID;

	int16_t				s_Subject_ID;
	int16_t				s_ID_Count;

	_AGIP_CHECKSUM_ 	checksum;
}SAGIPChargeAuditDetail, *PSAGIPChargeAuditDetail;

typedef struct _AGIP_CHARGE_AUDIT_DETAIL_RES_ {
	_AGIP_HEADER_		header;

	int32_t				n_Result_Code;

	uint16_t			pad_2;
	int16_t				s_ID_Count;

	uint64_t			un_Detail_ID;			// into DETAIL_LEN
	uint32_t			un_User_ID;				// into DETAIL_LEN
	int32_t				n_Amount;				// into DETAIL_LEN

	_AGIP_CHECKSUM_ 	checksum;
}SAGIPChargeAuditDetailRes, *PSAGIPChargeAuditDetailRes;

#define DETAIL_LEN	(sizeof(uint64_t)+sizeof(uint32_t)+sizeof(int32_t))

// Detail 
class Engine_Export AGIPChargeAuditDetail : public SysProtocol {
public:
	AGIPChargeAuditDetail();
	
	uint64_t		getBeginID(int32_t nflag = 1) const;
	void			setBeginID(uint64_t begin_id, int32_t nflag = 1);
	uint64_t		getEndID(int32_t nflag = 1) const;
	void			setEndID(uint64_t end_id, int32_t nflag = 1);

	int16_t			getSubjectID() const;
	void			setSubjectID(int16_t subject_id);

	int16_t			getIDCount() const;
	void			setIDCount(int16_t id_count);

	int 			showInfo();
};

class Engine_Export AGIPChargeAuditDetailRes : public SysProtocol {
public:
	AGIPChargeAuditDetailRes();

	int32_t			getResultCode() const;
	void			setResultCode(int32_t result_code);

	int16_t			getIDCount() const;

	int				addChargeDetail(uint64_t detail_id, 
						uint32_t user_id, int32_t amount, int32_t nflag = 1);
	int				getChargeDetail(int16_t index, uint64_t &detail_id, 
						uint32_t &user_id, int32_t &amount, int32_t nflag = 1);

	uint64_t		getDetailID(int16_t index = 0, int32_t nflag = 1) const;
	uint32_t		getUserID(int16_t index = 0) const;
	int32_t			getAmount(int16_t index = 0) const;

	int 			showInfo();

private:
	void			setIDCount(int16_t id_count);
};

#endif

