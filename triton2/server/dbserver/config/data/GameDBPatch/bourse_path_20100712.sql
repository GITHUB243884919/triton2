alter table UMS_BOURSE_LOG_LOCKED add column sell_role  int(11) NOT NULL ;
alter table UMS_BOURSE_LOG_LOCKED add column buy_role  int(11) NOT NULL;
alter table UMS_BOURSE_LOG add column buy_role  int(11) NOT NULL;
alter table UMS_BOURSE_LOG add column sell_role  int(11) NOT NULL;
