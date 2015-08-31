-- ------------------------------------------ --
-- Event Spawns (Map 0 and 1 deleted)         --
-- gaps intentional to maintain 3.3.5a ranges --
-- ------------------------------------------ --

SET @Event = 26; -- game_event.entry - Set by TDB
SET @CGUID := 138255; -- Set by TDB - 4.x
SET @OLDCGUID := 925; -- Set by TDB - 4.x
SET @OGUID := 76055; -- Set by TDB - 4.x
SET @OLDOGUID := 49007; -- Set by TDB - 4.x

DELETE FROM `creature` WHERE `guid` BETWEEN @CGUID AND @CGUID+3998;
DELETE FROM `creature` WHERE `guid` BETWEEN @OLDCGUID AND @OLDCGUID+67;
INSERT INTO `creature` (`guid`,`id`,`map`,`spawnMask`,`phaseMask`,`modelid`,`equipment_id`,`position_x`,`position_y`,`position_z`,`orientation`,`spawntimesecs`,`spawndist`,`currentwaypoint`,`curhealth`,`curmana`,`MovementType`,`npcflag`,`unit_flags`,`dynamicflags`) VALUES
(@CGUID+0,20102,530,1,1,0,0,3038.56,3635.53,144.012,3.32713,300,0,0,42,0,0,0,0,0),
(@CGUID+91,32823,530,1,1,0,0,-3975.02,-11870.6,0.510499,1.83564,300,0,0,1,0,0,0,0,0),
(@CGUID+92,32823,530,1,1,0,0,-3966.15,-11868.9,0.677072,4.56098,300,0,0,1,0,0,0,0,0),
(@CGUID+93,32823,530,1,1,0,0,-3967.7,-11879,0.698275,4.67093,300,0,0,1,0,0,0,0,0),
(@CGUID+94,32823,530,1,1,0,0,-3975.57,-11879.1,0.563379,4.59239,300,0,0,1,0,0,0,0,0),
(@CGUID+95,34812,530,1,1,0,0,-3972.39,-11880.5,0.620286,3.0805,300,0,0,112,120,0,0,0,0),
(@CGUID+96,34812,530,1,1,0,0,-3964.17,-11880.1,0.780479,3.19438,300,0,0,112,120,0,0,0,0),
(@CGUID+97,34812,530,1,1,0,0,-3963.04,-11870.8,0.75601,2.91164,300,0,0,112,120,0,0,0,0),
(@CGUID+98,34812,530,1,1,0,0,-3978.56,-11870.4,0.457089,0.131328,300,0,0,112,120,0,0,0,0),
(@CGUID+99,34823,530,1,1,0,0,-3976.38,-11866.3,0.459333,5.05185,300,0,0,112,120,0,0,0,0),
(@CGUID+100,34823,530,1,1,0,0,-3966.71,-11873.1,0.697868,1.5097,300,0,0,112,120,0,0,0,0),
(@CGUID+101,34823,530,1,1,0,0,-3967.81,-11883.4,0.71311,1.655,300,0,0,112,120,0,0,0,0),
(@CGUID+102,34823,530,1,1,0,0,-3976.18,-11882.9,0.572993,1.26623,300,0,0,112,120,0,0,0,0),
(@CGUID+103,34819,530,1,1,0,0,-3978.81,-11880.6,0.5216,0.00173759,300,0,0,112,120,0,0,0,0),
(@CGUID+104,34822,530,1,1,0,0,-3977.04,-11876.4,0.52305,4.85943,300,0,0,112,120,0,0,0,0),
(@CGUID+105,34824,530,1,1,0,0,-3973.01,-11877.4,0.588198,3.97193,300,0,0,112,120,0,0,0,0),
(@CGUID+106,34819,530,1,1,0,0,-3972.32,-11868,0.553342,3.75202,300,0,0,112,120,0,0,0,0),
(@CGUID+107,34822,530,1,1,0,0,-3972.28,-11872,0.574069,2.48753,300,0,0,112,120,0,0,0,0),
(@CGUID+108,34824,530,1,1,0,0,-3975.92,-11873.2,0.516282,1.21125,300,0,0,112,120,0,0,0,0),
(@CGUID+109,34819,530,1,1,0,0,-3969.74,-11869.6,0.615128,5.97469,300,0,0,112,120,0,0,0,0),
(@CGUID+110,34822,530,1,1,0,0,-3967.42,-11866.3,0.628005,5.11075,300,0,0,112,120,0,0,0,0),
(@CGUID+111,34824,530,1,1,0,0,-3963.33,-11867.2,0.713348,3.7638,300,0,0,112,120,0,0,0,0),
(@CGUID+112,34819,530,1,1,0,0,-3970.89,-11880,0.636388,6.24172,300,0,0,112,120,0,0,0,0),
(@CGUID+113,34822,530,1,1,0,0,-3969.61,-11876.8,0.648169,5.31496,300,0,0,112,120,0,0,0,0),
(@CGUID+114,34824,530,1,1,0,0,-3965.25,-11877.4,0.745982,3.85019,300,0,0,112,120,0,0,0,0),
(@CGUID+115,34653,530,1,1,0,0,-3978.75,-11863,0.438156,3.94051,300,0,0,5342,0,0,0,0,0),
(@CGUID+116,34785,530,1,1,0,0,-3957.89,-11863,0.778528,3.47713,600,0,0,1524,0,0,0,0,0),
(@CGUID+117,34783,530,1,1,0,0,-3981.01,-11861.1,1.12189,4.16906,600,0,0,1524,0,0,0,0,0),
(@CGUID+118,35340,530,1,1,0,0,-3955.87,-11866.3,0.850197,5.07934,600,0,0,1524,0,0,0,0,0),
(@CGUID+119,32823,530,1,1,0,0,9298,-7223.78,16.7799,1.75493,300,0,0,1,0,0,0,0,0),
(@CGUID+120,32823,530,1,1,0,0,9296.96,-7213.65,16.3391,1.67246,300,0,0,1,0,0,0,0,0),
(@CGUID+121,32823,530,1,1,0,0,9289.92,-7224.35,16.434,1.69603,300,0,0,1,0,0,0,0,0),
(@CGUID+122,32823,530,1,1,0,0,9288.17,-7214.89,16.2739,1.80598,300,0,0,1,0,0,0,0,0),
(@CGUID+123,34812,530,1,1,0,0,9284.31,-7214.59,16.2382,0.203766,300,0,0,112,120,0,0,0,0),
(@CGUID+124,34812,530,1,1,0,0,9293.31,-7213.1,16.3096,0.0702483,300,0,0,112,120,0,0,0,0),
(@CGUID+125,34812,530,1,1,0,0,9286.66,-7223.4,16.2791,0.105589,300,0,0,112,120,0,0,0,0),
(@CGUID+126,34812,530,1,1,0,0,9294.7,-7222.76,16.6244,0.0663194,300,0,0,112,120,0,0,0,0),
(@CGUID+127,34823,530,1,1,0,0,9297.16,-7219.19,16.6719,4.90438,300,0,0,112,120,0,0,0,0),
(@CGUID+128,34819,530,1,1,0,0,9301.47,-7221.08,16.7434,3.52208,300,0,0,112,120,0,0,0,0),
(@CGUID+129,34822,530,1,1,0,0,9300.59,-7225.64,16.6246,2.31649,300,0,0,112,120,0,0,0,0),
(@CGUID+130,34824,530,1,1,0,0,9296.27,-7226.62,16.7478,1.09127,300,0,0,112,120,0,0,0,0),
(@CGUID+131,34823,530,1,1,0,0,9289.11,-7219.58,16.2432,4.95542,300,0,0,112,120,0,0,0,0),
(@CGUID+132,34819,530,1,1,0,0,9292.91,-7221.98,16.4884,3.49066,300,0,0,112,120,0,0,0,0),
(@CGUID+133,34822,530,1,1,0,0,9292.41,-7226.39,16.6304,2.39895,300,0,0,112,120,0,0,0,0),
(@CGUID+134,34824,530,1,1,0,0,9288.09,-7226.98,16.5047,1.02451,300,0,0,112,120,0,0,0,0),
(@CGUID+135,34823,530,1,1,0,0,9287.09,-7210.47,16.4172,4.94364,300,0,0,112,120,0,0,0,0),
(@CGUID+136,34819,530,1,1,0,0,9290.92,-7212.86,16.314,3.40426,300,0,0,112,120,0,0,0,0),
(@CGUID+137,34822,530,1,1,0,0,9290.26,-7216.64,16.2758,2.35968,300,0,0,112,120,0,0,0,0),
(@CGUID+138,34824,530,1,1,0,0,9286.61,-7217.47,16.1966,1.24049,300,0,0,112,120,0,0,0,0),
(@CGUID+139,34823,530,1,1,0,0,9296.25,-7209.03,16.2939,4.86118,300,0,0,112,120,0,0,0,0),
(@CGUID+140,34819,530,1,1,0,0,9300.17,-7211.93,16.2018,3.35715,300,0,0,112,120,0,0,0,0),
(@CGUID+141,34822,530,1,1,0,0,9299.22,-7215.91,16.5658,2.45001,300,0,0,112,120,0,0,0,0),
(@CGUID+142,34824,530,1,1,0,0,9295.06,-7215.99,16.3911,0.918483,300,0,0,112,120,0,0,0,0),
(@CGUID+143,34654,530,1,1,0,0,9304.13,-7219.49,16.4335,5.7801,300,0,0,5342,0,0,0,0,0),
(@CGUID+144,34787,530,1,1,0,0,9305.74,-7211.06,16.6184,5.76831,600,0,0,1524,0,0,0,0,0),
(@CGUID+145,34786,530,1,1,0,0,9282.31,-7206.7,16.6141,5.7094,600,0,0,1524,0,0,0,0,0),
(@CGUID+146,35342,530,1,1,0,0,9285.98,-7199.02,17.4229,5.64657,600,0,0,1524,0,0,0,0,0),
(@CGUID+242,19171,530,1,1,0,0,-3909.22,-11614.8,-138.101,3.1765,300,0,0,42,0,0,0,0,0),
(@CGUID+3973,19169,530,1,1,0,0,9659.86,-7115.63,14.3239,5.88552,300,0,0,42,0,0,0,0,0),
(@CGUID+3978,20102,530,1,1,0,0,-1888.02,5400.44,-12.4278,5.97919,300,0,0,42,0,0,0,0,0),
(@CGUID+3979,20102,530,1,1,0,0,3035.51,3635.08,144.47,0.901821,300,0,0,42,0,0,0,0,0),
(@CGUID+3980,19169,571,1,1,0,0,5889.57,550.355,639.637,1.57167,300,0,0,42,0,0,0,0,0),
(@CGUID+3981,18927,571,1,1,0,0,5719.3,687.257,645.752,5.72721,300,0,0,42,0,0,0,0,0),
(@CGUID+3983,18927,571,1,1,0,0,5678.09,658.93,647.134,0.088838,300,0,0,42,0,0,0,0,0),
(@CGUID+3985,19171,530,1,1,0,0,-3910.91,-11612.4,-138.243,4.99941,300,0,0,42,0,0,0,0,0),
(@CGUID+3988,19169,571,1,1,0,0,5928.98,639.593,645.557,3.01052,300,0,0,42,0,0,0,0,0),
(@CGUID+3989,19169,530,1,1,0,0,9664.38,-7117.91,14.324,2.63397,300,0,0,42,0,0,0,0,0),
(@CGUID+3998,20102,530,1,1,0,0,-1884.63,5397.52,-12.4278,2.51637,300,0,0,42,0,0,0,0,0);

DELETE FROM `gameobject` WHERE `guid` BETWEEN @OGUID AND @OGUID+591;
DELETE FROM `gameobject` WHERE `guid` BETWEEN @OLDOGUID AND @OLDOGUID+128;
INSERT INTO `gameobject` (`guid`,`id`,`map`,`spawnMask`,`phaseMask`,`position_x`,`position_y`,`position_z`,`orientation`,`rotation0`,`rotation1`,`rotation2`,`rotation3`,`spawntimesecs`,`animprogress`,`state`) VALUES
-- (@OGUID+111,180353,1,1,1,9993.38,2218.72,1328.65,0.0553825,0,0,0.0276877,0.999617,300,0,1),
(@OGUID+112,195191,530,1,1,-3955.34,-11863.2,0.830986,3.63028,0,0,0.970296,-0.24192,300,0,1),
(@OGUID+113,195164,530,1,1,-3955.34,-11863.2,1.71971,3.52818,0,0,0.981377,-0.192092,300,0,1),
(@OGUID+114,195195,530,1,1,-3954.82,-11864.6,1.71971,1.88277,0,0,0.808374,0.588669,300,0,1),
(@OGUID+115,195192,530,1,1,-3956.34,-11862.3,1.71989,2.16551,0,0,0.883254,0.468896,300,0,1),
(@OGUID+116,195215,530,1,1,-3955.13,-11864.2,1.97205,2.12624,0,0,0.873877,0.486147,300,0,1),
(@OGUID+117,195200,530,1,1,-3951.67,-11868.6,0.949688,3.61065,0,0,0.972624,-0.232383,300,0,1),
(@OGUID+118,195198,530,1,1,-3955.82,-11857.2,0.764875,3.7206,0,0,0.958385,-0.285478,300,0,1),
(@OGUID+119,195196,530,1,1,-3959.87,-11860,0.710028,3.96408,0,0,0.916625,-0.399748,300,0,1),
(@OGUID+120,195194,530,1,1,-3957.05,-11866.4,0.827674,3.32397,0,0,0.995845,-0.0910648,300,0,1),
(@OGUID+121,195197,530,1,1,-3954.63,-11867.9,0.891499,3.11192,0,0,0.99989,0.0148376,300,0,1),
(@OGUID+122,195212,530,1,1,-3953.94,-11866.2,0.8877,3.1237,0,0,0.99996,0.00894757,300,0,1),
(@OGUID+123,195212,530,1,1,-3953.9,-11859.5,0.824961,3.48498,0,0,0.985297,-0.170852,300,0,1),
(@OGUID+124,195212,530,1,1,-3956.95,-11858.9,0.758281,3.66955,0,0,0.96536,-0.260923,300,0,1),
(@OGUID+125,195212,530,1,1,-3959,-11857.7,0.705579,3.79521,0,0,0.947071,-0.321024,300,0,1),
(@OGUID+126,195303,530,1,1,-3958.59,-11860.3,1.02229,4.30965,0,0,0.834249,-0.551388,300,0,1),
(@OGUID+127,195303,530,1,1,-3957.48,-11861.1,0.769591,4.15257,0,0,0.874938,-0.484235,300,0,1),
(@OGUID+128,180353,530,1,1,-3954.67,-11860.3,0.817822,3.70882,0,0,0.960051,-0.279826,300,0,1),
(@OGUID+129,180353,530,1,1,-3952.77,-11864.2,0.892707,3.59886,0,0,0.973977,-0.226648,300,0,1),
(@OGUID+130,180353,530,1,1,-3978.55,-11860.6,0.446388,3.98763,0,0,0.911853,-0.410517,300,0,1),
(@OGUID+131,180353,530,1,1,-3980.7,-11859.3,0.420101,4.24681,0,0,0.851157,-0.524911,300,0,1),
(@OGUID+132,180353,530,1,1,-3976.57,-11861.8,0.465867,4.09759,0,0,0.887918,-0.460003,300,0,1),
(@OGUID+133,180353,530,1,1,-3974.81,-11863.1,0.483395,6.25743,0,0,0.0128756,-0.999917,300,0,1),
(@OGUID+134,180353,530,1,1,-3970.8,-11863.2,0.532856,6.25743,0,0,0.0128756,-0.999917,300,0,1),
(@OGUID+135,180353,530,1,1,-3965.14,-11863.3,0.639776,6.25743,0,0,0.0128756,-0.999917,300,0,1),
(@OGUID+136,180353,530,1,1,-3958.77,-11871.5,0.842487,4.22718,0,0,0.856269,-0.516529,300,0,1),
(@OGUID+137,180353,530,1,1,-3961.29,-11876.7,0.829859,4.26252,0,0,0.847008,-0.53158,300,0,1),
(@OGUID+138,180353,530,1,1,-3963.99,-11882.9,0.794779,4.0701,0,0,0.894156,-0.447755,300,0,1),
(@OGUID+139,180353,530,1,1,-3969.7,-11887.5,0.697352,2.84095,0,0,0.988723,0.149756,300,0,1),
(@OGUID+140,180353,530,1,1,-3972.96,-11886.6,0.632589,2.86059,0,0,0.990146,0.140042,300,0,1),
(@OGUID+141,180353,530,1,1,-3977.27,-11885.7,0.550238,2.99803,0,0,0.997425,0.0717196,300,0,1),
(@OGUID+142,180353,530,1,1,-3982.69,-11880.4,0.446558,1.34869,0,0,0.624387,0.781115,300,0,1),
(@OGUID+143,180353,530,1,1,-3981.39,-11874.6,0.447613,1.34869,0,0,0.624387,0.781115,300,0,1),
(@OGUID+144,180353,530,1,1,-3981.4,-11869.1,0.40929,1.43509,0,0,0.657536,0.753423,300,0,1),
(@OGUID+228,179968,530,1,1,-3981.01,-11861.1,0.416226,4.16906,0,0,0.870916,-0.491432,300,0,1),
(@OGUID+229,179968,530,1,1,-3980.44,-11860.2,0.423322,4.16513,0,0,0.871879,-0.489721,300,0,1),
(@OGUID+230,179968,530,1,1,-3980.04,-11861.9,0.426034,4.23975,0,0,0.853007,-0.5219,300,0,1),
(@OGUID+231,179968,530,1,1,-3979.42,-11860.7,0.432217,4.23975,0,0,0.853007,-0.5219,300,0,1),
(@OGUID+232,179968,530,1,1,-3962.22,-11859.2,0.655255,4.65994,0,0,0.725406,-0.688321,300,0,1),
(@OGUID+233,179968,530,1,1,-3962.17,-11858.3,0.647246,4.65994,0,0,0.725406,-0.688321,300,0,1),
(@OGUID+234,179968,530,1,1,-3962.17,-11858.3,1.35331,4.65994,0,0,0.725406,-0.688321,300,0,1),
(@OGUID+235,179968,530,1,1,-3963.3,-11858,0.622251,4.49893,0,0,0.778409,-0.627758,300,0,1),
(@OGUID+236,179968,530,1,1,-3963.57,-11858.9,0.625538,4.59711,0,0,0.746669,-0.665196,300,0,1),
(@OGUID+505,179968,530,1,1,9305.97,-7211.26,15.9125,5.77224,0,0,0.252701,-0.967544,300,0,1),
(@OGUID+506,179968,530,1,1,9305.18,-7213.52,15.9982,6.0864,0,0,0.0982337,-0.995163,300,0,1),
(@OGUID+507,179968,530,1,1,9307.05,-7209.16,15.8791,5.57197,0,0,0.348162,-0.937434,300,0,1),
(@OGUID+508,195191,530,1,1,9281.03,-7204.93,16.7354,5.63479,0,0,0.318548,-0.947907,300,0,1),
(@OGUID+509,195164,530,1,1,9281.03,-7204.93,17.6235,4.91223,0,0,0.633045,-0.774115,300,0,1),
(@OGUID+510,195195,530,1,1,9282.63,-7203.82,17.6235,3.74591,0,0,0.954697,-0.297581,300,0,1),
(@OGUID+511,195192,530,1,1,9280.84,-7205.99,17.6245,4.04436,0,0,0.899845,-0.436211,300,0,1),
(@OGUID+512,195215,530,1,1,9280.06,-7205.31,17.623,4.23678,0,0,0.85378,-0.520635,300,0,1),
(@OGUID+513,195200,530,1,1,9284.67,-7200.88,17.2278,5.11643,0,0,0.550848,-0.834605,300,0,1),
(@OGUID+514,195198,530,1,1,9277.36,-7207.52,16.424,6.09424,0,0,0.0943323,-0.995541,300,0,1),
(@OGUID+515,195196,530,1,1,9279.56,-7209.09,16.4021,6.25525,0,0,0.0139694,-0.999902,300,0,1),
(@OGUID+516,195194,530,1,1,9283.41,-7205.43,16.7583,1.28368,0,0,0.598669,0.800997,300,0,1),
(@OGUID+517,195197,530,1,1,9286.75,-7200.71,17.2161,1.58213,0,0,0.711101,0.703089,300,0,1),
(@OGUID+518,195197,530,1,1,9277.64,-7209.89,16.2857,2.34789,0,0,0.922283,0.386516,300,0,1),
(@OGUID+519,195212,530,1,1,9276.6,-7204.24,16.638,5.61908,0,0,0.325986,-0.945375,300,0,1),
(@OGUID+520,195212,530,1,1,9278.68,-7200.5,17.0822,5.58766,0,0,0.340795,-0.940138,300,0,1),
(@OGUID+521,195212,530,1,1,9277.92,-7202.53,16.8511,5.67405,0,0,0.299879,-0.953977,300,0,1),
(@OGUID+522,195199,530,1,1,9275.5,-7205.98,16.4592,5.67013,0,0,0.301752,-0.953387,300,0,1),
(@OGUID+523,195199,530,1,1,9280.95,-7200.67,17.17,1.21299,0,0,0.569991,0.821651,300,0,1),
(@OGUID+524,195303,530,1,1,9282.95,-7202.64,17.0309,5.42273,0,0,0.417079,-0.90887,300,0,1),
(@OGUID+525,195303,530,1,1,9279.83,-7207.33,16.9276,6.00392,0,0,0.139179,-0.990267,300,0,1),
(@OGUID+526,180353,530,1,1,9278.21,-7205.61,16.6083,0.957736,0,0,0.460775,0.887517,300,0,1),
(@OGUID+527,180353,530,1,1,9280.55,-7202.27,16.9974,0.957736,0,0,0.460775,0.887517,300,0,1),
(@OGUID+528,180353,530,1,1,9306.54,-7210.01,16.0835,5.78401,0,0,0.247004,-0.969014,300,0,1),
(@OGUID+529,180353,530,1,1,9305.3,-7212.38,15.9549,6.14922,0,0,0.0669331,-0.997757,300,0,1),
(@OGUID+530,180353,530,1,1,9302.77,-7211.58,16.0804,3.41211,0,0,0.990867,-0.134845,300,0,1),
(@OGUID+531,180353,530,1,1,9303.11,-7223.44,16.4783,4.7355,0,0,0.698888,-0.715231,300,0,1),
(@OGUID+532,180353,530,1,1,9299.34,-7229.96,16.5578,3.149,0,0,0.999993,-0.00370244,300,0,1),
(@OGUID+533,180353,530,1,1,9293.58,-7230.07,16.8357,3.16078,0,0,0.999954,-0.00959279,300,0,1),
(@OGUID+534,180353,530,1,1,9288.71,-7230.16,16.7451,3.16078,0,0,0.999954,-0.00959279,300,0,1),
(@OGUID+535,180353,530,1,1,9283.86,-7225.95,16.2737,1.83346,0,0,0.793615,0.60842,300,0,1),
(@OGUID+536,180353,530,1,1,9282.67,-7221.53,16.1109,1.83346,0,0,0.793615,0.60842,300,0,1),
(@OGUID+537,180353,530,1,1,9281.12,-7215.74,16.1288,1.83346,0,0,0.793615,0.60842,300,0,1),
(@OGUID+538,180353,530,1,1,9286.53,-7207.75,16.5461,0.117361,0,0,0.0586467,0.998279,300,0,1),
(@OGUID+539,180353,530,1,1,9292.12,-7207.09,16.4871,0.117361,0,0,0.0586467,0.998279,300,0,1),
(@OGUID+540,180353,530,1,1,9297.13,-7206.5,16.3099,0.117361,0,0,0.0586467,0.998279,300,0,1),
(@OGUID+541,179968,530,1,1,9275.02,-7207.7,16.313,0.160557,0,0,0.0801923,0.996779,300,0,1),
(@OGUID+542,179968,530,1,1,9274.35,-7207.81,16.313,0.160557,0,0,0.0801923,0.996779,300,0,1),
(@OGUID+543,179968,530,1,1,9274.35,-7207.81,17.0197,0.160557,0,0,0.0801923,0.996779,300,0,1),
(@OGUID+544,179968,530,1,1,9274.96,-7208.85,16.233,3.30608,0,0,0.99662,-0.0821494,300,0,1),
(@OGUID+545,179968,530,1,1,9280.72,-7215.14,16.1388,0.368687,0,0,0.183301,0.983057,300,0,1),
(@OGUID+546,179968,530,1,1,9281.43,-7216.52,16.1099,0.490423,0,0,0.242762,0.970086,300,0,1),
(@OGUID+547,179968,530,1,1,9282.59,-7220.71,16.0999,0.309781,0,0,0.154272,0.988028,300,0,1),
(@OGUID+548,179968,530,1,1,9282.9,-7222.09,16.1318,0.352978,0,0,0.175574,0.984466,300,0,1),
(@OGUID+549,179968,530,1,1,9283.53,-7225.25,16.2282,0.553255,0,0,0.273113,0.961982,300,0,1),
(@OGUID+550,179968,530,1,1,9284.43,-7226.71,16.3315,0.404029,0,0,0.200643,0.979664,300,0,1),
(@OGUID+551,179968,530,1,1,9287.94,-7231.05,16.794,1.66459,0,0,0.739479,0.673179,300,0,1),
(@OGUID+552,179968,530,1,1,9289.7,-7230.64,16.8237,1.73528,0,0,0.762805,0.646629,300,0,1),
(@OGUID+553,179968,530,1,1,9292.94,-7230.5,16.8598,1.6214,0,0,0.724769,0.688993,300,0,1),
(@OGUID+554,179968,530,1,1,9294.52,-7230.09,16.842,1.77455,0,0,0.775353,0.631527,300,0,1),
(@OGUID+555,179968,530,1,1,9298.32,-7229.79,16.6317,1.53108,0,0,0.692925,0.72101,300,0,1),
(@OGUID+556,179968,530,1,1,9300.41,-7229.9,16.4162,1.64103,0,0,0.731498,0.681844,300,0,1),
(@OGUID+557,179968,530,1,1,9302.88,-7224.19,16.4577,2.9173,0,0,0.993718,0.11191,300,0,1),
(@OGUID+558,179968,530,1,1,9303.3,-7222.68,16.4909,2.99977,0,0,0.997487,0.0708516,300,0,1),
(@OGUID+559,179968,530,1,1,9302.54,-7210.81,16.0667,3.32964,0,0,0.995583,-0.0938842,300,0,1),
(@OGUID+560,179968,530,1,1,9302.88,-7212.35,16.1022,3.40818,0,0,0.99113,-0.132898,300,0,1),
(@OGUID+561,179968,530,1,1,9298.13,-7206.27,16.2716,4.76692,0,0,0.687567,-0.726121,300,0,1),
(@OGUID+562,179968,530,1,1,9296.25,-7206.56,16.3527,4.85331,0,0,0.65557,-0.755135,300,0,1),
(@OGUID+563,179968,530,1,1,9293.1,-7206.86,16.4519,4.85331,0,0,0.65557,-0.755135,300,0,1),
(@OGUID+564,179968,530,1,1,9291.38,-7207.12,16.5108,4.84703,0,0,0.657939,-0.753071,300,0,1),
(@OGUID+565,179968,530,1,1,9287.17,-7207.68,16.5565,4.91379,0,0,0.63244,-0.77461,300,0,1),
(@OGUID+566,179968,530,1,1,9285.93,-7208.03,16.53,4.83132,0,0,0.663832,-0.747882,300,0,1),
(@OGUID+567,195164,530,1,1,9275.34,-7208.59,16.8732,2.23558,0,0,0.899136,0.437669,300,0,1),
(@OGUID+568,195164,530,1,1,9280.83,-7214.95,16.8446,0.287796,0,0,0.143402,0.989665,300,0,1),
(@OGUID+569,195164,530,1,1,9281.47,-7216.43,16.8154,0.653006,0,0,0.320733,0.94717,300,0,1),
(@OGUID+570,195164,530,1,1,9282.72,-7220.66,16.8067,0.169986,0,0,0.0848909,0.99639,300,0,1),
(@OGUID+571,195164,530,1,1,9282.76,-7222.2,16.8376,0.307431,0,0,0.153111,0.988209,300,0,1),
(@OGUID+572,195164,530,1,1,9283.49,-7225.2,16.934,0.668714,0,0,0.328162,0.944621,300,0,1),
(@OGUID+573,195164,530,1,1,9284.4,-7226.69,17.0374,0.495927,0,0,0.24543,0.969414,300,0,1),
(@OGUID+574,195164,530,1,1,9287.91,-7231.42,17.4998,1.74471,0,0,0.765845,0.643025,300,0,1),
(@OGUID+575,195164,530,1,1,9289.82,-7230.78,17.5293,1.8154,0,0,0.788089,0.615562,300,0,1),
(@OGUID+576,195164,530,1,1,9292.8,-7230.59,17.5657,1.64653,0,0,0.733371,0.679829,300,0,1),
(@OGUID+577,195164,530,1,1,9294.49,-7230.15,17.5487,1.61119,0,0,0.721244,0.692681,300,0,1),
(@OGUID+578,195164,530,1,1,9298.31,-7229.87,17.3376,1.67402,0,0,0.742645,0.669685,300,0,1),
(@OGUID+579,195164,530,1,1,9300.33,-7229.86,17.1223,1.54051,0,0,0.696317,0.717734,300,0,1),
(@OGUID+580,195164,530,1,1,9302.9,-7224.07,17.1636,3.09952,0,0,0.999779,0.0210337,300,0,1),
(@OGUID+581,195164,530,1,1,9303.32,-7222.57,17.197,3.05633,0,0,0.999091,0.0426208,300,0,1),
(@OGUID+582,195164,530,1,1,9305.22,-7213.43,16.7041,2.90317,0,0,0.992903,0.118928,300,0,1),
(@OGUID+583,195164,530,1,1,9307.03,-7209.21,16.5859,2.57331,0,0,0.959902,0.280336,300,0,1),
(@OGUID+584,195164,530,1,1,9302.46,-7210.65,16.7734,3.27231,0,0,0.997865,-0.065312,300,0,1),
(@OGUID+585,195164,530,1,1,9302.7,-7212.36,16.8081,3.26838,0,0,0.997991,-0.0633526,300,0,1),
(@OGUID+586,195164,530,1,1,9298.14,-7206.38,16.9784,4.81562,0,0,0.669685,-0.742645,300,0,1),
(@OGUID+587,195164,530,1,1,9296.39,-7206.61,17.0586,4.77242,0,0,0.685567,-0.728009,300,0,1),
(@OGUID+588,195164,530,1,1,9293.13,-7206.97,17.1583,4.741,0,0,0.696919,-0.71715,300,0,1),
(@OGUID+589,195164,530,1,1,9291.36,-7207.04,17.2164,4.88237,0,0,0.644529,-0.76458,300,0,1),
(@OGUID+590,195164,530,1,1,9287.29,-7207.67,17.2622,4.92164,0,0,0.629394,-0.777087,300,0,1),
(@OGUID+591,195164,530,1,1,9285.87,-7208.02,17.2357,4.89415,0,0,0.640014,-0.768363,300,0,1);

-- Link Creatures to event
DELETE FROM `game_event_creature` WHERE `eventEntry`=@Event AND `guid` BETWEEN @CGUID AND @CGUID+3998;
DELETE FROM `game_event_creature` WHERE `eventEntry`=@Event AND `guid` BETWEEN @OLDCGUID AND @OLDCGUID+3998;
INSERT INTO game_event_creature SELECT @Event, creature.guid FROM `creature` WHERE creature.guid BETWEEN @CGUID AND @CGUID+3998;
-- Link Objects to event
DELETE FROM `game_event_gameobject` WHERE `eventEntry`=@Event AND `guid` BETWEEN @OGUID AND @OGUID+591;
DELETE FROM `game_event_gameobject` WHERE `eventEntry`=@Event AND `guid` BETWEEN @OLDOGUID AND @OLDOGUID+591;
INSERT INTO `game_event_gameobject` SELECT @Event, gameobject.guid FROM `gameobject` WHERE gameobject.guid BETWEEN @OGUID AND @OGUID+591;

DELETE FROM `game_event_creature` WHERE `guid` NOT IN (SELECT `guid` FROM `creature`);
DELETE FROM `game_event_gameobject` WHERE `guid` NOT IN (SELECT `guid` FROM `gameobject`);