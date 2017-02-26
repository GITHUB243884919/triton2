
DROP PROCEDURE IF EXISTS `CalTax2Consumed`;
DELIMITER ;;
CREATE PROCEDURE CalTax2Consumed()
BEGIN
	DECLARE fetchSeqOk boolean;
	DECLARE nAccountID int unsigned;
	DECLARE nCurrentTax int unsigned;
	DECLARE fetchSeqCursor CURSOR FOR SELECT account_id, sum(discount_price * item_count) FROM GMS_CONSUME_LOG where subject_id = 5 and ib_code='67726' group by account_id ;
	DECLARE continue handler FOR NOT FOUND set fetchSeqOk = true; 
	
	SET fetchSeqOk = false;
	
	OPEN fetchSeqCursor;
	fetchSeqLoop:Loop
		if fetchSeqOk then
			leave fetchSeqLoop;
		else
			FETCH fetchSeqCursor into nAccountID, nCurrentTax;

			UPDATE UMS_ACCOUNT SET consumed_golden_yb = consumed_golden_yb + nCurrentTax where account_id = nAccountID;
		end if;
	end Loop;
	
	CLOSE fetchSeqCursor;
END;;
DELIMITER ;

call CalTax2Consumed();

DROP PROCEDURE IF EXISTS `CalTax2Consumed`;
