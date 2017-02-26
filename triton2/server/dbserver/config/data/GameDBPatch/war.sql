DROP TABLE IF EXISTS `UMS_WORLDWAR`;
CREATE TABLE `UMS_WORLDWAR` (
  `world_id` int(11) NOT NULL,
  `totallevel` int(11) NOT NULL DEFAULT '0',
  `totalnum` int(11) NOT NULL DEFAULT '0',
  `campid` int(11) NOT NULL DEFAULT '0',
  `bidmoney` int(11) NOT NULL DEFAULT '0',
  `bidcampid` int(11) NOT NULL DEFAULT '0',
  `basicaward` int(11) NOT NULL DEFAULT '0',
  `bestaward` int(11) NOT NULL DEFAULT '0',
  `source0` int(11) NOT NULL DEFAULT '0',
  `source1` int(11) NOT NULL DEFAULT '0',
  `source2` int(11) NOT NULL DEFAULT '0',
  `source3` int(11) NOT NULL DEFAULT '0',
  `source4` int(11) NOT NULL DEFAULT '0',
  `submitmoney` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`world_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

INSERT INTO UMS_WORLDWAR( world_id ) VALUES( 1 );
INSERT INTO UMS_WORLDWAR( world_id ) VALUES( 2 );
INSERT INTO UMS_WORLDWAR( world_id ) VALUES( 3 );
INSERT INTO UMS_WORLDWAR( world_id ) VALUES( 4 );

DROP TABLE IF EXISTS `UMS_WORLDCHAR`;
CREATE TABLE `UMS_WORLDCHAR` (
  `world_id` int(11) NOT NULL,
  `charid` int(11) NOT NULL DEFAULT '0',
  `honor` int(11) NOT NULL DEFAULT '0',
  `killnum` int(11) NOT NULL DEFAULT '0',
  `rank` int(11) NOT NULL DEFAULT '0',
  `metier` int(11) NOT NULL DEFAULT '0'
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 1, 0, 1 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 1, 1, 1 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 1, 2, 1 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 1, 0, 2 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 1, 1, 2 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 1, 2, 2 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 1, 0, 3 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 1, 1, 3 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 1, 2, 3 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 1, 0, 4 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 1, 1, 4 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 1, 2, 4 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 1, 0, 5 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 1, 1, 5 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 1, 2, 5 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 1, 0, 6 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 1, 1, 6 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 1, 2, 6 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 2, 0, 1 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 2, 1, 1 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 2, 2, 1 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 2, 0, 2 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 2, 1, 2 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 2, 2, 2 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 2, 0, 3 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 2, 1, 3 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 2, 2, 3 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 2, 0, 4 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 2, 1, 4 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 2, 2, 4 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 2, 0, 5 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 2, 1, 5 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 2, 2, 5 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 2, 0, 6 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 2, 1, 6 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 2, 2, 6 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 3, 0, 1 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 3, 1, 1 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 3, 2, 1 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 3, 0, 2 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 3, 1, 2 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 3, 2, 2 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 3, 0, 3 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 3, 1, 3 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 3, 2, 3 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 3, 0, 4 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 3, 1, 4 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 3, 2, 4 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 3, 0, 5 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 3, 1, 5 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 3, 2, 5 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 3, 0, 6 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 3, 1, 6 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 3, 2, 6 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 4, 0, 1 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 4, 1, 1 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 4, 2, 1 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 4, 0, 2 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 4, 1, 2 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 4, 2, 2 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 4, 0, 3 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 4, 1, 3 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 4, 2, 3 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 4, 0, 4 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 4, 1, 4 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 4, 2, 4 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 4, 0, 5 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 4, 1, 5 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 4, 2, 5 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 4, 0, 6 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 4, 1, 6 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 4, 2, 6 );

DROP TABLE IF EXISTS `UMS_WORLDGLOBAL`;
CREATE TABLE `UMS_WORLDGLOBAL` (
  `war_serial` int(11) NOT NULL DEFAULT '0',
  `war_status` int(11) NOT NULL,
  `war_titletime` int(11) NOT NULL,
  `war_winworld` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

INSERT INTO UMS_WORLDGLOBAL( war_serial, war_status ) VALUES( 1, 0 );