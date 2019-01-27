/*
 * Copyright (C) 2008-2018 TrinityCore <https://www.trinitycore.org/>
 * Copyright (C) 2006-2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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

#ifndef DEF_MAGISTERS_TERRACE_H
#define DEF_MAGISTERS_TERRACE_H

#include "CreatureAIImpl.h"

#define MGTScriptName "instance_magisters_terrace"
#define DataHeader "MT"

uint32 const EncounterCount = 4;

enum MTDataTypes
{
    // Encounter states
    DATA_SELIN,
    DATA_VEXALLUS,
    DATA_DELRISSA,
    DATA_KAELTHAS_SUNSTRIDER,

    // Encounter related
    DATA_KAELTHAS_INTRO,
    DATA_DELRISSA_DEATH_COUNT,

    // Additional data
    DATA_KAELTHAS_INTRO_STATE,

    DATA_ESCAPE_ORB
};

enum MTCreatureIds
{
    // Bosses
    BOSS_KAELTHAS_SUNSTRIDER    = 24664,

    // Encounter related
    /*Kael'thas Sunstrider*/
    NPC_ARCANE_SPHERE           = 24708,
    NPC_FLAME_STRIKE            = 24666,
    NPC_PHOENIX                 = 24674,
    NPC_PHOENIX_EGG             = 24675,

    NPC_SELIN                   = 24723,
    NPC_DELRISSA                = 24560,
    NPC_FEL_CRYSTAL             = 24722,
    NPC_KALECGOS                = 24844,

    // Event related
    NPC_HUMAN_KALECGOS          = 24848,
    NPC_COILSKAR_WITCH          = 24696,
    NPC_SUNBLADE_WARLOCK        = 24686,
    NPC_SUNBLADE_MAGE_GUARD     = 24683,
    NPC_SISTER_OF_TORMENT       = 24697,
    NPC_ETHEREUM_SMUGGLER       = 24698,
    NPC_SUNBLADE_BLOOD_KNIGHT   = 24684,
};

enum MTGameObjectIds
{
    GO_VEXALLUS_DOOR        = 187896,
    GO_SELIN_DOOR           = 187979,
    GO_SELIN_ENCOUNTER_DOOR = 188065,
    GO_DELRISSA_DOOR        = 187770,
    GO_ASYLUM_DOOR          = 188064,
    GO_KAEL_STATUE_1        = 188165,
    GO_KAEL_STATUE_2        = 188166,
    GO_ESCAPE_ORB           = 188173
};

enum MTInstanceEventIds
{
    EVENT_SPAWN_KALECGOS    = 16547
};

enum MTInstanceText
{
    SAY_KALECGOS_SPAWN      = 0
};

enum MTMovementData
{
    PATH_KALECGOS_FLIGHT    = 248440
};

template <class AI, class T>
inline AI* GetMagistersTerraceAI(T* obj)
{
    return GetInstanceAI<AI>(obj, MGTScriptName);
}

#define RegisterMagistersTerraceCreatureAI(ai_name) RegisterCreatureAIWithFactory(ai_name, GetMagistersTerraceAI)

#endif
