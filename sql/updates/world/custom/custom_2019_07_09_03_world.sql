-- Pathing for  Entry: 46630 'TDB FORMAT' 
SET @NPC := 291289;
SET @PATH := @NPC * 10;
UPDATE `creature` SET `spawndist`= 0, `MovementType`= 2, `position_x`= -47.3455, `position_y`= 1927.43, `position_z`= 15.18369 WHERE `guid`= @NPC;
DELETE FROM `creature_addon` WHERE `guid`= @NPC;
INSERT INTO `creature_addon` (`guid`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES (@NPC, @PATH, 0, 0, 1, 0, '');
DELETE FROM `waypoint_data` WHERE `id`=@PATH;
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_type`, `action`, `action_chance`, `wpguid`) VALUES
(@PATH, 1, -47.3455, 1927.43, 15.18369, 0, 248, 0, 0, 100, 0),
(@PATH, 2, -61.7899, 1936.95, 13.35688, 0, 0, 0, 0, 100, 0),
(@PATH, 3, -77.6042, 1941.48, 13.15267, 0, 0, 0, 0, 100, 0),
(@PATH, 4, -93.9271, 1944.83, 13.52767, 0, 0, 0, 0, 100, 0),
(@PATH, 5, -116.524, 1945.28, 2.376272, 0, 0, 0, 0, 100, 0),
(@PATH, 6, -119.642, 1947.87, 2.477835, 0, 0, 0, 0, 100, 0),
(@PATH, 7, -123.938, 1959.33, 3.936331, 0, 0, 0, 0, 100, 0),
(@PATH, 8, -118.938, 1975.82, 3.167124, 0, 0, 0, 0, 100, 0),
(@PATH, 9, -111.585, 1979.43, 3.166222, 0, 0, 0, 0, 100, 0),
(@PATH, 10, -97.8507, 1978.49, 3.167476, 0, 0, 0, 0, 100, 0),
(@PATH, 11, -79.4861, 1976.72, 3.166646, 0, 168, 0, 0, 100, 0),
(@PATH, 12, -97.8507, 1978.49, 3.167476, 0, 0, 0, 0, 100, 0),
(@PATH, 13, -111.585, 1979.43, 3.166222, 0, 0, 0, 0, 100, 0),
(@PATH, 14, -118.938, 1975.82, 3.167124, 0, 0, 0, 0, 100, 0),
(@PATH, 15, -123.938, 1959.33, 3.936331, 0, 0, 0, 0, 100, 0),
(@PATH, 16, -119.642, 1947.87, 2.477835, 0, 0, 0, 0, 100, 0),
(@PATH, 17, -116.524, 1945.28, 2.376272, 0, 0, 0, 0, 100, 0),
(@PATH, 18, -93.9271, 1944.83, 13.52767, 0, 0, 0, 0, 100, 0),
(@PATH, 19, -77.6042, 1941.48, 13.15267, 0, 0, 0, 0, 100, 0),
(@PATH, 20, -61.7899, 1936.95, 13.35688, 0, 0, 0, 0, 100, 0);
-- 0x20208C5B802D8980006BD10003A4C699 .go xyz -47.3455 1927.43 15.18369

-- Pathing for  Entry: 46630 'TDB FORMAT' 
SET @NPC := 291276;
SET @PATH := @NPC * 10;
UPDATE `creature` SET `spawndist`= 0, `MovementType`= 2, `position_x`= -85.3663, `position_y`= 1901.74, `position_z`= 13.28359 WHERE `guid`= @NPC;
DELETE FROM `creature_addon` WHERE `guid`= @NPC;
INSERT INTO `creature_addon` (`guid`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES (@NPC, @PATH, 0, 0, 1, 0, '');
DELETE FROM `waypoint_data` WHERE `id`=@PATH;
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_type`, `action`, `action_chance`, `wpguid`) VALUES
(@PATH, 1, -85.3663, 1901.74, 13.28359, 0, 232, 0, 0, 100, 0),
(@PATH, 2, -78.4288, 1915.45, 13.35195, 0, 0, 0, 0, 100, 0),
(@PATH, 3, -77.3889, 1929.7, 13.47695, 0, 0, 0, 0, 100, 0),
(@PATH, 4, -79.7986, 1942.23, 13.27767, 0, 0, 0, 0, 100, 0),
(@PATH, 5, -74.2986, 1968.11, 13.6554, 0, 0, 0, 0, 100, 0),
(@PATH, 6, -72.3733, 2012.08, 7.229702, 0, 186, 0, 0, 100, 0),
(@PATH, 7, -74.2986, 1968.11, 13.6554, 0, 0, 0, 0, 100, 0),
(@PATH, 8, -79.7986, 1942.23, 13.27767, 0, 0, 0, 0, 100, 0),
(@PATH, 9, -77.3889, 1929.7, 13.47695, 0, 0, 0, 0, 100, 0),
(@PATH, 10, -78.4288, 1915.45, 13.35195, 0, 0, 0, 0, 100, 0);
-- 0x20208C5B802D8980006BD10001A4C699 .go xyz -85.3663 1901.74 13.28359

-- Pathing for  Entry: 46597 'TDB FORMAT' 
SET @NPC := 291154;
SET @PATH := @NPC * 10;
UPDATE `creature` SET `spawndist`= 0, `MovementType`= 2, `position_x`= -657.835, `position_y`= 1741.23, `position_z`= 50.06901 WHERE `guid`= @NPC;
DELETE FROM `creature_addon` WHERE `guid`= @NPC;
INSERT INTO `creature_addon` (`guid`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES (@NPC, @PATH, 0, 0, 1, 0, '');
DELETE FROM `waypoint_data` WHERE `id`=@PATH;
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_type`, `action`, `action_chance`, `wpguid`) VALUES
(@PATH, 1, -657.835, 1741.23, 50.06901, 0, 0, 0, 0, 100, 0),
(@PATH, 2, -651.932, 1707.25, 50.06901, 0, 0, 0, 0, 100, 0),
(@PATH, 3, -631.731, 1681.9, 48.74969, 0, 0, 0, 0, 100, 0),
(@PATH, 4, -596.438, 1665.56, 49.67484, 0, 0, 0, 0, 100, 0),
(@PATH, 5, -556.359, 1672.88, 49.41657, 0, 119, 0, 0, 100, 0),
(@PATH, 6, -596.438, 1665.56, 49.67484, 0, 0, 0, 0, 100, 0),
(@PATH, 7, -631.731, 1681.9, 48.74969, 0, 0, 0, 0, 100, 0),
(@PATH, 8, -651.932, 1707.25, 50.06901, 0, 0, 0, 0, 100, 0),
(@PATH, 9, -657.835, 1741.23, 50.06901, 0, 0, 0, 0, 100, 0),
(@PATH, 10, -653.134, 1772.32, 51.88462, 0, 0, 0, 0, 100, 0),
(@PATH, 11, -634.451, 1797.44, 53.76865, 0, 0, 0, 0, 100, 0),
(@PATH, 12, -599.863, 1812.2, 54.10814, 0, 0, 0, 0, 100, 0),
(@PATH, 13, -582.906, 1811.03, 54.11937, 0, 134, 0, 0, 100, 0),
(@PATH, 14, -599.863, 1812.2, 54.10814, 0, 0, 0, 0, 100, 0),
(@PATH, 15, -634.451, 1797.44, 53.76865, 0, 0, 0, 0, 100, 0),
(@PATH, 16, -653.134, 1772.32, 51.88462, 0, 0, 0, 0, 100, 0);
-- 0x20208C5B802D8140006BD1000024C698 .go xyz -657.835 1741.23 50.06901

UPDATE `creature` SET `position_x`= -657.835, `position_y`= 1741.23, `position_z`= 50.06901 WHERE `guid`= 291153;
DELETE FROM `creature_formations` WHERE `leaderGUID`= 291154;
INSERT INTO `creature_formations` (`leaderGUID`, `memberGUID`, `dist`, `angle`, `groupAI`, `point_1`, `point_2`) VALUES
(291154, 291154, 0, 0, 515, 0, 0),
(291154, 291153, 2, 90, 515, 0, 0);

-- Pathing for  Entry: 46597 'TDB FORMAT' 
SET @NPC := 291134;
SET @PATH := @NPC * 10;
UPDATE `creature` SET `spawndist`= 0, `MovementType`= 2, `position_x`= -446.361, `position_y`= 1895.53, `position_z`= 46.93295 WHERE `guid`= @NPC;
DELETE FROM `creature_addon` WHERE `guid`= @NPC;
INSERT INTO `creature_addon` (`guid`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES (@NPC, @PATH, 0, 0, 1, 0, '');
DELETE FROM `waypoint_data` WHERE `id`=@PATH;
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_type`, `action`, `action_chance`, `wpguid`) VALUES
(@PATH, 1, -446.361, 1895.53, 46.93295, 0, 132, 0, 0, 100, 0),
(@PATH, 2, -432.307, 1878.55, 46.57799, 0, 0, 0, 0, 100, 0),
(@PATH, 3, -424.212, 1854.49, 46.5528, 0, 0, 0, 0, 100, 0),
(@PATH, 4, -423.806, 1834.49, 46.5528, 0, 0, 0, 0, 100, 0),
(@PATH, 5, -432.858, 1822.56, 46.50397, 0, 0, 0, 0, 100, 0),
(@PATH, 6, -451.028, 1811.49, 46.44185, 0, 0, 0, 0, 100, 0),
(@PATH, 7, -474.845, 1808.78, 46.6778, 0, 0, 0, 0, 100, 0),
(@PATH, 8, -502.033, 1806.61, 48.09459, 0, 0, 0, 0, 100, 0),
(@PATH, 9, -516.01, 1805.14, 49.70116, 0, 0, 0, 0, 100, 0),
(@PATH, 10, -538.149, 1801.64, 52.30355, 0, 381, 0, 0, 100, 0),
(@PATH, 11, -516.01, 1805.14, 49.70116, 0, 0, 0, 0, 100, 0),
(@PATH, 12, -502.033, 1806.61, 48.09459, 0, 0, 0, 0, 100, 0),
(@PATH, 13, -474.845, 1808.78, 46.6778, 0, 0, 0, 0, 100, 0),
(@PATH, 14, -451.028, 1811.49, 46.44185, 0, 0, 0, 0, 100, 0),
(@PATH, 15, -432.858, 1822.56, 46.50397, 0, 0, 0, 0, 100, 0),
(@PATH, 16, -423.806, 1834.49, 46.5528, 0, 0, 0, 0, 100, 0),
(@PATH, 17, -424.212, 1854.49, 46.5528, 0, 0, 0, 0, 100, 0),
(@PATH, 18, -432.307, 1878.55, 46.57799, 0, 0, 0, 0, 100, 0);
-- 0x20208C5B802D8140006BD1000024C699 .go xyz -446.361 1895.53 46.93295

UPDATE `creature` SET `position_x`= -446.361, `position_y`= 1895.53, `position_z`= 46.93295 WHERE `guid`= 291133;

-- Pathing for  Entry: 46597 'TDB FORMAT' 
SET @NPC := 291132;
SET @PATH := @NPC * 10;
UPDATE `creature` SET `spawndist`= 0, `MovementType`= 2, `position_x`= -619.689, `position_y`= 1814.77, `position_z`= 53.90987 WHERE `guid`= @NPC;
DELETE FROM `creature_addon` WHERE `guid`= @NPC;
INSERT INTO `creature_addon` (`guid`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES (@NPC, @PATH, 0, 0, 1, 0, '');
DELETE FROM `waypoint_data` WHERE `id`=@PATH;
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_type`, `action`, `action_chance`, `wpguid`) VALUES
(@PATH, 1, -619.689, 1814.77, 53.90987, 0, 234, 0, 0, 100, 0),
(@PATH, 2, -615.899, 1828.45, 53.20479, 0, 0, 0, 0, 100, 0),
(@PATH, 3, -589.785, 1859.83, 48.66382, 0, 0, 0, 0, 100, 0),
(@PATH, 4, -562.514, 1888.3, 46.6778, 0, 0, 0, 0, 100, 0),
(@PATH, 5, -544.854, 1903.63, 46.5528, 0, 0, 0, 0, 100, 0),
(@PATH, 6, -526.96, 1908.78, 46.55291, 0, 0, 0, 0, 100, 0),
(@PATH, 7, -499.627, 1912.78, 46.53251, 0, 0, 0, 0, 100, 0),
(@PATH, 8, -476.771, 1914.26, 47.19901, 0, 0, 0, 0, 100, 0),
(@PATH, 9, -461.146, 1909.23, 47.47685, 0, 0, 0, 0, 100, 0),
(@PATH, 10, -449.418, 1899.05, 47.05795, 0, 336, 0, 0, 100, 0),
(@PATH, 11, -461.146, 1909.23, 47.47685, 0, 0, 0, 0, 100, 0),
(@PATH, 12, -476.771, 1914.26, 47.19901, 0, 0, 0, 0, 100, 0),
(@PATH, 13, -499.627, 1912.78, 46.53251, 0, 0, 0, 0, 100, 0),
(@PATH, 14, -526.96, 1908.78, 46.55291, 0, 0, 0, 0, 100, 0),
(@PATH, 15, -544.854, 1903.63, 46.5528, 0, 0, 0, 0, 100, 0),
(@PATH, 16, -562.514, 1888.3, 46.6778, 0, 0, 0, 0, 100, 0),
(@PATH, 17, -589.785, 1859.83, 48.66382, 0, 0, 0, 0, 100, 0),
(@PATH, 18, -615.899, 1828.45, 53.20479, 0, 0, 0, 0, 100, 0);
-- 0x20208C5B802D8140006BD1000124C699 .go xyz -619.689 1814.77 53.90987

UPDATE `creature` SET  `position_x`= -619.689, `position_y`= 1814.77, `position_z`= 53.90987 WHERE `guid`= 291131;

-- Pathing for  Entry: 47627 'TDB FORMAT' 
SET @NPC := 291632;
SET @PATH := @NPC * 10;
UPDATE `creature` SET `spawndist`= 0, `MovementType`= 2, `position_x`= -25.3733, `position_y`= 1145.88, `position_z`= 15.17712 WHERE `guid`= @NPC;
DELETE FROM `creature_addon` WHERE `guid`= @NPC;
INSERT INTO `creature_addon` (`guid`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES (@NPC, @PATH, 0, 0, 1, 0, '10022 77806');
DELETE FROM `waypoint_data` WHERE `id`=@PATH;
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_type`, `action`, `action_chance`, `wpguid`) VALUES
(@PATH, 1, -25.3733, 1145.88, 15.17712, 0, 200, 0, 0, 100, 0),
(@PATH, 2, -31.7656, 1147.63, 15.17712, 0, 0, 0, 0, 100, 0),
(@PATH, 3, -37.5156, 1148.42, 15.18193, 0, 0, 0, 0, 100, 0),
(@PATH, 4, -43.559, 1146.89, 15.18193, 0, 0, 0, 0, 100, 0),
(@PATH, 5, -46.7361, 1140.44, 15.18193, 0, 0, 0, 0, 100, 0),
(@PATH, 6, -48.2344, 1130.78, 15.18454, 0, 0, 0, 0, 100, 0),
(@PATH, 7, -47.5538, 1117.41, 15.18454, 0, 0, 0, 0, 100, 0),
(@PATH, 8, -46.7344, 1100.12, 15.18454, 0, 0, 0, 0, 100, 0),
(@PATH, 9, -45.3872, 1086.96, 15.18193, 0, 0, 0, 0, 100, 0),
(@PATH, 10, -45.4358, 1077.46, 15.18193, 0, 0, 0, 0, 100, 0),
(@PATH, 11, -42.1649, 1069.45, 15.18193, 0, 0, 0, 0, 100, 0),
(@PATH, 12, -32.8594, 1061.31, 15.17357, 0, 85, 0, 0, 100, 0),
(@PATH, 13, -42.1649, 1069.45, 15.18193, 0, 0, 0, 0, 100, 0),
(@PATH, 14, -45.4358, 1077.46, 15.18193, 0, 0, 0, 0, 100, 0),
(@PATH, 15, -45.3872, 1086.96, 15.18193, 0, 0, 0, 0, 100, 0),
(@PATH, 16, -46.7344, 1100.12, 15.18454, 0, 0, 0, 0, 100, 0),
(@PATH, 17, -47.5538, 1117.41, 15.18454, 0, 0, 0, 0, 100, 0),
(@PATH, 18, -48.2344, 1130.78, 15.18454, 0, 0, 0, 0, 100, 0),
(@PATH, 19, -46.7361, 1140.44, 15.18193, 0, 0, 0, 0, 100, 0),
(@PATH, 20, -43.559, 1146.89, 15.18193, 0, 0, 0, 0, 100, 0),
(@PATH, 21, -37.5156, 1148.42, 15.18193, 0, 0, 0, 0, 100, 0),
(@PATH, 22, -31.7656, 1147.63, 15.17712, 0, 0, 0, 0, 100, 0);
-- 0x20208C5B802E82C0006BD1000024C699 .go xyz -25.3733 1145.88 15.17712

-- Pathing for  Entry: 47627 'TDB FORMAT' 
SET @NPC := 291564;
SET @PATH := @NPC * 10;
UPDATE `creature` SET `spawndist`= 0, `MovementType`= 2, `position_x`= -70.3299, `position_y`= 1345.4, `position_z`= 22.16791 WHERE `guid`= @NPC;
DELETE FROM `creature_addon` WHERE `guid`= @NPC;
INSERT INTO `creature_addon` (`guid`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES (@NPC, @PATH, 0, 0, 1, 0, '10022 77806');
DELETE FROM `waypoint_data` WHERE `id`=@PATH;
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_type`, `action`, `action_chance`, `wpguid`) VALUES
(@PATH, 1, -70.3299, 1345.4, 22.16791, 0, 102, 0, 0, 100, 0),
(@PATH, 2, -50.1337, 1346.6, 22.17456, 0, 0, 0, 0, 100, 0),
(@PATH, 3, -34.9635, 1344.77, 22.17456, 0, 0, 0, 0, 100, 0),
(@PATH, 4, -23.4253, 1338.11, 22.16056, 0, 0, 0, 0, 100, 0),
(@PATH, 5, -15.5833, 1328.88, 22.06785, 0, 0, 0, 0, 100, 0),
(@PATH, 6, -10.2865, 1319.69, 22.06785, 0, 0, 0, 0, 100, 0),
(@PATH, 7, -2.62153, 1313.33, 22.06785, 0, 0, 0, 0, 100, 0),
(@PATH, 8, 8.4375, 1305.21, 21.8435, 0, 0, 0, 0, 100, 0),
(@PATH, 9, 21.9792, 1292.71, 18.73246, 0, 0, 0, 0, 100, 0),
(@PATH, 10, 27.2552, 1289.27, 17.14848, 0, 0, 0, 0, 100, 0),
(@PATH, 11, 27.4219, 1278.25, 15.53129, 0, 0, 0, 0, 100, 0),
(@PATH, 12, 22.7726, 1264.66, 15.18193, 0, 0, 0, 0, 100, 0),
(@PATH, 13, 16.4688, 1259.8, 15.18193, 0, 0, 0, 0, 100, 0),
(@PATH, 14, 2.49479, 1255.74, 15.18193, 0, 0, 0, 0, 100, 0),
(@PATH, 15, -13.6563, 1247.76, 15.18193, 0, 0, 0, 0, 100, 0),
(@PATH, 16, -21.6823, 1237.41, 15.18193, 0, 0, 0, 0, 100, 0),
(@PATH, 17, -24.908, 1219.61, 15.18193, 0, 33, 0, 0, 100, 0),
(@PATH, 18, -21.6823, 1237.41, 15.18193, 0, 0, 0, 0, 100, 0),
(@PATH, 19, -13.6563, 1247.76, 15.18193, 0, 0, 0, 0, 100, 0),
(@PATH, 20, 2.49479, 1255.74, 15.18193, 0, 0, 0, 0, 100, 0),
(@PATH, 21, 16.4688, 1259.8, 15.18193, 0, 0, 0, 0, 100, 0),
(@PATH, 22, 22.7726, 1264.66, 15.18193, 0, 0, 0, 0, 100, 0),
(@PATH, 23, 27.4219, 1278.25, 15.53129, 0, 0, 0, 0, 100, 0),
(@PATH, 24, 27.25, 1289.25, 17.05693, 0, 0, 0, 0, 100, 0),
(@PATH, 25, 21.9792, 1292.71, 18.73246, 0, 0, 0, 0, 100, 0),
(@PATH, 26, 8.4375, 1305.21, 21.8435, 0, 0, 0, 0, 100, 0),
(@PATH, 27, -2.62153, 1313.33, 22.06785, 0, 0, 0, 0, 100, 0),
(@PATH, 28, -10.2865, 1319.69, 22.06785, 0, 0, 0, 0, 100, 0),
(@PATH, 29, -15.5833, 1328.88, 22.06785, 0, 0, 0, 0, 100, 0),
(@PATH, 30, -23.4253, 1338.11, 22.16056, 0, 0, 0, 0, 100, 0),
(@PATH, 31, -34.9635, 1344.77, 22.17456, 0, 0, 0, 0, 100, 0),
(@PATH, 32, -50.1337, 1346.6, 22.17456, 0, 0, 0, 0, 100, 0);
-- 0x20208C5B802E82C0006BD10000A4C698 .go xyz -70.3299 1345.4 22.16791

-- Pathing for  Entry: 47627 'TDB FORMAT' 
SET @NPC := 291630;
SET @PATH := @NPC * 10;
UPDATE `creature` SET `spawndist`= 0, `MovementType`= 2, `position_x`= -30.0087, `position_y`= 1052.1, `position_z`= 15.04857 WHERE `guid`= @NPC;
DELETE FROM `creature_addon` WHERE `guid`= @NPC;
INSERT INTO `creature_addon` (`guid`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES (@NPC, @PATH, 0, 0, 1, 0, '10022 77806');
DELETE FROM `waypoint_data` WHERE `id`=@PATH;
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_type`, `action`, `action_chance`, `wpguid`) VALUES
(@PATH, 1, -30.0087, 1052.1, 15.04857, 0, 46, 0, 0, 100, 0),
(@PATH, 2, -35.3733, 1044.53, 14.88611, 0, 0, 0, 0, 100, 0),
(@PATH, 3, -40.9149, 1039.92, 15.10499, 0, 0, 0, 0, 100, 0),
(@PATH, 4, -48.276, 1037.7, 15.10499, 0, 0, 0, 0, 100, 0),
(@PATH, 5, -55.5903, 1036.36, 15.10499, 0, 0, 0, 0, 100, 0),
(@PATH, 6, -63.9462, 1034.84, 15.10499, 0, 0, 0, 0, 100, 0),
(@PATH, 7, -68.7865, 1032.46, 15.18194, 0, 0, 0, 0, 100, 0),
(@PATH, 8, -72.9219, 1028.36, 15.34124, 0, 0, 0, 0, 100, 0),
(@PATH, 9, -78.1007, 1023.07, 16.88921, 0, 0, 0, 0, 100, 0),
(@PATH, 10, -88.0868, 1019.99, 19.60345, 0, 0, 0, 0, 100, 0),
(@PATH, 11, -95.9497, 1019.97, 21.21722, 0, 0, 0, 0, 100, 0),
(@PATH, 12, -105.168, 1024.99, 21.99273, 0, 0, 0, 0, 100, 0),
(@PATH, 13, -112.592, 1030, 22.11773, 0, 0, 0, 0, 100, 0),
(@PATH, 14, -119.587, 1037.93, 22.05444, 0, 0, 0, 0, 100, 0),
(@PATH, 15, -125.304, 1043.46, 22.05444, 0, 0, 0, 0, 100, 0),
(@PATH, 16, -134.939, 1047.9, 22.17456, 0, 336, 0, 0, 100, 0),
(@PATH, 17, -125.304, 1043.46, 22.05444, 0, 0, 0, 0, 100, 0),
(@PATH, 18, -119.587, 1037.93, 22.05444, 0, 0, 0, 0, 100, 0),
(@PATH, 19, -112.592, 1030, 22.11773, 0, 0, 0, 0, 100, 0),
(@PATH, 20, -105.168, 1024.99, 21.99273, 0, 0, 0, 0, 100, 0),
(@PATH, 21, -95.9497, 1019.97, 21.21722, 0, 0, 0, 0, 100, 0),
(@PATH, 22, -88.0868, 1019.99, 19.60345, 0, 0, 0, 0, 100, 0),
(@PATH, 23, -78.1007, 1023.07, 16.88921, 0, 0, 0, 0, 100, 0),
(@PATH, 24, -72.9219, 1028.36, 15.34124, 0, 0, 0, 0, 100, 0),
(@PATH, 25, -68.7865, 1032.46, 15.18194, 0, 0, 0, 0, 100, 0),
(@PATH, 26, -63.9462, 1034.84, 15.10499, 0, 0, 0, 0, 100, 0),
(@PATH, 27, -55.5903, 1036.36, 15.10499, 0, 0, 0, 0, 100, 0),
(@PATH, 28, -48.276, 1037.7, 15.10499, 0, 0, 0, 0, 100, 0),
(@PATH, 29, -40.9149, 1039.92, 15.10499, 0, 0, 0, 0, 100, 0),
(@PATH, 30, -35.3733, 1044.53, 14.88611, 0, 0, 0, 0, 100, 0);
-- 0x20208C5B802E82C0006BD10000A4C699 .go xyz -30.0087 1052.1 15.04857

-- Pathing for  Entry: 47627 'TDB FORMAT' 
SET @NPC := 291567;
SET @PATH := @NPC * 10;
UPDATE `creature` SET `spawndist`= 0, `MovementType`= 2, `position_x`= -28.1563, `position_y`= 1058.62, `position_z`= 15.17357 WHERE `guid`= @NPC;
DELETE FROM `creature_addon` WHERE `guid`= @NPC;
INSERT INTO `creature_addon` (`guid`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES (@NPC, @PATH, 0, 0, 1, 0, '10022 77806');
DELETE FROM `waypoint_data` WHERE `id`=@PATH;
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_type`, `action`, `action_chance`, `wpguid`) VALUES
(@PATH, 1, -28.1563, 1058.62, 15.17357, 0, 209, 0, 0, 100, 0),
(@PATH, 2, -7.24826, 1062.68, 15.17357, 0, 0, 0, 0, 100, 0),
(@PATH, 3, 14.5747, 1068.18, 15.10874, 0, 0, 0, 0, 100, 0),
(@PATH, 4, 26.5903, 1070.73, 15.10874, 0, 0, 0, 0, 100, 0),
(@PATH, 5, 43.7708, 1073.77, 15.1206, 0, 0, 0, 0, 100, 0),
(@PATH, 6, 56.3212, 1078.98, 14.9956, 0, 0, 0, 0, 100, 0),
(@PATH, 7, 60.4201, 1082.13, 14.9956, 0, 0, 0, 0, 100, 0),
(@PATH, 8, 61.908, 1087.52, 15.32152, 0, 0, 0, 0, 100, 0),
(@PATH, 9, 62.941, 1110.32, 3.78336, 0, 0, 0, 0, 100, 0),
(@PATH, 10, 66.6389, 1116.18, 3.78336, 0, 0, 0, 0, 100, 0),
(@PATH, 11, 67.5226, 1123.15, 3.802567, 0, 0, 0, 0, 100, 0),
(@PATH, 12, 63.8177, 1133.49, 3.836599, 0, 0, 0, 0, 100, 0),
(@PATH, 13, 61.0139, 1143.34, 3.836599, 0, 0, 0, 0, 100, 0),
(@PATH, 14, 62.9028, 1151.23, 3.836599, 0, 0, 0, 0, 100, 0),
(@PATH, 15, 74.3819, 1165.13, 3.76411, 0, 0, 0, 0, 100, 0),
(@PATH, 16, 83.0139, 1170.07, 3.963068, 0, 250, 0, 0, 100, 0),
(@PATH, 17, 74.3819, 1165.13, 3.76411, 0, 0, 0, 0, 100, 0),
(@PATH, 18, 62.9028, 1151.23, 3.836599, 0, 0, 0, 0, 100, 0),
(@PATH, 19, 61.0139, 1143.34, 3.836599, 0, 0, 0, 0, 100, 0),
(@PATH, 20, 63.8177, 1133.49, 3.836599, 0, 0, 0, 0, 100, 0),
(@PATH, 21, 67.5226, 1123.15, 3.802567, 0, 0, 0, 0, 100, 0),
(@PATH, 22, 66.6389, 1116.18, 3.78336, 0, 0, 0, 0, 100, 0),
(@PATH, 23, 62.941, 1110.32, 3.78336, 0, 0, 0, 0, 100, 0),
(@PATH, 24, 61.908, 1087.52, 15.32152, 0, 0, 0, 0, 100, 0),
(@PATH, 25, 60.4201, 1082.13, 14.9956, 0, 0, 0, 0, 100, 0),
(@PATH, 26, 56.3212, 1078.98, 14.9956, 0, 0, 0, 0, 100, 0),
(@PATH, 27, 43.7708, 1073.77, 15.1206, 0, 0, 0, 0, 100, 0),
(@PATH, 28, 26.5903, 1070.73, 15.10874, 0, 0, 0, 0, 100, 0),
(@PATH, 29, 14.5747, 1068.18, 15.10874, 0, 0, 0, 0, 100, 0),
(@PATH, 30, -7.24826, 1062.68, 15.17357, 0, 0, 0, 0, 100, 0),
(@PATH, 31, -28.1563, 1058.62, 15.17357, 0, 280, 0, 0, 100, 0);
-- 0x20208C5B802E82C0006BD1000124C698 .go xyz -28.1563 1058.62 15.17357

-- Pathing for  Entry: 47627 'TDB FORMAT' 
SET @NPC := 291624;
SET @PATH := @NPC * 10;
UPDATE `creature` SET `spawndist`= 0, `MovementType`= 2, `position_x`= -21.7587, `position_y`= 1202.55, `position_z`= 15.18193 WHERE `guid`= @NPC;
DELETE FROM `creature_addon` WHERE `guid`= @NPC;
INSERT INTO `creature_addon` (`guid`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES (@NPC, @PATH, 0, 0, 1, 0, '10022 77806');
DELETE FROM `waypoint_data` WHERE `id`=@PATH;
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_type`, `action`, `action_chance`, `wpguid`) VALUES
(@PATH, 1, -21.7587, 1202.55, 15.18193, 0, 316, 0, 0, 100, 0),
(@PATH, 2, -25.9757, 1189.87, 15.18193, 0, 0, 0, 0, 100, 0),
(@PATH, 3, -29.5139, 1177.68, 15.18193, 0, 0, 0, 0, 100, 0),
(@PATH, 4, -43.5469, 1172.71, 14.98891, 0, 0, 0, 0, 100, 0),
(@PATH, 5, -55.1528, 1163.6, 15.18193, 0, 0, 0, 0, 100, 0),
(@PATH, 6, -77.3611, 1168.9, 15.21195, 0, 0, 0, 0, 100, 0),
(@PATH, 7, -103.851, 1175.79, 17.89649, 0, 0, 0, 0, 100, 0),
(@PATH, 8, -126.1328, 1181.428, 21.96449, 0, 0, 0, 0, 100, 0),
(@PATH, 9, -138.214, 1185.04, 22.10073, 0, 241, 0, 0, 100, 0),
(@PATH, 10, -126.212, 1181.44, 21.92249, 0, 0, 0, 0, 100, 0),
(@PATH, 11, -103.9316, 1175.802, 17.83949, 0, 0, 0, 0, 100, 0),
(@PATH, 12, -77.3611, 1168.9, 15.21195, 0, 0, 0, 0, 100, 0),
(@PATH, 13, -55.1528, 1163.6, 15.18193, 0, 0, 0, 0, 100, 0),
(@PATH, 14, -43.5469, 1172.71, 14.98891, 0, 0, 0, 0, 100, 0),
(@PATH, 15, -29.5139, 1177.68, 15.18193, 0, 0, 0, 0, 100, 0),
(@PATH, 16, -25.9757, 1189.87, 15.18193, 0, 0, 0, 0, 100, 0);
-- 0x20208C5B802E82C0006BD10001A4C698 .go xyz -21.7587 1202.55 15.18193

-- Pathing for  Entry: 47593 'TDB FORMAT' 
SET @NPC := 251167;
SET @PATH := @NPC * 10;
UPDATE `creature` SET `spawndist`= 0, `MovementType`= 2, `position_x`= -1378.655, `position_y`= 1161.519, `position_z`= 124.2998 WHERE `guid`= @NPC;
DELETE FROM `creature_addon` WHERE `guid`= @NPC;
INSERT INTO `creature_addon` (`guid`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES (@NPC, @PATH, 0, 0, 1, 0, '');
DELETE FROM `waypoint_data` WHERE `id`=@PATH;
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_type`, `action`, `action_chance`, `wpguid`) VALUES
(@PATH, 1, -1378.655, 1161.519, 124.2998, 0, 0, 0, 0, 100, 0),
(@PATH, 2, -1400.745, 1186.356, 129.1367, 0, 0, 0, 0, 100, 0),
(@PATH, 3, -1398.576, 1183.932, 128.4394, 0, 0, 0, 0, 100, 0),
(@PATH, 4, -1407.431, 1234.599, 133.7088, 0, 0, 0, 0, 100, 0),
(@PATH, 5, -1400.415, 1243.816, 133.7088, 0, 0, 0, 0, 100, 0),
(@PATH, 6, -1390.104, 1245.656, 133.6457, 0, 0, 0, 0, 100, 0),
(@PATH, 7, -1372.99, 1250.243, 133.5207, 0, 0, 0, 0, 100, 0),
(@PATH, 8, -1360.087, 1265.545, 133.4511, 0, 0, 0, 0, 100, 0),
(@PATH, 9, -1350.358, 1285.007, 132.8137, 0, 0, 0, 0, 100, 0),
(@PATH, 10, -1342.486, 1304.977, 131.0785, 0, 0, 0, 0, 100, 0),
(@PATH, 11, -1336.116, 1310.247, 130.5641, 0, 0, 0, 0, 100, 0),
(@PATH, 12, -1317.595, 1314.276, 127.7791, 0, 0, 0, 0, 100, 0),
(@PATH, 13, -1292.21, 1326.363, 125.9124, 0, 0, 0, 0, 100, 0),
(@PATH, 14, -1275.038, 1344.901, 124.7795, 0, 0, 0, 0, 100, 0),
(@PATH, 15, -1243.885, 1376.017, 123.4099, 0, 0, 0, 0, 100, 0),
(@PATH, 16, -1214.623, 1386.981, 123.488, 0, 0, 0, 0, 100, 0),
(@PATH, 17, -1185.733, 1388.156, 124.7073, 0, 0, 0, 0, 100, 0),
(@PATH, 18, -1165.563, 1381.438, 127.5676, 0, 0, 0, 0, 100, 0),
(@PATH, 19, -1143.608, 1363.873, 131.1086, 0, 0, 0, 0, 100, 0),
(@PATH, 20, -1115.262, 1355.42, 137.5011, 0, 0, 0, 0, 100, 0),
(@PATH, 21, -1101.153, 1351.358, 140.0758, 0, 0, 0, 0, 100, 0),
(@PATH, 22, -1090.254, 1343.743, 139.0586, 0, 0, 0, 0, 100, 0),
(@PATH, 23, -1083.257, 1322.55, 131.8793, 0, 0, 0, 0, 100, 0),
(@PATH, 24, -1084.724, 1309.161, 128.1983, 0, 0, 0, 0, 100, 0),
(@PATH, 25, -1075.16, 1276.009, 123.5373, 0, 0, 0, 0, 100, 0),
(@PATH, 26, -1067.059, 1256.05, 123.4451, 0, 0, 0, 0, 100, 0),
(@PATH, 27, -1052.882, 1238.927, 123.4499, 0, 0, 0, 0, 100, 0),
(@PATH, 28, -1035.962, 1215.156, 123.4967, 0, 0, 0, 0, 100, 0),
(@PATH, 29, -1026.618, 1190.872, 123.4705, 0, 0, 0, 0, 100, 0),
(@PATH, 30, -1024.569, 1167.609, 123.4705, 0, 0, 0, 0, 100, 0),
(@PATH, 31, -1032.222, 1152.62, 123.53, 0, 0, 0, 0, 100, 0),
(@PATH, 32, -1046.281, 1126.884, 123.4647, 0, 0, 0, 0, 100, 0),
(@PATH, 33, -1054.804, 1100.186, 123.4647, 0, 0, 0, 0, 100, 0),
(@PATH, 34, -1048.325, 1067.797, 123.4864, 0, 0, 0, 0, 100, 0),
(@PATH, 35, -1044.259, 1034.674, 123.5119, 0, 0, 0, 0, 100, 0),
(@PATH, 36, -1057.898, 986.7118, 123.5463, 0, 0, 0, 0, 100, 0),
(@PATH, 37, -1057.248, 965.0555, 123.4311, 0, 0, 0, 0, 100, 0),
(@PATH, 38, -1050.911, 947.8646, 123.4311, 0, 0, 0, 0, 100, 0),
(@PATH, 39, -1052.469, 927.8455, 123.4291, 0, 0, 0, 0, 100, 0),
(@PATH, 40, -1056.097, 913.0364, 123.4291, 0, 0, 0, 0, 100, 0),
(@PATH, 41, -1054.231, 898.5417, 123.4956, 0, 0, 0, 0, 100, 0),
(@PATH, 42, -1046.5, 886.3958, 123.4956, 0, 0, 0, 0, 100, 0),
(@PATH, 43, -1047.658, 863.6146, 123.5891, 0, 0, 0, 0, 100, 0),
(@PATH, 44, -1051.599, 844.9879, 123.5891, 0, 0, 0, 0, 100, 0),
(@PATH, 45, -1063.813, 826.0504, 123.6474, 0, 0, 0, 0, 100, 0),
(@PATH, 46, -1084.455, 818.7952, 123.5782, 0, 0, 0, 0, 100, 0),
(@PATH, 47, -1123.998, 817.3629, 123.6085, 0, 0, 0, 0, 100, 0),
(@PATH, 48, -1138.97, 811.0226, 123.6338, 0, 0, 0, 0, 100, 0),
(@PATH, 49, -1153.391, 787.4948, 123.8407, 0, 0, 0, 0, 100, 0),
(@PATH, 50, -1153.637, 773.4951, 124.7462, 0, 0, 0, 0, 100, 0),
(@PATH, 51, -1162.896, 761.2691, 124.6249, 0, 0, 0, 0, 100, 0),
(@PATH, 52, -1188.363, 755.3212, 123.4885, 0, 0, 0, 0, 100, 0),
(@PATH, 53, -1204.34, 747.3906, 123.4266, 0, 0, 0, 0, 100, 0),
(@PATH, 54, -1219.521, 736.3125, 123.4266, 0, 0, 0, 0, 100, 0),
(@PATH, 55, -1223.601, 733.1493, 123.5103, 0, 0, 0, 0, 100, 0),
(@PATH, 56, -1239.837, 720.5781, 123.4436, 0, 0, 0, 0, 100, 0),
(@PATH, 57, -1270.858, 723.1649, 123.5187, 0, 0, 0, 0, 100, 0),
(@PATH, 58, -1301.368, 748.1979, 123.5413, 0, 0, 0, 0, 100, 0),
(@PATH, 59, -1317.637, 756.2952, 123.5413, 0, 0, 0, 0, 100, 0),
(@PATH, 60, -1336.149, 754.8924, 123.4389, 0, 0, 0, 0, 100, 0),
(@PATH, 61, -1351.894, 757.967, 123.4389, 0, 0, 0, 0, 100, 0),
(@PATH, 62, -1356.078, 768.8559, 123.4219, 0, 0, 0, 0, 100, 0),
(@PATH, 63, -1352.648, 789.9305, 123.4219, 0, 0, 0, 0, 100, 0),
(@PATH, 64, -1349.153, 814.1024, 123.2765, 0, 0, 0, 0, 100, 0),
(@PATH, 65, -1356.354, 854.6684, 122.7169, 0, 0, 0, 0, 100, 0),
(@PATH, 66, -1359.899, 883.4601, 122.3055, 0, 0, 0, 0, 100, 0),
(@PATH, 67, -1352.483, 903.1614, 122.9454, 0, 0, 0, 0, 100, 0),
(@PATH, 68, -1348.642, 924.7483, 123.3204, 0, 0, 0, 0, 100, 0),
(@PATH, 69, -1348.356, 951.7952, 123.3204, 0, 0, 0, 0, 100, 0),
(@PATH, 70, -1343.53, 979.3246, 123.2835, 0, 0, 0, 0, 100, 0),
(@PATH, 71, -1343.464, 1001.337, 123.3857, 0, 0, 0, 0, 100, 0),
(@PATH, 72, -1353.495, 1020.071, 123.3857, 0, 0, 0, 0, 100, 0),
(@PATH, 73, -1353.398, 1048.089, 123.428, 0, 0, 0, 0, 100, 0),
(@PATH, 74, -1346.583, 1080.597, 123.5432, 0, 0, 0, 0, 100, 0),
(@PATH, 75, -1348.875, 1110.958, 123.5332, 0, 0, 0, 0, 100, 0),
(@PATH, 76, -1359.28, 1138.097, 123.515, 0, 0, 0, 0, 100, 0);
-- 0x20208C5B802E7A40006BD1000024D83D .go xyz -1378.655 1161.519 124.2998

-- Creature Problim 47593 SAI
SET @ENTRY := 47593;
UPDATE `creature_template` SET `AIName`="SmartAI", `DamageModifier`= 20 WHERE `entry`= @ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@ENTRY, 0, 0, 0, 0, 0, 100, 0, 6000, 9000, 18000, 19000, 11, 88421, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "When in combat and timer at the begining between 6000 and 9000 ms (and later repeats every 18000 and 19000 ms) - Self: Cast spell Log Smash (88421) on Self // "),
(@ENTRY, 0, 1, 0, 0, 0, 100, 0, 15000, 16000, 18000, 19000, 11, 80146, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, "When in combat and timer at the begining between 15000 and 16000 ms (and later repeats every 18000 and 19000 ms) - Self: Cast spell Bonk (80146) on Victim // ");
