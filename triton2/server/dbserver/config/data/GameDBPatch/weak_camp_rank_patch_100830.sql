-- ------------------------------------------------
-- Alter table structure for `UMS_WORLDSTATUS`
-- ------------------------------------------------
alter table UMS_WORLDSTATUS add column weak_level tinyint(4) not null default '0';
