-- change a hunter trainer's gossip menu to match the creature_trainer record.
UPDATE `creature_template` SET `gossip_menu_id` = 4466 WHERE `entry` = 38467;

-- add `creature_trainer` records for low level hunter trainers.
DELETE FROM `creature_trainer` WHERE `CreatureId` IN (4892,16270,34673,35758,35874,36518,38122,38467,39116,39214,44461,49806,49927,49946,50007,50157,50609);
INSERT INTO `creature_trainer` (`CreatureId`, `TrainerId`, `MenuId`, `Optionid`) VALUES 
(4892,40,12835,0),
(16270,40,14144,0),
(34673,15,10675,0),
(35758,40,10549,0),
(35874,15,10697,0),
(36518,40,10814,0),
(38122,40,11045,0),
(38467,40,4466,0),
(39116,40,0,0),
(39214,15,11185,0),
(44461,40,10697,0),
(49806,40,12528,0),
(49927,40,12535,0),
(49946,40,0,0),
(50007,40,12924,0),
(50157,40,12535,0),
(50609,40,0,0);
