-- Creature Frantic Geist 47131 SAI
SET @ENTRY := 47131;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`= @ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@ENTRY, 0, 0, 0, 0, 0, 100, 1, 100, 100, 0, 0, 11, 91077, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, "When in combat and timer at the begining between 100 and 100 ms (and later repeats every 0 and 0 ms) - Self: Cast spell 91077 on Victim // "),
(@ENTRY, 0, 1, 0, 0, 0, 100, 0, 6000, 7000, 3000, 3000, 11, 91064, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, "When in combat and timer at the begining between 6000 and 7000 ms (and later repeats every 3000 and 3000 ms) - Self: Cast spell 91064 on Victim // ");
