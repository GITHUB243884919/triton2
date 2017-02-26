alter table UMS_RANK_PLAYER  CHANGE value2 value2 bigint(20)  DEFAULT 0 ;
update UMS_RANK_PLAYER set  value2=0;