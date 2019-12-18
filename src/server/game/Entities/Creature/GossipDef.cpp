/*
 * Copyright (C) 2008-2018 TrinityCore <https://www.trinitycore.org/>
 * Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
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

#include "GossipDef.h"
#include "Formulas.h"
#include "ObjectMgr.h"
#include "QuestDef.h"
#include "QuestPackets.h"
#include "WorldSession.h"

GossipMenu::GossipMenu()
{
    _menuId = 0;
    _locale = DEFAULT_LOCALE;
    _senderGUID.Clear();
}

GossipMenu::~GossipMenu()
{
    ClearMenu();
}

void GossipMenu::AddMenuItem(int32 menuItemId, uint8 icon, std::string const& message, uint32 sender, uint32 action, std::string const& boxMessage, uint32 boxMoney, bool coded /*= false*/)
{
    ASSERT(_menuItems.size() <= GOSSIP_MAX_MENU_ITEMS);

    // Find a free new id - script case
    if (menuItemId == -1)
    {
        menuItemId = 0;
        if (!_menuItems.empty())
        {
            for (GossipMenuItemContainer::const_iterator itr = _menuItems.begin(); itr != _menuItems.end(); ++itr)
            {
                if (int32(itr->first) > menuItemId)
                    break;

                menuItemId = itr->first + 1;
            }
        }
    }

    GossipMenuItem& menuItem = _menuItems[menuItemId];

    menuItem.MenuItemIcon    = icon;
    menuItem.Message         = message;
    menuItem.IsCoded         = coded;
    menuItem.Sender          = sender;
    menuItem.OptionType      = action;
    menuItem.BoxMessage      = boxMessage;
    menuItem.BoxMoney        = boxMoney;
}

/**
 * @name AddMenuItem
 * @brief Adds a localized gossip menu item from db by menu id and menu item id.
 * @param menuId Gossip menu id.
 * @param menuItemId Gossip menu item id.
 * @param sender Identifier of the current menu.
 * @param action Custom action given to OnGossipHello.
 */
void GossipMenu::AddMenuItem(uint32 menuId, uint32 menuItemId, uint32 sender, uint32 action)
{
    /// Find items for given menu id.
    GossipMenuItemsMapBounds bounds = sObjectMgr->GetGossipMenuItemsMapBounds(menuId);
    /// Return if there are none.
    if (bounds.first == bounds.second)
        return;

    /// Iterate over each of them.
    for (GossipMenuItemsContainer::const_iterator itr = bounds.first; itr != bounds.second; ++itr)
    {
        /// Find the one with the given menu item id.
        if (itr->second.OptionID != menuItemId)
            continue;

        /// Store texts for localization.
        std::string strOptionText, strBoxText;
        BroadcastText const* optionBroadcastText = sObjectMgr->GetBroadcastText(itr->second.OptionBroadcastTextID);
        BroadcastText const* boxBroadcastText = sObjectMgr->GetBroadcastText(itr->second.BoxBroadcastTextID);

        /// OptionText
        if (optionBroadcastText)
            strOptionText = optionBroadcastText->GetText(GetLocale());
        else
            strOptionText = itr->second.OptionText;

        /// BoxText
        if (boxBroadcastText)
            strBoxText = boxBroadcastText->GetText(GetLocale());
        else
            strBoxText = itr->second.BoxText;

        /// Check need of localization.
        if (GetLocale() != DEFAULT_LOCALE)
        {
            if (!optionBroadcastText)
            {
                /// Find localizations from database.
                if (GossipMenuItemsLocale const* gossipMenuLocale = sObjectMgr->GetGossipMenuItemsLocale(MAKE_PAIR32(menuId, menuItemId)))
                    ObjectMgr::GetLocaleString(gossipMenuLocale->OptionText, GetLocale(), strOptionText);
            }

            if (!boxBroadcastText)
            {
                /// Find localizations from database.
                if (GossipMenuItemsLocale const* gossipMenuLocale = sObjectMgr->GetGossipMenuItemsLocale(MAKE_PAIR32(menuId, menuItemId)))
                    ObjectMgr::GetLocaleString(gossipMenuLocale->BoxText, GetLocale(), strBoxText);
            }
        }

        /// Add menu item with existing method. Menu item id -1 is also used in ADD_GOSSIP_ITEM macro.
        AddMenuItem(-1, itr->second.OptionIcon, strOptionText, sender, action, strBoxText, itr->second.BoxMoney, itr->second.BoxCoded);
    }
}

void GossipMenu::AddGossipMenuItemData(uint32 menuItemId, uint32 gossipActionMenuId, uint32 gossipActionPoi)
{
    GossipMenuItemData& itemData = _menuItemData[menuItemId];

    itemData.GossipActionMenuId  = gossipActionMenuId;
    itemData.GossipActionPoi     = gossipActionPoi;
}

uint32 GossipMenu::GetMenuItemSender(uint32 menuItemId) const
{
    GossipMenuItemContainer::const_iterator itr = _menuItems.find(menuItemId);
    if (itr == _menuItems.end())
        return 0;

    return itr->second.Sender;
}

uint32 GossipMenu::GetMenuItemAction(uint32 menuItemId) const
{
    GossipMenuItemContainer::const_iterator itr = _menuItems.find(menuItemId);
    if (itr == _menuItems.end())
        return 0;

    return itr->second.OptionType;
}

bool GossipMenu::IsMenuItemCoded(uint32 menuItemId) const
{
    GossipMenuItemContainer::const_iterator itr = _menuItems.find(menuItemId);
    if (itr == _menuItems.end())
        return false;

    return itr->second.IsCoded;
}

void GossipMenu::ClearMenu()
{
    _menuItems.clear();
    _menuItemData.clear();
}

PlayerMenu::PlayerMenu(WorldSession* session) : _session(session)
{
    if (_session)
        _gossipMenu.SetLocale(_session->GetSessionDbLocaleIndex());
}

PlayerMenu::~PlayerMenu()
{
    ClearMenus();
}

void PlayerMenu::ClearMenus()
{
    _gossipMenu.ClearMenu();
    _questMenu.ClearMenu();
}

void PlayerMenu::SendGossipMenu(uint32 titleTextId, ObjectGuid objectGUID)
{
    _gossipMenu.SetSenderGUID(objectGUID);

    WorldPacket data(SMSG_GOSSIP_MESSAGE, 100);         // guess size
    data << uint64(objectGUID);
    data << uint32(_gossipMenu.GetMenuId());            // new 2.4.0
    data << uint32(titleTextId);
    data << uint32(_gossipMenu.GetMenuItemCount());     // max count 0x10

    for (GossipMenuItemContainer::const_iterator itr = _gossipMenu.GetMenuItems().begin(); itr != _gossipMenu.GetMenuItems().end(); ++itr)
    {
        GossipMenuItem const& item = itr->second;
        data << uint32(itr->first);
        data << uint8(item.MenuItemIcon);
        data << uint8(item.IsCoded);                    // makes pop up box password
        data << uint32(item.BoxMoney);                  // money required to open menu, 2.0.3
        data << item.Message;                           // text for gossip item
        data << item.BoxMessage;                        // accept text (related to money) pop up box, 2.0.3
    }

    size_t count_pos = data.wpos();
    data << uint32(0);                                  // max count 0x20
    uint32 count = 0;

    // Store this instead of checking the Singleton every loop iteration
    bool questLevelInTitle = sWorld->getBoolConfig(CONFIG_UI_QUESTLEVELS_IN_DIALOGS);

    for (uint8 i = 0; i < _questMenu.GetMenuItemCount(); ++i)
    {
        QuestMenuItem const& item = _questMenu.GetItem(i);
        uint32 questID = item.QuestId;
        if (Quest const* quest = sObjectMgr->GetQuestTemplate(questID))
        {
            ++count;
            data << uint32(questID);
            data << uint32(item.QuestIcon);
            data << int32(quest->GetQuestLevel());
            data << uint32(quest->GetFlags()); // 3.3.3 quest flags
            data << uint8(quest->IsAutoComplete() && quest->IsRepeatable() && !quest->IsDailyOrWeekly() && !quest->IsMonthly()); // 3.3.3 icon changes - 0: yellow exclapamtion mark, 1: blue question mark
            std::string title = quest->GetTitle();

            LocaleConstant localeConstant = _session->GetSessionDbLocaleIndex();
            if (localeConstant != LOCALE_enUS)
                if (QuestLocale const* localeData = sObjectMgr->GetQuestLocale(questID))
                    ObjectMgr::GetLocaleString(localeData->Title, localeConstant, title);

            if (questLevelInTitle)
                AddQuestLevelToTitle(title, quest->GetQuestLevel());

            data << title;                                  // max 0x200
        }
    }

    data.put<uint8>(count_pos, count);
    _session->SendPacket(&data);
}

void PlayerMenu::SendCloseGossip()
{
    _gossipMenu.SetSenderGUID(ObjectGuid::Empty);

    WorldPacket data(SMSG_GOSSIP_COMPLETE, 0);
    _session->SendPacket(&data);
}

void PlayerMenu::SendPointOfInterest(uint32 id) const
{
    PointOfInterest const* poi = sObjectMgr->GetPointOfInterest(id);
    if (!poi)
    {
        TC_LOG_ERROR("sql.sql", "Request to send non-existing POI (Id: %u), ignored.", id);
        return;
    }

    std::string name = poi->Name;
    LocaleConstant localeConstant = _session->GetSessionDbLocaleIndex();
    if (localeConstant != LOCALE_enUS)
        if (PointOfInterestLocale const* localeData = sObjectMgr->GetPointOfInterestLocale(id))
            ObjectMgr::GetLocaleString(localeData->Name, localeConstant, name);

    WorldPacket data(SMSG_GOSSIP_POI, 4 + 4 + 4 + 4 + 4 + 10);  // guess size
    data << uint32(poi->Flags);
    data << float(poi->PositionX);
    data << float(poi->PositionY);
    data << uint32(poi->Icon);
    data << uint32(poi->Importance);
    data << name;

    _session->SendPacket(&data);
}

/*********************************************************/
/***                    QUEST SYSTEM                   ***/
/*********************************************************/

QuestMenu::QuestMenu()
{
    _questMenuItems.reserve(16);                                   // can be set for max from most often sizes to speedup push_back and less memory use
}

QuestMenu::~QuestMenu()
{
    ClearMenu();
}

void QuestMenu::AddMenuItem(uint32 QuestId, uint8 Icon)
{
    if (!sObjectMgr->GetQuestTemplate(QuestId))
        return;

    ASSERT(_questMenuItems.size() <= GOSSIP_MAX_MENU_ITEMS);

    QuestMenuItem questMenuItem;

    questMenuItem.QuestId        = QuestId;
    questMenuItem.QuestIcon      = Icon;

    _questMenuItems.push_back(questMenuItem);
}

bool QuestMenu::HasItem(uint32 questId) const
{
    for (QuestMenuItemList::const_iterator i = _questMenuItems.begin(); i != _questMenuItems.end(); ++i)
        if (i->QuestId == questId)
            return true;

    return false;
}

void QuestMenu::ClearMenu()
{
    _questMenuItems.clear();
}

void PlayerMenu::SendQuestGiverQuestList(QEmote const& eEmote, const std::string& Title, ObjectGuid guid)
{
    WorldPacket data(SMSG_QUESTGIVER_QUEST_LIST, 100);    // guess size
    data << uint64(guid);

    if (QuestGreeting const* questGreeting = sObjectMgr->GetQuestGreeting(guid))
    {
        std::string strGreeting = questGreeting->greeting;

        LocaleConstant localeConstant = _session->GetSessionDbLocaleIndex();
        if (localeConstant != LOCALE_enUS)
            if (QuestGreetingLocale const* questGreetingLocale = sObjectMgr->GetQuestGreetingLocale(MAKE_PAIR32(guid.GetEntry(), guid.GetTypeId())))
                ObjectMgr::GetLocaleString(questGreetingLocale->greeting, localeConstant, strGreeting);

        data << strGreeting;
        data << uint32(questGreeting->greetEmoteDelay);
        data << uint32(questGreeting->greetEmoteType);
    }
    else
    {
        data << Title;
        data << uint32(eEmote._Delay);                         // player emote
        data << uint32(eEmote._Emote);                         // NPC emote
    }

    size_t count_pos = data.wpos();
    data << uint8(0);
    uint32 count = 0;

    // Store this instead of checking the Singleton every loop iteration
    bool questLevelInTitle = sWorld->getBoolConfig(CONFIG_UI_QUESTLEVELS_IN_DIALOGS);

    for (uint32 i = 0; i < _questMenu.GetMenuItemCount(); ++i)
    {
        QuestMenuItem const& questMenuItem = _questMenu.GetItem(i);

        uint32 questID = questMenuItem.QuestId;

        if (Quest const* quest = sObjectMgr->GetQuestTemplate(questID))
        {
            ++count;
            std::string title = quest->GetTitle();

            LocaleConstant localeConstant = _session->GetSessionDbLocaleIndex();
            if (localeConstant != LOCALE_enUS)
                if (QuestLocale const* questTemplateLocaleData = sObjectMgr->GetQuestLocale(questID))
                    ObjectMgr::GetLocaleString(questTemplateLocaleData->Title, localeConstant, title);

            if (questLevelInTitle)
                AddQuestLevelToTitle(title, quest->GetQuestLevel());

            data << uint32(questID);
            data << uint32(questMenuItem.QuestIcon);
            data << int32(quest->GetQuestLevel());
            data << uint32(quest->GetFlags()); // 3.3.3 quest flags
            data << uint8(quest->IsAutoComplete() && quest->IsRepeatable() && !quest->IsDailyOrWeekly() && !quest->IsMonthly()); // 3.3.3 icon changes - 0: yellow exclapamtion mark, 1: blue question mark
            data << title;
        }
    }

    data.put<uint8>(count_pos, count);
    _session->SendPacket(&data);
    TC_LOG_DEBUG("network", "WORLD: Sent SMSG_QUESTGIVER_QUEST_LIST (QuestGiver: %s)", guid.ToString().c_str());
}

void PlayerMenu::SendQuestGiverStatus(uint32 questStatus, ObjectGuid npcGUID) const
{
    WorldPacket data(SMSG_QUESTGIVER_STATUS, 8 + 4);
    data << uint64(npcGUID);
    data << uint32(questStatus);

    _session->SendPacket(&data);
    TC_LOG_DEBUG("network", "WORLD: Sent SMSG_QUESTGIVER_STATUS NPC=%s, status=%u", npcGUID.ToString().c_str(), questStatus);
}

void PlayerMenu::SendQuestGiverQuestDetails(Quest const* quest, ObjectGuid npcGUID, bool autoLaunched, bool displayPopup) const
{
    std::string questTitle           = quest->GetTitle();
    std::string questDetails         = quest->GetDetails();
    std::string questObjectives      = quest->GetObjectives();
    std::string questAreaDescription = quest->GetAreaDescription();
    std::string questGiverTextWindow = quest->GetQuestGiverTextWindow();
    std::string questGiverTargetName = quest->GetQuestGiverTargetName();
    std::string questTurnTextWindow  = quest->GetQuestTurnTextWindow();
    std::string questTurnTargetName  = quest->GetQuestTurnTargetName();

    LocaleConstant localeConstant = _session->GetSessionDbLocaleIndex();
    if (localeConstant != LOCALE_enUS)
    {
        if (QuestLocale const* localeData = sObjectMgr->GetQuestLocale(quest->GetQuestId()))
        {
            ObjectMgr::GetLocaleString(localeData->Title, localeConstant, questTitle);
            ObjectMgr::GetLocaleString(localeData->Details, localeConstant, questDetails);
            ObjectMgr::GetLocaleString(localeData->Objectives, localeConstant, questObjectives);
            ObjectMgr::GetLocaleString(localeData->AreaDescription, localeConstant, questAreaDescription);
            ObjectMgr::GetLocaleString(localeData->QuestGiverTextWindow, localeConstant, questGiverTextWindow);
            ObjectMgr::GetLocaleString(localeData->QuestGiverTargetName, localeConstant, questGiverTargetName);
            ObjectMgr::GetLocaleString(localeData->QuestTurnTextWindow, localeConstant, questTurnTextWindow);
            ObjectMgr::GetLocaleString(localeData->QuestTurnTargetName, localeConstant, questTurnTargetName);
        }
    }

    if (sWorld->getBoolConfig(CONFIG_UI_QUESTLEVELS_IN_DIALOGS))
        AddQuestLevelToTitle(questTitle, quest->GetQuestLevel());

    WorldPacket data(SMSG_QUESTGIVER_QUEST_DETAILS, 100);   // guess size
    data << uint64(npcGUID);
    data << uint64(_session->GetPlayer()->GetPlayerSharingQuest());
    data << uint32(quest->GetQuestId());
    data << questTitle;
    data << questDetails;
    data << questObjectives;
    data << questGiverTextWindow;                           // 4.x
    data << questGiverTargetName;                           // 4.x
    data << questTurnTextWindow;                            // 4.x
    data << questTurnTargetName;                            // 4.x
    data << uint32(quest->GetQuestGiverPortrait());         // 4.x
    data << uint32(quest->GetQuestTurnInPortrait());        // 4.x
    data << uint8(autoLaunched);                            // auto finish
    data << uint32(quest->GetFlags() & (sWorld->getBoolConfig(CONFIG_QUEST_IGNORE_AUTO_ACCEPT) ? ~QUEST_FLAGS_AUTO_ACCEPT : ~0)); // 3.3.3 questFlags
    data << uint32(quest->GetSuggestedPlayers());
    data << uint8(0);                                       // IsFinished? value is sent back to server in quest accept packet
    data << uint8(displayPopup);                            // 4.x Starts at AreaTrigger?
    data << uint32(quest->GetRequiredSpell());              // 4.x

    quest->BuildExtraQuestInfo(data, _session->GetPlayer());

    data << uint32(QUEST_EMOTE_COUNT);
    for (uint8 i = 0; i < QUEST_EMOTE_COUNT; ++i)
    {
        data << uint32(quest->DetailsEmote[i]);
        data << uint32(quest->DetailsEmoteDelay[i]);       // DetailsEmoteDelay (in ms)
    }
    _session->SendPacket(&data);

    TC_LOG_DEBUG("network", "WORLD: Sent SMSG_QUESTGIVER_QUEST_DETAILS NPC=%s, questid=%u", npcGUID.ToString().c_str(), quest->GetQuestId());
}

void PlayerMenu::SendQuestQueryResponse(Quest const* quest) const
{
    std::string questTitle            = quest->GetTitle();
    std::string questDetails          = quest->GetDetails();
    std::string questObjectives       = quest->GetObjectives();
    std::string questAreaDescription  = quest->GetAreaDescription();
    std::string questCompletedText    = quest->GetCompletedText();
    std::string questGiverTextWindow = quest->GetQuestGiverTextWindow();
    std::string questGiverTargetName = quest->GetQuestGiverTargetName();
    std::string questTurnTextWindow = quest->GetQuestTurnTextWindow();
    std::string questTurnTargetName = quest->GetQuestTurnTargetName();

    std::string questObjectiveText[QUEST_OBJECTIVES_COUNT];
    for (uint8 i = 0; i < QUEST_OBJECTIVES_COUNT; ++i)
        questObjectiveText[i] = quest->ObjectiveText[i];

    LocaleConstant localeConstant = _session->GetSessionDbLocaleIndex();
    if (localeConstant != LOCALE_enUS)
    {
        if (QuestLocale const* localeData = sObjectMgr->GetQuestLocale(quest->GetQuestId()))
        {
            ObjectMgr::GetLocaleString(localeData->Title, localeConstant, questTitle);
            ObjectMgr::GetLocaleString(localeData->Details, localeConstant, questDetails);
            ObjectMgr::GetLocaleString(localeData->Objectives, localeConstant, questObjectives);
            ObjectMgr::GetLocaleString(localeData->AreaDescription, localeConstant, questAreaDescription);
            ObjectMgr::GetLocaleString(localeData->CompletedText, localeConstant, questCompletedText);
            ObjectMgr::GetLocaleString(localeData->QuestGiverTextWindow, localeConstant, questGiverTextWindow);
            ObjectMgr::GetLocaleString(localeData->QuestGiverTargetName, localeConstant, questGiverTargetName);
            ObjectMgr::GetLocaleString(localeData->QuestTurnTextWindow, localeConstant, questTurnTextWindow);
            ObjectMgr::GetLocaleString(localeData->QuestTurnTargetName, localeConstant, questTurnTargetName);

            for (uint8 i = 0; i < QUEST_OBJECTIVES_COUNT; ++i)
                ObjectMgr::GetLocaleString(localeData->ObjectiveText[i], localeConstant, questObjectiveText[i]);
        }
    }

    WorldPacket data(SMSG_QUEST_QUERY_RESPONSE, 100);       // guess size

    data << uint32(quest->GetQuestId());                    // quest id
    data << uint32(quest->GetQuestMethod());                // Accepted values: 0, 1 or 2. 0 == IsAutoComplete() (skip objectives/details)
    data << uint32(quest->GetQuestLevel());                 // may be -1, static data, in other cases must be used dynamic level: Player::GetQuestLevel (0 is not known, but assuming this is no longer valid for quest intended for client)
    data << uint32(quest->GetMinLevel());                   // min level
    data << uint32(quest->GetZoneOrSort());                 // zone or sort to display in quest log

    data << uint32(quest->GetType());                       // quest type
    data << uint32(quest->GetSuggestedPlayers());           // suggested players count

    data << uint32(quest->GetRepObjectiveFaction());        // shown in quest log as part of quest objective
    data << uint32(quest->GetRepObjectiveValue());          // shown in quest log as part of quest objective

    data << uint32(quest->GetRepObjectiveFaction2());       // shown in quest log as part of quest objective OPPOSITE faction
    data << uint32(quest->GetRepObjectiveValue2());         // shown in quest log as part of quest objective OPPOSITE faction

    data << uint32(quest->GetNextQuestInChain());           // client will request this quest from NPC, if not 0
    data << uint32(quest->GetXPId());                       // used for calculating rewarded experience

    if (quest->HasFlag(QUEST_FLAGS_HIDDEN_REWARDS))
        data << uint32(0);                                  // Hide money rewarded
    else
        data << uint32(quest->GetRewOrReqMoney(_session->GetPlayer())); // reward money (below max lvl)

    data << uint32(quest->GetRewMoneyMaxLevel());           // used in XP calculation at client
    data << uint32(quest->GetRewSpell());                   // reward spell, this spell will display (icon) (cast if RewSpellCast == 0)
    data << int32(quest->GetRewSpellCast());                // cast spell

    // rewarded honor points
    data << uint32(quest->GetRewHonorAddition());
    data << float(quest->GetRewHonorMultiplier());
    data << uint32(quest->GetSrcItemId());                  // source item id
    data << uint32(quest->GetFlags());                      // quest flags
    data << uint32(quest->GetMinimapTargetMark());          // minimap target mark (skull, etc. missing enum)
    data << uint32(quest->GetCharTitleId());                // CharTitleId, new 2.4.0, player gets this title (id from CharTitles)
    data << uint32(quest->GetPlayersSlain());               // players slain
    data << uint32(quest->GetBonusTalents());               // bonus talents
    data << uint32(quest->GetRewArenaPoints());             // bonus arena points FIXME: arena points were removed, right?
    data << uint32(quest->GetRewardSkillId());              // reward skill id
    data << uint32(quest->GetRewardSkillPoints());          // reward skill points
    data << uint32(quest->GetRewardReputationMask());       // rep mask (unsure on what it does)
    data << uint32(quest->GetQuestGiverPortrait());         // quest giver entry ?
    data << uint32(quest->GetQuestTurnInPortrait());        // quest turnin entry ?

    if (quest->HasFlag(QUEST_FLAGS_HIDDEN_REWARDS))
    {
        for (uint8 i = 0; i < QUEST_REWARDS_COUNT; ++i)
            data << uint32(0) << uint32(0);
        for (uint8 i = 0; i < QUEST_REWARD_CHOICES_COUNT; ++i)
            data << uint32(0) << uint32(0);
    }
    else
    {
        for (uint8 i = 0; i < QUEST_REWARDS_COUNT; ++i)
        {
            data << uint32(quest->RewardItemId[i]);
            data << uint32(quest->RewardItemIdCount[i]);
        }
        for (uint8 i = 0; i < QUEST_REWARD_CHOICES_COUNT; ++i)
        {
            data << uint32(quest->RewardChoiceItemId[i]);
            data << uint32(quest->RewardChoiceItemCount[i]);
        }
    }

    for (uint8 i = 0; i < QUEST_REPUTATIONS_COUNT; ++i)        // reward factions ids
        data << uint32(quest->RewardFactionId[i]);

    for (uint8 i = 0; i < QUEST_REPUTATIONS_COUNT; ++i)        // columnid+1 QuestFactionReward.dbc?
        data << int32(quest->RewardFactionValueId[i]);

    for (uint8 i = 0; i < QUEST_REPUTATIONS_COUNT; ++i)        // unknown usage
        data << int32(quest->RewardFactionValueIdOverride[i]);

    data << uint32(quest->GetPOIContinent());
    data << float(quest->GetPOIx());
    data << float(quest->GetPOIy());
    data << uint32(quest->GetPointOpt());

    if (sWorld->getBoolConfig(CONFIG_UI_QUESTLEVELS_IN_DIALOGS))
        AddQuestLevelToTitle(questTitle, quest->GetQuestLevel());

    data << questTitle;
    data << questObjectives;
    data << questDetails;
    data << questAreaDescription;
    data << questCompletedText;                                 // display in quest objectives window once all objectives are completed

    for (uint8 i = 0; i < QUEST_OBJECTIVES_COUNT; ++i)
    {
        if (quest->RequiredNpcOrGo[i] < 0)
            data << uint32((quest->RequiredNpcOrGo[i] * (-1)) | 0x80000000);    // client expects gameobject template id in form (id|0x80000000)
        else
            data << uint32(quest->RequiredNpcOrGo[i]);

        data << uint32(quest->RequiredNpcOrGoCount[i]);
        data << uint32(quest->ItemDrop[i]);
        data << uint32(quest->ItemDropQuantity[i]);
    }

    for (uint8 i = 0; i < QUEST_ITEM_OBJECTIVES_COUNT; ++i)
    {
        data << uint32(quest->RequiredItemId[i]);
        data << uint32(quest->RequiredItemCount[i]);
    }

    data << uint32(quest->GetRequiredSpell()); // Is it required to be cast, learned or what?

    for (uint8 i = 0; i < QUEST_OBJECTIVES_COUNT; ++i)
        data << questObjectiveText[i];

    for (uint32 i = 0; i < QUEST_REWARD_CURRENCY_COUNT; ++i)
    {
        data << uint32(quest->RewardCurrencyId[i]);
        data << uint32(quest->RewardCurrencyCount[i]);
    }

    for (uint32 i = 0; i < QUEST_REQUIRED_CURRENCY_COUNT; ++i)
    {
        data << uint32(quest->RequiredCurrencyId[i]);
        data << uint32(quest->RequiredCurrencyCount[i]);
    }

    data << questGiverTextWindow;
    data << questGiverTargetName;
    data << questTurnTextWindow;
    data << questTurnTargetName;
    data << uint32(quest->GetSoundAccept());
    data << uint32(quest->GetSoundTurnIn());

    _session->SendPacket(&data);
    TC_LOG_DEBUG("network", "WORLD: Sent SMSG_QUEST_QUERY_RESPONSE questid=%u", quest->GetQuestId());
}

void PlayerMenu::SendQuestGiverOfferReward(Quest const* quest, ObjectGuid npcGUID, bool autoLaunched) const
{
    std::string questTitle = quest->GetTitle();
    std::string questOfferRewardText = quest->GetOfferRewardText();
    std::string questGiverTextWindow = quest->GetQuestGiverTextWindow();
    std::string questGiverTargetName = quest->GetQuestGiverTargetName();
    std::string questTurnTextWindow = quest->GetQuestTurnTextWindow();
    std::string questTurnTargetName = quest->GetQuestTurnTargetName();

    LocaleConstant localeConstant = _session->GetSessionDbLocaleIndex();
    if (localeConstant != LOCALE_enUS)
    {
        if (QuestLocale const* localeData = sObjectMgr->GetQuestLocale(quest->GetQuestId()))
        {
            ObjectMgr::GetLocaleString(localeData->Title, localeConstant, questTitle);
            ObjectMgr::GetLocaleString(localeData->OfferRewardText, localeConstant, questOfferRewardText);
            ObjectMgr::GetLocaleString(localeData->QuestGiverTextWindow, localeConstant, questGiverTextWindow);
            ObjectMgr::GetLocaleString(localeData->QuestGiverTargetName, localeConstant, questGiverTargetName);
            ObjectMgr::GetLocaleString(localeData->QuestTurnTextWindow, localeConstant, questTurnTextWindow);
            ObjectMgr::GetLocaleString(localeData->QuestTurnTargetName, localeConstant, questTurnTargetName);
        }
    }

    if (sWorld->getBoolConfig(CONFIG_UI_QUESTLEVELS_IN_DIALOGS))
        AddQuestLevelToTitle(questTitle, quest->GetQuestLevel());

    WorldPacket data(SMSG_QUESTGIVER_OFFER_REWARD, 50);     // guess size
    data << uint64(npcGUID);
    data << uint32(quest->GetQuestId());
    data << questTitle;
    data << questOfferRewardText;

    data << questGiverTextWindow;
    data << questGiverTargetName;
    data << questTurnTextWindow;
    data << questTurnTargetName;
    data << uint32(quest->GetQuestGiverPortrait());
    data << uint32(quest->GetQuestTurnInPortrait());

    data << uint8(autoLaunched);                            // Auto Finish
    data << uint32(quest->GetFlags());                      // 3.3.3 questFlags
    data << uint32(quest->GetSuggestedPlayers());           // SuggestedGroupNum

    uint32 emoteCount = 0;
    for (uint8 i = 0; i < QUEST_EMOTE_COUNT; ++i)
    {
        if (quest->OfferRewardEmote[i] <= 0)
            break;
        ++emoteCount;
    }

    data << emoteCount;                                     // Emote Count
    for (uint8 i = 0; i < emoteCount; ++i)
    {
        data << uint32(quest->OfferRewardEmoteDelay[i]);    // Delay Emote
        data << uint32(quest->OfferRewardEmote[i]);
    }

    quest->BuildExtraQuestInfo(data, _session->GetPlayer());

    _session->SendPacket(&data);
    TC_LOG_DEBUG("network", "WORLD: Sent SMSG_QUESTGIVER_OFFER_REWARD NPC=%s, questid=%u", npcGUID.ToString().c_str(), quest->GetQuestId());
}

void PlayerMenu::SendQuestGiverRequestItems(Quest const* quest, ObjectGuid npcGUID, bool canComplete, bool autoLaunched) const
{
    // We can always call to RequestItems, but this packet only goes out if there are actually
    // items.  Otherwise, we'll skip straight to the OfferReward

    std::string questTitle = quest->GetTitle();
    std::string requestItemsText = quest->GetRequestItemsText();

    LocaleConstant localeConstant = _session->GetSessionDbLocaleIndex();
    if (localeConstant != LOCALE_enUS)
    {
        if (QuestLocale const* localeData = sObjectMgr->GetQuestLocale(quest->GetQuestId()))
        {
            ObjectMgr::GetLocaleString(localeData->Title, localeConstant, questTitle);
            ObjectMgr::GetLocaleString(localeData->RequestItemsText, localeConstant, requestItemsText);
        }
    }

    if (!quest->GetReqItemsCount() && canComplete)
    {
        SendQuestGiverOfferReward(quest, npcGUID, true);
        return;
    }

    if (sWorld->getBoolConfig(CONFIG_UI_QUESTLEVELS_IN_DIALOGS))
        AddQuestLevelToTitle(questTitle, quest->GetQuestLevel());

    WorldPackets::Quest::QuestGiverRequestItems packet;

    packet.QuestTitle = questTitle;
    packet.CompletionText = requestItemsText;
    packet.QuestID = quest->GetQuestId();
    packet.QuestGiverGUID = npcGUID;

    // There are no delays in any 4.x quest sniffs sent so we skip them and send them as 0
    if (canComplete)
        packet.CompEmoteType = canComplete ? quest->GetCompleteEmote() : quest->GetIncompleteEmote();

    packet.AutoLaunched = autoLaunched;
    packet.QuestFlags = quest->GetFlags();
    packet.SuggestPartyMembers = quest->GetSuggestedPlayers();
    packet.MoneyToGet = quest->GetRewOrReqMoney() < 0 ? -quest->GetRewOrReqMoney() : 0;

    QuestStatusData data;
    QuestStatusMap statusMap = _session->GetPlayer()->getQuestStatusMap();
    QuestStatusMap::const_iterator itr = statusMap.find(quest->GetQuestId());
    if (itr != statusMap.end())
        data = itr->second;

    bool collectObjectiveComplete = true;
    for (uint8 i = 0; i < QUEST_ITEM_OBJECTIVES_COUNT; ++i)
    {
        if (!quest->RequiredItemId[i])
            continue;

        uint32 displayId = 0;
        if (ItemTemplate const* itemTemplate = sObjectMgr->GetItemTemplate(quest->RequiredItemId[i]))
            displayId = itemTemplate->DisplayInfoID;

        if (data.ItemCount[i] != quest->RequiredItemCount[i])
            collectObjectiveComplete = false;

        packet.Collect.emplace_back(quest->RequiredItemId[i], quest->RequiredItemCount[i], displayId);
    }

    for (uint8 i = 0; i < QUEST_REQUIRED_CURRENCY_COUNT; ++i)
    {
        if (!quest->RequiredCurrencyId[i])
            continue;

        packet.Currency.emplace_back(quest->RequiredCurrencyId[i], quest->RequiredCurrencyCount[i]);
    }

    bool killCreditObjectiveComplete = true;
    for (uint8 i = 0; i < QUEST_OBJECTIVES_COUNT; ++i)
    {
        if (!quest->RequiredNpcOrGo[i])
            continue;

        if (data.CreatureOrGOCount[i] != quest->RequiredNpcOrGo[i])
            killCreditObjectiveComplete = false;
    }

    packet.StatusFlags[0] = killCreditObjectiveComplete ? QUEST_STATUS_FLAG_KILL_OBJECTIVE_COMPLETE : QUEST_STATUS_FLAG_NONE;
    packet.StatusFlags[1] = collectObjectiveComplete ? QUEST_STATUS_FLAG_COLLECT_OBJECTIVE_COMPLETE : QUEST_STATUS_FLAG_NONE;
    packet.StatusFlags[2] = QUEST_STATUS_FLAG_UNK_4;
    packet.StatusFlags[3] = QUEST_STATUS_FLAG_UNK_5;
    packet.StatusFlags[4] = QUEST_STATUS_FLAG_UNK_7;

    _session->SendPacket(packet.Write());
    TC_LOG_DEBUG("network", "WORLD: Sent SMSG_QUEST_GIVER_REQUEST_ITEMS NPC=%s, questid=%u", npcGUID.ToString().c_str(), quest->GetQuestId());
}

void PlayerMenu::AddQuestLevelToTitle(std::string &title, int32 level)
{
    // Adds the quest level to the front of the quest title
    // example: [13] Westfall Stew

    std::stringstream questTitlePretty;
    questTitlePretty << "[" << level << "] " << title;
    title = questTitlePretty.str();
}
