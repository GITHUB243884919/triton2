-- ------------------------------------------------
-- Alter table structure for `UMS_WORLDSTATUS`
-- ------------------------------------------------
alter table UMS_WORLDSTATUS add column honer_inc int(11) not null default '0';
alter table UMS_WORLDSTATUS add column level_score int(11) not null default '0';
alter table UMS_WORLDSTATUS add column honer_score int(11) not null default '0';
