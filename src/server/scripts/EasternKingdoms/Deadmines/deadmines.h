/*
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

#ifndef DEF_DEADMINES_H
#define DEF_DEADMINES_H

#define DataHeader "DM"
#define DMScriptName "instance_deadmines"

uint32 const EncounterCount = 6;

enum DMDataTypes
{
    // Encounters
    DATA_GLUBTOK                        = 0,
    DATA_HELIX_GEARBREAKER              = 1,
    DATA_FOE_REAPER_5000                = 2,
    DATA_ADMIRAL_RIPSNARL               = 3,
    DATA_CAPTAIN_COOKIE                 = 4,
    DATA_VANESSA_VAN_CLEEF              = 5,

    DATA_TEAM_IN_INSTANCE               = 6,
    DATA_FIREWALL_PLATTER               = 7,
    DATA_LUMBERING_OAF                  = 8,
    DATA_FOE_REAPER_INTRO               = 9,
    DATA_FOE_REAPER_BUNNY               = 10,
    DATA_PROTOTYPE_REAPER               = 11,
    DATA_BROKEN_DOOR                    = 13,
    DATA_IRON_CLAD_DOOR                 = 14,
    DATA_RIPSNARL_FOG                   = 15,
    DATA_VANESSA_VAN_CLEEF_ENCOUNTER    = 16,
    DATA_VANESSAS_TRAP_BUNNY            = 17,
    DATA_VANESSA_ANCHOR_BUNNY           = 18,
    DATA_ACTIVATED_VENT                 = 19,
};

enum DMCreatures
{
    // Bosses
    BOSS_GLUBTOK                    = 47162,
    BOSS_HELIX_GEARBREAKER          = 47296,
    BOSS_FOE_REAPER_5000            = 43778,
    BOSS_ADMIRAL_RIPSNARL           = 47626,
    BOSS_CAPTAIN_COOKIE             = 47739,
    BOSS_VANESSA_VAN_CLEEF          = 49541,

    // Horde Creatures
    NPC_SLINKY_SHARPSHIV            = 46906,
    NPC_KAGTHA                      = 46889,
    NPC_MISS_MAYHEM                 = 46902,
    NPC_SHATTERED_HAND_ASSASSIN     = 46890,
    NPC_MAYHEM_REAPER_PROTOTYPE     = 46903,

    // Alliance Creatures
    NPC_STORMWIND_INVESTIGATOR      = 46614,
    NPC_CRIME_SCENE_ALARM_O_BOT     = 46613,
    NPC_STORMWIND_DEFENDER          = 50595,
    NPC_LIEUTENANT_HORATIO_LAINE    = 46612,
    NPC_QUARTERMASTER_LEWIS         = 491,

    // Encounter Related
    NPC_GENERAL_PURPOSE_DUMMY_JMF   = 45979,
    NPC_GLUBTOK_FIREWALL_PLATTER    = 48974,
    NPC_FIREWALL_PLATTER_1A         = 48975,
    NPC_FIREWALL_PLATTER_1B         = 49039,
    NPC_FIREWALL_PLATTER_1C         = 49040,
    NPC_FIREWALL_PLATTER_2A         = 48976,
    NPC_FIREWALL_PLATTER_2B         = 49041,
    NPC_FIREWALL_PLATTER_2C         = 49042,
    NPC_GENERAL_PURPOSE_BUNNY_L2    = 47242,
    NPC_FIRE_BLOSSOM_BUNNY          = 47282,
    NPC_FROST_BLOSSOM_BUNNY         = 47284,
    NPC_LUMBERING_OAF               = 47297,
    NPC_HELIX_CREW                  = 49139,
    NPC_STICKY_BOMB                 = 47314,
    NPC_DEFIAS_REAPER               = 47403,
    NPC_DEFIAS_WATCHER              = 47404,
    NPC_FOE_REAPER_TARGETING_BUNNY  = 47468,
    NPC_PROTOTYPE_REAPER            = 49208,
    NPC_MOLTEN_SLAG                 = 49229,
    NPC_VAPOR                       = 47714,
    NPC_COOKIES_CAULDRON            = 47754,
    NPC_CORN                        = 48006,
    NPC_ROTTEN_CORN                 = 48276,
    NPC_MELON                       = 48294,
    NPC_ROTTEN_MELON                = 48293,
    NPC_STEAK                       = 48296,
    NPC_ROTTEN_STEAK                = 48295,
    NPC_MYSTERY_MEAT                = 48297,
    NPC_ROTTEN_MYSTERY_MEAT         = 48298,
    NPC_LOAF                        = 48300,
    NPC_ROTTEN_LOAF                 = 48299,
    NPC_BUN                         = 48301,
    NPC_ROTTEN_BUN                  = 48302,

    NPC_A_NOTE_FROM_VANESSA         = 49564,
    NPC_VANESSA_VAN_CLEEF_INTRO     = 49429,
    NPC_VANESSAS_TRAP_BUNNY         = 49454,
    NPC_VANESSA_ANCHOR_BUNNY_JMF    = 51624,
    NPC_STEAM_VALVE                 = 49457,
};

enum DMGameObjects
{
    GO_FACTORY_DOOR     = 13965,
    GO_IRON_CLAD_DOOR   = 16397,
    GO_DEFIAS_CANNON    = 16398,
    GO_DOOR_LEVER       = 101833,
    GO_MAST_ROOM_DOOR   = 16400,
    GO_HEAVY_DOOR       = 17153,
    GO_FOUNDRY_DOOR     = 16399
};

enum DMActions
{
    ACTION_ELECTRICAL_CHARGE = 1,
    ACTION_EJECT_PLAYERS     = 1,
};

enum DMSpells
{
    // -- Admiral Ripsnarl Encounter
    SPELL_FOG                       = 88768,

    // -- Vanessa VanCleef Encounter
    // Vanessa's Trap Bunny
    SPELL_RIDE_MAGMA_VEHICLE        = 92378,
    SPELL_MOLTEN_MAGMA              = 92424,
    SPELL_FORCE_MAGMA_TRAP_THROW    = 92489,

    // Vanessas Anchor Bunny JMF
    SPELL_ROPE_BEAM                 = 43785,

    // Steam Valve
    SPELL_STEAM_VENT                = 92399,
};

enum DMVanessasNightmareStages
{
    NIGHTMARE_STATE_PREPARE_TRAP    = 2,
    NIGHTMARE_STAGE_GLUBTOK         = 3,
    NIGHTMARE_STATE_HELIX           = 4,
    NIGHTMARE_STAGE_FOE_REAPER      = 5,
    NIGHTMARE_STAGE_RIPSNARL        = 6
};

Position const captainCookieSpawnPos = { -88.13194f, -819.3299f, 39.23453f };

Position const noteFromVanessaSpawnPos = { -74.36111f, -820.0139f, 40.37145f, 4.014257f };
Position const vanessaVanCleefSpawnPos = { -75.58507f, -819.9636f, 47.06727f, 6.178465f };

template<class AI>
AI* GetDeadminesAI(Creature* creature)
{
    return GetInstanceAI<AI>(creature, DMScriptName);
}

template<class AI>
AI* GetDeadminesAI(GameObject* go)
{
    return GetInstanceAI<AI>(go, DMScriptName);
}

#endif
