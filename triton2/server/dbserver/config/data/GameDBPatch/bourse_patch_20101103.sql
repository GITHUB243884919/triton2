alter table UMS_BOURSE_TRADE add column account_game_id int(11) default 0;
alter table UMS_BOURSE_TRADE_HISTORY add column account_game_id int(11) default 0;
alter table UMS_BOURSE_LOG add column buy_account_game_id int(11) default 0;
alter table UMS_BOURSE_LOG add column sell_account_game_id int(11) default 0;
alter table UMS_BOURSE_LOG_LOCKED add column buy_account_game_id int(11) default 0;
alter table UMS_BOURSE_LOG_LOCKED add column sell_account_game_id int(11) default 0;
