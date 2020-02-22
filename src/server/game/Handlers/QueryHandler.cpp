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

#include "WorldSession.h"
#include "CharacterCache.h"
#include "Common.h"
#include "DatabaseEnv.h"
#include "DBCStores.h"
#include "GameTime.h"
#include "Log.h"
#include "MapManager.h"
#include "NPCHandler.h"
#include "ObjectAccessor.h"
#include "ObjectMgr.h"
#include "Player.h"
#include "QueryPackets.h"
#include "UpdateMask.h"
#include "World.h"
#include "WorldPacket.h"

void WorldSession::SendNameQueryOpcode(ObjectGuid guid)
{
    Player* player = ObjectAccessor::FindConnectedPlayer(guid);
    CharacterCacheEntry const* nameData = sCharacterCache->GetCharacterCacheByGuid(guid);

    WorldPacket data(SMSG_NAME_QUERY_RESPONSE, (8+1+1+1+1+1+10));
    data << guid.WriteAsPacked();
    if (!nameData)
    {
        data << uint8(1);                           // name unknown
        SendPacket(&data);
        return;
    }

    data << uint8(0);                               // name known
    data << nameData->Name;                         // played name
    data << uint8(0);                               // realm name - only set for cross realm interaction (such as Battlegrounds)
    data << uint8(nameData->Race);
    data << uint8(nameData->Sex);
    data << uint8(nameData->Class);

    if (DeclinedName const* names = (player ? player->GetDeclinedNames() : nullptr))
    {
        data << uint8(1);                           // Name is declined
        for (uint8 i = 0; i < MAX_DECLINED_NAME_CASES; ++i)
            data << names->name[i];
    }
    else
        data << uint8(0);                           // Name is not declined

    SendPacket(&data);
}

void WorldSession::HandleNameQueryOpcode(WorldPacket& recvData)
{
    ObjectGuid guid;
    recvData >> guid;

    // This is disable by default to prevent lots of console spam
    // TC_LOG_INFO("network", "HandleNameQueryOpcode %u", guid);

    SendNameQueryOpcode(guid);
}

void WorldSession::HandleQueryTimeOpcode(WorldPacket & /*recvData*/)
{
    SendQueryTimeResponse();
}

void WorldSession::SendQueryTimeResponse()
{
    WorldPacket data(SMSG_QUERY_TIME_RESPONSE, 4+4);
    data << uint32(GameTime::GetGameTime());
    data << uint32(sWorld->GetNextDailyQuestsResetTime() - GameTime::GetGameTime());
    SendPacket(&data);
}

/// Only _static_ data is sent in this packet !!!
void WorldSession::HandleCreatureQueryOpcode(WorldPacket& recvData)
{
    uint32 entry;
    recvData >> entry;
    ObjectGuid guid;
    recvData >> guid;

    CreatureTemplate const* creatureInfo = sObjectMgr->GetCreatureTemplate(entry);
    if (creatureInfo)
    {
        std::string Name, FemaleName, Title;
        Name = creatureInfo->Name;
        FemaleName = creatureInfo->FemaleName;
        Title = creatureInfo->Title;

        LocaleConstant locale = GetSessionDbLocaleIndex();
        if (locale != LOCALE_enUS)
        {
            if (CreatureLocale const* creatureLocale = sObjectMgr->GetCreatureLocale(entry))
            {
                ObjectMgr::GetLocaleString(creatureLocale->Name, locale, Name);
                ObjectMgr::GetLocaleString(creatureLocale->FemaleName, locale, FemaleName);
                ObjectMgr::GetLocaleString(creatureLocale->Title, locale, Title);
            }
        }

        TC_LOG_DEBUG("network", "WORLD: CMSG_CREATURE_QUERY '%s' - Entry: %u.", creatureInfo->Name.c_str(), entry);

        WorldPacket data(SMSG_CREATURE_QUERY_RESPONSE, 100);          // guess size
        data << uint32(entry);                                        // creature entry
        data << Name;                                                 // Name

        for (uint8 i = 0; i < 3; i++)
            data << uint8(0);                                         // name2, ..., name3

        data << FemaleName;                                           // FemaleName

        for (uint8 i = 0; i < 3; i++)
            data << uint8(0);                                         // name5, ..., name8

        data << Title;                                                // Title
        data << creatureInfo->IconName;                               // "Directions" for guard, string for Icons 2.3.0
        data << uint32(creatureInfo->type_flags);                     // flags
        data << uint32(creatureInfo->type_flags2);                    // unknown meaning
        data << uint32(creatureInfo->type);                           // CreatureType.dbc
        data << uint32(creatureInfo->family);                         // CreatureFamily.dbc
        data << uint32(creatureInfo->rank);                           // Creature Rank (elite, boss, etc)
        data << uint32(creatureInfo->KillCredit[0]);                  // new in 3.1, kill credit
        data << uint32(creatureInfo->KillCredit[1]);                  // new in 3.1, kill credit
        data << uint32(creatureInfo->Modelid1);                       // Modelid1
        data << uint32(creatureInfo->Modelid2);                       // Modelid2
        data << uint32(creatureInfo->Modelid3);                       // Modelid3
        data << uint32(creatureInfo->Modelid4);                       // Modelid4
        data << float(creatureInfo->ModHealth);                       // dmg/hp modifier
        data << float(creatureInfo->ModMana);                         // dmg/mana modifier
        data << uint8(creatureInfo->RacialLeader);                    // RacialLeader

        std::vector<uint32> const* items = sObjectMgr->GetCreatureQuestItemList(entry);
        if (items)
            for (uint32 i = 0; i < MAX_CREATURE_QUEST_ITEMS; ++i)
                data << (i < items->size() ? uint32((*items)[i]) : uint32(0));
        else
            for (uint32 i = 0; i < MAX_CREATURE_QUEST_ITEMS; ++i)
                data << uint32(0);

        data << uint32(creatureInfo->movementId);                     // CreatureMovementInfo.dbc
        data << uint32(creatureInfo->expansionUnknown);               // unknown meaning

        SendPacket(&data);

        TC_LOG_DEBUG("network", "WORLD: Sent SMSG_CREATURE_QUERY_RESPONSE");
    }
    else
    {
        TC_LOG_DEBUG("network", "WORLD: CMSG_CREATURE_QUERY - NO CREATURE INFO! (%s, ENTRY: %u)",
            guid.ToString().c_str(), entry);

        WorldPacket data(SMSG_CREATURE_QUERY_RESPONSE, 4);
        data << uint32(entry | 0x80000000);
        SendPacket(&data);

        TC_LOG_DEBUG("network", "WORLD: Sent SMSG_CREATURE_QUERY_RESPONSE");
    }
}

/// Only _static_ data is sent in this packet !!!
void WorldSession::HandleGameObjectQueryOpcode(WorldPacket& recvData)
{
    uint32 entry;
    recvData >> entry;
    ObjectGuid guid;
    recvData >> guid;

    GameObjectTemplate const* info = sObjectMgr->GetGameObjectTemplate(entry);
    if (info)
    {
        std::string Name;
        std::string IconName;
        std::string CastBarCaption;

        Name = info->name;
        IconName = info->IconName;
        CastBarCaption = info->castBarCaption;

        LocaleConstant localeConstant = GetSessionDbLocaleIndex();
        if (localeConstant != LOCALE_enUS)
        {
            if (GameObjectLocale const* gameObjectLocale = sObjectMgr->GetGameObjectLocale(entry))
            {
                ObjectMgr::GetLocaleString(gameObjectLocale->Name, localeConstant, Name);
                ObjectMgr::GetLocaleString(gameObjectLocale->CastBarCaption, localeConstant, CastBarCaption);
            }
        }

        TC_LOG_DEBUG("network", "WORLD: CMSG_GAMEOBJECT_QUERY '%s' - Entry: %u. ", info->name.c_str(), entry);
        WorldPacket data (SMSG_GAMEOBJECT_QUERY_RESPONSE, 150);
        data << uint32(entry);
        data << uint32(info->type);
        data << uint32(info->displayId);
        data << Name;
        data << uint8(0) << uint8(0) << uint8(0);           // name2, name3, name4
        data << IconName;                                   // 2.0.3, string. Icon name to use instead of default icon for go's (ex: "Attack" makes sword)
        data << CastBarCaption;                             // 2.0.3, string. Text will appear in Cast Bar when using GO (ex: "Collecting")
        data << info->unk1;                                 // 2.0.3, string
        data.append(info->raw.data, MAX_GAMEOBJECT_DATA);
        data << float(info->size);                          // go size
        data << int32(info->RequiredLevel);                 // 4.x, Required level

        std::vector<uint32> const* items = sObjectMgr->GetGameObjectQuestItemList(entry);
        if (items)
            for (size_t i = 0; i < MAX_GAMEOBJECT_QUEST_ITEMS; ++i)
                data << (i < items->size() ? uint32((*items)[i]) : uint32(0));
        else
            for (size_t i = 0; i < MAX_GAMEOBJECT_QUEST_ITEMS; ++i)
                data << uint32(0);

        SendPacket(&data);
        TC_LOG_DEBUG("network", "WORLD: Sent SMSG_GAMEOBJECT_QUERY_RESPONSE");
    }
    else
    {
        TC_LOG_DEBUG("network", "WORLD: CMSG_GAMEOBJECT_QUERY - Missing gameobject info for (%s, ENTRY: %u)",
            guid.ToString().c_str(), entry);
        WorldPacket data (SMSG_GAMEOBJECT_QUERY_RESPONSE, 4);
        data << uint32(entry | 0x80000000);
        SendPacket(&data);
        TC_LOG_DEBUG("network", "WORLD: Sent SMSG_GAMEOBJECT_QUERY_RESPONSE");
    }
}

void WorldSession::HandleCorpseQueryOpcode(WorldPacket& /*recvData*/)
{
    if (!_player->HasCorpse())
    {
        WorldPacket data(MSG_CORPSE_QUERY, 1);
        data << uint8(0);                                   // corpse not found
        SendPacket(&data);
        return;
    }

    WorldLocation corpseLocation = _player->GetCorpseLocation();
    uint32 corpseMapID = corpseLocation.GetMapId();
    uint32 mapID = corpseLocation.GetMapId();
    float x = corpseLocation.GetPositionX();
    float y = corpseLocation.GetPositionY();
    float z = corpseLocation.GetPositionZ();

    // if corpse at different map
    if (mapID != _player->GetMapId())
    {
        // search entrance map for proper show entrance
        if (MapEntry const* corpseMapEntry = sMapStore.LookupEntry(mapID))
        {
            if (corpseMapEntry->IsDungeon() && corpseMapEntry->CorpseMapID >= 0)
            {
                // if corpse map have entrance
                if (Map const* entranceMap = sMapMgr->CreateBaseMap(corpseMapEntry->CorpseMapID))
                {
                    mapID = corpseMapEntry->CorpseMapID;
                    x = corpseMapEntry->Corpse.X;
                    y = corpseMapEntry->Corpse.Y;
                    z = entranceMap->GetHeight(GetPlayer()->GetPhaseShift(), x, y, MAX_HEIGHT);
                }
            }
        }
    }

    WorldPacket data(MSG_CORPSE_QUERY, 1+(6*4));
    data << uint8(1);                                       // corpse found
    data << int32(mapID);
    data << float(x);
    data << float(y);
    data << float(z);
    data << int32(corpseMapID);
    data << uint32(0);                                      // unknown
    SendPacket(&data);
}

void WorldSession::HandleNpcTextQueryOpcode(WorldPacket& recvData)
{
    uint32 textID;
    uint64 guid;

    recvData >> textID;
    TC_LOG_DEBUG("network", "WORLD: CMSG_NPC_TEXT_QUERY TextId: %u", textID);

    recvData >> guid;

    GossipText const* gossip = sObjectMgr->GetGossipText(textID);

    WorldPacket data(SMSG_NPC_TEXT_UPDATE, 100);          // guess size
    data << textID;

    if (!gossip)
    {
        for (uint8 i = 0; i < MAX_GOSSIP_TEXT_OPTIONS; ++i)
        {
            data << float(0);
            data << "Greetings $N";
            data << "Greetings $N";
            data << uint32(0);
            data << uint32(0);
            data << uint32(0);
            data << uint32(0);
            data << uint32(0);
            data << uint32(0);
            data << uint32(0);
        }
    }
    else
    {
        std::string text0[MAX_GOSSIP_TEXT_OPTIONS], text1[MAX_GOSSIP_TEXT_OPTIONS];
        LocaleConstant locale = GetSessionDbLocaleIndex();

        for (uint8 i = 0; i < MAX_GOSSIP_TEXT_OPTIONS; ++i)
        {
            BroadcastText const* bct = sObjectMgr->GetBroadcastText(gossip->Options[i].BroadcastTextID);
            if (bct)
            {
                text0[i] = bct->GetText(locale, GENDER_MALE, true);
                text1[i] = bct->GetText(locale, GENDER_FEMALE, true);
            }
            else
            {
                text0[i] = gossip->Options[i].Text_0;
                text1[i] = gossip->Options[i].Text_1;
            }

            if (locale != DEFAULT_LOCALE && !bct)
            {
                if (NpcTextLocale const* npcTextLocale = sObjectMgr->GetNpcTextLocale(textID))
                {
                    ObjectMgr::GetLocaleString(npcTextLocale->Text_0[i], locale, text0[i]);
                    ObjectMgr::GetLocaleString(npcTextLocale->Text_1[i], locale, text1[i]);
                }
            }

            data << gossip->Options[i].Probability;

            if (text0[i].empty())
                data << text1[i];
            else
                data << text0[i];

            if (text1[i].empty())
                data << text0[i];
            else
                data << text1[i];

            data << gossip->Options[i].Language;

            for (uint8 j = 0; j < MAX_GOSSIP_TEXT_EMOTES; ++j)
            {
                data << gossip->Options[i].Emotes[j]._Delay;
                data << gossip->Options[i].Emotes[j]._Emote;
            }
        }
    }

    SendPacket(&data);

    TC_LOG_DEBUG("network", "WORLD: Sent SMSG_NPC_TEXT_UPDATE");
}

/// Only _static_ data is sent in this packet !!!
void WorldSession::HandlePageTextQueryOpcode(WorldPacket& recvData)
{
    TC_LOG_DEBUG("network", "WORLD: Received CMSG_PAGE_TEXT_QUERY");

    uint32 pageID;
    recvData >> pageID;
    recvData.read_skip<uint64>();                          // guid

    while (pageID)
    {
        PageText const* pageText = sObjectMgr->GetPageText(pageID);
                                                            // guess size
        WorldPacket data(SMSG_PAGE_TEXT_QUERY_RESPONSE, 50);
        data << pageID;

        if (!pageText)
        {
            data << "Item page missing.";
            data << uint32(0);
            pageID = 0;
        }
        else
        {
            std::string Text = pageText->Text;

            LocaleConstant localeConstant = GetSessionDbLocaleIndex();
            if (localeConstant != LOCALE_enUS)
                if (PageTextLocale const* pageTextLocale = sObjectMgr->GetPageTextLocale(pageID))
                    ObjectMgr::GetLocaleString(pageTextLocale->Text, localeConstant, Text);

            data << Text;
            data << uint32(pageText->NextPageID);
            pageID = pageText->NextPageID;
        }
        SendPacket(&data);

        TC_LOG_DEBUG("network", "WORLD: Sent SMSG_PAGE_TEXT_QUERY_RESPONSE");
    }
}

void WorldSession::HandleCorpseMapPositionQuery(WorldPacket& recvData)
{
    TC_LOG_DEBUG("network", "WORLD: Recv CMSG_CORPSE_MAP_POSITION_QUERY");

    uint32 transportGuidLow;
    recvData >> transportGuidLow;

    WorldPacket data(SMSG_CORPSE_MAP_POSITION_QUERY_RESPONSE, 4+4+4+4);
    data << float(0);
    data << float(0);
    data << float(0);
    data << float(0);
    SendPacket(&data);
}

void WorldSession::HandleQuestNPCQuery(WorldPacket& recvData)
{
    uint32 count = recvData.ReadBits(24);
    std::map<uint32, std::vector<uint32>> quests;

    for (uint32 i = 0; i < count; ++i)
    {
        uint32 questId;
        recvData >> questId;

        if (!sObjectMgr->GetQuestTemplate(questId))
        {
            TC_LOG_DEBUG("network", "WORLD: Unknown quest %u in CMSG_QUEST_NPC_QUERY by player %u", questId, m_GUIDLow);
            continue;
        }

        auto creatures = sObjectMgr->GetCreatureQuestInvolvedRelationReverseBounds(questId);
        for (auto it = creatures.first; it != creatures.second; ++it)
            quests[questId].push_back(it->second);

        auto gos = sObjectMgr->GetGOQuestInvolvedRelationReverseBounds(questId);
        for (auto it = gos.first; it != gos.second; ++it)
            quests[questId].push_back(it->second | 0x80000000); // GO mask
    }

    WorldPacket data(SMSG_QUEST_NPC_QUERY_RESPONSE, 3 + quests.size() * 14);
    data.WriteBits(quests.size(), 23);

    for (auto it = quests.begin(); it != quests.end(); ++it)
        data.WriteBits(it->second.size(), 24);

    for (auto it = quests.begin(); it != quests.end(); ++it)
    {
        data << uint32(it->first);
        for (const auto& entry : it->second)
            data << uint32(entry);
    }

    SendPacket(&data);
}

void WorldSession::HandleQuestPOIQuery(WorldPacket& recvData)
{
    uint32 count;
    recvData >> count; // quest count, max=25

    if (count > MAX_QUEST_LOG_SIZE)
    {
        recvData.rfinish();
        return;
    }

    // Read quest ids and add the in a unordered_set so we don't send POIs for the same quest multiple times
    std::unordered_set<uint32> questIds;
    for (uint32 i = 0; i < count; ++i)
        questIds.insert(recvData.read<uint32>()); // quest id

    WorldPacket data(SMSG_QUEST_POI_QUERY_RESPONSE, 4 + (4 + 4)*questIds.size());
    data << uint32(questIds.size()); // count

    for (auto itr = questIds.begin(); itr != questIds.end(); ++itr)
    {
        uint32 questId = *itr;

        bool questOk = false;

        uint16 questSlot = _player->FindQuestSlot(questId);

        if (questSlot != MAX_QUEST_LOG_SIZE)
            questOk =_player->GetQuestSlotQuestId(questSlot) == questId;

        if (questOk)
        {
            QuestPOIVector const* POI = sObjectMgr->GetQuestPOIVector(questId);

            if (POI)
            {
                data << uint32(questId); // quest ID
                data << uint32(POI->size()); // POI count

                for (QuestPOIVector::const_iterator itr = POI->begin(); itr != POI->end(); ++itr)
                {
                    data << uint32(itr->Id);                // POI index
                    data << int32(itr->ObjectiveIndex);     // objective index
                    data << uint32(itr->MapId);             // mapid
                    data << uint32(itr->AreaId);            // areaid
                    data << uint32(itr->FloorId);           // floorid
                    data << uint32(itr->Unk3);              // unknown
                    data << uint32(itr->Unk4);              // unknown
                    data << uint32(itr->points.size());     // POI points count

                    for (std::vector<QuestPOIPoint>::const_iterator itr2 = itr->points.begin(); itr2 != itr->points.end(); ++itr2)
                    {
                        data << int32(itr2->x); // POI point x
                        data << int32(itr2->y); // POI point y
                    }
                }
            }
            else
            {
                data << uint32(questId); // quest ID
                data << uint32(0); // POI count
            }
        }
        else
        {
            data << uint32(questId); // quest ID
            data << uint32(0); // POI count
        }
    }

    SendPacket(&data);
}

void WorldSession::HandleDBQueryBulk(WorldPackets::Query::DBQueryBulk& packet)
{
    DB2StorageBase const* store = sDB2Manager.GetStorage(packet.TableHash);
    if (!store)
    {
        TC_LOG_ERROR("network", "CMSG_DB_QUERY_BULK: Received unknown hotfix type: %u", packet.TableHash);
        return;
    }

    for (WorldPackets::Query::DBQueryRecord const& rec : packet.Queries)
    {
        WorldPackets::Query::DBReply response;
        response.TableHash = packet.TableHash;
        response.Timestamp = GameTime::GetGameTime() + (5 * IN_MILLISECONDS); // according to sniffs the server sends its local time + 5 seconds

        if (store->HasRecord(rec.RecordID))
        {
            response.RecordID = rec.RecordID;
            store->WriteRecord(rec.RecordID, GetSessionDbcLocale(), response.Data);
        }
        else
        {
            TC_LOG_TRACE("network", "CMSG_DB_QUERY_BULK: %s requested non-existing entry %u in datastore: %u", GetPlayerInfo().c_str(), rec.RecordID, packet.TableHash);
            response.RecordID = -int32(rec.RecordID);
        }

        SendPacket(response.Write());
    }
}
