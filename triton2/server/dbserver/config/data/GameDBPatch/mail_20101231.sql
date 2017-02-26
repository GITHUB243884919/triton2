	insert into UMS_ROLE_MAIL_01( mail_type, mail_status, sender_role_id, sender_role_name, recver_role_id, mail_title, mail_body, send_time,
	money_attachment, item_number, item_attachment, post_recver, expired_time, recver_role_name )
	select  mail_type, mail_status, sender_role_id, sender_role_name, recver_role_id, mail_title, mail_body, send_time,
	money_attachment, item_number, item_attachment, post_recver, expired_time, recver_role_name from UMS_ROLE_MAIL_02;
	
	insert into UMS_ROLE_MAIL_01( mail_type, mail_status, sender_role_id, sender_role_name, recver_role_id, mail_title, mail_body, send_time,
	money_attachment, item_number, item_attachment, post_recver, expired_time, recver_role_name )
	select  mail_type, mail_status, sender_role_id, sender_role_name, recver_role_id, mail_title, mail_body, send_time,
	money_attachment, item_number, item_attachment, post_recver, expired_time, recver_role_name from UMS_ROLE_MAIL_03;
	
	insert into UMS_ROLE_MAIL_01( mail_type, mail_status, sender_role_id, sender_role_name, recver_role_id, mail_title, mail_body, send_time,
	money_attachment, item_number, item_attachment, post_recver, expired_time, recver_role_name )
	select  mail_type, mail_status, sender_role_id, sender_role_name, recver_role_id, mail_title, mail_body, send_time,
	money_attachment, item_number, item_attachment, post_recver, expired_time, recver_role_name from UMS_ROLE_MAIL_04;