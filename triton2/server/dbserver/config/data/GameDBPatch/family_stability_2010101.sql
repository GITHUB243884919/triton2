alter table UMS_FAMILY add column family_stability int default 200;
update UMS_FAMILY set family_stability=200 where family_level=1;
update UMS_FAMILY set family_stability=600 where family_level=2;
update UMS_FAMILY set family_stability=1200 where family_level=3;
update UMS_FAMILY set family_stability=2000 where family_level=4;