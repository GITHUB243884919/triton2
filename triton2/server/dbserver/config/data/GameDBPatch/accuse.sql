alter table UMS_WORLDSTATUS add column accuse_state int not null default 0;
alter table UMS_WORLDSTATUS add column accuse_yes int not null default 0;
alter table UMS_WORLDSTATUS add column accuse_no int not null default 0;
alter table UMS_WORLDSTATUS add column accuse_abort int not null default 0;
alter table UMS_WORLDSTATUS add column gate_times int not null default 0;
alter table UMS_WORLDSTATUS add column flag_times int not null default 0;

alter table UMS_CORPS add column corps_forbidtime int not null default 0;