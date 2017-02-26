alter table UMS_WORLDGLOBAL add column tax_setrate int not null default 0;

alter table UMS_WORLDGLOBAL add column tax_taxrate int not null default 5;

alter table UMS_WORLDGLOBAL add column tax_taxmoney int not null default 0;

alter table UMS_WORLDGLOBAL add column tax_corpsmoney int not null default 0;

alter table UMS_WORLDGLOBAL add column tax_settimes int not null default 0;