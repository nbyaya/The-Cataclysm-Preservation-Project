ALTER TABLE `access_requirement` MODIFY `mapId` int(10) UNSIGNED NOT NULL;
ALTER TABLE `access_requirement` MODIFY `item` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `access_requirement` MODIFY `item2` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `access_requirement` MODIFY `quest_done_A` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `access_requirement` MODIFY `quest_done_H` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `access_requirement` MODIFY `completed_achievement` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `achievement_criteria_data` MODIFY `criteria_id` int(10) NOT NULL;
ALTER TABLE `achievement_criteria_data` MODIFY `value1` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `achievement_criteria_data` MODIFY `value2` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `achievement_reward` MODIFY `ID` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `achievement_reward` MODIFY `TitleA` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `achievement_reward` MODIFY `TitleH` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `achievement_reward` MODIFY `ItemID` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `achievement_reward` MODIFY `Sender` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `achievement_reward` MODIFY `MailTemplateID` int(10) UNSIGNED NULL DEFAULT '0';
ALTER TABLE `achievement_reward_locale` MODIFY `ID` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `areatrigger_involvedrelation` MODIFY `id` int(10) UNSIGNED NOT NULL DEFAULT '0' COMMENT 'Identifier';
ALTER TABLE `areatrigger_involvedrelation` MODIFY `quest` int(10) UNSIGNED NOT NULL DEFAULT '0' COMMENT 'Quest Identifier';
ALTER TABLE `areatrigger_scripts` MODIFY `entry` int(10) NOT NULL;
ALTER TABLE `areatrigger_tavern` MODIFY `id` int(10) UNSIGNED NOT NULL DEFAULT '0' COMMENT 'Identifier'; 
ALTER TABLE `areatrigger_teleport` MODIFY `ID` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `battleground_template` MODIFY `ID` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `battleground_template` MODIFY `AllianceStartLoc` int(10) UNSIGNED NOT NULL;
ALTER TABLE `battleground_template` MODIFY `HordeStartLoc` int(10) UNSIGNED NOT NULL;
ALTER TABLE `battlemaster_entry` MODIFY `entry` int(10) UNSIGNED NOT NULL DEFAULT '0' COMMENT 'Entry of a creature';
ALTER TABLE `battlemaster_entry` MODIFY `bg_template` int(10) UNSIGNED NOT NULL DEFAULT '0' COMMENT 'Battleground template id';
ALTER TABLE `broadcast_text` MODIFY `ID` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `broadcast_text` MODIFY `LanguageID` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `broadcast_text` MODIFY `EmoteID1` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `broadcast_text` MODIFY `EmoteID2` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `broadcast_text` MODIFY `EmoteID3` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `broadcast_text` MODIFY `EmoteDelay1` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `broadcast_text` MODIFY `EmoteDelay2` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `broadcast_text` MODIFY `EmoteDelay3` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `broadcast_text` MODIFY `SoundEntriesID` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `broadcast_text` MODIFY `EmotesID` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `broadcast_text` MODIFY `Flags` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `broadcast_text_locale` MODIFY `ID` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `conditions` MODIFY `SourceTypeOrReferenceId` int(10) NOT NULL DEFAULT '0';
ALTER TABLE `conditions` MODIFY `SourceGroup` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `conditions` MODIFY `SourceEntry` int(10) NOT NULL DEFAULT '0';
ALTER TABLE `conditions` MODIFY `ElseGroup` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `conditions` MODIFY `ConditionTypeOrReference` int(10) NOT NULL DEFAULT '0';
ALTER TABLE `conditions` MODIFY `ConditionValue1` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `conditions` MODIFY `ConditionValue2` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `conditions` MODIFY `ConditionValue3` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `conditions` MODIFY `ErrorType` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `conditions` MODIFY `ErrorTextId` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `creature` MODIFY `id` int(10) UNSIGNED NOT NULL DEFAULT '0' COMMENT 'Creature Identifier';
ALTER TABLE `creature` MODIFY `modelid` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `creature` MODIFY `currentwaypoint` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `creature_addon` MODIFY `mount` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `creature_classlevelstats` MODIFY `basehp0` int(10) UNSIGNED NOT NULL DEFAULT '1';
ALTER TABLE `creature_classlevelstats` MODIFY `basehp1` int(10) UNSIGNED NOT NULL DEFAULT '1';
ALTER TABLE `creature_classlevelstats` MODIFY `basehp2` int(10) UNSIGNED NOT NULL DEFAULT '1';
ALTER TABLE `creature_classlevelstats` MODIFY `basehp3` int(10) UNSIGNED NOT NULL DEFAULT '1';
ALTER TABLE `creature_classlevelstats` MODIFY `basemana` int(10) UNSIGNED NOT NULL DEFAULT '1';
ALTER TABLE `creature_classlevelstats` MODIFY `basearmor` int(10) UNSIGNED NOT NULL DEFAULT '1';
ALTER TABLE `creature_equip_template` MODIFY `CreatureID` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `creature_equip_template` MODIFY `ItemID1` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `creature_equip_template` MODIFY `ItemID2` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `creature_equip_template` MODIFY `ItemID3` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `creature_loot_template` MODIFY `Entry` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `creature_loot_template` MODIFY `Item` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `creature_loot_template` MODIFY `Reference` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `creature_model_info` MODIFY `DisplayID` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `creature_model_info` MODIFY `DisplayID_Other_Gender` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `creature_onkill_reward` MODIFY `creature_id` int(10) UNSIGNED NOT NULL DEFAULT '0' COMMENT 'Creature Identifier';
ALTER TABLE `creature_onkill_reward` MODIFY `RewOnKillRepValue1` int(10) NOT NULL DEFAULT '0';
ALTER TABLE `creature_onkill_reward` MODIFY `RewOnKillRepValue2` int(10) NOT NULL DEFAULT '0';
ALTER TABLE `creature_onkill_reward` MODIFY `CurrencyId1` int(10) NULL DEFAULT '0';
ALTER TABLE `creature_onkill_reward` MODIFY `CurrencyId2` int(10) NULL DEFAULT '0';
ALTER TABLE `creature_onkill_reward` MODIFY `CurrencyId3` int(10) NULL DEFAULT '0';
ALTER TABLE `creature_onkill_reward` MODIFY `CurrencyCount1` int(10) NULL DEFAULT '0';
ALTER TABLE `creature_onkill_reward` MODIFY `CurrencyCount2` int(10) NULL DEFAULT '0';
ALTER TABLE `creature_onkill_reward` MODIFY `CurrencyCount3` int(10) NULL DEFAULT '0';
ALTER TABLE `creature_questender` MODIFY `id` int(10) UNSIGNED NOT NULL DEFAULT '0' COMMENT 'Identifier';
ALTER TABLE `creature_questender` MODIFY `quest` int(10) UNSIGNED NOT NULL DEFAULT '0' COMMENT 'Quest Identifier';
ALTER TABLE `creature_queststarter` MODIFY `id` int(10) UNSIGNED NOT NULL DEFAULT '0' COMMENT 'Identifier';
ALTER TABLE `creature_queststarter` MODIFY `quest` int(10) UNSIGNED NOT NULL DEFAULT '0' COMMENT 'Quest Identifier';
ALTER TABLE `creature_sparring_template` MODIFY `CreatureID` int(10) UNSIGNED NOT NULL;
ALTER TABLE `creature_summon_groups` MODIFY `summonerId` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `creature_summon_groups` MODIFY `entry` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `creature_template` MODIFY `entry` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `creature_template` MODIFY `difficulty_entry_1` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `creature_template` MODIFY `difficulty_entry_2` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `creature_template` MODIFY `difficulty_entry_3` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `creature_template` MODIFY `modelid1` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `creature_template` MODIFY `modelid2` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `creature_template` MODIFY `modelid3` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `creature_template` MODIFY `modelid4` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `creature_template` MODIFY `gossip_menu_id` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `creature_template` MODIFY `lootid` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `creature_template` MODIFY `pickpocketloot` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `creature_template` MODIFY `skinloot` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `creature_template` MODIFY `spell1` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `creature_template` MODIFY `spell2` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `creature_template` MODIFY `spell3` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `creature_template` MODIFY `spell4` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `creature_template` MODIFY `spell5` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `creature_template` MODIFY `spell6` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `creature_template` MODIFY `spell7` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `creature_template` MODIFY `spell8` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `creature_template` MODIFY `PetSpellDataId` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `creature_template` MODIFY `VehicleId` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `creature_template_addon` MODIFY `entry` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `creature_template_addon` MODIFY `mount` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `creature_template_addon` MODIFY `emote` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `creature_template_locale` MODIFY `entry` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `creature_text` MODIFY `CreatureID` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `creature_text` MODIFY `Emote` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `creature_text` MODIFY `Duration` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `creature_text` MODIFY `Sound` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `creature_text` MODIFY `SoundType` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `creature_text` MODIFY `BroadcastTextId` int(10) NOT NULL DEFAULT '0';
ALTER TABLE `creature_text_locale` MODIFY `CreatureID` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `disenchant_loot_template` MODIFY `Entry` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `disenchant_loot_template` MODIFY `Item` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `disenchant_loot_template` MODIFY `Reference` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `event_scripts` MODIFY `id` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `event_scripts` MODIFY `command` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `event_scripts` MODIFY `datalong` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `exploration_basexp` MODIFY `basexp` int(10) NOT NULL DEFAULT '0';
ALTER TABLE `fishing_loot_template` MODIFY `Entry` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `fishing_loot_template` MODIFY `Item` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `fishing_loot_template` MODIFY `Reference` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `game_event` MODIFY `holiday` int(10) UNSIGNED NOT NULL DEFAULT '0' COMMENT 'Client side holiday id';
ALTER TABLE `game_event_condition` MODIFY `condition_id` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `game_event_creature_quest` MODIFY `id` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `game_event_creature_quest` MODIFY `quest` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `game_event_gameobject_quest` MODIFY `id` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `game_event_gameobject_quest` MODIFY `quest` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `game_event_model_equip` MODIFY `modelid` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `game_event_npc_vendor` MODIFY `guid` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `game_event_npc_vendor` MODIFY `item` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `game_event_npc_vendor` MODIFY `maxcount` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `game_event_npc_vendor` MODIFY `incrtime` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `game_event_npc_vendor` MODIFY `ExtendedCost` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `game_event_npcflag` MODIFY `guid` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `game_event_pool` MODIFY `pool_entry` int(10) UNSIGNED NOT NULL DEFAULT '0' COMMENT 'Id of the pool';
ALTER TABLE `game_event_prerequisite` MODIFY `prerequisite_event` int(10) UNSIGNED NOT NULL;
ALTER TABLE `game_event_quest_condition` MODIFY `quest` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `game_event_quest_condition` MODIFY `condition_id` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `game_event_seasonal_questrelation` MODIFY `eventEntry` int(10) UNSIGNED NOT NULL DEFAULT '0' COMMENT 'Entry of the game event';
ALTER TABLE `game_tele` MODIFY `id` int(10) UNSIGNED NOT NULL AUTO_INCREMENT;
ALTER TABLE `game_weather` MODIFY `zone` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `gameobject` MODIFY `id` int(10) UNSIGNED NOT NULL DEFAULT '0' COMMENT 'Gameobject Identifier';
ALTER TABLE `gameobject_loot_template` MODIFY `Entry` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `gameobject_loot_template` MODIFY `Item` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `gameobject_loot_template` MODIFY `Reference` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `gameobject_questender` MODIFY `id` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `gameobject_questender` MODIFY `quest` int(10) UNSIGNED NOT NULL DEFAULT '0' COMMENT 'Quest Identifier';
ALTER TABLE `gameobject_queststarter` MODIFY `id` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `gameobject_queststarter` MODIFY `quest` int(10) UNSIGNED NOT NULL DEFAULT '0' COMMENT 'Quest Identifier';
ALTER TABLE `gameobject_template` MODIFY `entry` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `gameobject_template` MODIFY `displayId` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `gameobject_template_addon` MODIFY `entry` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `gameobject_template_locale` MODIFY `entry` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `graveyard_zone` MODIFY `ID` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `graveyard_zone` MODIFY `GhostZone` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `guild_challenges` MODIFY `ChallengeType` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `guild_challenges` MODIFY `ChallengeCount` int(10) UNSIGNED NULL DEFAULT '1';
ALTER TABLE `guild_challenges` MODIFY `Experience` int(10) UNSIGNED NULL DEFAULT '0';
ALTER TABLE `guild_challenges` MODIFY `Gold` int(10) UNSIGNED NULL DEFAULT '0';
ALTER TABLE `guild_challenges` MODIFY `GoldMaxLevel` int(10) UNSIGNED NULL DEFAULT '0';
ALTER TABLE `item_loot_template` MODIFY `Entry` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `item_loot_template` MODIFY `Item` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `item_loot_template` MODIFY `Reference` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `mail_level_reward` MODIFY `raceMask` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `mail_level_reward` MODIFY `mailTemplateId` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `mail_level_reward` MODIFY `senderEntry` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `mail_loot_template` MODIFY `Entry` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `mail_loot_template` MODIFY `Item` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `mail_loot_template` MODIFY `Reference` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `milling_loot_template` MODIFY `Entry` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `milling_loot_template` MODIFY `Item` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `milling_loot_template` MODIFY `Reference` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `npc_text` MODIFY `ID` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `npc_text` MODIFY `BroadcastTextID0` int(10) NOT NULL DEFAULT '0';
ALTER TABLE `npc_text` MODIFY `BroadcastTextID1` int(10) NOT NULL DEFAULT '0';
ALTER TABLE `npc_text` MODIFY `BroadcastTextID2` int(10) NOT NULL DEFAULT '0';
ALTER TABLE `npc_text` MODIFY `BroadcastTextID3` int(10) NOT NULL DEFAULT '0';
ALTER TABLE `npc_text` MODIFY `BroadcastTextID4` int(10) NOT NULL DEFAULT '0';
ALTER TABLE `npc_text` MODIFY `BroadcastTextID5` int(10) NOT NULL DEFAULT '0';
ALTER TABLE `npc_text` MODIFY `BroadcastTextID6` int(10) NOT NULL DEFAULT '0';
ALTER TABLE `npc_text` MODIFY `BroadcastTextID7` int(10) NOT NULL DEFAULT '0';
ALTER TABLE `npc_text_locale` MODIFY `ID` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `npc_vendor` MODIFY `entry` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `npc_vendor` MODIFY `item` int(10) NOT NULL DEFAULT '0';
ALTER TABLE `npc_vendor` MODIFY `maxcount` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `npc_vendor` MODIFY `ExtendedCost` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `page_text` MODIFY `ID` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `page_text` MODIFY `NextPageID` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `page_text_locale` MODIFY `ID` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `pet_levelstats` MODIFY `creature_entry` int(10) UNSIGNED NOT NULL;
ALTER TABLE `pet_name_generation` MODIFY `id` int(10) UNSIGNED NOT NULL AUTO_INCREMENT;
ALTER TABLE `pet_name_generation` MODIFY `entry` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `pickpocketing_loot_template` MODIFY `Entry` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `pickpocketing_loot_template` MODIFY `Item` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `pickpocketing_loot_template` MODIFY `Reference` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `playercreateinfo` MODIFY `zone` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `playercreateinfo_cast_spell` MODIFY `spell` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `playercreateinfo_item` MODIFY `itemid` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `playercreateinfo_spell_custom` MODIFY `Spell` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `points_of_interest` MODIFY `ID` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `points_of_interest` MODIFY `Icon` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `points_of_interest` MODIFY `Flags` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `points_of_interest` MODIFY `Importance` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `points_of_interest_locale` MODIFY `ID` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `pool_template` MODIFY `entry` int(10) UNSIGNED NOT NULL DEFAULT '0' COMMENT 'Pool entry';
ALTER TABLE `prospecting_loot_template` MODIFY `Entry` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `prospecting_loot_template` MODIFY `Item` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `prospecting_loot_template` MODIFY `Reference` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `quest_details` MODIFY `ID` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `quest_greeting` MODIFY `ID` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `quest_greeting_locale` MODIFY `ID` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `quest_offer_reward` MODIFY `ID` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `quest_pool_template` MODIFY `poolId` int(10) UNSIGNED NOT NULL;
ALTER TABLE `quest_request_items` MODIFY `ID` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `quest_template` MODIFY `ID` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `quest_template` MODIFY `RequiredFactionValue1` int(10) NOT NULL DEFAULT '0';
ALTER TABLE `quest_template` MODIFY `RequiredFactionValue2` int(10) NOT NULL DEFAULT '0';
ALTER TABLE `quest_template` MODIFY `RewardNextQuest` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `quest_template` MODIFY `RewardDisplaySpell` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `quest_template` MODIFY `StartItem` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `quest_template` MODIFY `QuestGiverPortrait` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `quest_template` MODIFY `QuestTurnInPortrait` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `quest_template` MODIFY `RewardItem1` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `quest_template` MODIFY `RewardItem2` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `quest_template` MODIFY `RewardItem3` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `quest_template` MODIFY `RewardItem4` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `quest_template` MODIFY `ItemDrop1` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `quest_template` MODIFY `ItemDrop2` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `quest_template` MODIFY `ItemDrop3` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `quest_template` MODIFY `ItemDrop4` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `quest_template` MODIFY `RewardChoiceItemID1` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `quest_template` MODIFY `RewardChoiceItemID2` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `quest_template` MODIFY `RewardChoiceItemID3` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `quest_template` MODIFY `RewardChoiceItemID4` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `quest_template` MODIFY `RewardChoiceItemID5` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `quest_template` MODIFY `RewardChoiceItemID6` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `quest_template` MODIFY `POIPriority` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `quest_template` MODIFY `RewardFactionValue1` int(10) NOT NULL DEFAULT '0';
ALTER TABLE `quest_template` MODIFY `RewardFactionOverride1` int(10) NOT NULL DEFAULT '0';
ALTER TABLE `quest_template` MODIFY `RewardFactionValue2` int(10) NOT NULL DEFAULT '0';
ALTER TABLE `quest_template` MODIFY `RewardFactionOverride2` int(10) NOT NULL DEFAULT '0';
ALTER TABLE `quest_template` MODIFY `RewardFactionValue3` int(10) NOT NULL DEFAULT '0';
ALTER TABLE `quest_template` MODIFY `RewardFactionOverride3` int(10) NOT NULL DEFAULT '0';
ALTER TABLE `quest_template` MODIFY `RewardFactionValue4` int(10) NOT NULL DEFAULT '0';
ALTER TABLE `quest_template` MODIFY `RewardFactionOverride4` int(10) NOT NULL DEFAULT '0';
ALTER TABLE `quest_template` MODIFY `RewardFactionValue5` int(10) NOT NULL DEFAULT '0';
ALTER TABLE `quest_template` MODIFY `RewardFactionOverride5` int(10) NOT NULL DEFAULT '0';
ALTER TABLE `quest_template` MODIFY `RequiredNpcOrGo1` int(10) NOT NULL DEFAULT '0';
ALTER TABLE `quest_template` MODIFY `RequiredNpcOrGo2` int(10) NOT NULL DEFAULT '0';
ALTER TABLE `quest_template` MODIFY `RequiredNpcOrGo3` int(10) NOT NULL DEFAULT '0';
ALTER TABLE `quest_template` MODIFY `RequiredNpcOrGo4` int(10) NOT NULL DEFAULT '0';
ALTER TABLE `quest_template` MODIFY `RequiredItemId1` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `quest_template` MODIFY `RequiredItemId2` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `quest_template` MODIFY `RequiredItemId3` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `quest_template` MODIFY `RequiredItemId4` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `quest_template` MODIFY `RequiredItemId5` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `quest_template` MODIFY `RequiredItemId6` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `quest_template` MODIFY `RequiredSpell` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `quest_template_addon` MODIFY `ID` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `quest_template_addon` MODIFY `SourceSpellID` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `quest_template_addon` MODIFY `PrevQuestID` int(10) NOT NULL DEFAULT '0';
ALTER TABLE `quest_template_addon` MODIFY `NextQuestID` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `quest_template_addon` MODIFY `ExclusiveGroup` int(10) NOT NULL DEFAULT '0';
ALTER TABLE `quest_template_addon` MODIFY `BreadcrumbForQuestId` int(10) NOT NULL DEFAULT '0';
ALTER TABLE `quest_template_addon` MODIFY `RewardMailTemplateID` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `quest_template_addon` MODIFY `RequiredMinRepValue` int(10) NOT NULL DEFAULT '0';
ALTER TABLE `quest_template_addon` MODIFY `RequiredMaxRepValue` int(10) NOT NULL DEFAULT '0';
ALTER TABLE `quest_template_addon` MODIFY `AllowableRaces` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `quest_template_locale` MODIFY `ID` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `reference_loot_template` MODIFY `Entry` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `reference_loot_template` MODIFY `Item` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `reference_loot_template` MODIFY `Reference` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `reputation_reward_rate` MODIFY `faction` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `script_waypoint` MODIFY `entry` int(10) UNSIGNED NOT NULL DEFAULT '0' COMMENT 'creature_template entry';
ALTER TABLE `script_waypoint` MODIFY `pointid` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `skill_discovery_template` MODIFY `spellId` int(10) UNSIGNED NOT NULL DEFAULT '0' COMMENT 'SpellId of the discoverable spell';
ALTER TABLE `skill_discovery_template` MODIFY `reqSpell` int(10) UNSIGNED NOT NULL DEFAULT '0' COMMENT 'spell requirement';
ALTER TABLE `skill_extra_item_template` MODIFY `spellId` int(10) UNSIGNED NOT NULL DEFAULT '0' COMMENT 'SpellId of the item creation spell';
ALTER TABLE `skill_extra_item_template` MODIFY `requiredSpecialization` int(10) UNSIGNED NOT NULL DEFAULT '0' COMMENT 'Specialization spell id';
ALTER TABLE `skill_fishing_base_level` MODIFY `entry` int(10) UNSIGNED NOT NULL DEFAULT '0' COMMENT 'Area identifier';
ALTER TABLE `skill_perfect_item_template` MODIFY `spellId` int(10) UNSIGNED NOT NULL DEFAULT '0' COMMENT 'SpellId of the item creation spell';
ALTER TABLE `skill_perfect_item_template` MODIFY `requiredSpecialization` int(10) UNSIGNED NOT NULL DEFAULT '0' COMMENT 'Specialization spell id';
ALTER TABLE `skill_perfect_item_template` MODIFY `perfectItemType` int(10) UNSIGNED NOT NULL DEFAULT '0' COMMENT 'perfect item type to create instead';
ALTER TABLE `skinning_loot_template` MODIFY `Entry` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `skinning_loot_template` MODIFY `Item` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `skinning_loot_template` MODIFY `Reference` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `spell_area` MODIFY `spell` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `spell_area` MODIFY `area` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `spell_area` MODIFY `quest_start` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `spell_area` MODIFY `quest_end` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `spell_area` MODIFY `aura_spell` int(10) NOT NULL DEFAULT '0';
ALTER TABLE `spell_area` MODIFY `racemask` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `spell_bonus_data` MODIFY `entry` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `spell_custom_attr` MODIFY `entry` int(10) UNSIGNED NOT NULL DEFAULT '0' COMMENT 'spell id';
ALTER TABLE `spell_custom_attr` MODIFY `attributes` int(10) UNSIGNED NOT NULL DEFAULT '0' COMMENT 'SpellCustomAttributes';
ALTER TABLE `spell_linked_spell` MODIFY `spell_trigger` int(10) NOT NULL;
ALTER TABLE `spell_linked_spell` MODIFY `spell_effect` int(10) NOT NULL DEFAULT '0';
ALTER TABLE `spell_loot_template` MODIFY `Entry` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `spell_loot_template` MODIFY `Item` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `spell_loot_template` MODIFY `Reference` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `spell_pet_auras` MODIFY `spell` int(10) UNSIGNED NOT NULL COMMENT 'dummy spell id';
ALTER TABLE `spell_pet_auras` MODIFY `pet` int(10) UNSIGNED NOT NULL DEFAULT '0' COMMENT 'pet id; 0 = all';
ALTER TABLE `spell_pet_auras` MODIFY `aura` int(10) UNSIGNED NOT NULL COMMENT 'pet aura id';
ALTER TABLE `spell_proc_event` MODIFY `entry` int(10) NOT NULL DEFAULT '0';
ALTER TABLE `spell_required` MODIFY `spell_id` int(10) NOT NULL DEFAULT '0';
ALTER TABLE `spell_required` MODIFY `req_spell` int(10) NOT NULL DEFAULT '0';
ALTER TABLE `spell_scripts` MODIFY `id` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `spell_scripts` MODIFY `command` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `spell_scripts` MODIFY `datalong` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `spell_target_position` MODIFY `ID` int(10) UNSIGNED NOT NULL DEFAULT '0' COMMENT 'Identifier';
ALTER TABLE `spell_threat` MODIFY `entry` int(10) UNSIGNED NOT NULL;
ALTER TABLE `trainer` MODIFY `Requirement` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `transports` MODIFY `entry` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `trinity_string` MODIFY `entry` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `vehicle_accessory` MODIFY `guid` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `vehicle_accessory` MODIFY `accessory_entry` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `vehicle_template_accessory` MODIFY `entry` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `vehicle_template_accessory` MODIFY `accessory_entry` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `waypoint_data` MODIFY `point` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `waypoints` MODIFY `entry` int(10) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `waypoints` MODIFY `pointid` int(10) UNSIGNED NOT NULL DEFAULT '0';
