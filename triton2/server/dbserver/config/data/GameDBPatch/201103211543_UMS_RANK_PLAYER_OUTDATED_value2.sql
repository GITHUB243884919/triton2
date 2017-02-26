alter table UMS_RANK_PLAYER_OUTDATED  CHANGE value2 value2 bigint(20)  DEFAULT 0 ;
update UMS_RANK_PLAYER_OUTDATED set  value2=0;