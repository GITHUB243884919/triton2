update UMS_FAMILY set family_stability=400 where family_level=1 and family_stability < 400;
update UMS_FAMILY set family_stability=1200 where family_level=2 and family_stability < 1200;
update UMS_FAMILY set family_stability=2400 where family_level=3 and family_stability < 2400;
update UMS_FAMILY set family_stability=4000 where family_level=4 and family_stability < 4000;;