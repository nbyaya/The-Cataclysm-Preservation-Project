/*
 * This file is part of the TrinityCore Project. See AUTHORS file for Copyright information
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "QuestDef.h"
#include "DBCStores.h"
#include "Field.h"
#include "Log.h"
#include "ObjectMgr.h"
#include "Player.h"
#include "QuestPackets.h"
#include "World.h"

Quest::Quest(Field* questRecord)
{
    EmoteOnIncomplete = 0;
    EmoteOnComplete = 0;
    _reqItemsCount = 0;
    _reqNpcOrGoCount = 0;
    _rewItemsCount = 0;
    _rewChoiceItemsCount = 0;
    _eventIdForQuest = 0;
    _rewCurrencyCount = 0;
    _reqCurrencyCount = 0;

    Id = questRecord[0].GetUInt32();
    Method = questRecord[1].GetUInt8();
    Level = questRecord[2].GetInt16();
    MinLevel = uint32(questRecord[3].GetInt16());
    ZoneOrSort = questRecord[4].GetInt16();
    Type = questRecord[5].GetUInt16();
    SuggestedPlayers = questRecord[6].GetUInt8();
    TimeAllowed = questRecord[7].GetUInt32();
    AllowableRaces = questRecord[8].GetUInt32();
    RequiredFactionId1 = questRecord[9].GetUInt16();
    RequiredFactionId2 = questRecord[10].GetUInt16();
    RequiredFactionValue1 = questRecord[11].GetInt32();
    RequiredFactionValue2 = questRecord[12].GetInt32();
    RewardNextQuest = questRecord[13].GetUInt32();
    RewardXPDifficulty = questRecord[14].GetUInt8();
    RewardMoney = questRecord[15].GetInt32();
    RewardBonusMoney = questRecord[16].GetUInt32();
    RewardDisplaySpell = questRecord[17].GetUInt32();
    RewardSpell = questRecord[18].GetInt32();
    RewardHonor = questRecord[19].GetUInt32();
    RewardKillHonor = questRecord[20].GetFloat();
    StartItem = questRecord[21].GetUInt32();
    Flags = questRecord[22].GetUInt32();
    MinimapTargetMark = questRecord[23].GetUInt8();
    RewardTitleId = questRecord[24].GetUInt8();
    RequiredPlayerKills = questRecord[25].GetUInt8();
    RewardTalents = questRecord[26].GetUInt8();
    RewardArenaPoints = questRecord[27].GetUInt16();
    RewardSkillId = questRecord[28].GetUInt16();
    RewardSkillPoints = questRecord[29].GetUInt8();
    RewardReputationMask = questRecord[30].GetUInt8();
    QuestGiverPortrait = questRecord[31].GetUInt32();
    QuestTurnInPortrait = questRecord[32].GetUInt32();

    for (int i = 0; i < QUEST_REWARDS_COUNT; ++i)
    {
        RewardItemId[i] = questRecord[33 + i * 2].GetUInt32();
        RewardItemIdCount[i] = questRecord[34 + i * 2].GetUInt16();

        if (RewardItemId[i])
            ++_rewItemsCount;
    }

    for (int i = 0; i < QUEST_REWARD_CHOICES_COUNT; ++i)
    {
        RewardChoiceItemId[i] = questRecord[41 + i * 2].GetUInt32();
        RewardChoiceItemCount[i] = questRecord[42 + i * 2].GetUInt16();

        if (RewardChoiceItemId[i])
            ++_rewChoiceItemsCount;
    }

    for (int i = 0; i < QUEST_REPUTATIONS_COUNT; ++i)
    {
        RewardFactionId[i] = questRecord[53 + i * 3].GetUInt16();
        RewardFactionValueId[i] = questRecord[54 + i * 3].GetInt32();
        RewardFactionValueIdOverride[i] = questRecord[55 + i * 3].GetInt32();
    }

    POIContinent = questRecord[68].GetUInt16();
    POIx = questRecord[69].GetFloat();
    POIy = questRecord[70].GetFloat();
    POIPriority = questRecord[71].GetUInt32();
    Title = questRecord[72].GetString();
    Objectives = questRecord[73].GetString();
    Details = questRecord[74].GetString();
    AreaDescription = questRecord[75].GetString();
    CompletedText = questRecord[76].GetString();

    for (int i = 0; i < QUEST_OBJECTIVES_COUNT; ++i)
    {
        RequiredNpcOrGo[i] = questRecord[77 + i].GetInt32();
        RequiredNpcOrGoCount[i] = questRecord[81 + i].GetUInt16();
        ObjectiveText[i] = questRecord[106 + i].GetString();

        if (RequiredNpcOrGo[i])
            ++_reqNpcOrGoCount;
    }

    for (int i = 0; i < QUEST_SOURCE_ITEM_IDS_COUNT; ++i)
    {
        ItemDrop[i] = questRecord[85 + i].GetUInt32();
        ItemDropQuantity[i] = questRecord[89 + i].GetUInt16();
    }

    for (int i = 0; i < QUEST_ITEM_OBJECTIVES_COUNT; ++i)
    {
        RequiredItemId[i] = questRecord[93 + i].GetUInt32();
        RequiredItemCount[i] = questRecord[99 + i].GetUInt16();

        if (RequiredItemId[i])
            ++_reqItemsCount;
    }

    //int32 VerifiedBuild = questRecord[132].GetInt32();

    for (int i = 0; i < QUEST_EMOTE_COUNT; ++i)
    {
        DetailsEmote[i] = 0;
        DetailsEmoteDelay[i] = 0;
        OfferRewardEmote[i] = 0;
        OfferRewardEmoteDelay[i] = 0;
    }

    RequiredSpell = questRecord[105].GetUInt32();

    for (int i = 0; i < QUEST_REWARD_CURRENCY_COUNT; ++i)
    {
        RewardCurrencyId[i] = questRecord[110 + i].GetUInt16();
        RewardCurrencyCount[i] = questRecord[114 + i].GetUInt32();

        if (RewardCurrencyId[i])
            ++_rewCurrencyCount;
    }

    for (int i = 0; i < QUEST_REQUIRED_CURRENCY_COUNT; ++i)
    {
        RequiredCurrencyId[i] = questRecord[118 + i].GetUInt16();
        RequiredCurrencyCount[i] = questRecord[122 + i].GetUInt32();

        if (RequiredCurrencyId[i])
            ++_reqCurrencyCount;
    }

    QuestGiverTextWindow = questRecord[126].GetString();
    QuestGiverTargetName = questRecord[127].GetString();
    QuestTurnTextWindow = questRecord[128].GetString();
    QuestTurnTargetName = questRecord[129].GetString();
    SoundAccept = questRecord[130].GetUInt16();
    SoundTurnIn = questRecord[131].GetUInt16();

    StartsAtAreaTrigger = false;
}

void Quest::LoadQuestDetails(Field* fields)
{
    for (int i = 0; i < QUEST_EMOTE_COUNT; ++i)
   {
        if (!sEmotesStore.LookupEntry(fields[1+i].GetUInt16()))
        {
            TC_LOG_ERROR("sql.sql", "Table `quest_details` has non-existing Emote%i (%u) set for quest %u. Skipped.", 1+i, fields[1+i].GetUInt16(), fields[0].GetUInt32());
            continue;
        }

        DetailsEmote[i] = fields[1 + i].GetUInt16();
    }

    for (int i = 0; i < QUEST_EMOTE_COUNT; ++i)
        DetailsEmoteDelay[i] = fields[5 + i].GetUInt32();
}

void Quest::LoadQuestRequestItems(Field* fields)
{
    EmoteOnComplete = fields[1].GetUInt16();
    EmoteOnIncomplete = fields[2].GetUInt16();

    if (!sEmotesStore.LookupEntry(EmoteOnComplete))
        TC_LOG_ERROR("sql.sql", "Table `quest_request_items` has non-existing EmoteOnComplete (%u) set for quest %u.", EmoteOnComplete, fields[0].GetUInt32());

    if (!sEmotesStore.LookupEntry(EmoteOnIncomplete))
        TC_LOG_ERROR("sql.sql", "Table `quest_request_items` has non-existing EmoteOnIncomplete (%u) set for quest %u.", EmoteOnIncomplete, fields[0].GetUInt32());

    RequestItemsText = fields[3].GetString();
}

void Quest::LoadQuestOfferReward(Field* fields)
{
    for (int i = 0; i < QUEST_EMOTE_COUNT; ++i)
   {
        if (!sEmotesStore.LookupEntry(fields[1+i].GetUInt16()))
        {
            TC_LOG_ERROR("sql.sql", "Table `quest_offer_reward` has non-existing Emote%i (%u) set for quest %u. Skipped.", 1+i, fields[1+i].GetUInt16(), fields[0].GetUInt32());
            continue;
        }

        OfferRewardEmote[i] = fields[1 + i].GetUInt16();
    }

    for (int i = 0; i < QUEST_EMOTE_COUNT; ++i)
        OfferRewardEmoteDelay[i] = fields[5 + i].GetUInt32();

    OfferRewardText = fields[9].GetString();
}

void Quest::LoadQuestTemplateAddon(Field* fields)
{
    MaxLevel = fields[1].GetUInt8();
    AllowableClasses = fields[2].GetUInt32();
    SourceSpellid = fields[3].GetUInt32();
    PrevQuestId = fields[4].GetInt32();
    NextQuestId = fields[5].GetInt32();
    ExclusiveGroup = fields[6].GetInt32();
    RewardMailTemplateId = fields[7].GetUInt32();
    RewardMailDelay = fields[8].GetUInt32();
    RequiredSkillId = fields[9].GetUInt16();
    RequiredSkillPoints = fields[10].GetUInt16();
    RequiredMinRepFaction = fields[11].GetUInt16();
    RequiredMaxRepFaction = fields[12].GetUInt16();
    RequiredMinRepValue = fields[13].GetInt32();
    RequiredMaxRepValue = fields[14].GetInt32();
    StartItemCount = fields[15].GetUInt8();
    RewardMailSenderEntry = fields[16].GetUInt32();
    SpecialFlags = fields[17].GetUInt8();

    if (SpecialFlags & QUEST_SPECIAL_FLAGS_AUTO_ACCEPT)
        Flags |= QUEST_FLAGS_AUTO_ACCEPT;
}

uint32 Quest::GetXPReward(Player const* player) const
{
    if (player)
    {
        int32 quest_level = (Level == -1 ? player->getLevel() : Level);
        const QuestXPEntry* xpentry = sQuestXPStore.LookupEntry(quest_level);
        if (!xpentry)
            return 0;

        int32 diffFactor = 2 * (quest_level - player->getLevel()) + 20;
        if (diffFactor < 1)
            diffFactor = 1;
        else if (diffFactor > 10)
            diffFactor = 10;

        uint32 xp = RoundXPValue(diffFactor * xpentry->Difficulty[RewardXPDifficulty] / 10);
        if (sWorld->getIntConfig(CONFIG_MIN_QUEST_SCALED_XP_RATIO))
        {
            uint32 minScaledXP = RoundXPValue(xpentry->Difficulty[RewardXPDifficulty]) * sWorld->getIntConfig(CONFIG_MIN_QUEST_SCALED_XP_RATIO) / 100;
            xp = std::max(minScaledXP, xp);
        }

        return xp;
    }

    return 0;
}

int32 Quest::GetRewOrReqMoney(Player const* player) const
{
    // RequiredMoney: the amount is the negative copper sum.
    if (RewardMoney < 0)
        return RewardMoney;

    // RewardMoney: the positive amount
    if (!player || player->getLevel() < sWorld->getIntConfig(CONFIG_MAX_PLAYER_LEVEL))
        return int32(RewardMoney * sWorld->getRate(RATE_MONEY_QUEST));
    else // At level cap, the money reward is the maximum amount between normal and bonus money reward
        return std::max(int32(GetRewMoneyMaxLevel()), int32(RewardMoney * sWorld->getRate(RATE_MONEY_QUEST)));
}

void Quest::BuildQuestRewards(WorldPackets::Quest::QuestRewards& rewards, Player* player) const
{
    rewards.ChoiceItemCount = GetRewChoiceItemsCount();
    rewards.ItemCount = GetReqItemsCount();
    rewards.Money = GetRewOrReqMoney(player);
    rewards.XP = GetXPReward(player) * sWorld->getRate(RATE_XP_QUEST);
    rewards.Title = GetCharTitleId();
    rewards.FactionFlags = GetRewardReputationMask();
    rewards.SpellCompletionDisplayID = GetRewSpell();
    rewards.SpellCompletionID = GetRewSpellCast();
    rewards.SkillLineID = GetRewardSkillId();
    rewards.NumSkillUps = GetRewardSkillPoints();
    rewards.NumBonusTalents = GetBonusTalents();

    for (uint32 i = 0; i < QUEST_REWARD_CHOICES_COUNT; ++i)
    {
        rewards.ChoiceItems[i].ItemID = RewardChoiceItemId[i];
        rewards.ChoiceItems[i].Quantity = RewardChoiceItemCount[i];
        if (ItemTemplate const* itemTemplate = sObjectMgr->GetItemTemplate(RewardChoiceItemId[i]))
            rewards.ChoiceItems[i].DisplayID = itemTemplate->DisplayInfoID;
    }

    for (uint32 i = 0; i < QUEST_REWARDS_COUNT; ++i)
    {
        rewards.ItemID[i] = RewardItemId[i];
        rewards.ItemQty[i] = RewardItemIdCount[i];
        if (ItemTemplate const* itemTemplate = sObjectMgr->GetItemTemplate(RewardItemId[i]))
            rewards.ItemDisplayID[i] = itemTemplate->DisplayInfoID;
    }

    for (uint32 i = 0; i < QUEST_REPUTATIONS_COUNT; ++i)
    {
        rewards.FactionID[i] = RewardFactionId[i];
        rewards.FactionValue[i] = RewardFactionValueId[i];
        rewards.FactionOverride[i] = RewardFactionValueIdOverride[i];
    }

    for (uint32 i = 0; i < QUEST_REWARD_CURRENCY_COUNT; ++i)
    {
        rewards.CurrencyID[i] = RewardCurrencyId[i];
        rewards.CurrencyQty[i] = RewardCurrencyCount[i];
    }
}

uint32 Quest::GetRewMoneyMaxLevel() const
{
    // If Quest has flag to not give money on max level, it's 0
    if (HasFlag(QUEST_FLAGS_NO_MONEY_FROM_XP))
        return 0;

    // Else, return the rewarded copper sum modified by the rate
    return uint32(RewardBonusMoney * sWorld->getRate(RATE_MONEY_MAX_LEVEL_QUEST));
}

bool Quest::IsAutoAccept() const
{
    return !sWorld->getBoolConfig(CONFIG_QUEST_IGNORE_AUTO_ACCEPT) && HasFlag(QUEST_FLAGS_AUTO_ACCEPT);
}

bool Quest::IsAutoComplete() const
{
    return !sWorld->getBoolConfig(CONFIG_QUEST_IGNORE_AUTO_COMPLETE) && Method == 0;
}

bool Quest::IsRaidQuest(Difficulty difficulty) const
{
    switch (Type)
    {
        case QUEST_TYPE_RAID:
            return true;
        case QUEST_TYPE_RAID_10:
            return !(difficulty & RAID_DIFFICULTY_MASK_25MAN);
        case QUEST_TYPE_RAID_25:
            return difficulty & RAID_DIFFICULTY_MASK_25MAN;
        default:
            break;
    }

    if ((Flags & QUEST_FLAGS_RAID) != 0)
        return true;

    return false;
}

bool Quest::IsAllowedInRaid(Difficulty difficulty) const
{
    if (IsRaidQuest(difficulty))
        return true;

    return sWorld->getBoolConfig(CONFIG_QUEST_IGNORE_RAID);
}

uint32 Quest::CalculateHonorGain(uint8 level) const
{
    if (level > GT_MAX_LEVEL)
        level = GT_MAX_LEVEL;

    uint32 honor = 0;

    /*if (GetRewHonorAddition() > 0 || GetRewHonorMultiplier() > 0.0f)
    {
        // values stored from 0.. for 1...
        TeamContributionPointsEntry const* tc = sTeamContributionPointsStore.LookupEntry(level);
        if (!tc)
            return 0;

        honor = uint32(tc->value * GetRewHonorMultiplier() * 0.1f);
        honor += GetRewHonorAddition();
    }*/

    return honor;
}

bool Quest::CanIncreaseRewardedQuestCounters() const
{
    // Dungeon Finder/Daily/Repeatable (if not weekly, monthly or seasonal) quests are never considered rewarded serverside.
    // This affects counters and client requests for completed quests.
    return (!IsDFQuest() && !IsDaily() && (!IsRepeatable() || IsWeekly() || IsMonthly() || IsSeasonal()));
}

uint32 Quest::RoundXPValue(uint32 xp)
{
    if (xp <= 100)
        return 5 * ((xp + 2) / 5);
    else if (xp <= 500)
        return 10 * ((xp + 5) / 10);
    else if (xp <= 1000)
        return 25 * ((xp + 12) / 25);
    else
        return 50 * ((xp + 25) / 50);
}
