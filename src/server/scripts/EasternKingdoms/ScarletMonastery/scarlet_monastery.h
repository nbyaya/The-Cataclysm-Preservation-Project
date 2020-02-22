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

#ifndef SCARLET_M_
#define SCARLET_M_

#include "CreatureAIImpl.h"

#define SMScriptName "instance_scarlet_monastery"
#define DataHeader "SM"

uint32 const EncounterCount = 10;

enum SMDataTypes
{
    // Bosses
    DATA_INTERROGATOR_VISHAS    = 1,
    DATA_BLOODMAGE_THALNOS      = 2,

    DATA_MOGRAINE_AND_WHITE_EVENT,
    DATA_MOGRAINE,
    DATA_WHITEMANE,
    DATA_HORSEMAN_EVENT,
    DATA_PUMPKIN_SHRINE,
    DATA_VORREL,
    DATA_ARCANIST_DOAN,
    DATA_AZSHIR,
    DATA_HEROD,
    DATA_HIGH_INQUISITOR_FAIRBANKS,
    DATA_HOUNDMASTER_LOKSEY,
    DATA_SCORN
};

enum SMCreatureIds
{
    // Bosses
    BOSS_INTERROGATOR_VISHAS    = 3983,
    BOSS_BLOODMAGE_THALNOS      = 4543,

    NPC_MOGRAINE                = 3976,
    NPC_WHITEMANE               = 3977,
    NPC_VORREL                  = 3981,

    NPC_HORSEMAN                = 23682,
    NPC_HEAD                    = 23775,
    NPC_PUMPKIN                 = 23694
};

enum SMGameObjectIds
{
    GO_HIGH_INQUISITORS_DOOR        = 104600,
    GO_PUMPKIN_SHRINE               = 186267
};

template <class AI, class T>
inline AI* GetScarletMonasteryAI(T* obj)
{
    return GetInstanceAI<AI>(obj, SMScriptName);
}

#define RegisterScarletMonastryCreatureAI(ai_name) RegisterCreatureAIWithFactory(ai_name, GetScarletMonasteryAI)


#endif // SCARLET_M_
