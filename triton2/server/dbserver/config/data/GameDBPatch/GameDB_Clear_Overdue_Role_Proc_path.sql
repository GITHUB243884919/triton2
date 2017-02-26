
DELIMITER ;

/*!50003 DROP PROCEDURE IF EXISTS `proc_delete_overdue` */;

DELIMITER ;;
/*!50003 CREATE*/ /*!50003 PROCEDURE `proc_delete_overdue`()
BEGIN
DECLARE TABLE_NAME CHAR(48);
DECLARE TABLE_IDX  int;

SET TABLE_IDX=0;
	WHILE ( TABLE_IDX < 16 ) DO
		IF( TABLE_IDX  < 10 ) then
			SET TABLE_NAME=CONCAT( 'UMS_ROLE_DETAIL_0', TABLE_IDX );
		ELSE
			SET TABLE_NAME=CONCAT( 'UMS_ROLE_DETAIL_', TABLE_IDX );
		END IF;
SET @SqlCmd =concat( 'delete from ', TABLE_NAME,
' where role_id in ( select role_id from  UMS_ROLE  where account_id in ( select account_id from UMS_ACCOUNT where  charged_golden_yb = 0 and consumed_golden_yb = 0 and charged_silver_yb=0  and golden_yb = 0 ) and role_marriageid = 0 and role_level <= 30 and login_time <= UNIX_TIMESTAMP(NOW())  - 90* 86400)');
		PREPARE stmt FROM @SqlCmd;
		EXECUTE stmt;
SET TABLE_IDX=TABLE_IDX+1;
	END WHILE;

SET TABLE_IDX=0;
	WHILE ( TABLE_IDX < 16 ) DO
		IF( TABLE_IDX  < 10 ) then
			SET TABLE_NAME=CONCAT( 'UMS_ROLE_DETAIL1_0', TABLE_IDX );
		ELSE
			SET TABLE_NAME=CONCAT( 'UMS_ROLE_DETAIL1_', TABLE_IDX );
		END IF;
SET @SqlCmd =concat( 'delete  from ', TABLE_NAME,
' where role_id in ( select role_id from  UMS_ROLE  where account_id in ( select account_id from UMS_ACCOUNT where  charged_golden_yb = 0 and consumed_golden_yb = 0 and charged_silver_yb=0  and golden_yb = 0 ) and role_marriageid = 0 and role_level <= 30 and login_time <= UNIX_TIMESTAMP(NOW())  - 90* 86400)');
		PREPARE stmt FROM @SqlCmd;
		EXECUTE stmt;
SET TABLE_IDX=TABLE_IDX+1;
	END WHILE;

SET TABLE_IDX=1;
	WHILE ( TABLE_IDX < 5 ) DO
			SET TABLE_NAME=CONCAT( 'UMS_ROLE_MAIL_0', TABLE_IDX );
SET @SqlCmd =concat( 'delete  from ', TABLE_NAME,
' where recver_role_id  in ( select role_id from  UMS_ROLE  where account_id in ( select account_id from UMS_ACCOUNT where  charged_golden_yb = 0 and consumed_golden_yb = 0 and charged_silver_yb=0  and golden_yb = 0 )and role_marriageid = 0  and role_level <=30 and login_time <= UNIX_TIMESTAMP(NOW())  - 90 * 86400)');
		PREPARE stmt FROM @SqlCmd;
		EXECUTE stmt;
SET TABLE_IDX=TABLE_IDX+1;
	END WHILE;

delete from  UMS_ROLE  where account_id in ( select account_id from UMS_ACCOUNT where  charged_golden_yb = 0 and consumed_golden_yb = 0 and charged_silver_yb=0 and golden_yb = 0 )and role_marriageid = 0  and role_level <= 30 and login_time <= UNIX_TIMESTAMP(NOW())  - 90 * 86400;


END */;;
