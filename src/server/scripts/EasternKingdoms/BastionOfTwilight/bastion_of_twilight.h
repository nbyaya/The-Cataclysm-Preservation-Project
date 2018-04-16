﻿/*
* Copyright (C) 2008-2018 TrinityCore <https://www.trinitycore.org/>
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

#ifndef DEF_BASTION_OF_TWILIGHT_H
#define DEF_BASTION_OF_TWILIGHT_H

#define DataHeader "BoT"
#define BoTScriptName "instance_bastion_of_twilight"

uint32 const EncounterCountNormal = 4;
uint32 const EncounterCountHeroic = 5;

enum BoTDataTypes
{
    // Encounter Types
    DATA_HALFUS_WYRMBREAKER     = 0,
    DATA_THERALION_AND_VALIONA  = 1,
    DATA_ASCENDANT_COUNCIL      = 2,
    DATA_CHOGALL                = 3,
    DATA_SINESTRA               = 4,

    // Creature Types
    DATA_PROTO_BEHEMOTH         = 5,
    DATA_THERALION              = 6,
    DATA_VALIONA                = 7,
    DATA_IGNACIOUS              = 8,
    DATA_FELUDIUS               = 9,
    DATA_TERRASTRA              = 10,
    DATA_ARION                  = 11,
    DATA_ELEMENTIUM_MONSTROSITY = 12
};

enum BoTCreatures
{
    // Bosses
    BOSS_HALFUS_WYRMBREAKER     = 44600,
    BOSS_PROTO_BEHEMOTH         = 44687,
    BOSS_THERALION              = 45993,
    BOSS_VALIONA                = 45992,
    BOSS_IGNACIOUS              = 43686,
    BOSS_FELUDIUS               = 43687,
    BOSS_TERRASTRA              = 43689,
    BOSS_ARION                  = 43688,
    BOSS_ELEMENTIUM_MONSTROSITY = 43735,
    BOSS_CHOGALL                = 43324,
    BOSS_SINESTRA               = 45213
};

enum BoTGameObjects
{
    GO_HALFUS_ENTRANCE                  = 205222,
    GO_HALFUS_EXIT                      = 205223,
    GO_DRAGON_SIBLINGS_DOOR_ENTRANCE    = 205224,
    GO_DRAGON_SIBLINGS_DOOR_EXIT        = 205225,
    GO_ASCENDANT_COUNCIL_ENTRANCE       = 205226,
    GO_ASCENDANT_COUNCIL_EXIT           = 205227,
    GO_CHOGALL_ENTRANCE                 = 205228
};

enum UnresponsiveDragonCases
{

};

template<class AI>
AI* GetBastionOfTwilightAI(Creature* creature)
{
    return GetInstanceAI<AI>(creature, BoTScriptName);
}

template<class AI>
AI* GetBastionOfTwilightAI(GameObject* go)
{
    return GetInstanceAI<AI>(go, BoTScriptName);
}

#endif