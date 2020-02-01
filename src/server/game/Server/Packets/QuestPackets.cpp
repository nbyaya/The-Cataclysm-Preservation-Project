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

#include "QuestPackets.h"
#include "Errors.h"

WorldPacket const* WorldPackets::Quest::QuestGiverRequestItems::Write()
{
    _worldPacket << QuestGiverGUID;
    _worldPacket << uint32(QuestID);
    _worldPacket << QuestTitle;
    _worldPacket << CompletionText;
    _worldPacket << uint32(CompEmoteDelay);
    _worldPacket << uint32(CompEmoteType);
    _worldPacket << uint32(AutoLaunched);
    _worldPacket << uint32(QuestFlags);
    _worldPacket << uint32(SuggestPartyMembers);
    _worldPacket << uint32(MoneyToGet);

    _worldPacket << uint32(Collect.size());
    for (QuestObjectiveCollect const& obj : Collect)
    {
        _worldPacket << uint32(obj.ObjectID);
        _worldPacket << uint32(obj.Amount);
        _worldPacket << uint32(obj.DisplayID);
    }

    _worldPacket << uint32(Currency.size());
    for (QuestCurrency const& cur : Currency)
    {
        _worldPacket << uint32(cur.CurrencyID);
        _worldPacket << uint32(cur.Amount);
    }

    for (uint8 i = 0; i < 5; i++)
        _worldPacket << uint32(StatusFlags[i]);

    return &_worldPacket;
}

void WorldPackets::Quest::QuestGiverAcceptQuest::Read()
{
    _worldPacket >> QuestGiverGUID;
    _worldPacket >> QuestID;
    _worldPacket >> StartCheat;
}

void WorldPackets::Quest::QuestGiverChooseReward::Read()
{
    _worldPacket >> QuestGiverGUID;
    _worldPacket >> QuestID;
    _worldPacket >> ItemChoiceID;
}

void WorldPackets::Quest::QuestGiverCompleteQuest::Read()
{
    _worldPacket >> QuestGiverGUID;
    _worldPacket >> QuestID;
    _worldPacket >> FromScript;
}

void WorldPackets::Quest::QuestGiverHello::Read()
{
    _worldPacket >> QuestGiverGUID;
}

void WorldPackets::Quest::QuestGiverQueryQuest::Read()
{
    _worldPacket >> QuestGiverGUID;
    _worldPacket >> QuestID;
    _worldPacket >> RespondToGiver;
}

void WorldPackets::Quest::QuestGiverRequestReward::Read()
{
    _worldPacket >> QuestGiverGUID;
    _worldPacket >> QuestID;
}

void WorldPackets::Quest::QuestGiverStatusQuery::Read()
{
    _worldPacket >> QuestGiverGUID;
}

void WorldPackets::Quest::QuestLogRemoveQuest::Read()
{
    _worldPacket >> Entry;
}

void WorldPackets::Quest::QuestConfirmAccept::Read()
{
    _worldPacket >> QuestID;
}

void WorldPackets::Quest::QueryQuestInfo::Read()
{
    _worldPacket >> QuestID;
}

WorldPacket const* WorldPackets::Quest::QuestGiverQuestListMessage::Write()
{
    _worldPacket << QuestGiverGUID;
    _worldPacket << Greeting;
    _worldPacket << uint32(GreetEmoteDelay);
    _worldPacket << uint32(GreetEmoteType);

    _worldPacket << uint8(QuestDataText.size());
    for (GossipText const& gossip : QuestDataText)
    {
        _worldPacket << uint32(gossip.QuestID);
        _worldPacket << uint32(gossip.QuestType);
        _worldPacket << int32(gossip.QuestLevel);
        _worldPacket << uint32(gossip.QuestFlags);
        _worldPacket << uint8(gossip.Repeatable);
        _worldPacket << gossip.QuestTitle;
    }

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Quest::QuestGiverStatus::Write()
{
    _worldPacket << QuestGiver.Guid;
    _worldPacket << uint32(QuestGiver.Status);

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Quest::QuestGiverQuestDetails::Write()
{
    _worldPacket << QuestGiverGUID;
    _worldPacket << InformUnit;
    _worldPacket << uint32(QuestID);
    _worldPacket << QuestTitle;
    _worldPacket << LogDescription;
    _worldPacket << Objectives;
    _worldPacket << PortraitGiverText;
    _worldPacket << PortraitGiverName;
    _worldPacket << PortraitTurnInText;
    _worldPacket << PortraitTurnInName;
    _worldPacket << uint32(PortraitGiver);
    _worldPacket << uint32(PortraitTurnIn);
    _worldPacket << uint8(AutoLaunched);
    _worldPacket << uint32(QuestFlags);
    _worldPacket << uint32(SuggestedPartyMembers);
    _worldPacket << uint8(StartCheat); // to-do: validate
    _worldPacket << uint8(DisplayPopup);
    _worldPacket << uint32(RequiredSpellID);

    _worldPacket << Rewards;

    _worldPacket << uint32(DescEmotes.size());
    for (WorldPackets::Quest::QuestDescEmote const& emote : DescEmotes)
    {
        _worldPacket << uint32(emote.Type);
        _worldPacket << uint32(emote.Delay);
    }

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Quest::QuestGiverQuestComplete::Write()
{
    _worldPacket << uint32(TalentReward);
    _worldPacket << uint32(NumSkillUpsReward);
    _worldPacket << uint32(MoneyReward);
    _worldPacket << uint32(XPReward);
    _worldPacket << uint32(QuestID);
    _worldPacket << uint32(SkillLineIDReward);

    _worldPacket.WriteBit(LaunchGossip);
    _worldPacket.WriteBit(UseQuestReward);
    _worldPacket.FlushBits();

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Quest::QuestConfirmAcceptResponse::Write()
{
    _worldPacket << uint32(QuestID);
    _worldPacket << QuestTitle;
    _worldPacket << InitiatedBy;
    return &_worldPacket;
}

WorldPacket const* WorldPackets::Quest::QuestUpdateAddCredit::Write()
{
    _worldPacket << uint32(QuestID);
    _worldPacket << uint32(ObjectID);
    _worldPacket << uint32(Count);
    _worldPacket << uint32(Required);
    _worldPacket << VictimGUID;
    return &_worldPacket;
}

WorldPacket const* WorldPackets::Quest::QuestUpdateAddPvPCredit::Write()
{
    _worldPacket << uint32(QuestID);
    _worldPacket << uint32(Count);
    _worldPacket << uint32(Required);
    return &_worldPacket;
}

ByteBuffer& operator<<(ByteBuffer& data, WorldPackets::Quest::QuestRewards const& questRewards)
{
    data << uint32(questRewards.ChoiceItemCount);
    for (uint32 i = 0; i < QUEST_REWARD_CHOICES_COUNT; ++i)
        data << uint32(questRewards.ChoiceItems[i].ItemID);
    for (uint32 i = 0; i < QUEST_REWARD_CHOICES_COUNT; ++i)
        data << uint32(questRewards.ChoiceItems[i].Quantity);
    for (uint32 i = 0; i < QUEST_REWARD_CHOICES_COUNT; ++i)
        data << uint32(questRewards.ChoiceItems[i].DisplayID);

    data << uint32(questRewards.ItemCount);
    for (uint32 i = 0; i < QUEST_REWARDS_COUNT; ++i)
        data << uint32(questRewards.ItemID[i]);
    for (uint32 i = 0; i < QUEST_REWARDS_COUNT; ++i)
        data << uint32(questRewards.ItemQty[i]);
    for (uint32 i = 0; i < QUEST_REWARDS_COUNT; ++i)
        data << uint32(questRewards.ItemDisplayID[i]);

    data << uint32(questRewards.Money);
    data << uint32(questRewards.XP);
    data << uint32(questRewards.Title);
    data << uint32(0); // unkown
    data << float(0.0f); // unknown
    data << uint32(questRewards.NumBonusTalents);
    data << uint32(0); // unkown
    data << uint32(questRewards.FactionFlags);


    for (uint32 i = 0; i < QUEST_REPUTATIONS_COUNT; ++i)
        data << uint32(questRewards.FactionID[i]);

    for (uint32 i = 0; i < QUEST_REPUTATIONS_COUNT; ++i)
        data << int32(questRewards.FactionValue[i]);

    for (uint32 i = 0; i < QUEST_REPUTATIONS_COUNT; ++i)
        data << uint32(questRewards.FactionOverride[i]);

    data << uint32(questRewards.SpellCompletionDisplayID);
    data << uint32(questRewards.SpellCompletionID);

    for (uint32 i = 0; i < QUEST_REWARD_CURRENCY_COUNT; ++i)
        data << uint32(questRewards.CurrencyID[i]);

    for (uint32 i = 0; i < QUEST_REWARD_CURRENCY_COUNT; ++i)
        data << uint32(questRewards.CurrencyQty[i]);

    data << uint32(questRewards.SkillLineID);
    data << uint32(questRewards.NumSkillUps);

    return data;
}
