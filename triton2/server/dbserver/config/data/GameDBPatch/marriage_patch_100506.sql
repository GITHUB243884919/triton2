
alter table UMS_ROLE ADD  role_marriageid int(11) default 0; 

DROP TABLE IF EXISTS `UMS_MARRIAGE`;

CREATE TABLE `UMS_MARRIAGE` (
  `server_sequence` int(11) NOT NULL AUTO_INCREMENT,
  `groom_id` int(11) NOT NULL,
  `bride_id` int(11) NOT NULL,
  `groom_name` varchar(32) NOT NULL,
  `bride_name` varchar(32) NOT NULL,
  `marriage_status` int(11) DEFAULT NULL,
  `wedding_time` int(11) DEFAULT NULL,
  `wedding_timelength` int(11) DEFAULT NULL,
  `camp_sequence` int(11) DEFAULT NULL,
  `camp_id` int(11) DEFAULT NULL,
  PRIMARY KEY (`server_sequence`)
) ENGINE=InnoDB AUTO_INCREMENT=0 DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `UMS_CAMP`;

CREATE TABLE `UMS_CAMP` (
  `campid` int(11) NOT NULL,
  `marriednum` int(11) DEFAULT NULL,
  PRIMARY KEY (`campid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;


alter table UMS_WORLD modify column version int not null default 1;

INSERT INTO UMS_WORLD( world_id, country_id, official ) VALUES( 1, 4, NULL );
INSERT INTO UMS_WORLD( world_id, country_id, official ) VALUES( 2, 4, NULL );
INSERT INTO UMS_WORLD( world_id, country_id, official ) VALUES( 3, 4, NULL );
INSERT INTO UMS_WORLD( world_id, country_id, official ) VALUES( 4, 4, NULL );

