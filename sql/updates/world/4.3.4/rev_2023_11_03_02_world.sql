-- gossip_menu indexes
DELETE FROM `gossip_menu_option` WHERE `MenuId` IN (4466,10549,10675,10697,10814,11045,11185,12528,12535,12835,12924,14144);
INSERT INTO `gossip_menu_option` (`MenuID`, `OptionID`, `OptionIcon`, `OptionText`, `OptionBroadcastTextID`, `OptionType`, `OptionNpcflag`, `ActionMenuID`, `ActionPoiID`, `BoxCoded`, `BoxMoney`, `BoxText`, `BoxBroadcastTextID`, `VerifiedBuild`) VALUES
(4466, 0, 3, 'I seek more training in the priestly ways.', 7169, 5, 16, 0, 0, 0, 0, NULL, 0, 15211),
(4466, 1, 0, 'I wish to unlearn my talents.', 62295, 16, 16, 4461, 0, 0, 0, NULL, 0, 0),
(10549, 0, 3, 'I wish to train.', 7556, 5, 16, 0, 0, 0, 0, NULL, 0, 0),
(10549, 1, 0, 'I wish to unlearn my talents.', 62295, 16, 16, 4461, 0, 0, 0, NULL, 0, 0),
(10549, 2, 0, 'I wish to know about Dual Talent Specialization.', 33762, 20, 1, 10371, 0, 0, 0, NULL, 0, 0),
(4466, 2, 0, 'I wish to know about Dual Talent Specialization.', 33762, 20, 1, 10371, 0, 0, 0, NULL, 0, 0),
(10675, 0, 3, 'I wish to train.', 7556, 5, 16, 0, 0, 0, 0, NULL, 0, 0),
(10675, 1, 0, 'I wish to unlearn my talents.', 62295, 16, 16, 4461, 0, 0, 0, NULL, 0, 0),
(10675, 2, 0, 'I wish to know about Dual Talent Specialization.', 33762, 20, 1, 10371, 0, 0, 0, NULL, 0, 0),
(10697, 0, 3, 'I wish to train.', 7556, 5, 16, 0, 0, 0, 0, NULL, 0, 0),
(10697, 1, 0, 'I wish to unlearn my talents.', 62295, 16, 16, 4461, 0, 0, 0, NULL, 0, 0),
(10697, 2, 0, 'I wish to know about Dual Talent Specialization.', 33762, 20, 1, 10371, 0, 0, 0, NULL, 0, 0),
(10814, 0, 3, 'I wish to train.', 7556, 5, 16, 0, 0, 0, 0, NULL, 0, 0),
(10814, 1, 0, 'I wish to unlearn my talents.', 62295, 16, 16, 4461, 0, 0, 0, NULL, 0, 0),
(10814, 2, 0, 'I wish to know about Dual Talent Specialization.', 33762, 20, 1, 10371, 0, 0, 0, NULL, 0, 0),
(11045, 0, 3, 'I wish to train.', 7556, 5, 16, 0, 0, 0, 0, NULL, 0, 0),
(11045, 1, 0, 'I wish to unlearn my talents.', 62295, 16, 16, 4461, 0, 0, 0, NULL, 0, 0),
(11045, 2, 0, 'I wish to know about Dual Talent Specialization.', 33762, 20, 1, 10371, 0, 0, 0, NULL, 0, 0),
(11185, 0, 3, 'I wish to train.', 7556, 5, 16, 0, 0, 0, 0, NULL, 0, 0),
(11185, 1, 0, 'I wish to unlearn my talents.', 62295, 16, 16, 4461, 0, 0, 0, NULL, 0, 0),
(11185, 2, 0, 'I wish to know about Dual Talent Specialization.', 33762, 20, 1, 10371, 0, 0, 0, NULL, 0, 0),
(12528, 0, 3, 'I wish to train.', 7556, 5, 16, 0, 0, 0, 0, NULL, 0, 0),
(12528, 1, 0, 'I wish to unlearn my talents.', 62295, 16, 16, 4461, 0, 0, 0, NULL, 0, 0),
(12528, 2, 0, 'I wish to know about Dual Talent Specialization.', 33762, 20, 1, 10371, 0, 0, 0, NULL, 0, 0),
(12535, 0, 3, 'I wish to train.', 7556, 5, 16, 0, 0, 0, 0, NULL, 0, 0),
(12535, 1, 0, 'I wish to unlearn my talents.', 62295, 16, 16, 4461, 0, 0, 0, NULL, 0, 0),
(12535, 2, 0, 'I wish to know about Dual Talent Specialization.', 33762, 20, 1, 10371, 0, 0, 0, NULL, 0, 0),
(12835, 0, 3, 'I wish to train.', 7556, 5, 16, 0, 0, 0, 0, NULL, 0, 0),
(12835, 1, 0, 'I wish to unlearn my talents.', 62295, 16, 16, 4461, 0, 0, 0, NULL, 0, 0),
(12835, 2, 0, 'I wish to know about Dual Talent Specialization.', 33762, 20, 1, 10371, 0, 0, 0, NULL, 0, 0),
(12924, 0, 3, 'I wish to train.', 7556, 5, 16, 0, 0, 0, 0, NULL, 0, 0),
(12924, 1, 0, 'I wish to unlearn my talents.', 62295, 16, 16, 4461, 0, 0, 0, NULL, 0, 0),
(12924, 2, 0, 'I wish to know about Dual Talent Specialization.', 33762, 20, 1, 10371, 0, 0, 0, NULL, 0, 0),
(14144, 0, 3, 'I wish to train.', 7556, 5, 16, 0, 0, 0, 0, NULL, 0, 0),
(14144, 1, 0, 'I wish to unlearn my talents.', 62295, 16, 16, 4461, 0, 0, 0, NULL, 0, 0),
(14144, 2, 0, 'I wish to know about Dual Talent Specialization.', 33762, 20, 1, 10371, 0, 0, 0, NULL, 0, 0);
