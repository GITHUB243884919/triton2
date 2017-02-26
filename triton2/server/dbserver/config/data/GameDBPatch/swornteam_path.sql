alter table UMS_ROLE add column role_swornid int default 0;
CREATE TABLE `UMS_ROLE_SWORN` (
  `sworn_id` int(11) NOT NULL,
  `sworn_name` varchar(31) DEFAULT NULL,
  `camp_id` int(11) DEFAULT NULL,
  `sworn_member` blob,
  PRIMARY KEY (`sworn_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

