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

// DO NOT EDIT!
// Autogenerated from DB2Structure.h

#include "HotfixDatabase.h"

// Force locale statments to appear exactly in locale declaration order, right after normal data fetch statement
#define PREPARE_LOCALE_STMT(stmtBase, sql, con) \
    static_assert(stmtBase + 1 == stmtBase##_LOCALE, "Invalid prepared statement index for " #stmtBase "_LOCALE"); \
    PrepareStatement(stmtBase##_LOCALE, sql, con);

void HotfixDatabaseConnection::DoPrepareStatements()
{
    if (!m_reconnecting)
        m_stmts.resize(MAX_HOTFIXDATABASE_STATEMENTS);

    // KeyChain.db2
    PrepareStatement(HOTFIX_SEL_KEY_CHAIN, "SELECT Id, Key1, Key2, Key3, Key4, Key5, Key6, Key7, Key8, Key9, Key10, Key11, Key12, Key13, Key14, Key15, Key16, "
        "Key17, Key18, Key19, Key20, Key21, Key22, Key23, Key24, Key25, Key26, Key27, Key28, Key29, Key30, Key31, Key32 FROM key_chain ORDER BY ID DESC", CONNECTION_SYNCH);

    // Item.db2
    PrepareStatement(HOTFIX_SEL_ITEM, "SELECT ID, Class, SubClass, SoundOverrideSubclass, Material, InventoryType, Sheath FROM item ORDER BY ID DESC", CONNECTION_SYNCH);

    // ItemCurrencyCost.db2
    PrepareStatement(HOTFIX_SEL_ITEM_CURRENCY_COST, "SELECT ID, ItemID FROM item_currency_cost ORDER BY ItemID DESC", CONNECTION_SYNCH);

    // ItemExtendedCost.db2
    PrepareStatement(HOTFIX_SEL_ITEM_EXTENDED_COST, "SELECT ID, RequiredHonorPoints, RequiredArenaPoints, RequiredArenaSlot, "
        "RequiredItem1, RequiredItem2, RequiredItem3, RequiredItem4, RequiredItem5, "
        "RequiredItemCount1, RequiredItemCount2, RequiredItemCount3, RequiredItemCount4, RequiredItemCount5, "
        "RequiredPersonalArenaRating, ItemPurchaseGroup, "
        "RequiredCurrency1, RequiredCurrency2, RequiredCurrency3, RequiredCurrency4, RequiredCurrency5, "
        "RequiredCurrencyCount1, RequiredCurrencyCount2, RequiredCurrencyCount3, RequiredCurrencyCount4, RequiredCurrencyCount5, "
        "RequiredFactionId, RequiredFactionStanding, RequirementFlags, RequiredAchievement FROM item_extended_cost ORDER BY ID DESC", CONNECTION_SYNCH);

    // Item-sparse.db2
    PrepareStatement(HOTFIX_SEL_ITEM_SPARSE, "SELECT ID, Quality, Flags1, Flags2, Unk1, Unk2, BuyCount, BuyPrice, SellPrice, InventoryType, "
        "AllowableClass, AllowableRace, ItemLevel, RequiredLevel, RequiredSkill, RequiredSkillRank, RequiredSpell, RequiredHonorRank, "
        "RequiredCityRank, RequiredReputationFaction, RequiredReputationRank, MaxCount, Stackable, ContainerSlots, "
        "ItemStatType1, ItemStatType2, ItemStatType3, ItemStatType4, ItemStatType5, "
        "ItemStatType6, ItemStatType7, ItemStatType8, ItemStatType9, ItemStatType10, "
        "ItemStatValue1, ItemStatValue2, ItemStatValue3, ItemStatValue4, ItemStatValue5, "
        "ItemStatValue6, ItemStatValue7, ItemStatValue8, ItemStatValue9, ItemStatValue10, "
        "ItemStatAllocation1, ItemStatAllocation2, ItemStatAllocation3, ItemStatAllocation4, ItemStatAllocation5, "
        "ItemStatAllocation6, ItemStatAllocation7, ItemStatAllocation8, ItemStatAllocation9, ItemStatAllocation10, "
        "ItemStatSocketCostMultiplier1, ItemStatSocketCostMultiplier2, ItemStatSocketCostMultiplier3, ItemStatSocketCostMultiplier4, ItemStatSocketCostMultiplier5, "
        "ItemStatSocketCostMultiplier6, ItemStatSocketCostMultiplier7, ItemStatSocketCostMultiplier8, ItemStatSocketCostMultiplier9, ItemStatSocketCostMultiplier10, "
        "ScalingStatDistribution, DamageType, Delay, RangedModRange, "
        "SpellID1, SpellID2, SpellID3, SpellID4, SpellID5, "
        "SpellTrigger1, SpellTrigger2, SpellTrigger3, SpellTrigger4, SpellTrigger5, "
        "SpellCharges1, SpellCharges2, SpellCharges3, SpellCharges4, SpellCharges5, "
        "SpellCooldown1, SpellCooldown2, SpellCooldown3, SpellCooldown4, SpellCooldown5, "
        "SpellCategory1, SpellCategory2, SpellCategory3, SpellCategory4, SpellCategory5, "
        "SpellCategoryCooldown1, SpellCategoryCooldown2, SpellCategoryCooldown3, SpellCategoryCooldown4, SpellCategoryCooldown5, "
        "Bonding, Name, Name2, Name3, Name4, Description, PageText, LanguageID, PageMaterial, StartQuest, LockID, Material, Sheath, RandomProperty, RandomSuffix, ItemSet, "
        "Area, Map, BagFamily, TotemCategory, SocketColor1, SocketColor2, SocketColor3, Content1, Content2, Content3, SocketBonus, GemProperties, ArmorDamageModifier, "
        "Duration, ItemLimitCategory, HolidayID, StatScalingFactor, CurrencySubstitutionID, CurrencySubstitutionCount FROM item_sparse ORDER BY ID DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_ITEM_SPARSE, "SELECT ID, Name_lang, Name2_lang, Name3_lang, Name4_lang, Description_lang FROM item_sparse_locale WHERE locale = ?", CONNECTION_SYNCH);
}

HotfixDatabaseConnection::HotfixDatabaseConnection(MySQLConnectionInfo& connInfo) : MySQLConnection(connInfo)
{
}

HotfixDatabaseConnection::HotfixDatabaseConnection(ProducerConsumerQueue<SQLOperation*>* q, MySQLConnectionInfo& connInfo) : MySQLConnection(q, connInfo)
{
}

HotfixDatabaseConnection::~HotfixDatabaseConnection()
{
}
