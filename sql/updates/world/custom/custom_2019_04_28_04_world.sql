/*
TRUNCATE TABLE `creature_sparring_template`;

ALTER TABLE `creature_sparring_template`   
  DROP COLUMN `VictimEntry`, 
  CHANGE `AttackerEntry` `CreatureID` MEDIUMINT(8) UNSIGNED NOT NULL, 
  DROP PRIMARY KEY,
  ADD PRIMARY KEY (`CreatureID`);
  
 */
INSERT INTO `creature_sparring_template` (`CreatureID`, `HealthLimitPct`) VALUES
(98, 85),
(123, 85),
(124, 85),
(125, 85),
(449, 85),
(452, 85),
(453, 85),
(501, 85),
(589, 85),
(594, 85),
(1036, 85),
(1726, 85),
(2694, 85),
(2952, 85),
(2991, 85),
(3374, 85),
(3375, 85),
(4294, 85),
(4298, 85),
(4300, 85),
(4301, 85),
(6190, 85),
(6195, 85),
(7275, 85),
(7604, 85),
(7605, 85),
(7606, 85),
(7607, 85),
(7608, 85),
(7787, 85),
(7788, 85),
(7789, 85),
(7796, 85),
(7855, 85),
(7856, 85),
(7858, 85),
(7939, 85),
(8876, 85),
(8877, 85),
(11858, 85),
(11910, 85),
(11911, 85),
(11912, 85),
(11913, 85),
(33082, 85),
(34486, 85),
(34511, 85),
(34689, 85),
(34696, 85),
(34835, 85),
(34850, 85),
(34876, 85),
(34877, 85),
(34878, 85),
(34884, 85),
(34913, 85),
(34916, 85),
(34957, 85),
(34958, 85),
(34959, 85),
(35118, 85),
(35203, 85),
(35204, 85),
(35232, 85),
(35333, 85),
(35334, 85),
(35504, 85),
(35627, 85),
(35839, 85),
(35872, 85),
(35893, 85),
(35894, 85),
(35915, 85),
(36057, 85),
(36103, 85),
(36104, 85),
(36115, 85),
(36117, 85),
(36140, 85),
(36211, 85),
(36236, 85),
(36454, 85),
(36455, 85),
(36464, 85),
(36488, 85),
(36491, 85),
(36492, 85),
(36602, 85),
(36634, 85),
(36653, 85),
(36671, 85),
(36690, 85),
(36752, 85),
(36795, 85),
(36809, 85),
(36810, 85),
(36815, 85),
(36816, 85),
(36830, 85),
(36882, 85),
(36925, 85),
(36942, 85),
(36943, 85),
(36954, 85),
(36988, 85),
(37067, 85),
(37070, 85),
(37078, 85),
(37161, 85),
(37170, 85),
(37175, 85),
(37177, 85),
(37178, 85),
(37216, 85),
(37499, 85),
(37580, 85),
(37581, 85),
(37591, 85),
(37592, 85),
(37692, 85),
(37699, 85),
(37700, 85),
(37701, 85),
(37716, 85),
(37733, 85),
(37735, 85),
(37784, 85),
(37884, 85),
(37885, 85),
(37914, 85),
(37946, 85),
(38022, 85),
(38027, 85),
(38037, 85),
(38038, 85),
(38196, 85),
(38242, 85),
(38243, 85),
(38244, 85),
(38245, 85),
(38246, 85),
(38247, 85),
(38268, 85),
(38272, 85),
(38278, 85),
(38279, 85),
(38280, 85),
(38281, 85),
(38282, 85),
(38300, 85),
(38324, 85),
(38326, 85),
(38345, 85),
(38624, 85),
(38646, 85),
(38648, 85),
(38649, 85),
(38650, 85),
(38658, 85),
(38661, 85),
(38664, 85),
(38719, 85),
(38823, 85),
(38824, 85),
(38857, 85),
(38896, 85),
(38902, 85),
(38913, 85),
(38915, 85),
(38917, 85),
(38926, 85),
(38934, 85),
(38951, 85),
(38952, 85),
(38997, 85),
(38998, 85),
(39028, 85),
(39044, 85),
(39065, 85),
(39066, 85),
(39068, 85),
(39128, 85),
(39140, 85),
(39147, 85),
(39153, 85),
(39157, 85),
(39226, 85),
(39294, 85),
(39339, 85),
(39344, 85),
(39390, 85),
(39410, 85),
(39411, 85),
(39582, 85),
(39589, 85),
(39591, 85),
(39594, 85),
(39595, 85),
(39596, 85),
(39608, 85),
(39625, 85),
(39637, 85),
(39641, 85),
(39645, 85),
(39650, 85),
(39749, 85),
(39828, 85),
(39867, 85),
(39869, 85),
(39871, 85),
(39874, 85),
(39877, 85),
(39882, 85),
(39895, 85),
(39915, 85),
(39916, 85),
(39923, 85),
(39924, 85),
(39925, 85),
(39939, 85),
(39946, 85),
(39947, 85),
(39949, 85),
(39951, 85),
(39963, 85),
(39972, 85),
(39992, 85),
(39993, 85),
(40025, 85),
(40065, 85),
(40080, 85),
(40108, 85),
(40123, 85),
(40139, 85),
(40147, 85),
(40148, 85),
(40149, 85),
(40150, 85),
(40177, 85),
(40178, 85),
(40185, 85),
(40227, 85),
(40229, 85),
(40230, 85),
(40238, 85),
(40278, 85),
(40313, 85),
(40319, 85),
(40360, 85),
(40398, 85),
(40463, 85),
(40482, 85),
(40504, 85),
(40535, 85),
(40537, 85),
(40583, 85),
(40590, 85),
(40593, 85),
(40632, 85),
(40635, 85),
(40636, 85),
(40642, 85),
(40643, 85),
(40688, 85),
(40689, 85),
(40702, 85),
(40705, 85),
(40709, 85),
(40729, 85),
(40730, 85),
(40731, 85),
(40732, 85),
(40733, 85),
(40734, 85),
(40735, 85),
(40736, 85),
(40737, 85),
(40738, 85),
(40739, 85),
(40740, 85),
(40741, 85),
(40742, 85),
(40744, 85),
(40746, 85),
(40755, 85),
(40767, 85),
(40775, 85),
(40780, 85),
(40816, 85),
(40833, 85),
(40841, 85),
(40843, 85),
(40882, 85),
(40934, 85),
(40960, 85),
(40972, 85),
(40978, 85),
(41008, 85),
(41015, 85),
(41035, 85),
(41046, 85),
(41047, 85),
(41049, 85),
(41050, 85),
(41095, 85),
(41115, 85),
(41138, 85),
(41146, 85),
(41158, 85),
(41175, 85),
(41181, 85),
(41182, 85),
(41237, 85),
(41243, 85),
(41256, 85),
(41281, 85),
(41360, 85),
(41367, 85),
(41368, 85),
(41369, 85),
(41370, 85),
(41392, 85),
(41407, 85),
(41421, 85),
(41431, 85),
(41439, 85),
(41444, 85),
(41446, 85),
(41447, 85),
(41448, 85),
(41452, 85),
(41455, 85),
(41456, 85),
(41457, 85),
(41509, 85),
(41523, 85),
(41524, 85),
(41541, 85),
(41553, 85),
(41554, 85),
(41556, 85),
(41558, 85),
(41559, 85),
(41560, 85),
(41562, 85),
(41586, 85),
(41588, 85),
(41589, 85),
(41592, 85),
(41593, 85),
(41601, 85),
(41611, 85),
(41642, 85),
(41763, 85),
(41769, 85),
(41771, 85),
(41772, 85),
(41773, 85),
(41774, 85),
(41775, 85),
(41777, 85),
(41779, 85),
(41783, 85),
(41784, 85),
(41793, 85),
(41794, 85),
(41796, 85),
(41797, 85),
(41798, 85),
(41799, 85),
(41800, 85),
(41897, 85),
(41898, 85),
(41899, 85),
(41902, 85),
(41909, 85),
(41911, 85),
(41912, 85),
(41914, 85),
(41915, 85),
(41924, 85),
(41980, 85),
(41985, 85),
(41995, 85),
(41999, 85),
(42008, 85),
(42010, 85),
(42012, 85),
(42048, 85),
(42049, 85),
(42053, 85),
(42065, 85),
(42066, 85),
(42068, 85),
(42071, 85),
(42072, 85),
(42073, 85),
(42074, 85),
(42075, 85),
(42160, 85),
(42284, 85),
(42295, 85),
(42307, 85),
(42320, 85),
(42359, 85),
(42367, 85),
(42369, 85),
(42370, 85),
(42397, 85),
(42398, 85),
(42407, 85),
(42516, 85),
(42518, 85),
(42519, 85),
(42536, 85),
(42554, 85),
(42555, 85),
(42561, 85),
(42564, 85),
(42566, 85),
(42567, 85),
(42568, 85),
(42571, 85),
(42574, 85),
(42577, 85),
(42608, 85),
(42618, 85),
(42619, 85),
(42653, 85),
(42730, 85),
(42731, 85),
(42788, 85),
(42801, 85),
(42806, 85),
(42820, 85),
(42821, 85),
(42847, 85),
(42848, 85),
(42885, 85),
(42946, 85),
(42964, 85),
(43007, 85),
(43016, 85),
(43018, 85),
(43026, 85),
(43089, 85),
(43092, 85),
(43100, 85),
(43134, 85),
(43138, 85),
(43153, 85),
(43169, 85),
(43170, 85),
(43174, 85),
(43184, 85),
(43213, 85),
(43215, 85),
(43218, 85),
(43228, 85),
(43232, 85),
(43233, 85),
(43234, 85),
(43237, 85),
(43248, 85),
(43249, 85),
(43250, 85),
(43276, 85);