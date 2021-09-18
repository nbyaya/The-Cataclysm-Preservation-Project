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

#include "SpellInfo.h"
#include "Battleground.h"
#include "Corpse.h"
#include "Creature.h"
#include "DBCStores.h"
#include "InstanceScript.h"
#include "Item.h"
#include "ItemTemplate.h"
#include "Log.h"
#include "ObjectAccessor.h"
#include "Player.h"
#include "Random.h"
#include "Spell.h"
#include "SpellAuraDefines.h"
#include "SpellAuraEffects.h"
#include "SpellMgr.h"
#include "TemporarySummon.h"
#include "Vehicle.h"

uint32 GetTargetFlagMask(SpellTargetObjectTypes objType)
{
    switch (objType)
    {
        case TARGET_OBJECT_TYPE_DEST:
            return TARGET_FLAG_DEST_LOCATION;
        case TARGET_OBJECT_TYPE_UNIT_AND_DEST:
            return TARGET_FLAG_DEST_LOCATION | TARGET_FLAG_UNIT;
        case TARGET_OBJECT_TYPE_CORPSE_ALLY:
            return TARGET_FLAG_CORPSE_ALLY;
        case TARGET_OBJECT_TYPE_CORPSE_ENEMY:
            return TARGET_FLAG_CORPSE_ENEMY;
        case TARGET_OBJECT_TYPE_CORPSE:
            return TARGET_FLAG_CORPSE_ALLY | TARGET_FLAG_CORPSE_ENEMY;
        case TARGET_OBJECT_TYPE_UNIT:
            return TARGET_FLAG_UNIT;
        case TARGET_OBJECT_TYPE_GOBJ:
            return TARGET_FLAG_GAMEOBJECT;
        case TARGET_OBJECT_TYPE_GOBJ_ITEM:
            return TARGET_FLAG_GAMEOBJECT_ITEM;
        case TARGET_OBJECT_TYPE_ITEM:
            return TARGET_FLAG_ITEM;
        case TARGET_OBJECT_TYPE_SRC:
            return TARGET_FLAG_SOURCE_LOCATION;
        default:
            return TARGET_FLAG_NONE;
    }
}

SpellImplicitTargetInfo::SpellImplicitTargetInfo(uint32 target)
{
    _target = Targets(target);
}

bool SpellImplicitTargetInfo::IsArea() const
{
    return GetSelectionCategory() == TARGET_SELECT_CATEGORY_AREA || GetSelectionCategory() == TARGET_SELECT_CATEGORY_CONE;
}

SpellTargetSelectionCategories SpellImplicitTargetInfo::GetSelectionCategory() const
{
    return _data[_target].SelectionCategory;
}

SpellTargetReferenceTypes SpellImplicitTargetInfo::GetReferenceType() const
{
    return _data[_target].ReferenceType;
}

SpellTargetObjectTypes SpellImplicitTargetInfo::GetObjectType() const
{
    return _data[_target].ObjectType;
}

SpellTargetCheckTypes SpellImplicitTargetInfo::GetCheckType() const
{
    return _data[_target].SelectionCheckType;
}

SpellTargetDirectionTypes SpellImplicitTargetInfo::GetDirectionType() const
{
    return _data[_target].DirectionType;
}

float SpellImplicitTargetInfo::CalcDirectionAngle() const
{
    switch (GetDirectionType())
    {
        case TARGET_DIR_FRONT:
            return 0.0f;
        case TARGET_DIR_BACK:
            return static_cast<float>(M_PI);
        case TARGET_DIR_RIGHT:
            return static_cast<float>(-M_PI/2);
        case TARGET_DIR_LEFT:
            return static_cast<float>(M_PI/2);
        case TARGET_DIR_FRONT_RIGHT:
            return static_cast<float>(-M_PI/4);
        case TARGET_DIR_BACK_RIGHT:
            return static_cast<float>(-3*M_PI/4);
        case TARGET_DIR_BACK_LEFT:
            return static_cast<float>(3*M_PI/4);
        case TARGET_DIR_FRONT_LEFT:
            return static_cast<float>(M_PI/4);
        case TARGET_DIR_RANDOM:
            return float(rand_norm())*static_cast<float>(2*M_PI);
        default:
            return 0.0f;
    }
}

Targets SpellImplicitTargetInfo::GetTarget() const
{
    return _target;
}

uint32 SpellImplicitTargetInfo::GetExplicitTargetMask(bool& srcSet, bool& dstSet) const
{
    uint32 targetMask = 0;
    if (GetTarget() == TARGET_DEST_TRAJ)
    {
        if (!srcSet)
            targetMask = TARGET_FLAG_SOURCE_LOCATION;
        if (!dstSet)
            targetMask |= TARGET_FLAG_DEST_LOCATION;
    }
    else
    {
        switch (GetReferenceType())
        {
            case TARGET_REFERENCE_TYPE_SRC:
                if (srcSet)
                    break;
                targetMask = TARGET_FLAG_SOURCE_LOCATION;
                break;
            case TARGET_REFERENCE_TYPE_DEST:
                if (dstSet)
                    break;
                targetMask = TARGET_FLAG_DEST_LOCATION;
                break;
            case TARGET_REFERENCE_TYPE_TARGET:
                switch (GetObjectType())
                {
                    case TARGET_OBJECT_TYPE_GOBJ:
                        targetMask = TARGET_FLAG_GAMEOBJECT;
                        break;
                    case TARGET_OBJECT_TYPE_GOBJ_ITEM:
                        targetMask = TARGET_FLAG_GAMEOBJECT_ITEM;
                        break;
                    case TARGET_OBJECT_TYPE_UNIT_AND_DEST:
                    case TARGET_OBJECT_TYPE_UNIT:
                    case TARGET_OBJECT_TYPE_DEST:
                        switch (GetCheckType())
                        {
                            case TARGET_CHECK_ENEMY:
                                targetMask = TARGET_FLAG_UNIT_ENEMY;
                                break;
                            case TARGET_CHECK_ALLY:
                                targetMask = TARGET_FLAG_UNIT_ALLY;
                                break;
                            case TARGET_CHECK_PARTY:
                                targetMask = TARGET_FLAG_UNIT_PARTY;
                                break;
                            case TARGET_CHECK_RAID:
                                targetMask = TARGET_FLAG_UNIT_RAID;
                                break;
                            case TARGET_CHECK_PASSENGER:
                                targetMask = TARGET_FLAG_UNIT_PASSENGER;
                                break;
                            case TARGET_CHECK_RAID_CLASS:
                                [[fallthrough]];
                            default:
                                targetMask = TARGET_FLAG_UNIT;
                                break;
                        }
                        break;
                    default:
                        break;
                }
                break;
            default:
                break;
        }
    }

    switch (GetObjectType())
    {
        case TARGET_OBJECT_TYPE_SRC:
            srcSet = true;
            break;
        case TARGET_OBJECT_TYPE_DEST:
        case TARGET_OBJECT_TYPE_UNIT_AND_DEST:
            dstSet = true;
            break;
        default:
            break;
    }
    return targetMask;
}

SpellImplicitTargetInfo::StaticData  SpellImplicitTargetInfo::_data[TOTAL_SPELL_TARGETS] =
{
    {TARGET_OBJECT_TYPE_NONE, TARGET_REFERENCE_TYPE_NONE,   TARGET_SELECT_CATEGORY_NYI,     TARGET_CHECK_DEFAULT,  TARGET_DIR_NONE},        //
    {TARGET_OBJECT_TYPE_UNIT, TARGET_REFERENCE_TYPE_CASTER, TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_DEFAULT,  TARGET_DIR_NONE},        // 1 TARGET_UNIT_CASTER
    {TARGET_OBJECT_TYPE_UNIT, TARGET_REFERENCE_TYPE_CASTER, TARGET_SELECT_CATEGORY_NEARBY,  TARGET_CHECK_ENEMY,    TARGET_DIR_NONE},        // 2 TARGET_UNIT_NEARBY_ENEMY
    {TARGET_OBJECT_TYPE_UNIT, TARGET_REFERENCE_TYPE_CASTER, TARGET_SELECT_CATEGORY_NEARBY,  TARGET_CHECK_ALLY,     TARGET_DIR_NONE},        // 3 TARGET_UNIT_NEARBY_PARTY
    {TARGET_OBJECT_TYPE_UNIT, TARGET_REFERENCE_TYPE_CASTER, TARGET_SELECT_CATEGORY_NEARBY,  TARGET_CHECK_PARTY,    TARGET_DIR_NONE},        // 4 TARGET_UNIT_NEARBY_ALLY
    {TARGET_OBJECT_TYPE_UNIT, TARGET_REFERENCE_TYPE_CASTER, TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_DEFAULT,  TARGET_DIR_NONE},        // 5 TARGET_UNIT_PET
    {TARGET_OBJECT_TYPE_UNIT, TARGET_REFERENCE_TYPE_TARGET, TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_ENEMY,    TARGET_DIR_NONE},        // 6 TARGET_UNIT_TARGET_ENEMY
    {TARGET_OBJECT_TYPE_UNIT, TARGET_REFERENCE_TYPE_SRC,    TARGET_SELECT_CATEGORY_AREA,    TARGET_CHECK_ENTRY,    TARGET_DIR_NONE},        // 7 TARGET_UNIT_SRC_AREA_ENTRY
    {TARGET_OBJECT_TYPE_UNIT, TARGET_REFERENCE_TYPE_DEST,   TARGET_SELECT_CATEGORY_AREA,    TARGET_CHECK_ENTRY,    TARGET_DIR_NONE},        // 8 TARGET_UNIT_DEST_AREA_ENTRY
    {TARGET_OBJECT_TYPE_DEST, TARGET_REFERENCE_TYPE_CASTER, TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_DEFAULT,  TARGET_DIR_NONE},        // 9 TARGET_DEST_HOME
    {TARGET_OBJECT_TYPE_NONE, TARGET_REFERENCE_TYPE_NONE,   TARGET_SELECT_CATEGORY_NYI,     TARGET_CHECK_DEFAULT,  TARGET_DIR_NONE},        // 10
    {TARGET_OBJECT_TYPE_UNIT, TARGET_REFERENCE_TYPE_SRC,    TARGET_SELECT_CATEGORY_NYI,     TARGET_CHECK_DEFAULT,  TARGET_DIR_NONE},        // 11 TARGET_UNIT_SRC_AREA_UNK_11
    {TARGET_OBJECT_TYPE_NONE, TARGET_REFERENCE_TYPE_NONE,   TARGET_SELECT_CATEGORY_NYI,     TARGET_CHECK_DEFAULT,  TARGET_DIR_NONE},        // 12
    {TARGET_OBJECT_TYPE_NONE, TARGET_REFERENCE_TYPE_NONE,   TARGET_SELECT_CATEGORY_NYI,     TARGET_CHECK_DEFAULT,  TARGET_DIR_NONE},        // 13
    {TARGET_OBJECT_TYPE_NONE, TARGET_REFERENCE_TYPE_NONE,   TARGET_SELECT_CATEGORY_NYI,     TARGET_CHECK_DEFAULT,  TARGET_DIR_NONE},        // 14
    {TARGET_OBJECT_TYPE_UNIT, TARGET_REFERENCE_TYPE_SRC,    TARGET_SELECT_CATEGORY_AREA,    TARGET_CHECK_ENEMY,    TARGET_DIR_NONE},        // 15 TARGET_UNIT_SRC_AREA_ENEMY
    {TARGET_OBJECT_TYPE_UNIT, TARGET_REFERENCE_TYPE_DEST,   TARGET_SELECT_CATEGORY_AREA,    TARGET_CHECK_ENEMY,    TARGET_DIR_NONE},        // 16 TARGET_UNIT_DEST_AREA_ENEMY
    {TARGET_OBJECT_TYPE_DEST, TARGET_REFERENCE_TYPE_CASTER, TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_DEFAULT,  TARGET_DIR_NONE},        // 17 TARGET_DEST_DB
    {TARGET_OBJECT_TYPE_DEST, TARGET_REFERENCE_TYPE_CASTER, TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_DEFAULT,  TARGET_DIR_NONE},        // 18 TARGET_DEST_CASTER
    {TARGET_OBJECT_TYPE_NONE, TARGET_REFERENCE_TYPE_NONE,   TARGET_SELECT_CATEGORY_NYI,     TARGET_CHECK_DEFAULT,  TARGET_DIR_NONE},        // 19
    {TARGET_OBJECT_TYPE_UNIT, TARGET_REFERENCE_TYPE_CASTER, TARGET_SELECT_CATEGORY_AREA,    TARGET_CHECK_PARTY,    TARGET_DIR_NONE},        // 20 TARGET_UNIT_CASTER_AREA_PARTY
    {TARGET_OBJECT_TYPE_UNIT, TARGET_REFERENCE_TYPE_TARGET, TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_ALLY,     TARGET_DIR_NONE},        // 21 TARGET_UNIT_TARGET_ALLY
    {TARGET_OBJECT_TYPE_SRC,  TARGET_REFERENCE_TYPE_CASTER, TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_DEFAULT,  TARGET_DIR_NONE},        // 22 TARGET_SRC_CASTER
    {TARGET_OBJECT_TYPE_GOBJ, TARGET_REFERENCE_TYPE_TARGET, TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_DEFAULT,  TARGET_DIR_NONE},        // 23 TARGET_GAMEOBJECT_TARGET
    {TARGET_OBJECT_TYPE_UNIT, TARGET_REFERENCE_TYPE_CASTER, TARGET_SELECT_CATEGORY_CONE,    TARGET_CHECK_ENEMY,    TARGET_DIR_FRONT},       // 24 TARGET_UNIT_CONE_ENEMY_24
    {TARGET_OBJECT_TYPE_UNIT, TARGET_REFERENCE_TYPE_TARGET, TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_DEFAULT,  TARGET_DIR_NONE},        // 25 TARGET_UNIT_TARGET_ANY
    {TARGET_OBJECT_TYPE_GOBJ_ITEM, TARGET_REFERENCE_TYPE_TARGET, TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_DEFAULT, TARGET_DIR_NONE},    // 26 TARGET_GAMEOBJECT_ITEM_TARGET
    {TARGET_OBJECT_TYPE_UNIT, TARGET_REFERENCE_TYPE_CASTER, TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_DEFAULT,  TARGET_DIR_NONE},        // 27 TARGET_UNIT_MASTER
    {TARGET_OBJECT_TYPE_DEST, TARGET_REFERENCE_TYPE_DEST,   TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_ENEMY,    TARGET_DIR_NONE},        // 28 TARGET_DEST_DYNOBJ_ENEMY
    {TARGET_OBJECT_TYPE_DEST, TARGET_REFERENCE_TYPE_DEST,   TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_ALLY,     TARGET_DIR_NONE},        // 29 TARGET_DEST_DYNOBJ_ALLY
    {TARGET_OBJECT_TYPE_UNIT, TARGET_REFERENCE_TYPE_SRC,    TARGET_SELECT_CATEGORY_AREA,    TARGET_CHECK_ALLY,     TARGET_DIR_NONE},        // 30 TARGET_UNIT_SRC_AREA_ALLY
    {TARGET_OBJECT_TYPE_UNIT, TARGET_REFERENCE_TYPE_DEST,   TARGET_SELECT_CATEGORY_AREA,    TARGET_CHECK_ALLY,     TARGET_DIR_NONE},        // 31 TARGET_UNIT_DEST_AREA_ALLY
    {TARGET_OBJECT_TYPE_DEST, TARGET_REFERENCE_TYPE_CASTER, TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_DEFAULT,  TARGET_DIR_FRONT_LEFT},  // 32 TARGET_DEST_CASTER_SUMMON
    {TARGET_OBJECT_TYPE_UNIT, TARGET_REFERENCE_TYPE_SRC,    TARGET_SELECT_CATEGORY_AREA,    TARGET_CHECK_PARTY,    TARGET_DIR_NONE},        // 33 TARGET_UNIT_SRC_AREA_PARTY
    {TARGET_OBJECT_TYPE_UNIT, TARGET_REFERENCE_TYPE_DEST,   TARGET_SELECT_CATEGORY_AREA,    TARGET_CHECK_PARTY,    TARGET_DIR_NONE},        // 34 TARGET_UNIT_DEST_AREA_PARTY
    {TARGET_OBJECT_TYPE_UNIT, TARGET_REFERENCE_TYPE_TARGET, TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_PARTY,    TARGET_DIR_NONE},        // 35 TARGET_UNIT_TARGET_PARTY
    {TARGET_OBJECT_TYPE_DEST, TARGET_REFERENCE_TYPE_CASTER, TARGET_SELECT_CATEGORY_NYI,     TARGET_CHECK_DEFAULT,  TARGET_DIR_NONE},        // 36 TARGET_DEST_CASTER_UNK_36
    {TARGET_OBJECT_TYPE_UNIT, TARGET_REFERENCE_TYPE_LAST,   TARGET_SELECT_CATEGORY_AREA,    TARGET_CHECK_PARTY,    TARGET_DIR_NONE},        // 37 TARGET_UNIT_LASTTARGET_AREA_PARTY
    {TARGET_OBJECT_TYPE_UNIT, TARGET_REFERENCE_TYPE_CASTER, TARGET_SELECT_CATEGORY_NEARBY,  TARGET_CHECK_ENTRY,    TARGET_DIR_NONE},        // 38 TARGET_UNIT_NEARBY_ENTRY
    {TARGET_OBJECT_TYPE_DEST, TARGET_REFERENCE_TYPE_CASTER, TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_DEFAULT,  TARGET_DIR_NONE},        // 39 TARGET_DEST_CASTER_FISHING
    {TARGET_OBJECT_TYPE_GOBJ, TARGET_REFERENCE_TYPE_CASTER, TARGET_SELECT_CATEGORY_NEARBY,  TARGET_CHECK_ENTRY,    TARGET_DIR_NONE},        // 40 TARGET_GAMEOBJECT_NEARBY_ENTRY
    {TARGET_OBJECT_TYPE_DEST, TARGET_REFERENCE_TYPE_CASTER, TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_DEFAULT,  TARGET_DIR_FRONT_RIGHT}, // 41 TARGET_DEST_CASTER_FRONT_RIGHT
    {TARGET_OBJECT_TYPE_DEST, TARGET_REFERENCE_TYPE_CASTER, TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_DEFAULT,  TARGET_DIR_BACK_RIGHT},  // 42 TARGET_DEST_CASTER_BACK_RIGHT
    {TARGET_OBJECT_TYPE_DEST, TARGET_REFERENCE_TYPE_CASTER, TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_DEFAULT,  TARGET_DIR_BACK_LEFT},   // 43 TARGET_DEST_CASTER_BACK_LEFT
    {TARGET_OBJECT_TYPE_DEST, TARGET_REFERENCE_TYPE_CASTER, TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_DEFAULT,  TARGET_DIR_FRONT_LEFT},  // 44 TARGET_DEST_CASTER_FRONT_LEFT
    {TARGET_OBJECT_TYPE_UNIT, TARGET_REFERENCE_TYPE_TARGET, TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_ALLY,     TARGET_DIR_NONE},        // 45 TARGET_UNIT_TARGET_CHAINHEAL_ALLY
    {TARGET_OBJECT_TYPE_DEST, TARGET_REFERENCE_TYPE_CASTER, TARGET_SELECT_CATEGORY_NEARBY,  TARGET_CHECK_ENTRY,    TARGET_DIR_NONE},        // 46 TARGET_DEST_NEARBY_ENTRY
    {TARGET_OBJECT_TYPE_DEST, TARGET_REFERENCE_TYPE_CASTER, TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_DEFAULT,  TARGET_DIR_FRONT},       // 47 TARGET_DEST_CASTER_FRONT
    {TARGET_OBJECT_TYPE_DEST, TARGET_REFERENCE_TYPE_CASTER, TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_DEFAULT,  TARGET_DIR_BACK},        // 48 TARGET_DEST_CASTER_BACK
    {TARGET_OBJECT_TYPE_DEST, TARGET_REFERENCE_TYPE_CASTER, TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_DEFAULT,  TARGET_DIR_RIGHT},       // 49 TARGET_DEST_CASTER_RIGHT
    {TARGET_OBJECT_TYPE_DEST, TARGET_REFERENCE_TYPE_CASTER, TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_DEFAULT,  TARGET_DIR_LEFT},        // 50 TARGET_DEST_CASTER_LEFT
    {TARGET_OBJECT_TYPE_GOBJ, TARGET_REFERENCE_TYPE_SRC,    TARGET_SELECT_CATEGORY_AREA,    TARGET_CHECK_DEFAULT,  TARGET_DIR_NONE},        // 51 TARGET_GAMEOBJECT_SRC_AREA
    {TARGET_OBJECT_TYPE_GOBJ, TARGET_REFERENCE_TYPE_DEST,   TARGET_SELECT_CATEGORY_AREA,    TARGET_CHECK_DEFAULT,  TARGET_DIR_NONE},        // 52 TARGET_GAMEOBJECT_DEST_AREA
    {TARGET_OBJECT_TYPE_DEST, TARGET_REFERENCE_TYPE_TARGET, TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_ENEMY,    TARGET_DIR_NONE},        // 53 TARGET_DEST_TARGET_ENEMY
    {TARGET_OBJECT_TYPE_UNIT, TARGET_REFERENCE_TYPE_CASTER, TARGET_SELECT_CATEGORY_CONE,    TARGET_CHECK_ENEMY,    TARGET_DIR_FRONT},       // 54 TARGET_UNIT_CONE_ENEMY_54
    {TARGET_OBJECT_TYPE_DEST, TARGET_REFERENCE_TYPE_CASTER, TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_DEFAULT,  TARGET_DIR_NONE},        // 55 TARGET_DEST_CASTER_FRONT_LEAP
    {TARGET_OBJECT_TYPE_UNIT, TARGET_REFERENCE_TYPE_CASTER, TARGET_SELECT_CATEGORY_AREA,    TARGET_CHECK_RAID,     TARGET_DIR_NONE},        // 56 TARGET_UNIT_CASTER_AREA_RAID
    {TARGET_OBJECT_TYPE_UNIT, TARGET_REFERENCE_TYPE_TARGET, TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_RAID,     TARGET_DIR_NONE},        // 57 TARGET_UNIT_TARGET_RAID
    {TARGET_OBJECT_TYPE_UNIT, TARGET_REFERENCE_TYPE_CASTER, TARGET_SELECT_CATEGORY_NEARBY,  TARGET_CHECK_RAID,     TARGET_DIR_NONE},        // 58 TARGET_UNIT_NEARBY_RAID
    {TARGET_OBJECT_TYPE_UNIT, TARGET_REFERENCE_TYPE_CASTER, TARGET_SELECT_CATEGORY_CONE,    TARGET_CHECK_ALLY,     TARGET_DIR_FRONT},       // 59 TARGET_UNIT_CONE_ALLY
    {TARGET_OBJECT_TYPE_UNIT, TARGET_REFERENCE_TYPE_CASTER, TARGET_SELECT_CATEGORY_CONE,    TARGET_CHECK_ENTRY,    TARGET_DIR_FRONT},       // 60 TARGET_UNIT_CONE_ENTRY
    {TARGET_OBJECT_TYPE_UNIT, TARGET_REFERENCE_TYPE_TARGET, TARGET_SELECT_CATEGORY_AREA,    TARGET_CHECK_RAID_CLASS, TARGET_DIR_NONE},      // 61 TARGET_UNIT_TARGET_AREA_RAID_CLASS
    {TARGET_OBJECT_TYPE_NONE, TARGET_REFERENCE_TYPE_NONE,   TARGET_SELECT_CATEGORY_NYI,     TARGET_CHECK_DEFAULT,  TARGET_DIR_NONE},        // 62 TARGET_UNK_62
    {TARGET_OBJECT_TYPE_DEST, TARGET_REFERENCE_TYPE_TARGET, TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_DEFAULT,  TARGET_DIR_NONE},        // 63 TARGET_DEST_TARGET_ANY
    {TARGET_OBJECT_TYPE_DEST, TARGET_REFERENCE_TYPE_TARGET, TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_DEFAULT,  TARGET_DIR_FRONT},       // 64 TARGET_DEST_TARGET_FRONT
    {TARGET_OBJECT_TYPE_DEST, TARGET_REFERENCE_TYPE_TARGET, TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_DEFAULT,  TARGET_DIR_BACK},        // 65 TARGET_DEST_TARGET_BACK
    {TARGET_OBJECT_TYPE_DEST, TARGET_REFERENCE_TYPE_TARGET, TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_DEFAULT,  TARGET_DIR_RIGHT},       // 66 TARGET_DEST_TARGET_RIGHT
    {TARGET_OBJECT_TYPE_DEST, TARGET_REFERENCE_TYPE_TARGET, TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_DEFAULT,  TARGET_DIR_LEFT},        // 67 TARGET_DEST_TARGET_LEFT
    {TARGET_OBJECT_TYPE_DEST, TARGET_REFERENCE_TYPE_TARGET, TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_DEFAULT,  TARGET_DIR_FRONT_RIGHT}, // 68 TARGET_DEST_TARGET_FRONT_RIGHT
    {TARGET_OBJECT_TYPE_DEST, TARGET_REFERENCE_TYPE_TARGET, TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_DEFAULT,  TARGET_DIR_BACK_RIGHT},  // 69 TARGET_DEST_TARGET_BACK_RIGHT
    {TARGET_OBJECT_TYPE_DEST, TARGET_REFERENCE_TYPE_TARGET, TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_DEFAULT,  TARGET_DIR_BACK_LEFT},   // 70 TARGET_DEST_TARGET_BACK_LEFT
    {TARGET_OBJECT_TYPE_DEST, TARGET_REFERENCE_TYPE_TARGET, TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_DEFAULT,  TARGET_DIR_FRONT_LEFT},  // 71 TARGET_DEST_TARGET_FRONT_LEFT
    {TARGET_OBJECT_TYPE_DEST, TARGET_REFERENCE_TYPE_CASTER, TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_DEFAULT,  TARGET_DIR_RANDOM},      // 72 TARGET_DEST_CASTER_RANDOM
    {TARGET_OBJECT_TYPE_DEST, TARGET_REFERENCE_TYPE_CASTER, TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_DEFAULT,  TARGET_DIR_RANDOM},      // 73 TARGET_DEST_CASTER_RADIUS
    {TARGET_OBJECT_TYPE_DEST, TARGET_REFERENCE_TYPE_TARGET, TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_DEFAULT,  TARGET_DIR_RANDOM},      // 74 TARGET_DEST_TARGET_RANDOM
    {TARGET_OBJECT_TYPE_DEST, TARGET_REFERENCE_TYPE_TARGET, TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_DEFAULT,  TARGET_DIR_RANDOM},      // 75 TARGET_DEST_TARGET_RADIUS
    {TARGET_OBJECT_TYPE_DEST, TARGET_REFERENCE_TYPE_CASTER, TARGET_SELECT_CATEGORY_CHANNEL, TARGET_CHECK_DEFAULT,  TARGET_DIR_NONE},        // 76 TARGET_DEST_CHANNEL_TARGET
    {TARGET_OBJECT_TYPE_UNIT, TARGET_REFERENCE_TYPE_CASTER, TARGET_SELECT_CATEGORY_CHANNEL, TARGET_CHECK_DEFAULT,  TARGET_DIR_NONE},        // 77 TARGET_UNIT_CHANNEL_TARGET
    {TARGET_OBJECT_TYPE_DEST, TARGET_REFERENCE_TYPE_DEST,   TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_DEFAULT,  TARGET_DIR_FRONT},       // 78 TARGET_DEST_DEST_FRONT
    {TARGET_OBJECT_TYPE_DEST, TARGET_REFERENCE_TYPE_DEST,   TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_DEFAULT,  TARGET_DIR_BACK},        // 79 TARGET_DEST_DEST_BACK
    {TARGET_OBJECT_TYPE_DEST, TARGET_REFERENCE_TYPE_DEST,   TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_DEFAULT,  TARGET_DIR_RIGHT},       // 80 TARGET_DEST_DEST_RIGHT
    {TARGET_OBJECT_TYPE_DEST, TARGET_REFERENCE_TYPE_DEST,   TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_DEFAULT,  TARGET_DIR_LEFT},        // 81 TARGET_DEST_DEST_LEFT
    {TARGET_OBJECT_TYPE_DEST, TARGET_REFERENCE_TYPE_DEST,   TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_DEFAULT,  TARGET_DIR_FRONT_RIGHT}, // 82 TARGET_DEST_DEST_FRONT_RIGHT
    {TARGET_OBJECT_TYPE_DEST, TARGET_REFERENCE_TYPE_DEST,   TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_DEFAULT,  TARGET_DIR_BACK_RIGHT},  // 83 TARGET_DEST_DEST_BACK_RIGHT
    {TARGET_OBJECT_TYPE_DEST, TARGET_REFERENCE_TYPE_DEST,   TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_DEFAULT,  TARGET_DIR_BACK_LEFT},   // 84 TARGET_DEST_DEST_BACK_LEFT
    {TARGET_OBJECT_TYPE_DEST, TARGET_REFERENCE_TYPE_DEST,   TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_DEFAULT,  TARGET_DIR_FRONT_LEFT},  // 85 TARGET_DEST_DEST_FRONT_LEFT
    {TARGET_OBJECT_TYPE_DEST, TARGET_REFERENCE_TYPE_DEST,   TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_DEFAULT,  TARGET_DIR_RANDOM},      // 86 TARGET_DEST_DEST_RANDOM
    {TARGET_OBJECT_TYPE_DEST, TARGET_REFERENCE_TYPE_DEST,   TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_DEFAULT,  TARGET_DIR_NONE},        // 87 TARGET_DEST_DEST
    {TARGET_OBJECT_TYPE_DEST, TARGET_REFERENCE_TYPE_DEST,   TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_DEFAULT,  TARGET_DIR_NONE},        // 88 TARGET_DEST_DYNOBJ_NONE
    {TARGET_OBJECT_TYPE_DEST, TARGET_REFERENCE_TYPE_DEST,   TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_DEFAULT,  TARGET_DIR_NONE},        // 89 TARGET_DEST_TRAJ
    {TARGET_OBJECT_TYPE_UNIT, TARGET_REFERENCE_TYPE_TARGET, TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_DEFAULT,  TARGET_DIR_NONE},        // 90 TARGET_UNIT_TARGET_MINIPET
    {TARGET_OBJECT_TYPE_DEST, TARGET_REFERENCE_TYPE_DEST,   TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_DEFAULT,  TARGET_DIR_RANDOM},      // 91 TARGET_DEST_DEST_RADIUS
    {TARGET_OBJECT_TYPE_UNIT, TARGET_REFERENCE_TYPE_CASTER, TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_DEFAULT,  TARGET_DIR_NONE},        // 92 TARGET_UNIT_SUMMONER
    {TARGET_OBJECT_TYPE_CORPSE, TARGET_REFERENCE_TYPE_SRC,   TARGET_SELECT_CATEGORY_AREA,   TARGET_CHECK_ENEMY,    TARGET_DIR_NONE},        // 93 TARGET_CORPSE_SRC_AREA_ENEMY
    {TARGET_OBJECT_TYPE_UNIT, TARGET_REFERENCE_TYPE_CASTER, TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_DEFAULT,  TARGET_DIR_NONE},        // 94 TARGET_UNIT_VEHICLE
    {TARGET_OBJECT_TYPE_UNIT, TARGET_REFERENCE_TYPE_TARGET, TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_PASSENGER, TARGET_DIR_NONE},       // 95 TARGET_UNIT_TARGET_PASSENGER
    {TARGET_OBJECT_TYPE_UNIT, TARGET_REFERENCE_TYPE_CASTER, TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_DEFAULT,  TARGET_DIR_NONE},        // 96 TARGET_UNIT_PASSENGER_0
    {TARGET_OBJECT_TYPE_UNIT, TARGET_REFERENCE_TYPE_CASTER, TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_DEFAULT,  TARGET_DIR_NONE},        // 97 TARGET_UNIT_PASSENGER_1
    {TARGET_OBJECT_TYPE_UNIT, TARGET_REFERENCE_TYPE_CASTER, TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_DEFAULT,  TARGET_DIR_NONE},        // 98 TARGET_UNIT_PASSENGER_2
    {TARGET_OBJECT_TYPE_UNIT, TARGET_REFERENCE_TYPE_CASTER, TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_DEFAULT,  TARGET_DIR_NONE},        // 99 TARGET_UNIT_PASSENGER_3
    {TARGET_OBJECT_TYPE_UNIT, TARGET_REFERENCE_TYPE_CASTER, TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_DEFAULT,  TARGET_DIR_NONE},        // 100 TARGET_UNIT_PASSENGER_4
    {TARGET_OBJECT_TYPE_UNIT, TARGET_REFERENCE_TYPE_CASTER, TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_DEFAULT,  TARGET_DIR_NONE},        // 101 TARGET_UNIT_PASSENGER_5
    {TARGET_OBJECT_TYPE_UNIT, TARGET_REFERENCE_TYPE_CASTER, TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_DEFAULT,  TARGET_DIR_NONE},        // 102 TARGET_UNIT_PASSENGER_6
    {TARGET_OBJECT_TYPE_UNIT, TARGET_REFERENCE_TYPE_CASTER, TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_DEFAULT,  TARGET_DIR_NONE},        // 103 TARGET_UNIT_PASSENGER_7
    {TARGET_OBJECT_TYPE_UNIT, TARGET_REFERENCE_TYPE_CASTER, TARGET_SELECT_CATEGORY_CONE,    TARGET_CHECK_ENEMY,    TARGET_DIR_FRONT},       // 104 TARGET_UNIT_CONE_ENEMY_104
    {TARGET_OBJECT_TYPE_UNIT, TARGET_REFERENCE_TYPE_NONE,   TARGET_SELECT_CATEGORY_NYI,     TARGET_CHECK_DEFAULT,  TARGET_DIR_NONE},        // 105 TARGET_UNIT_UNK_105
    {TARGET_OBJECT_TYPE_DEST, TARGET_REFERENCE_TYPE_CASTER, TARGET_SELECT_CATEGORY_CHANNEL, TARGET_CHECK_DEFAULT,  TARGET_DIR_NONE},        // 106 TARGET_DEST_CHANNEL_CASTER
    {TARGET_OBJECT_TYPE_NONE, TARGET_REFERENCE_TYPE_DEST,   TARGET_SELECT_CATEGORY_NYI,     TARGET_CHECK_DEFAULT,  TARGET_DIR_NONE},        // 107 TARGET_UNK_DEST_AREA_UNK_107
    {TARGET_OBJECT_TYPE_GOBJ, TARGET_REFERENCE_TYPE_CASTER, TARGET_SELECT_CATEGORY_CONE,    TARGET_CHECK_DEFAULT,  TARGET_DIR_FRONT},       // 108 TARGET_GAMEOBJECT_CONE_108
    {TARGET_OBJECT_TYPE_NONE, TARGET_REFERENCE_TYPE_NONE,   TARGET_SELECT_CATEGORY_CONE,    TARGET_CHECK_DEFAULT,  TARGET_DIR_FRONT},       // 109 TARGET_GAMEOBJECT_CONE_109
    {TARGET_OBJECT_TYPE_UNIT, TARGET_REFERENCE_TYPE_CASTER, TARGET_SELECT_CATEGORY_CONE,    TARGET_CHECK_ENTRY,    TARGET_DIR_FRONT},       // 110 TARGET_UNIT_CONE_ENTRY_110
    {TARGET_OBJECT_TYPE_NONE, TARGET_REFERENCE_TYPE_NONE,   TARGET_SELECT_CATEGORY_NYI,     TARGET_CHECK_DEFAULT,  TARGET_DIR_NONE},        // 111
    {TARGET_OBJECT_TYPE_DEST, TARGET_REFERENCE_TYPE_TARGET, TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_DEFAULT,  TARGET_DIR_NONE},        // 112 TARGET_DEST_TARGET_ANY_2
    {TARGET_OBJECT_TYPE_NONE, TARGET_REFERENCE_TYPE_NONE,   TARGET_SELECT_CATEGORY_NYI,     TARGET_CHECK_DEFAULT,  TARGET_DIR_NONE},        // 113
    {TARGET_OBJECT_TYPE_NONE, TARGET_REFERENCE_TYPE_NONE,   TARGET_SELECT_CATEGORY_NYI,     TARGET_CHECK_DEFAULT,  TARGET_DIR_NONE},        // 114
    {TARGET_OBJECT_TYPE_NONE, TARGET_REFERENCE_TYPE_NONE,   TARGET_SELECT_CATEGORY_NYI,     TARGET_CHECK_DEFAULT,  TARGET_DIR_NONE},        // 115
    {TARGET_OBJECT_TYPE_NONE, TARGET_REFERENCE_TYPE_NONE,   TARGET_SELECT_CATEGORY_NYI,     TARGET_CHECK_DEFAULT,  TARGET_DIR_NONE},        // 116
    {TARGET_OBJECT_TYPE_NONE, TARGET_REFERENCE_TYPE_NONE,   TARGET_SELECT_CATEGORY_NYI,     TARGET_CHECK_DEFAULT,  TARGET_DIR_NONE},        // 117
    {TARGET_OBJECT_TYPE_UNIT, TARGET_REFERENCE_TYPE_CASTER, TARGET_SELECT_CATEGORY_AREA,    TARGET_CHECK_RAID,     TARGET_DIR_NONE},        // 118 TARGET_UNIT_TARGET_ALLY_OR_RAID
    {TARGET_OBJECT_TYPE_CORPSE, TARGET_REFERENCE_TYPE_CASTER, TARGET_SELECT_CATEGORY_AREA,  TARGET_CHECK_RAID,     TARGET_DIR_NONE},        // 119 TARGET_CORPSE_SRC_AREA_RAID
    {TARGET_OBJECT_TYPE_UNIT, TARGET_REFERENCE_TYPE_CASTER, TARGET_SELECT_CATEGORY_AREA,    TARGET_CHECK_SUMMONED, TARGET_DIR_NONE},        // 120 TARGET_UNIT_SELF_AND_SUMMONS
    {TARGET_OBJECT_TYPE_NONE, TARGET_REFERENCE_TYPE_NONE,   TARGET_SELECT_CATEGORY_NYI,     TARGET_CHECK_DEFAULT,  TARGET_DIR_NONE},        // 121
    {TARGET_OBJECT_TYPE_UNIT, TARGET_REFERENCE_TYPE_CASTER, TARGET_SELECT_CATEGORY_AREA,    TARGET_CHECK_THREAT,   TARGET_DIR_NONE},        // 122 TARGET_UNIT_AREA_THREAT_LIST
    {TARGET_OBJECT_TYPE_UNIT, TARGET_REFERENCE_TYPE_CASTER, TARGET_SELECT_CATEGORY_AREA,    TARGET_CHECK_TAP,      TARGET_DIR_NONE},        // 123 TARGET_UNIT_AREA_TAP_LIST
    {TARGET_OBJECT_TYPE_NONE, TARGET_REFERENCE_TYPE_NONE,   TARGET_SELECT_CATEGORY_NYI,     TARGET_CHECK_DEFAULT,  TARGET_DIR_NONE},        // 124
    {TARGET_OBJECT_TYPE_DEST, TARGET_REFERENCE_TYPE_CASTER, TARGET_SELECT_CATEGORY_DEFAULT, TARGET_CHECK_DEFAULT,  TARGET_DIR_NONE},        // 125 TARGET_DEST_CASTER_FLOOR
    {TARGET_OBJECT_TYPE_UNIT, TARGET_REFERENCE_TYPE_CASTER, TARGET_SELECT_CATEGORY_AREA,    TARGET_CHECK_ENEMY,    TARGET_DIR_NONE },       // 126 TARGET_UNIT_AREA_ENEMY
    {TARGET_OBJECT_TYPE_NONE, TARGET_REFERENCE_TYPE_NONE,   TARGET_SELECT_CATEGORY_NYI,     TARGET_CHECK_DEFAULT,  TARGET_DIR_NONE},        // 127
};

SpellEffectInfo::SpellEffectInfo(SpellEntry const* /*spellEntry*/, SpellInfo const* spellInfo, uint8 effIndex, SpellEffectEntry const* _effect)
{
    SpellScalingEntry const* scaling = spellInfo->GetSpellScaling();

    _spellInfo = spellInfo;
    _effIndex = _effect ? _effect->EffectIndex : effIndex;
    Effect = _effect ? _effect->Effect : 0;
    ApplyAuraName = _effect ? _effect->EffectAura : 0;
    AuraPeriod = _effect ? _effect->EffectAuraPeriod : 0;
    DieSides = _effect ? _effect->EffectDieSides : 0;
    RealPointsPerLevel = _effect ? _effect->EffectRealPointsPerLevel : 0.0f;
    BasePoints = _effect ? _effect->EffectBasePoints : 0;
    PointsPerComboPoint = _effect ? _effect->EffectPointsPerResource : 0.0f;
    Amplitude = _effect ? _effect->EffectAmplitude : 0.0f;
    DamageMultiplier = _effect ? _effect->EffectChainAmplitude : 0.0f;
    BonusMultiplier = _effect ? _effect->EffectBonusCoefficient : 0.0f;
    MiscValue = _effect ? _effect->EffectMiscValue : 0;
    MiscValueB = _effect ? _effect->EffectMiscValueB : 0;
    Mechanic = Mechanics(_effect ? _effect->EffectMechanic : 0);
    TargetA = SpellImplicitTargetInfo(_effect ? _effect->EffectImplicitTargetA : 0);
    TargetB = SpellImplicitTargetInfo(_effect ? _effect->EffectImplicitTargetB : 0);
    RadiusEntry = _effect && _effect->EffectRadiusIndex ? sSpellRadiusStore.LookupEntry(_effect->EffectRadiusIndex) : nullptr;
    MaxRadiusEntry = _effect && _effect->EffectRadiusMaxIndex ? sSpellRadiusStore.LookupEntry(_effect->EffectRadiusMaxIndex) : nullptr;
    ChainTarget = _effect ? _effect->EffectChainTargets : 0;
    ItemType = _effect ? _effect->EffectItemType : 0;
    TriggerSpell = _effect ? _effect->EffectTriggerSpell : 0;
    SpellClassMask = _effect ? _effect->EffectSpellClassMask : flag96(0);
    ImplicitTargetConditions = nullptr;
    ScalingMultiplier = scaling ? scaling->Coefficient[_effIndex] : 0.0f;
    DeltaScalingMultiplier = scaling ? scaling->Variance[_effIndex] : 0.0f;
    ComboScalingMultiplier = scaling ? scaling->ComboPointsCoefficient[_effIndex] : 0.0f;
}

bool SpellEffectInfo::IsEffect() const
{
    return Effect != 0;
}

bool SpellEffectInfo::IsEffect(SpellEffects effectName) const
{
    return Effect == uint32(effectName);
}

bool SpellEffectInfo::IsAura() const
{
    return (IsUnitOwnedAuraEffect() || Effect == SPELL_EFFECT_PERSISTENT_AREA_AURA) && ApplyAuraName != 0;
}

bool SpellEffectInfo::IsAura(AuraType aura) const
{
    return IsAura() && ApplyAuraName == uint32(aura);
}

bool SpellEffectInfo::IsTargetingArea() const
{
    return TargetA.IsArea() || TargetB.IsArea();
}

bool SpellEffectInfo::IsAreaAuraEffect() const
{
    if (Effect == SPELL_EFFECT_APPLY_AREA_AURA_PARTY    ||
        Effect == SPELL_EFFECT_APPLY_AREA_AURA_RAID     ||
        Effect == SPELL_EFFECT_APPLY_AREA_AURA_FRIEND   ||
        Effect == SPELL_EFFECT_APPLY_AREA_AURA_ENEMY    ||
        Effect == SPELL_EFFECT_APPLY_AREA_AURA_PET      ||
        Effect == SPELL_EFFECT_APPLY_AREA_AURA_OWNER)
        return true;
    return false;
}

bool SpellEffectInfo::IsFarUnitTargetEffect() const
{
    return (Effect == SPELL_EFFECT_SUMMON_PLAYER)
        || (Effect == SPELL_EFFECT_SUMMON_RAF_FRIEND)
        || (Effect == SPELL_EFFECT_RESURRECT)
        || (Effect == SPELL_EFFECT_RESURRECT_NEW);
}

bool SpellEffectInfo::IsFarDestTargetEffect() const
{
    return Effect == SPELL_EFFECT_TELEPORT_UNITS;
}

bool SpellEffectInfo::IsUnitOwnedAuraEffect() const
{
    return IsAreaAuraEffect() || Effect == SPELL_EFFECT_APPLY_AURA || Effect == SPELL_EFFECT_APPLY_AURA_2;
}

uint32 SpellEffectInfo::CalcPeriod(Unit* caster, Spell* spell /* = nullptr */) const
{
    // Default value found in client.
    uint32 period = AuraPeriod;
    if (period == 0)
        period = 5200;

    if (caster != nullptr)
    {
        if (Player* modOwner = caster->GetSpellModOwner())
            modOwner->ApplySpellMod(_spellInfo->Id, SPELLMOD_ACTIVATION_TIME, period, spell);

        if (_spellInfo->HasAttribute(SPELL_ATTR5_SPELL_HASTE_AFFECTS_PERIODIC) && !_spellInfo->HasAttribute(SPELL_ATTR3_NO_DONE_BONUS))
            period = int32(period * caster->GetFloatValue(UNIT_MOD_CAST_HASTE));
    }

    return period;
}

int32 SpellEffectInfo::CalcValue(Unit const* caster, int32 const* bp, Unit const* target) const
{
    float basePointsPerLevel = RealPointsPerLevel;
    int32 basePoints = bp ? *bp : BasePoints;
    float comboDamage = PointsPerComboPoint;

    // base amount modification based on spell lvl vs caster lvl
    if (ScalingMultiplier != 0.0f)
    {
        if (caster)
        {
            int32 level = caster->getLevel();
            if (target && _spellInfo->IsPositiveEffect(_effIndex) && (Effect == SPELL_EFFECT_APPLY_AURA || Effect == SPELL_EFFECT_APPLY_AURA_2))
                level = target->getLevel();

            if (GtSpellScalingEntry const* gtScaling = sGtSpellScalingStore.LookupEntry((_spellInfo->ScalingClass != -1 ? _spellInfo->ScalingClass - 1 : MAX_CLASSES - 1) * 100 + level - 1))
            {
                float multiplier = gtScaling->value;
                if (_spellInfo->CastTimeMax > 0 && _spellInfo->CastTimeMaxLevel > level)
                    multiplier *= float(_spellInfo->CastTimeMin + (level - 1) * (_spellInfo->CastTimeMax - _spellInfo->CastTimeMin) / (_spellInfo->CastTimeMaxLevel - 1)) / float(_spellInfo->CastTimeMax);
                if (_spellInfo->CoefLevelBase > level)
                    multiplier *= (1.0f - _spellInfo->CoefBase) * (float)(level - 1) / (float)(_spellInfo->CoefLevelBase - 1) + _spellInfo->CoefBase;

                float preciseBasePoints = ScalingMultiplier * multiplier;
                if (DeltaScalingMultiplier)
                {
                    float delta = fabs(DeltaScalingMultiplier * ScalingMultiplier * multiplier * 0.5f);
                    preciseBasePoints += frand(-delta, delta);
                }

                basePoints = int32(preciseBasePoints);

                if (ComboScalingMultiplier)
                    comboDamage = ComboScalingMultiplier * multiplier;
            }
        }
    }
    else
    {
        if (caster)
        {
            int32 level = int32(caster->getLevel());
            if (level > int32(_spellInfo->MaxLevel) && _spellInfo->MaxLevel > 0)
                level = int32(_spellInfo->MaxLevel);
            else if (level < int32(_spellInfo->BaseLevel))
                level = int32(_spellInfo->BaseLevel);
            level -= int32(_spellInfo->SpellLevel);
            basePoints += int32(level * basePointsPerLevel);
        }

        // roll in a range <1;EffectDieSides> as of patch 3.3.3
        int32 randomPoints = int32(DieSides);
        switch (randomPoints)
        {
            case 0: break;
            case 1: basePoints += 1; break;                     // range 1..1
            default:
            {
                // range can have positive (1..rand) and negative (rand..1) values, so order its for irand
                int32 randvalue = (randomPoints >= 1)
                    ? irand(1, randomPoints)
                    : irand(randomPoints, 1);

                basePoints += randvalue;
                break;
            }
        }
    }

    float value = float(basePoints);

    // random damage
    if (caster)
    {
        // bonus amount from combo points
        if (_spellInfo->HasAttribute(SPELL_ATTR1_FINISHING_MOVE_DAMAGE) && caster->m_playerMovingMe && comboDamage)
            if (uint8 comboPoints = caster->m_playerMovingMe->GetComboPoints())
                value += comboDamage * comboPoints;

        value = caster->ApplyEffectModifiers(_spellInfo, _effIndex, value);

        if (!caster->IsControlledByPlayer() &&
            _spellInfo->SpellLevel && _spellInfo->SpellLevel != caster->getLevel() &&
            !basePointsPerLevel && _spellInfo->HasAttribute(SPELL_ATTR0_LEVEL_DAMAGE_CALCULATION))
        {
            bool canEffectScale = false;
            switch (Effect)
            {
                case SPELL_EFFECT_SCHOOL_DAMAGE:
                case SPELL_EFFECT_DUMMY:
                case SPELL_EFFECT_POWER_DRAIN:
                case SPELL_EFFECT_HEALTH_LEECH:
                case SPELL_EFFECT_HEAL:
                case SPELL_EFFECT_WEAPON_DAMAGE:
                case SPELL_EFFECT_POWER_BURN:
                case SPELL_EFFECT_SCRIPT_EFFECT:
                case SPELL_EFFECT_NORMALIZED_WEAPON_DMG:
                case SPELL_EFFECT_FORCE_CAST_WITH_VALUE:
                case SPELL_EFFECT_TRIGGER_SPELL_WITH_VALUE:
                case SPELL_EFFECT_TRIGGER_MISSILE_SPELL_WITH_VALUE:
                    canEffectScale = true;
                    break;
                default:
                    break;
            }

            switch (ApplyAuraName)
            {
                case SPELL_AURA_PERIODIC_DAMAGE:
                case SPELL_AURA_DUMMY:
                case SPELL_AURA_PERIODIC_HEAL:
                case SPELL_AURA_DAMAGE_SHIELD:
                case SPELL_AURA_PROC_TRIGGER_DAMAGE:
                case SPELL_AURA_PERIODIC_LEECH:
                case SPELL_AURA_PERIODIC_MANA_LEECH:
                case SPELL_AURA_SCHOOL_ABSORB:
                case SPELL_AURA_PERIODIC_TRIGGER_SPELL_WITH_VALUE:
                    canEffectScale = true;
                    break;
                default:
                    break;
            }

            if (canEffectScale)
            {
                GtNPCManaCostScalerEntry const* spellScaler = sGtNPCManaCostScalerStore.LookupEntry(_spellInfo->SpellLevel - 1);
                GtNPCManaCostScalerEntry const* casterScaler = sGtNPCManaCostScalerStore.LookupEntry(caster->getLevel() - 1);
                if (spellScaler && casterScaler)
                {
                    value *= casterScaler->ratio / spellScaler->ratio;
                    if (caster->getLevel() > 80)
                        value *= (caster->getLevel() - 80) * 4.4f; // this value has been reversed by calculating default scalings against sniffed spell damage log packets
                }
            }
        }
    }

    return uint32(std::floor(value + 0.5f));
}

int32 SpellEffectInfo::CalcBaseValue(int32 value) const
{
    if (DieSides == 0)
        return value;
    else
        return value - 1;
}

float SpellEffectInfo::CalcValueMultiplier(Unit* caster, Spell* spell) const
{
    float multiplier = Amplitude;
    if (Player* modOwner = (caster ? caster->GetSpellModOwner() : nullptr))
        modOwner->ApplySpellMod(_spellInfo->Id, SPELLMOD_VALUE_MULTIPLIER, multiplier, spell);
    return multiplier;
}

float SpellEffectInfo::CalcDamageMultiplier(Unit* caster, Spell* spell) const
{
    float multiplierPercent = DamageMultiplier * 100.0f;
    if (Player* modOwner = (caster ? caster->GetSpellModOwner() : nullptr))
        modOwner->ApplySpellMod(_spellInfo->Id, SPELLMOD_DAMAGE_MULTIPLIER, multiplierPercent, spell);
    return multiplierPercent / 100.0f;
}

bool SpellEffectInfo::HasRadius() const
{
    return RadiusEntry != nullptr;
}

bool SpellEffectInfo::HasMaxRadius() const
{
    return MaxRadiusEntry != nullptr;
}

float SpellEffectInfo::CalcRadius(Unit* caster, Spell* spell) const
{
    const SpellRadiusEntry* entry = RadiusEntry;
    if (!HasRadius() && HasMaxRadius())
        entry = MaxRadiusEntry;

    if (!entry)
        return 0.0f;

    float radius = entry->RadiusMin;
    if (caster)
    {
        radius += entry->RadiusPerLevel * caster->getLevel();
        radius = std::min(radius, entry->RadiusMax);
        if (Player* modOwner = caster->GetSpellModOwner())
            modOwner->ApplySpellMod(_spellInfo->Id, SPELLMOD_RADIUS, radius, spell);
    }

    return radius;
}

uint32 SpellEffectInfo::GetProvidedTargetMask() const
{
    return GetTargetFlagMask(TargetA.GetObjectType()) | GetTargetFlagMask(TargetB.GetObjectType());
}

uint32 SpellEffectInfo::GetMissingTargetMask(bool srcSet /*= false*/, bool dstSet /*= false*/, uint32 mask /*=0*/) const
{
    uint32 effImplicitTargetMask = GetTargetFlagMask(GetUsedTargetObjectType());
    uint32 providedTargetMask = GetProvidedTargetMask() | mask;

    // remove all flags covered by effect target mask
    if (providedTargetMask & TARGET_FLAG_UNIT_MASK)
        effImplicitTargetMask &= ~(TARGET_FLAG_UNIT_MASK);
    if (providedTargetMask & TARGET_FLAG_CORPSE_MASK)
        effImplicitTargetMask &= ~(TARGET_FLAG_UNIT_MASK | TARGET_FLAG_CORPSE_MASK);
    if (providedTargetMask & TARGET_FLAG_GAMEOBJECT_ITEM)
        effImplicitTargetMask &= ~(TARGET_FLAG_GAMEOBJECT_ITEM | TARGET_FLAG_GAMEOBJECT | TARGET_FLAG_ITEM);
    if (providedTargetMask & TARGET_FLAG_GAMEOBJECT)
        effImplicitTargetMask &= ~(TARGET_FLAG_GAMEOBJECT | TARGET_FLAG_GAMEOBJECT_ITEM);
    if (providedTargetMask & TARGET_FLAG_ITEM)
        effImplicitTargetMask &= ~(TARGET_FLAG_ITEM | TARGET_FLAG_GAMEOBJECT_ITEM);
    if (dstSet || providedTargetMask & TARGET_FLAG_DEST_LOCATION)
        effImplicitTargetMask &= ~(TARGET_FLAG_DEST_LOCATION);
    if (srcSet || providedTargetMask & TARGET_FLAG_SOURCE_LOCATION)
        effImplicitTargetMask &= ~(TARGET_FLAG_SOURCE_LOCATION);

    return effImplicitTargetMask;
}

SpellEffectImplicitTargetTypes SpellEffectInfo::GetImplicitTargetType() const
{
    return _data[Effect].ImplicitTargetType;
}

SpellTargetObjectTypes SpellEffectInfo::GetUsedTargetObjectType() const
{
    return _data[Effect].UsedTargetObjectType;
}

SpellEffectInfo::StaticData SpellEffectInfo::_data[TOTAL_SPELL_EFFECTS] =
{
    // implicit target type           used target object type
    {EFFECT_IMPLICIT_TARGET_NONE,     TARGET_OBJECT_TYPE_NONE}, // 0
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 1 SPELL_EFFECT_INSTAKILL
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 2 SPELL_EFFECT_SCHOOL_DAMAGE
    {EFFECT_IMPLICIT_TARGET_NONE,     TARGET_OBJECT_TYPE_NONE}, // 3 SPELL_EFFECT_DUMMY
    {EFFECT_IMPLICIT_TARGET_NONE,     TARGET_OBJECT_TYPE_NONE}, // 4 SPELL_EFFECT_PORTAL_TELEPORT
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT_AND_DEST}, // 5 SPELL_EFFECT_TELEPORT_UNITS
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 6 SPELL_EFFECT_APPLY_AURA
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 7 SPELL_EFFECT_ENVIRONMENTAL_DAMAGE
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 8 SPELL_EFFECT_POWER_DRAIN
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 9 SPELL_EFFECT_HEALTH_LEECH
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 10 SPELL_EFFECT_HEAL
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 11 SPELL_EFFECT_BIND
    {EFFECT_IMPLICIT_TARGET_NONE,     TARGET_OBJECT_TYPE_NONE}, // 12 SPELL_EFFECT_PORTAL
    {EFFECT_IMPLICIT_TARGET_NONE,     TARGET_OBJECT_TYPE_NONE}, // 13 SPELL_EFFECT_RITUAL_BASE
    {EFFECT_IMPLICIT_TARGET_NONE,     TARGET_OBJECT_TYPE_NONE}, // 14 SPELL_EFFECT_RITUAL_SPECIALIZE
    {EFFECT_IMPLICIT_TARGET_NONE,     TARGET_OBJECT_TYPE_NONE}, // 15 SPELL_EFFECT_RITUAL_ACTIVATE_PORTAL
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 16 SPELL_EFFECT_QUEST_COMPLETE
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 17 SPELL_EFFECT_WEAPON_DAMAGE_NOSCHOOL
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_CORPSE_ALLY}, // 18 SPELL_EFFECT_RESURRECT
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 19 SPELL_EFFECT_ADD_EXTRA_ATTACKS
    {EFFECT_IMPLICIT_TARGET_NONE,     TARGET_OBJECT_TYPE_UNIT}, // 20 SPELL_EFFECT_DODGE
    {EFFECT_IMPLICIT_TARGET_NONE,     TARGET_OBJECT_TYPE_UNIT}, // 21 SPELL_EFFECT_EVADE
    {EFFECT_IMPLICIT_TARGET_NONE,     TARGET_OBJECT_TYPE_UNIT}, // 22 SPELL_EFFECT_PARRY
    {EFFECT_IMPLICIT_TARGET_NONE,     TARGET_OBJECT_TYPE_UNIT}, // 23 SPELL_EFFECT_BLOCK
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 24 SPELL_EFFECT_CREATE_ITEM
    {EFFECT_IMPLICIT_TARGET_NONE,     TARGET_OBJECT_TYPE_UNIT}, // 25 SPELL_EFFECT_WEAPON
    {EFFECT_IMPLICIT_TARGET_NONE,     TARGET_OBJECT_TYPE_UNIT}, // 26 SPELL_EFFECT_DEFENSE
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_DEST}, // 27 SPELL_EFFECT_PERSISTENT_AREA_AURA
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_DEST}, // 28 SPELL_EFFECT_SUMMON
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT_AND_DEST}, // 29 SPELL_EFFECT_LEAP
    {EFFECT_IMPLICIT_TARGET_NONE,     TARGET_OBJECT_TYPE_UNIT}, // 30 SPELL_EFFECT_ENERGIZE
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 31 SPELL_EFFECT_WEAPON_PERCENT_DAMAGE
    {EFFECT_IMPLICIT_TARGET_NONE,     TARGET_OBJECT_TYPE_NONE}, // 32 SPELL_EFFECT_TRIGGER_MISSILE
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_GOBJ_ITEM}, // 33 SPELL_EFFECT_OPEN_LOCK
    {EFFECT_IMPLICIT_TARGET_NONE,     TARGET_OBJECT_TYPE_UNIT}, // 34 SPELL_EFFECT_SUMMON_CHANGE_ITEM
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 35 SPELL_EFFECT_APPLY_AREA_AURA_PARTY
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 36 SPELL_EFFECT_LEARN_SPELL
    {EFFECT_IMPLICIT_TARGET_NONE,     TARGET_OBJECT_TYPE_UNIT}, // 37 SPELL_EFFECT_SPELL_DEFENSE
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 38 SPELL_EFFECT_DISPEL
    {EFFECT_IMPLICIT_TARGET_NONE,     TARGET_OBJECT_TYPE_UNIT}, // 39 SPELL_EFFECT_LANGUAGE
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 40 SPELL_EFFECT_DUAL_WIELD
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 41 SPELL_EFFECT_JUMP
    {EFFECT_IMPLICIT_TARGET_NONE,     TARGET_OBJECT_TYPE_DEST}, // 42 SPELL_EFFECT_JUMP_DEST
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT_AND_DEST}, // 43 SPELL_EFFECT_TELEPORT_UNITS_FACE_CASTER
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 44 SPELL_EFFECT_SKILL_STEP
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 45 SPELL_EFFECT_ADD_HONOR
    {EFFECT_IMPLICIT_TARGET_NONE,     TARGET_OBJECT_TYPE_UNIT}, // 46 SPELL_EFFECT_SPAWN
    {EFFECT_IMPLICIT_TARGET_NONE,     TARGET_OBJECT_TYPE_UNIT}, // 47 SPELL_EFFECT_TRADE_SKILL
    {EFFECT_IMPLICIT_TARGET_NONE,     TARGET_OBJECT_TYPE_UNIT}, // 48 SPELL_EFFECT_STEALTH
    {EFFECT_IMPLICIT_TARGET_NONE,     TARGET_OBJECT_TYPE_UNIT}, // 49 SPELL_EFFECT_DETECT
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_DEST}, // 50 SPELL_EFFECT_TRANS_DOOR
    {EFFECT_IMPLICIT_TARGET_NONE,     TARGET_OBJECT_TYPE_UNIT}, // 51 SPELL_EFFECT_FORCE_CRITICAL_HIT
    {EFFECT_IMPLICIT_TARGET_NONE,     TARGET_OBJECT_TYPE_UNIT}, // 52 SPELL_EFFECT_GUARANTEE_HIT
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_ITEM}, // 53 SPELL_EFFECT_ENCHANT_ITEM
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_ITEM}, // 54 SPELL_EFFECT_ENCHANT_ITEM_TEMPORARY
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 55 SPELL_EFFECT_TAMECREATURE
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_DEST}, // 56 SPELL_EFFECT_SUMMON_PET
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 57 SPELL_EFFECT_LEARN_PET_SPELL
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 58 SPELL_EFFECT_WEAPON_DAMAGE
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 59 SPELL_EFFECT_CREATE_RANDOM_ITEM
    {EFFECT_IMPLICIT_TARGET_NONE,     TARGET_OBJECT_TYPE_UNIT}, // 60 SPELL_EFFECT_PROFICIENCY
    {EFFECT_IMPLICIT_TARGET_NONE,     TARGET_OBJECT_TYPE_NONE}, // 61 SPELL_EFFECT_SEND_EVENT
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 62 SPELL_EFFECT_POWER_BURN
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 63 SPELL_EFFECT_THREAT
    {EFFECT_IMPLICIT_TARGET_NONE,     TARGET_OBJECT_TYPE_NONE}, // 64 SPELL_EFFECT_TRIGGER_SPELL
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 65 SPELL_EFFECT_APPLY_AREA_AURA_RAID
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 66 SPELL_EFFECT_CREATE_MANA_GEM
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 67 SPELL_EFFECT_HEAL_MAX_HEALTH
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 68 SPELL_EFFECT_INTERRUPT_CAST
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT_AND_DEST}, // 69 SPELL_EFFECT_DISTRACT
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 70 SPELL_EFFECT_PULL
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 71 SPELL_EFFECT_PICKPOCKET
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_DEST}, // 72 SPELL_EFFECT_ADD_FARSIGHT
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 73 SPELL_EFFECT_UNTRAIN_TALENTS
    {EFFECT_IMPLICIT_TARGET_NONE,     TARGET_OBJECT_TYPE_UNIT}, // 74 SPELL_EFFECT_APPLY_GLYPH
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 75 SPELL_EFFECT_HEAL_MECHANICAL
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_DEST}, // 76 SPELL_EFFECT_SUMMON_OBJECT_WILD
    {EFFECT_IMPLICIT_TARGET_NONE,     TARGET_OBJECT_TYPE_NONE}, // 77 SPELL_EFFECT_SCRIPT_EFFECT
    {EFFECT_IMPLICIT_TARGET_NONE,     TARGET_OBJECT_TYPE_UNIT}, // 78 SPELL_EFFECT_ATTACK
    {EFFECT_IMPLICIT_TARGET_NONE,     TARGET_OBJECT_TYPE_UNIT}, // 79 SPELL_EFFECT_SANCTUARY
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 80 SPELL_EFFECT_ADD_COMBO_POINTS
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_DEST}, // 81 SPELL_EFFECT_CREATE_HOUSE
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 82 SPELL_EFFECT_BIND_SIGHT
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT_AND_DEST}, // 83 SPELL_EFFECT_DUEL
    {EFFECT_IMPLICIT_TARGET_NONE,     TARGET_OBJECT_TYPE_UNIT}, // 84 SPELL_EFFECT_STUCK
    {EFFECT_IMPLICIT_TARGET_NONE,     TARGET_OBJECT_TYPE_NONE}, // 85 SPELL_EFFECT_SUMMON_PLAYER
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_GOBJ}, // 86 SPELL_EFFECT_ACTIVATE_OBJECT
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_GOBJ}, // 87 SPELL_EFFECT_GAMEOBJECT_DAMAGE
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_GOBJ}, // 88 SPELL_EFFECT_GAMEOBJECT_REPAIR
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_GOBJ}, // 89 SPELL_EFFECT_GAMEOBJECT_SET_DESTRUCTION_STATE
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 90 SPELL_EFFECT_KILL_CREDIT
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 91 SPELL_EFFECT_THREAT_ALL
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 92 SPELL_EFFECT_ENCHANT_HELD_ITEM
    {EFFECT_IMPLICIT_TARGET_NONE,     TARGET_OBJECT_TYPE_UNIT}, // 93 SPELL_EFFECT_FORCE_DESELECT
    {EFFECT_IMPLICIT_TARGET_NONE,     TARGET_OBJECT_TYPE_UNIT}, // 94 SPELL_EFFECT_SELF_RESURRECT
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 95 SPELL_EFFECT_SKINNING
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 96 SPELL_EFFECT_CHARGE
    {EFFECT_IMPLICIT_TARGET_NONE,     TARGET_OBJECT_TYPE_UNIT}, // 97 SPELL_EFFECT_CAST_BUTTON
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 98 SPELL_EFFECT_KNOCK_BACK
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_ITEM}, // 99 SPELL_EFFECT_DISENCHANT
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 100 SPELL_EFFECT_INEBRIATE
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_ITEM}, // 101 SPELL_EFFECT_FEED_PET
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 102 SPELL_EFFECT_DISMISS_PET
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 103 SPELL_EFFECT_REPUTATION
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_DEST}, // 104 SPELL_EFFECT_SUMMON_OBJECT_SLOT1
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_DEST}, // 105 SPELL_EFFECT_SUMMON_OBJECT_SLOT2
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_DEST}, // 106 SPELL_EFFECT_SUMMON_OBJECT_SLOT3
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_DEST}, // 107 SPELL_EFFECT_SUMMON_OBJECT_SLOT4
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 108 SPELL_EFFECT_DISPEL_MECHANIC
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_DEST}, // 109 SPELL_EFFECT_SUMMON_DEAD_PET
    {EFFECT_IMPLICIT_TARGET_NONE,     TARGET_OBJECT_TYPE_UNIT}, // 110 SPELL_EFFECT_DESTROY_ALL_TOTEMS
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 111 SPELL_EFFECT_DURABILITY_DAMAGE
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 112 SPELL_EFFECT_112
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_CORPSE_ALLY}, // 113 SPELL_EFFECT_RESURRECT_NEW
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 114 SPELL_EFFECT_ATTACK_ME
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 115 SPELL_EFFECT_DURABILITY_DAMAGE_PCT
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_CORPSE_ENEMY}, // 116 SPELL_EFFECT_SKIN_PLAYER_CORPSE
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 117 SPELL_EFFECT_SPIRIT_HEAL
    {EFFECT_IMPLICIT_TARGET_NONE,     TARGET_OBJECT_TYPE_UNIT}, // 118 SPELL_EFFECT_SKILL
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 119 SPELL_EFFECT_APPLY_AREA_AURA_PET
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 120 SPELL_EFFECT_TELEPORT_GRAVEYARD
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 121 SPELL_EFFECT_NORMALIZED_WEAPON_DMG
    {EFFECT_IMPLICIT_TARGET_NONE,     TARGET_OBJECT_TYPE_NONE}, // 122 SPELL_EFFECT_122
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 123 SPELL_EFFECT_SEND_TAXI
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 124 SPELL_EFFECT_PULL_TOWARDS
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 125 SPELL_EFFECT_MODIFY_THREAT_PERCENT
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 126 SPELL_EFFECT_STEAL_BENEFICIAL_BUFF
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_ITEM}, // 127 SPELL_EFFECT_PROSPECTING
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 128 SPELL_EFFECT_APPLY_AREA_AURA_FRIEND
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 129 SPELL_EFFECT_APPLY_AREA_AURA_ENEMY
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 130 SPELL_EFFECT_REDIRECT_THREAT
    {EFFECT_IMPLICIT_TARGET_NONE,     TARGET_OBJECT_TYPE_UNIT}, // 131 SPELL_EFFECT_PLAY_SOUND
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 132 SPELL_EFFECT_PLAY_MUSIC
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 133 SPELL_EFFECT_UNLEARN_SPECIALIZATION
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 134 SPELL_EFFECT_KILL_CREDIT2
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_DEST}, // 135 SPELL_EFFECT_CALL_PET
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 136 SPELL_EFFECT_HEAL_PCT
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 137 SPELL_EFFECT_ENERGIZE_PCT
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 138 SPELL_EFFECT_LEAP_BACK
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 139 SPELL_EFFECT_CLEAR_QUEST
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 140 SPELL_EFFECT_FORCE_CAST
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 141 SPELL_EFFECT_FORCE_CAST_WITH_VALUE
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 142 SPELL_EFFECT_TRIGGER_SPELL_WITH_VALUE
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 143 SPELL_EFFECT_APPLY_AREA_AURA_OWNER
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT_AND_DEST}, // 144 SPELL_EFFECT_KNOCK_BACK_DEST
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT_AND_DEST}, // 145 SPELL_EFFECT_PULL_TOWARDS_DEST
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 146 SPELL_EFFECT_ACTIVATE_RUNE
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 147 SPELL_EFFECT_QUEST_FAIL
    {EFFECT_IMPLICIT_TARGET_NONE,     TARGET_OBJECT_TYPE_NONE}, // 148 SPELL_EFFECT_TRIGGER_MISSILE_SPELL_WITH_VALUE
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_DEST}, // 149 SPELL_EFFECT_CHARGE_DEST
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 150 SPELL_EFFECT_QUEST_START
    {EFFECT_IMPLICIT_TARGET_NONE,     TARGET_OBJECT_TYPE_NONE}, // 151 SPELL_EFFECT_TRIGGER_SPELL_2
    {EFFECT_IMPLICIT_TARGET_NONE,     TARGET_OBJECT_TYPE_NONE}, // 152 SPELL_EFFECT_SUMMON_RAF_FRIEND
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 153 SPELL_EFFECT_CREATE_TAMED_PET
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 154 SPELL_EFFECT_DISCOVER_TAXI
    {EFFECT_IMPLICIT_TARGET_NONE,     TARGET_OBJECT_TYPE_UNIT}, // 155 SPELL_EFFECT_TITAN_GRIP
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_ITEM}, // 156 SPELL_EFFECT_ENCHANT_ITEM_PRISMATIC
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 157 SPELL_EFFECT_CREATE_ITEM_2
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_ITEM}, // 158 SPELL_EFFECT_MILLING
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 159 SPELL_EFFECT_ALLOW_RENAME_PET
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 160 SPELL_EFFECT_FORCE_CAST_2
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 161 SPELL_EFFECT_TALENT_SPEC_COUNT
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 162 SPELL_EFFECT_TALENT_SPEC_SELECT
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 163 SPELL_EFFECT_163
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 164 SPELL_EFFECT_REMOVE_AURA
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 165 SPELL_EFFECT_165
    {EFFECT_IMPLICIT_TARGET_CASTER,   TARGET_OBJECT_TYPE_UNIT}, // 166 SPELL_EFFECT_GIVE_CURRENCY
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 167 SPELL_EFFECT_167
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 168 SPELL_EFFECT_168
    {EFFECT_IMPLICIT_TARGET_NONE,     TARGET_OBJECT_TYPE_ITEM}, // 169 SPELL_EFFECT_DESTROY_ITEM
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 170 SPELL_EFFECT_170
    {EFFECT_IMPLICIT_TARGET_NONE,     TARGET_OBJECT_TYPE_DEST}, // 171 SPELL_EFFECT_SUMMON_PERSONAL_GAMEOBJECT
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_CORPSE_ALLY}, // 172 SPELL_EFFECT_RESURRECT_WITH_AURA
    {EFFECT_IMPLICIT_TARGET_NONE,     TARGET_OBJECT_TYPE_NONE}, // 173 SPELL_EFFECT_UNLOCK_GUILD_VAULT_TAB
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 174 SPELL_EFFECT_APPLY_AURA_2
    {EFFECT_IMPLICIT_TARGET_NONE,     TARGET_OBJECT_TYPE_NONE}, // 175 SPELL_EFFECT_175
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 176 SPELL_EFFECT_176
    {EFFECT_IMPLICIT_TARGET_EXPLICIT, TARGET_OBJECT_TYPE_UNIT}, // 177 SPELL_EFFECT_177
    {EFFECT_IMPLICIT_TARGET_NONE,     TARGET_OBJECT_TYPE_NONE}, // 178 SPELL_EFFECT_178
    {EFFECT_IMPLICIT_TARGET_NONE,     TARGET_OBJECT_TYPE_DEST}, // 179 SPELL_EFFECT_CREATE_AREATRIGGER
    {EFFECT_IMPLICIT_TARGET_NONE,     TARGET_OBJECT_TYPE_NONE}, // 180 SPELL_EFFECT_180
    {EFFECT_IMPLICIT_TARGET_NONE,     TARGET_OBJECT_TYPE_NONE}, // 181 SPELL_EFFECT_181
    {EFFECT_IMPLICIT_TARGET_NONE,     TARGET_OBJECT_TYPE_UNIT}, // 182 SPELL_EFFECT_182
};

SpellInfo::SpellInfo(SpellEntry const* spellEntry, SpellEffectEntry const** effects)
{
    Id = spellEntry->ID;
    Attributes = spellEntry->Attributes;
    AttributesEx = spellEntry->AttributesEx;
    AttributesEx2 = spellEntry->AttributesEx2;
    AttributesEx3 = spellEntry->AttributesEx3;
    AttributesEx4 = spellEntry->AttributesEx4;
    AttributesEx5 = spellEntry->AttributesEx5;
    AttributesEx6 = spellEntry->AttributesEx6;
    AttributesEx7 = spellEntry->AttributesEx7;
    AttributesEx8 = spellEntry->AttributesEx8;
    AttributesEx9 = spellEntry->AttributesEx9;
    AttributesEx10 = spellEntry->AttributesEx10;
    AttributesCu = 0;
    CastTimeEntry = spellEntry->CastingTimeIndex ? sSpellCastTimesStore.LookupEntry(spellEntry->CastingTimeIndex) : nullptr;
    DurationEntry = spellEntry->DurationIndex ? sSpellDurationStore.LookupEntry(spellEntry->DurationIndex) : nullptr;
    PowerType = spellEntry->PowerType;
    RangeEntry = spellEntry->RangeIndex ? sSpellRangeStore.LookupEntry(spellEntry->RangeIndex) : nullptr;
    Speed = spellEntry->Speed;

    for (uint8 i = 0; i < 2; ++i)
        SpellVisual[i] = spellEntry->SpellVisualID[i];

    SpellIconID = spellEntry->SpellIconID;
    ActiveIconID = spellEntry->ActiveIconID;
    SpellName = spellEntry->Name;
    Rank = spellEntry->NameSubtext;
    SchoolMask = spellEntry->SchoolMask;
    RuneCostID = spellEntry->RuneCostID;
    SpellDifficultyId = spellEntry->Difficulty;
    BonusCoefficient = spellEntry->BonusCoefficient;
    SpellScalingId = spellEntry->ScalingID;
    SpellAuraOptionsId = spellEntry->AuraOptionsID;
    SpellAuraRestrictionsId = spellEntry->AuraRestrictionsID;
    SpellCastingRequirementsId = spellEntry->CastingRequirementsID;
    SpellCategoriesId = spellEntry->CategoriesID;
    SpellClassOptionsId = spellEntry->ClassOptionsID;
    SpellCooldownsId = spellEntry->CooldownsID;
    SpellEquippedItemsId = spellEntry->EquippedItemsID;
    SpellInterruptsId = spellEntry->InterruptsID;
    SpellLevelsId = spellEntry->LevelsID;
    SpellPowerId = spellEntry->PowerDisplayID;
    SpellReagentsId = spellEntry->ReagentsID;
    SpellShapeshiftId = spellEntry->ShapeshiftID;
    SpellTargetRestrictionsId = spellEntry->TargetRestrictionsID;
    SpellTotemsId = spellEntry->TotemsID;
    ResearchProjectId = spellEntry->RequiredProjectID;

    // SpellDifficultyEntry
    for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
        Effects[i] = SpellEffectInfo(spellEntry, this, i, effects[i]);

    // SpellScalingEntry
    SpellScalingEntry const* _scaling = GetSpellScaling();
    CastTimeMin = _scaling ? _scaling->CastTimeMin : 0;
    CastTimeMax = _scaling ?_scaling->CastTimeMax : 0;
    CastTimeMaxLevel = _scaling ? _scaling->CastTimeMaxLevel : 0;
    ScalingClass = _scaling ? _scaling->Class : 0;
    CoefBase = _scaling ? _scaling->NerfFactor : 0;
    CoefLevelBase = _scaling ? _scaling->NerfMaxLevel : 0;

    // SpellAuraOptionsEntry
    SpellAuraOptionsEntry const* _options = GetSpellAuraOptions();
    ProcFlags = _options ? _options->ProcTypeMask : 0;
    ProcChance = _options ? _options->ProcChance : 0;
    ProcCharges = _options ? _options->ProcCharges : 0;
    StackAmount = _options ? _options->CumulativeAura : 0;

    // SpellAuraRestrictionsEntry
    SpellAuraRestrictionsEntry const* _aura = GetSpellAuraRestrictions();
    CasterAuraState = _aura ? _aura->CasterAuraState : 0;
    TargetAuraState = _aura ? _aura->TargetAuraState : 0;
    CasterAuraStateNot = _aura ? _aura->ExcludeCasterAuraState : 0;
    TargetAuraStateNot = _aura ? _aura->ExcludeTargetAuraState : 0;
    CasterAuraSpell = _aura ? _aura->CasterAuraSpell : 0;
    TargetAuraSpell = _aura ? _aura->TargetAuraSpell : 0;
    ExcludeCasterAuraSpell = _aura ? _aura->ExcludeCasterAuraSpell : 0;
    ExcludeTargetAuraSpell = _aura ? _aura->ExcludeTargetAuraSpell : 0;

    // SpellCastingRequirementsEntry
    SpellCastingRequirementsEntry const* _castreq = GetSpellCastingRequirements();
    RequiresSpellFocus = _castreq ? _castreq->RequiresSpellFocus : 0;
    FacingCasterFlags = _castreq ? _castreq->FacingCasterFlags : 0;
    AreaGroupId = _castreq ? _castreq->RequiredAreasID : -1;

    // SpellCategoriesEntry
    SpellCategoriesEntry const* _categorie = GetSpellCategories();
    CategoryEntry = _categorie ? sSpellCategoryStore.LookupEntry(_categorie->Category) : nullptr;
    Dispel = _categorie ? _categorie->DispelType : 0;
    Mechanic = _categorie ? _categorie->Mechanic : 0;
    StartRecoveryCategory = _categorie ? _categorie->StartRecoveryCategory : 0;
    DmgClass = _categorie ? _categorie->DefenseType : 0;
    PreventionType = _categorie ? _categorie->PreventionType : 0;

    // SpellClassOptionsEntry
    SpellClassOptionsEntry const* _class = GetSpellClassOptions();
    SpellFamilyName = _class ? _class->SpellClassSet : 0;
    SpellFamilyFlags = _class ? _class->SpellFamilyMask : flag96(0);

    // SpellCooldownsEntry
    SpellCooldownsEntry const* _cooldowns = GetSpellCooldowns();
    RecoveryTime = _cooldowns ? _cooldowns->RecoveryTime : 0;
    CategoryRecoveryTime = _cooldowns ? _cooldowns->CategoryRecoveryTime : 0;
    StartRecoveryTime = _cooldowns ? _cooldowns->StartRecoveryTime : 0;

    // SpellEquippedItemsEntry
    SpellEquippedItemsEntry const* _equipped = GetSpellEquippedItems();
    EquippedItemClass = _equipped ? _equipped->EquippedItemClass : -1;
    EquippedItemSubClassMask = _equipped ?_equipped->EquippedItemSubclass : -1;
    EquippedItemInventoryTypeMask = _equipped ? _equipped->EquippedItemInvTypes : -1;

    // SpellInterruptsEntry
    SpellInterruptsEntry const* _interrupt = GetSpellInterrupts();

    AuraInterruptFlags = _interrupt ? SpellAuraInterruptFlags(_interrupt->AuraInterruptFlags[0]) : SpellAuraInterruptFlags::None;
    AuraInterruptFlags2 = _interrupt? SpellAuraInterruptFlags2(_interrupt->AuraInterruptFlags[1]) : SpellAuraInterruptFlags2::None;
    ChannelInterruptFlags = _interrupt ? SpellAuraInterruptFlags(_interrupt->ChannelInterruptFlags[0] ) : SpellAuraInterruptFlags::None;
    ChannelInterruptFlags2 =  _interrupt ? SpellAuraInterruptFlags2(_interrupt->ChannelInterruptFlags[1]) : SpellAuraInterruptFlags2::None;
    InterruptFlags = _interrupt ? SpellInterruptFlags(_interrupt->InterruptFlags) : SpellInterruptFlags::None;

    // SpellLevelsEntry
    SpellLevelsEntry const* _levels = GetSpellLevels();
    MaxLevel = _levels ? _levels->MaxLevel : 0;
    BaseLevel = _levels ? _levels->BaseLevel : 0;
    SpellLevel = _levels ? _levels->SpellLevel : 0;

    // SpellPowerEntry
    SpellPowerEntry const* _power = GetSpellPower();
    ManaCost = _power ? _power->ManaCost : 0;
    ManaCostPerlevel = _power ? _power->ManaCostPerLevel : 0;
    ManaCostPercentage = _power ? _power->PowerCostPct : 0;
    ManaCostPercentage2 = _power ? _power->PowerCostPct2 : 0.f;
    ManaPerSecond = _power ? _power->ManaPerSecond : 0;

    // SpellReagentsEntry
    SpellReagentsEntry const* _reagents = GetSpellReagents();
    for (uint8 i = 0; i < MAX_SPELL_REAGENTS; ++i)
        Reagent[i] = _reagents ? _reagents->Reagents[i] : 0;
    for (uint8 i = 0; i < MAX_SPELL_REAGENTS; ++i)
        ReagentCount[i] = _reagents ? _reagents->ReagentCount[i] : 0;

    // SpellShapeshiftEntry
    if (SpellShapeshiftEntry const* _shapeshift = GetSpellShapeshift())
    {
        Stances = MAKE_PAIR64(_shapeshift->ShapeshiftMask[0], _shapeshift->ShapeshiftMask[1]);
        StancesNot = MAKE_PAIR64(_shapeshift->ShapeshiftExclude[0], _shapeshift->ShapeshiftExclude[1]);
    }
    else
    {
        Stances = UI64LIT(0);
        StancesNot = UI64LIT(0);
    }

    // SpellTargetRestrictionsEntry
    SpellTargetRestrictionsEntry const* _target = GetSpellTargetRestrictions();
    ConeAngle = _target ? _target->ConeAngle : 0.f;
    Targets = _target ? _target->Targets : 0;
    TargetCreatureType = _target ? _target->TargetCreatureType : 0;
    MaxAffectedTargets = _target ? _target->MaxTargets : 0;
    MaxTargetLevel = _target ? _target->MaxTargetLevel : 0;

    // SpellTotemsEntry
    SpellTotemsEntry const* _totem = GetSpellTotems();
    for (uint8 i = 0; i < 2; ++i)
        TotemCategory[i] = _totem ? _totem->RequiredTotemCategoryID[i] : 0;
    for (uint8 i = 0; i < 2; ++i)
        Totem[i] = _totem ? _totem->Totem[i] : 0;

    ChainEntry = nullptr;
    ExplicitTargetMask = 0;

    _spellSpecific = SPELL_SPECIFIC_NORMAL;
    _auraState = AURA_STATE_NONE;

    _allowedMechanicMask = 0;
    MaxAuraTargets = 0;
}

SpellInfo::~SpellInfo()
{
    _UnloadImplicitTargetConditionLists();
}

uint32 SpellInfo::GetCategory() const
{
    return CategoryEntry ? CategoryEntry->ID : 0;
}

bool SpellInfo::HasEffect(SpellEffects effect) const
{
    for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
        if (Effects[i].IsEffect(effect))
            return true;
    return false;
}

bool SpellInfo::HasAura(AuraType aura) const
{
    for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
        if (Effects[i].IsAura(aura))
            return true;
    return false;
}

bool SpellInfo::HasAreaAuraEffect() const
{
    for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
        if (Effects[i].IsAreaAuraEffect())
            return true;
    return false;
}

bool SpellInfo::HasOnlyDamageEffects() const
{
    for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
    {
        if (Effects[i].IsEffect())
        {
            switch (Effects[i].Effect)
            {
                case SPELL_EFFECT_WEAPON_DAMAGE:
                case SPELL_EFFECT_WEAPON_DAMAGE_NOSCHOOL:
                case SPELL_EFFECT_NORMALIZED_WEAPON_DMG:
                case SPELL_EFFECT_WEAPON_PERCENT_DAMAGE:
                case SPELL_EFFECT_SCHOOL_DAMAGE:
                case SPELL_EFFECT_ENVIRONMENTAL_DAMAGE:
                case SPELL_EFFECT_HEALTH_LEECH:
                    continue;
                default:
                    return false;
            }
        }
    }

    return true;
}

bool SpellInfo::CanBeInterrupted(Unit* interruptTarget) const
{
    return HasAttribute(SPELL_ATTR7_CAN_ALWAYS_BE_INTERRUPTED)
        || HasChannelInterruptFlag(SpellAuraInterruptFlags::Damage | SpellAuraInterruptFlags::EnteringCombat)
        || (interruptTarget->IsPlayer() && InterruptFlags.HasFlag(SpellInterruptFlags::DamageCancelsPlayerOnly))
        || (!(interruptTarget->GetMechanicImmunityMask() & (1 << MECHANIC_INTERRUPT))
        && PreventionType & SPELL_PREVENTION_TYPE_SILENCE);
}

bool SpellInfo::HasAnyAuraInterruptFlag() const
{
    return AuraInterruptFlags != SpellAuraInterruptFlags::None || AuraInterruptFlags2 != SpellAuraInterruptFlags2::None;
}

bool SpellInfo::IsExplicitDiscovery() const
{
    return ((Effects[0].Effect == SPELL_EFFECT_CREATE_RANDOM_ITEM
        || Effects[0].Effect == SPELL_EFFECT_CREATE_ITEM_2)
        && Effects[1].Effect == SPELL_EFFECT_SCRIPT_EFFECT)
        || Id == 64323;
}

bool SpellInfo::IsLootCrafting() const
{
    return (Effects[0].Effect == SPELL_EFFECT_CREATE_RANDOM_ITEM ||
        // different random cards from Inscription (121==Virtuoso Inking Set category) r without explicit item
        (Effects[0].Effect == SPELL_EFFECT_CREATE_ITEM_2 &&
        ((TotemCategory[0] != 0 || (Totem[0] != 0 && SpellIconID == 1)) || Effects[0].ItemType == 0)));
}

bool SpellInfo::IsQuestTame() const
{
    return Effects[0].Effect == SPELL_EFFECT_THREAT && Effects[1].Effect == SPELL_EFFECT_APPLY_AURA && Effects[1].ApplyAuraName == SPELL_AURA_DUMMY;
}

bool SpellInfo::IsProfessionOrRiding() const
{
    for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
    {
        if (Effects[i].Effect == SPELL_EFFECT_SKILL)
        {
            uint32 skill = Effects[i].MiscValue;

            if (IsProfessionOrRidingSkill(skill))
                return true;
        }
    }
    return false;
}

bool SpellInfo::IsProfession() const
{
    for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
    {
        if (Effects[i].Effect == SPELL_EFFECT_SKILL)
        {
            uint32 skill = Effects[i].MiscValue;

            if (IsProfessionSkill(skill))
                return true;
        }
    }
    return false;
}

bool SpellInfo::IsPrimaryProfession() const
{
    for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
    {
        if (Effects[i].Effect == SPELL_EFFECT_SKILL)
        {
            uint32 skill = Effects[i].MiscValue;

            if (IsPrimaryProfessionSkill(skill))
                return true;
        }
    }
    return false;
}

bool SpellInfo::IsPrimaryProfessionFirstRank() const
{
    return IsPrimaryProfession() && GetRank() == 1;
}

bool SpellInfo::IsAbilityLearnedWithProfession() const
{
    SkillLineAbilityMapBounds bounds = sSpellMgr->GetSkillLineAbilityMapBounds(Id);

    for (SkillLineAbilityMap::const_iterator _spell_idx = bounds.first; _spell_idx != bounds.second; ++_spell_idx)
    {
        SkillLineAbilityEntry const* pAbility = _spell_idx->second;
        if (!pAbility || pAbility->AcquireMethod != SKILL_LINE_ABILITY_LEARNED_ON_SKILL_VALUE)
            continue;

        if (pAbility->MinSkillLineRank > 0)
            return true;
    }

    return false;
}

bool SpellInfo::IsAbilityOfSkillType(uint32 skillType) const
{
    SkillLineAbilityMapBounds bounds = sSpellMgr->GetSkillLineAbilityMapBounds(Id);

    for (SkillLineAbilityMap::const_iterator _spell_idx = bounds.first; _spell_idx != bounds.second; ++_spell_idx)
        if (_spell_idx->second->SkillLine == uint32(skillType))
            return true;

    return false;
}

bool SpellInfo::IsAffectingArea() const
{
    for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
        if (Effects[i].IsEffect() && (Effects[i].IsTargetingArea() || Effects[i].IsEffect(SPELL_EFFECT_PERSISTENT_AREA_AURA) || Effects[i].IsAreaAuraEffect()))
            return true;
    return false;
}

// checks if spell targets are selected from area, doesn't include spell effects in check (like area wide auras for example)
bool SpellInfo::IsTargetingArea() const
{
    for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
        if (Effects[i].IsEffect() && Effects[i].IsTargetingArea())
            return true;
    return false;
}

bool SpellInfo::NeedsExplicitUnitTarget() const
{
    return (GetExplicitTargetMask() & TARGET_FLAG_UNIT_MASK) != 0;
}

bool SpellInfo::NeedsToBeTriggeredByCaster(SpellInfo const* triggeringSpell) const
{
    if (NeedsExplicitUnitTarget())
        return true;

    /*
    for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
    {
        if (Effects[i].IsEffect())
        {
            if (Effects[i].TargetA.GetSelectionCategory() == TARGET_SELECT_CATEGORY_CHANNEL
                || Effects[i].TargetB.GetSelectionCategory() == TARGET_SELECT_CATEGORY_CHANNEL)
                return true;
        }
    }
    */

    if (triggeringSpell->IsChanneled())
    {
        uint32 mask = 0;
        for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
        {
            if (Effects[i].TargetA.GetTarget() != TARGET_UNIT_CASTER && Effects[i].TargetA.GetTarget() != TARGET_DEST_CASTER
                && Effects[i].TargetB.GetTarget() != TARGET_UNIT_CASTER && Effects[i].TargetB.GetTarget() != TARGET_DEST_CASTER)
            {
                mask |= Effects[i].GetProvidedTargetMask();
            }
        }

        if (mask & TARGET_FLAG_UNIT_MASK)
            return true;
    }

    return false;
}

bool SpellInfo::IsSelfCast() const
{
    for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
        if (Effects[i].Effect && Effects[i].TargetA.GetTarget() != TARGET_UNIT_CASTER)
            return false;
    return true;
}

bool SpellInfo::IsPassive() const
{
    return HasAttribute(SPELL_ATTR0_PASSIVE);
}

bool SpellInfo::IsRaidMarker() const
{
    return AttributesEx8 & SPELL_ATTR8_RAID_MARKER;
}

bool SpellInfo::IsAutocastable() const
{
    if (IsPassive())
        return false;
    if (HasAttribute(SPELL_ATTR1_UNAUTOCASTABLE_BY_PET))
        return false;
    return true;
}

bool SpellInfo::IsStackableWithRanks() const
{
    if (IsPassive())
        return false;
    if (PowerType != POWER_MANA && PowerType != POWER_HEALTH)
        return false;
    if (IsProfessionOrRiding())
        return false;

    if (IsAbilityLearnedWithProfession())
        return false;

    // All stance spells. if any better way, change it.
    for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
    {
        switch (SpellFamilyName)
        {
            case SPELLFAMILY_PALADIN:
                // Paladin aura Spell
                if (Effects[i].Effect == SPELL_EFFECT_APPLY_AREA_AURA_RAID)
                    return false;
                break;
            case SPELLFAMILY_DRUID:
                // Druid form Spell
                if (Effects[i].Effect == SPELL_EFFECT_APPLY_AURA &&
                    Effects[i].ApplyAuraName == SPELL_AURA_MOD_SHAPESHIFT)
                    return false;
                break;
        }
    }
    return true;
}

bool SpellInfo::IsPassiveStackableWithRanks() const
{
    return IsPassive() && !HasEffect(SPELL_EFFECT_APPLY_AURA);
}

bool SpellInfo::IsMultiSlotAura() const
{
    if (IsPassive() || HasAura(SPELL_AURA_CONTROL_VEHICLE))
        return true;

    switch (Id)
    {
        case 55849: // Power Spark
        case 40075: // Fel Flak Fire
        case 44413: // Incanter's Absorption
        case 79010: // Point of Vulnerability
        case 79920: // Shared Health
            return true;
        default:
            return false;
    }
}

bool SpellInfo::IsStackableOnOneSlotWithDifferentCasters() const
{
    /// TODO: Re-verify meaning of SPELL_ATTR3_STACK_FOR_DIFF_CASTERS and update conditions here
    return StackAmount > 1 && !IsChanneled() && !HasAttribute(SPELL_ATTR3_STACK_FOR_DIFF_CASTERS);
}

bool SpellInfo::IsCooldownStartedOnEvent() const
{
    if (HasAttribute(SPELL_ATTR0_DISABLED_WHILE_ACTIVE))
        return true;

    return CategoryEntry && CategoryEntry->GetFlags().HasFlag(SpellCategoryFlags::CooldownEventOnLeaveCombat);
}

bool SpellInfo::IsDeathPersistent() const
{
    return HasAttribute(SPELL_ATTR3_DEATH_PERSISTENT);
}

bool SpellInfo::IsRequiringDeadTarget() const
{
    return HasAttribute(SPELL_ATTR3_ONLY_TARGET_GHOSTS);
}

bool SpellInfo::IsAllowingDeadTarget() const
{
    if (HasAttribute(SPELL_ATTR2_CAN_TARGET_DEAD) || Targets & (TARGET_FLAG_CORPSE_ALLY | TARGET_FLAG_CORPSE_ENEMY | TARGET_FLAG_UNIT_DEAD))
        return true;

    for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
        if (Effects[i].TargetA.GetTarget() == TARGET_CORPSE_SRC_AREA_RAID)
            return true;

    return false;
}

bool SpellInfo::IsGroupBuff() const
{
    for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
    {
        switch (Effects[i].TargetA.GetCheckType())
        {
            case TARGET_CHECK_PARTY:
            case TARGET_CHECK_RAID:
            case TARGET_CHECK_RAID_CLASS:
                return true;
            default:
                break;
        }
    }

    return false;
}

bool SpellInfo::CanBeUsedInCombat() const
{
    return !HasAttribute(SPELL_ATTR0_CANT_USED_IN_COMBAT);
}

bool SpellInfo::IsPositive() const
{
    return !HasAttribute(SPELL_ATTR0_CU_NEGATIVE);
}

bool SpellInfo::IsPositiveEffect(uint8 effIndex) const
{
    switch (effIndex)
    {
        default:
        case 0:
            return !HasAttribute(SPELL_ATTR0_CU_NEGATIVE_EFF0);
        case 1:
            return !HasAttribute(SPELL_ATTR0_CU_NEGATIVE_EFF1);
        case 2:
            return !HasAttribute(SPELL_ATTR0_CU_NEGATIVE_EFF2);
    }
}

bool SpellInfo::IsChanneled() const
{
    return HasAttribute(SpellAttr1(SPELL_ATTR1_CHANNELED_1 | SPELL_ATTR1_CHANNELED_2));
}

bool SpellInfo::IsMoveAllowedChannel() const
{
    return IsChanneled() && ((HasAttribute(SPELL_ATTR5_CAN_CHANNEL_WHEN_MOVING) && !ChannelInterruptFlags.HasFlag(SpellAuraInterruptFlags::Moving | SpellAuraInterruptFlags::Turning)));
}

bool SpellInfo::NeedsComboPoints() const
{
    return HasAttribute(SpellAttr1(SPELL_ATTR1_FINISHING_MOVE_DAMAGE | SPELL_ATTR1_FINISHING_MOVE_DURATION));
}

bool SpellInfo::IsNextMeleeSwingSpell() const
{
    return HasAttribute(SpellAttr0(SPELL_ATTR0_ON_NEXT_SWING | SPELL_ATTR0_ON_NEXT_SWING_2));
}

bool SpellInfo::IsBreakingStealth() const
{
    return !HasAttribute(SPELL_ATTR1_NOT_BREAK_STEALTH);
}

bool SpellInfo::IsRangedWeaponSpell() const
{
    return (EquippedItemClass == ITEM_CLASS_WEAPON) && (EquippedItemSubClassMask & ITEM_SUBCLASS_MASK_WEAPON_RANGED);
}

bool SpellInfo::IsAutoRepeatRangedSpell() const
{
    return HasAttribute(SPELL_ATTR2_AUTOREPEAT_FLAG);
}

bool SpellInfo::CausesInitialThreat() const
{
    return !(HasAttribute(SPELL_ATTR1_NO_THREAT) || HasAttribute(SPELL_ATTR2_NO_INITIAL_THREAT));
}

WeaponAttackType SpellInfo::GetAttackType() const
{
    WeaponAttackType result;
    switch (DmgClass)
    {
        case SPELL_DAMAGE_CLASS_MELEE:
            if (HasAttribute(SPELL_ATTR3_REQ_OFFHAND))
                result = OFF_ATTACK;
            else
                result = BASE_ATTACK;
            break;
        case SPELL_DAMAGE_CLASS_RANGED:
            result = IsRangedWeaponSpell() ? RANGED_ATTACK : BASE_ATTACK;
            break;
        default:
            // Wands
            if (IsAutoRepeatRangedSpell())
                result = RANGED_ATTACK;
            else
                result = BASE_ATTACK;
            break;
    }

    return result;
}

bool SpellInfo::IsAffected(uint32 familyName, flag96 const& familyFlags) const
{
    if (!familyName)
        return true;

    if (familyName != SpellFamilyName)
        return false;

    if (familyFlags && !(familyFlags & SpellFamilyFlags))
        return false;

    return true;
}

bool SpellInfo::IsAffectedBySpellMods() const
{
    return !HasAttribute(SPELL_ATTR3_NO_DONE_BONUS);
}

bool SpellInfo::IsAffectedBySpellMod(SpellModifier const* mod) const
{
    if (!IsAffectedBySpellMods())
        return false;

    SpellInfo const* affectSpell = sSpellMgr->GetSpellInfo(mod->spellId);
    if (!affectSpell)
        return false;

    // TEMP: dont use IsAffected - !familyName and !familyFlags are not valid options for spell mods
    // TODO: investigate if the !familyName and !familyFlags conditions are even valid for all other (nonmod) uses of SpellInfo::IsAffected
    return affectSpell->SpellFamilyName == SpellFamilyName && mod->mask & SpellFamilyFlags;
}

bool SpellInfo::CanPierceImmuneAura(SpellInfo const* auraSpellInfo) const
{
    // aura can't be pierced
    if (!auraSpellInfo || auraSpellInfo->HasAttribute(SPELL_ATTR0_UNAFFECTED_BY_INVULNERABILITY))
        return false;

    // these spells pierce all available spells (Resurrection Sickness for example)
    if (HasAttribute(SPELL_ATTR0_UNAFFECTED_BY_INVULNERABILITY))
        return true;

    // these spells (Cyclone for example) can pierce all...
    if (HasAttribute(SPELL_ATTR1_UNAFFECTED_BY_SCHOOL_IMMUNE) || HasAttribute(SPELL_ATTR2_UNAFFECTED_BY_AURA_SCHOOL_IMMUNE))
    {
        // ...but not these (Divine shield, Ice block, Cyclone and Banish for example)
        if (auraSpellInfo->Mechanic != MECHANIC_IMMUNE_SHIELD &&
               auraSpellInfo->Mechanic != MECHANIC_INVULNERABILITY &&
               (auraSpellInfo->Mechanic != MECHANIC_BANISH || (IsRankOf(auraSpellInfo) && auraSpellInfo->Dispel != DISPEL_NONE))) // Banish shouldn't be immune to itself, but Cyclone should
            return true;
    }

    // Dispels other auras on immunity, check if this spell makes the unit immune to aura
    if (HasAttribute(SPELL_ATTR1_DISPEL_AURAS_ON_IMMUNITY) && CanSpellProvideImmunityAgainstAura(auraSpellInfo))
        return true;

    return false;
}

bool SpellInfo::CanDispelAura(SpellInfo const* auraSpellInfo) const
{
    // These auras (like Divine Shield) can't be dispelled
    if (auraSpellInfo->HasAttribute(SPELL_ATTR0_UNAFFECTED_BY_INVULNERABILITY))
        return false;

    // These spells (like Mass Dispel) can dispel all auras
    if (HasAttribute(SPELL_ATTR0_UNAFFECTED_BY_INVULNERABILITY))
        return true;

    // These auras (Cyclone for example) are not dispelable
    if ((auraSpellInfo->HasAttribute(SPELL_ATTR1_UNAFFECTED_BY_SCHOOL_IMMUNE) && auraSpellInfo->Mechanic != MECHANIC_NONE)
        || auraSpellInfo->HasAttribute(SPELL_ATTR2_UNAFFECTED_BY_AURA_SCHOOL_IMMUNE))
        return false;

    return true;
}

bool SpellInfo::IsSingleTarget() const
{
    // all other single target spells have if it has AttributesEx5
    if (HasAttribute(SPELL_ATTR5_SINGLE_TARGET_SPELL))
        return true;

    switch (GetSpellSpecific())
    {
        case SPELL_SPECIFIC_JUDGEMENT:
            return true;
        default:
            break;
    }

    return false;
}

bool SpellInfo::IsAuraExclusiveBySpecificWith(SpellInfo const* spellInfo) const
{
    SpellSpecificType spellSpec1 = GetSpellSpecific();
    SpellSpecificType spellSpec2 = spellInfo->GetSpellSpecific();
    switch (spellSpec1)
    {
        case SPELL_SPECIFIC_WARLOCK_ARMOR:
        case SPELL_SPECIFIC_MAGE_ARMOR:
        case SPELL_SPECIFIC_ELEMENTAL_SHIELD:
        case SPELL_SPECIFIC_MAGE_POLYMORPH:
        case SPELL_SPECIFIC_PRESENCE:
        case SPELL_SPECIFIC_CHARM:
        case SPELL_SPECIFIC_SCROLL:
        case SPELL_SPECIFIC_WARRIOR_ENRAGE:
        case SPELL_SPECIFIC_MAGE_ARCANE_BRILLANCE:
        case SPELL_SPECIFIC_PRIEST_DIVINE_SPIRIT:
            return spellSpec1 == spellSpec2;
        case SPELL_SPECIFIC_FOOD:
            return spellSpec2 == SPELL_SPECIFIC_FOOD
                || spellSpec2 == SPELL_SPECIFIC_FOOD_AND_DRINK;
        case SPELL_SPECIFIC_DRINK:
            return spellSpec2 == SPELL_SPECIFIC_DRINK
                || spellSpec2 == SPELL_SPECIFIC_FOOD_AND_DRINK;
        case SPELL_SPECIFIC_FOOD_AND_DRINK:
            return spellSpec2 == SPELL_SPECIFIC_FOOD
                || spellSpec2 == SPELL_SPECIFIC_DRINK
                || spellSpec2 == SPELL_SPECIFIC_FOOD_AND_DRINK;
        default:
            return false;
    }
}

bool SpellInfo::IsAuraExclusiveBySpecificPerCasterWith(SpellInfo const* spellInfo) const
{
    SpellSpecificType spellSpec = GetSpellSpecific();
    switch (spellSpec)
    {
        case SPELL_SPECIFIC_SEAL:
        case SPELL_SPECIFIC_HAND:
        case SPELL_SPECIFIC_AURA:
        case SPELL_SPECIFIC_STING:
        case SPELL_SPECIFIC_CURSE:
        case SPELL_SPECIFIC_BANE:
        case SPELL_SPECIFIC_ASPECT:
        case SPELL_SPECIFIC_JUDGEMENT:
        case SPELL_SPECIFIC_WARLOCK_CORRUPTION:
            return spellSpec == spellInfo->GetSpellSpecific();
        default:
            return false;
    }
}

SpellCastResult SpellInfo::CheckShapeshift(uint32 form) const
{
    // talents that learn spells can have stance requirements that need ignore
    // (this requirement only for client-side stance show in talent description)
    if (sDBCManager.GetTalentSpellCost(Id) > 0 &&
        (Effects[0].Effect == SPELL_EFFECT_LEARN_SPELL || Effects[1].Effect == SPELL_EFFECT_LEARN_SPELL || Effects[2].Effect == SPELL_EFFECT_LEARN_SPELL))
        return SPELL_CAST_OK;

    uint64 stanceMask = (form ? UI64LIT(1) << (form - 1) : 0);

    if (stanceMask & StancesNot)                 // can explicitly not be cast in this stance
        return SPELL_FAILED_NOT_SHAPESHIFT;

    if (stanceMask & Stances)                    // can explicitly be cast in this stance
        return SPELL_CAST_OK;

    bool actAsShifted = false;
    SpellShapeshiftFormEntry const* shapeInfo = nullptr;
    if (form > 0)
    {
        shapeInfo = sSpellShapeshiftFormStore.LookupEntry(form);
        if (!shapeInfo)
        {
            TC_LOG_ERROR("spells", "GetErrorAtShapeshiftedCast: unknown shapeshift %u", form);
            return SPELL_CAST_OK;
        }
        actAsShifted = !shapeInfo->GetFlags().HasFlag(SpellShapeshiftFormFlags::Stance);
    }

    if (actAsShifted)
    {
        if (HasAttribute(SPELL_ATTR0_NOT_SHAPESHIFT) || (shapeInfo && shapeInfo->GetFlags().HasFlag(SpellShapeshiftFormFlags::CanOnlyCastShapeshiftSpells))) // not while shapeshifted
            return SPELL_FAILED_NOT_SHAPESHIFT;
        else if (Stances != 0)                   // needs other shapeshift
            return SPELL_FAILED_ONLY_SHAPESHIFT;
    }
    else
    {
        // needs shapeshift
        if (!HasAttribute(SPELL_ATTR2_NOT_NEED_SHAPESHIFT) && Stances != 0)
            return SPELL_FAILED_ONLY_SHAPESHIFT;
    }

    // Check if stance disables cast of not-stance spells
    // Example: cannot cast any other spells in zombie or ghoul form
    /// @todo Find a way to disable use of these spells clientside
    if (shapeInfo && shapeInfo->Flags & 0x400)
    {
        if (!(stanceMask & Stances))
            return SPELL_FAILED_ONLY_SHAPESHIFT;
    }

    return SPELL_CAST_OK;
}

SpellCastResult SpellInfo::CheckLocation(uint32 map_id, uint32 zone_id, uint32 area_id, Player const* player) const
{
    // normal case
    if (AreaGroupId > 0)
    {
        bool found = false;
        AreaGroupEntry const* groupEntry = sAreaGroupStore.LookupEntry(AreaGroupId);
        while (groupEntry)
        {
            for (uint8 i = 0; i < MAX_GROUP_AREA_IDS; ++i)
                if (groupEntry->AreaID[i] == zone_id || groupEntry->AreaID[i] == area_id)
                    found = true;
            if (found || !groupEntry->NextAreaID)
                break;
            // Try search in next group
            groupEntry = sAreaGroupStore.LookupEntry(groupEntry->NextAreaID);
        }

        if (!found)
            return SPELL_FAILED_INCORRECT_AREA;
    }

    // continent limitation (virtual continent)
    if (HasAttribute(SPELL_ATTR4_CAST_ONLY_IN_OUTLAND))
    {
        uint32 v_map = sDBCManager.GetVirtualMapForMapAndZone(map_id, zone_id);
        MapEntry const* mapEntry = sMapStore.LookupEntry(v_map);

        if (!mapEntry || mapEntry->ExpansionID < 1 || !mapEntry->IsContinent())
            return SPELL_FAILED_INCORRECT_AREA;
    }

    // raid instance limitation
    if (HasAttribute(SPELL_ATTR6_NOT_IN_RAID_INSTANCE))
    {
        MapEntry const* mapEntry = sMapStore.LookupEntry(map_id);
        if (!mapEntry || mapEntry->IsRaid())
            return SPELL_FAILED_NOT_IN_RAID_INSTANCE;
    }

    // DB base check (if non empty then must fit at least single for allow)
    SpellAreaMapBounds saBounds = sSpellMgr->GetSpellAreaMapBounds(Id);
    if (saBounds.first != saBounds.second)
    {
        for (SpellAreaMap::const_iterator itr = saBounds.first; itr != saBounds.second; ++itr)
        {
            if (itr->second.IsFitToRequirements(player, zone_id, area_id))
                return SPELL_CAST_OK;
        }
        return SPELL_FAILED_INCORRECT_AREA;
    }

    // bg spell checks
    switch (Id)
    {
        case 23333:                                         // Horde Flag
        case 23335:                                         // Alliance Flag
            return (map_id == 489 || map_id == 726) && player && player->InBattleground() ? SPELL_CAST_OK : SPELL_FAILED_REQUIRES_AREA;
        case 34976:                                         // Netherstorm Flag
            return map_id == 566 && player && player->InBattleground() ? SPELL_CAST_OK : SPELL_FAILED_REQUIRES_AREA;
        case 2584:                                          // Waiting to Resurrect
        case 22011:                                         // Spirit Heal Channel
        case 22012:                                         // Spirit Heal
        case 24171:                                         // Resurrection Impact Visual
        case 42792:                                         // Recently Dropped Flag
        case 43681:                                         // Inactive
        case 44535:                                         // Spirit Heal (mana)
        {
            MapEntry const* mapEntry = sMapStore.LookupEntry(map_id);
            if (!mapEntry)
                return SPELL_FAILED_INCORRECT_AREA;

            return zone_id == 4197 || (mapEntry->IsBattleground() && player && player->InBattleground()) ? SPELL_CAST_OK : SPELL_FAILED_REQUIRES_AREA;
        }
        case 44521:                                         // Preparation
        {
            if (!player)
                return SPELL_FAILED_REQUIRES_AREA;

            MapEntry const* mapEntry = sMapStore.LookupEntry(map_id);
            if (!mapEntry)
                return SPELL_FAILED_INCORRECT_AREA;

            if (!mapEntry->IsBattleground())
                return SPELL_FAILED_REQUIRES_AREA;

            Battleground* bg = player->GetBattleground();
            return bg && bg->GetStatus() == STATUS_WAIT_JOIN ? SPELL_CAST_OK : SPELL_FAILED_REQUIRES_AREA;
        }
        case 32724:                                         // Gold Team (Alliance)
        case 32725:                                         // Green Team (Alliance)
        case 35774:                                         // Gold Team (Horde)
        case 35775:                                         // Green Team (Horde)
        {
            MapEntry const* mapEntry = sMapStore.LookupEntry(map_id);
            if (!mapEntry)
                return SPELL_FAILED_INCORRECT_AREA;

            return mapEntry->IsBattleArena() && player && player->InBattleground() ? SPELL_CAST_OK : SPELL_FAILED_REQUIRES_AREA;
        }
        case 32727:                                         // Arena Preparation
        {
            if (!player)
                return SPELL_FAILED_REQUIRES_AREA;

            MapEntry const* mapEntry = sMapStore.LookupEntry(map_id);
            if (!mapEntry)
                return SPELL_FAILED_INCORRECT_AREA;

            if (!mapEntry->IsBattleArena())
                return SPELL_FAILED_REQUIRES_AREA;

            Battleground* bg = player->GetBattleground();
            return bg && bg->GetStatus() == STATUS_WAIT_JOIN ? SPELL_CAST_OK : SPELL_FAILED_REQUIRES_AREA;
        }
    }

    // aura limitations
    if (player)
    {
        for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
        {
            if (!Effects[i].IsAura())
                continue;

            switch (Effects[i].ApplyAuraName)
            {
                case SPELL_AURA_MOD_SHAPESHIFT:
                {
                    if (SpellShapeshiftFormEntry const* spellShapeshiftForm = sSpellShapeshiftFormStore.LookupEntry(Effects[i].MiscValue))
                        if (uint32 mountType = spellShapeshiftForm->MountTypeID)
                            if (!player->GetMountCapability(mountType))
                                return SPELL_FAILED_NOT_HERE;
                    break;
                }
                case SPELL_AURA_MOUNTED:
                {
                    if (Effects[i].MiscValueB && !player->GetMountCapability(Effects[i].MiscValueB))
                        return SPELL_FAILED_NOT_HERE;
                    break;
                }
                break;
            }
        }
    }
    return SPELL_CAST_OK;
}

SpellCastResult SpellInfo::CheckTarget(Unit const* caster, WorldObject const* target, bool implicit) const
{
    if (HasAttribute(SPELL_ATTR1_CANT_TARGET_SELF) && caster == target)
        return SPELL_FAILED_BAD_TARGETS;

    // check visibility - ignore stealth for implicit (area) targets
    if (!HasAttribute(SPELL_ATTR6_CAN_TARGET_INVISIBLE) && !caster->CanSeeOrDetect(target, implicit))
        return SPELL_FAILED_BAD_TARGETS;

    Unit const* unitTarget = target->ToUnit();

    // creature/player specific target checks
    if (unitTarget)
    {
        // spells cannot be cast if target has a pet in combat either
        if (HasAttribute(SPELL_ATTR1_CANT_TARGET_IN_COMBAT) && (unitTarget->IsInCombat() || unitTarget->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PET_IN_COMBAT)))
            return SPELL_FAILED_TARGET_AFFECTING_COMBAT;

        // only spells with SPELL_ATTR3_ONLY_TARGET_GHOSTS can target ghosts
        if (HasAttribute(SPELL_ATTR3_ONLY_TARGET_GHOSTS) != unitTarget->HasAuraType(SPELL_AURA_GHOST))
        {
            if (HasAttribute(SPELL_ATTR3_ONLY_TARGET_GHOSTS))
                return SPELL_FAILED_TARGET_NOT_GHOST;
            else
                return SPELL_FAILED_BAD_TARGETS;
        }

        if (caster != unitTarget)
        {
            if (caster->GetTypeId() == TYPEID_PLAYER)
            {
                // Do not allow these spells to target creatures not tapped by us (Banish, Polymorph, many quest spells)
                if (HasAttribute(SPELL_ATTR2_CANT_TARGET_TAPPED))
                    if (Creature const* targetCreature = unitTarget->ToCreature())
                        if (targetCreature->hasLootRecipient() && !targetCreature->isTappedBy(caster->ToPlayer()))
                            return SPELL_FAILED_CANT_CAST_ON_TAPPED;

                if (HasAttribute(SPELL_ATTR0_CU_PICKPOCKET))
                {
                    if (unitTarget->GetTypeId() == TYPEID_PLAYER)
                        return SPELL_FAILED_BAD_TARGETS;
                    else if ((unitTarget->GetCreatureTypeMask() & CREATURE_TYPEMASK_HUMANOID_OR_UNDEAD) == 0)
                        return SPELL_FAILED_TARGET_NO_POCKETS;
                }

                // Not allow disarm unarmed player
                if (Mechanic == MECHANIC_DISARM)
                {
                    if (unitTarget->GetTypeId() == TYPEID_PLAYER)
                    {
                        Player const* player = unitTarget->ToPlayer();
                        if (!player->GetWeaponForAttack(BASE_ATTACK) || !player->IsUseEquipedWeapon(true))
                            return SPELL_FAILED_TARGET_NO_WEAPONS;
                    }
                    else if (!unitTarget->GetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID))
                        return SPELL_FAILED_TARGET_NO_WEAPONS;
                }
            }
        }
    }
    // corpse specific target checks
    else if (Corpse const* corpseTarget = target->ToCorpse())
    {
        // cannot target bare bones
        if (corpseTarget->GetType() == CORPSE_BONES)
            return SPELL_FAILED_BAD_TARGETS;
        // we have to use owner for some checks (aura preventing resurrection for example)
        if (Player* owner = ObjectAccessor::FindPlayer(corpseTarget->GetOwnerGUID()))
            unitTarget = owner;
        // we're not interested in corpses without owner
        else
            return SPELL_FAILED_BAD_TARGETS;
    }
    // other types of objects - always valid
    else return SPELL_CAST_OK;

    // corpseOwner and unit specific target checks
    if (HasAttribute(SPELL_ATTR3_ONLY_TARGET_PLAYERS) && unitTarget->GetTypeId() != TYPEID_PLAYER)
        return SPELL_FAILED_TARGET_NOT_PLAYER;

    if (!IsAllowingDeadTarget() && !unitTarget->IsAlive())
        return SPELL_FAILED_TARGETS_DEAD;

    // check this flag only for implicit targets (chain and area), allow to explicitly target units for spells like Shield of Righteousness
    if (implicit && HasAttribute(SPELL_ATTR6_CANT_TARGET_CROWD_CONTROLLED) && !unitTarget->CanFreeMove())
        return SPELL_FAILED_BAD_TARGETS;

    // Do not allow pet or guardian targets if the spell is a raid buff or may not target pets at all
    if ((HasAttribute(SPELL_ATTR7_CONSOLIDATED_RAID_BUFF) || HasAttribute(SPELL_ATTR5_DONT_ALLOW_PET_TARGET)) && (!caster->IsPet() && !caster->IsGuardian()))
        if ((unitTarget->IsPet() || unitTarget->IsGuardian()))
            return SPELL_FAILED_BAD_TARGETS;

    // checked in Unit::IsValidAttack/AssistTarget, shouldn't be checked for ENTRY targets
    //if (!HasAttribute(SPELL_ATTR6_CAN_TARGET_UNTARGETABLE) && target->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE))
    //    return SPELL_FAILED_BAD_TARGETS;

    //if (!HasAttribute(SPELL_ATTR6_CAN_TARGET_POSSESSED_FRIENDS))

    if (!CheckTargetCreatureType(unitTarget))
    {
        if (target->GetTypeId() == TYPEID_PLAYER)
            return SPELL_FAILED_TARGET_IS_PLAYER;
        else
            return SPELL_FAILED_BAD_TARGETS;
    }

    // check GM mode and GM invisibility - only for player casts (npc casts are controlled by AI) and negative spells
    if (unitTarget != caster && (caster->IsControlledByPlayer() || !IsPositive()) && unitTarget->GetTypeId() == TYPEID_PLAYER)
    {
        if (!unitTarget->ToPlayer()->IsVisible())
            return SPELL_FAILED_BM_OR_INVISGOD;

        if (unitTarget->ToPlayer()->IsGameMaster())
            return SPELL_FAILED_BM_OR_INVISGOD;
    }

    // not allow casting on flying player
    if (unitTarget->HasUnitState(UNIT_STATE_IN_FLIGHT) && !HasAttribute(SPELL_ATTR0_CU_ALLOW_INFLIGHT_TARGET))
        return SPELL_FAILED_BAD_TARGETS;

    /* TARGET_UNIT_MASTER gets blocked here for passengers, because the whole idea of this check is to
    not allow passengers to be implicitly hit by spells, however this target type should be an exception,
    if this is left it kills spells that award kill credit from vehicle to master (few spells),
    the use of these 2 covers passenger target check, logically, if vehicle cast this to master it should always hit
    him, because it would be it's passenger, there's no such case where this gets to fail legitimacy, this problem
    cannot be solved from within the check in other way since target type cannot be called for the spell currently
    Spell examples: [ID - 52864 Devour Water, ID - 52862 Devour Wind, ID - 49370 Wyrmrest Defender: Destabilize Azure Dragonshrine Effect] */
    if (!caster->IsVehicle() && !(caster->GetCharmerOrOwner() == target))
    {
        if (TargetAuraState && !unitTarget->HasAuraState(AuraStateType(TargetAuraState), this, caster))
            return SPELL_FAILED_TARGET_AURASTATE;

        if (TargetAuraStateNot && unitTarget->HasAuraState(AuraStateType(TargetAuraStateNot), this, caster))
            return SPELL_FAILED_TARGET_AURASTATE;
    }

    if (TargetAuraSpell && !unitTarget->HasAura(sSpellMgr->GetSpellIdForDifficulty(TargetAuraSpell, caster)))
        return SPELL_FAILED_TARGET_AURASTATE;

    if (ExcludeTargetAuraSpell && unitTarget->HasAura(sSpellMgr->GetSpellIdForDifficulty(ExcludeTargetAuraSpell, caster)))
        return SPELL_FAILED_TARGET_AURASTATE;

    if (unitTarget->HasAuraType(SPELL_AURA_PREVENT_RESURRECTION))
        if (HasEffect(SPELL_EFFECT_SELF_RESURRECT) || HasEffect(SPELL_EFFECT_RESURRECT) || HasEffect(SPELL_EFFECT_RESURRECT_NEW))
            return SPELL_FAILED_TARGET_CANNOT_BE_RESURRECTED;

    if (!unitTarget->CheckPrivateObjectOwnerVisibility(caster))
        return SPELL_FAILED_BAD_TARGETS;

    return SPELL_CAST_OK;
}

SpellCastResult SpellInfo::CheckExplicitTarget(Unit const* caster, WorldObject const* target, Item const* itemTarget) const
{
    uint32 neededTargets = GetExplicitTargetMask();
    if (!target)
    {
        if (neededTargets & (TARGET_FLAG_UNIT_MASK | TARGET_FLAG_GAMEOBJECT_MASK | TARGET_FLAG_CORPSE_MASK))
            if (!(neededTargets & TARGET_FLAG_GAMEOBJECT_ITEM) || !itemTarget)
                return SPELL_FAILED_BAD_TARGETS;
        return SPELL_CAST_OK;
    }

    if (Unit const* unitTarget = target->ToUnit())
    {
        if (neededTargets & (TARGET_FLAG_UNIT_ENEMY | TARGET_FLAG_UNIT_ALLY | TARGET_FLAG_UNIT_RAID | TARGET_FLAG_UNIT_PARTY | TARGET_FLAG_UNIT_MINIPET | TARGET_FLAG_UNIT_PASSENGER))
        {
            if (neededTargets & TARGET_FLAG_UNIT_ENEMY)
                if (caster->_IsValidAttackTarget(unitTarget, this))
                    return SPELL_CAST_OK;
            if (neededTargets & TARGET_FLAG_UNIT_ALLY
                || (neededTargets & TARGET_FLAG_UNIT_PARTY && caster->IsInPartyWith(unitTarget))
                || (neededTargets & TARGET_FLAG_UNIT_RAID && caster->IsInRaidWith(unitTarget)))
                    if (caster->_IsValidAssistTarget(unitTarget, this))
                        return SPELL_CAST_OK;
            if (neededTargets & TARGET_FLAG_UNIT_MINIPET)
                if (unitTarget->GetGUID() == caster->GetCritterGUID())
                    return SPELL_CAST_OK;
            if (neededTargets & TARGET_FLAG_UNIT_PASSENGER)
                if (unitTarget->IsOnVehicle(caster))
                    return SPELL_CAST_OK;
            return SPELL_FAILED_BAD_TARGETS;
        }
    }
    return SPELL_CAST_OK;
}

SpellCastResult SpellInfo::CheckVehicle(Unit const* caster) const
{
    // All creatures should be able to cast as passengers freely, restriction and attribute are only for players
    if (caster->GetTypeId() != TYPEID_PLAYER)
        return SPELL_CAST_OK;

    Vehicle* vehicle = caster->GetVehicle();
    if (vehicle)
    {
        uint16 checkMask = 0;
        for (uint8 effIndex = EFFECT_0; effIndex < MAX_SPELL_EFFECTS; ++effIndex)
        {
            if (Effects[effIndex].ApplyAuraName == SPELL_AURA_MOD_SHAPESHIFT)
            {
                SpellShapeshiftFormEntry const* shapeShiftFromEntry = sSpellShapeshiftFormStore.LookupEntry(Effects[effIndex].MiscValue);
                if (shapeShiftFromEntry && (shapeShiftFromEntry->Flags & 1) == 0)  // unk flag
                    checkMask |= VEHICLE_SEAT_FLAG_UNCONTROLLED;
                break;
            }
        }

        if (HasAura(SPELL_AURA_MOUNTED))
            checkMask |= VEHICLE_SEAT_FLAG_CAN_CAST_MOUNT_SPELL;

        if (!checkMask)
            checkMask = VEHICLE_SEAT_FLAG_CAN_ATTACK;

        VehicleSeatEntry const* vehicleSeat = vehicle->GetSeatForPassenger(caster);
        if (!vehicleSeat)
            return SPELL_FAILED_CANT_DO_THAT_RIGHT_NOW;

        if (!HasAttribute(SPELL_ATTR6_CASTABLE_WHILE_ON_VEHICLE) && !HasAttribute(SPELL_ATTR0_CASTABLE_WHILE_MOUNTED)
            && (vehicleSeat->Flags & checkMask) != checkMask)
            return SPELL_FAILED_CANT_DO_THAT_RIGHT_NOW;

        // Can only summon uncontrolled minions/guardians when on controlled vehicle
        if (vehicleSeat->Flags & (VEHICLE_SEAT_FLAG_CAN_CONTROL | VEHICLE_SEAT_FLAG_UNK2))
        {
            for (uint32 i = EFFECT_0; i < MAX_SPELL_EFFECTS; ++i)
            {
                if (Effects[i].Effect != SPELL_EFFECT_SUMMON)
                    continue;

                SummonPropertiesEntry const* props = sSummonPropertiesStore.LookupEntry(Effects[i].MiscValueB);
                if (props && props->Control != SUMMON_CATEGORY_WILD)
                    return SPELL_FAILED_CANT_DO_THAT_RIGHT_NOW;
            }
        }
    }

    return SPELL_CAST_OK;
}

bool SpellInfo::CheckTargetCreatureType(Unit const* target) const
{
    // Curse of Doom & Exorcism: not find another way to fix spell target check :/
    if (SpellFamilyName == SPELLFAMILY_WARLOCK && GetCategory() == 1179)
    {
        // not allow cast at player
        if (target->GetTypeId() == TYPEID_PLAYER)
            return false;
        else
            return true;
    }

    // if target is magnet (i.e Grounding Totem) the check is skipped
    if (target->IsMagnet())
        return true;

    uint32 creatureType = target->GetCreatureTypeMask();
    return !TargetCreatureType || !creatureType || (creatureType & TargetCreatureType);
}

SpellSchoolMask SpellInfo::GetSchoolMask() const
{
    return SpellSchoolMask(SchoolMask);
}

uint32 SpellInfo::GetAllEffectsMechanicMask() const
{
    uint32 mask = 0;
    if (Mechanic)
        mask |= 1 << Mechanic;
    for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
        if (Effects[i].IsEffect() && Effects[i].Mechanic)
            mask |= 1 << Effects[i].Mechanic;
    return mask;
}

uint32 SpellInfo::GetEffectMechanicMask(uint8 effIndex) const
{
    uint32 mask = 0;
    if (Mechanic)
        mask |= 1 << Mechanic;
    if (Effects[effIndex].IsEffect() && Effects[effIndex].Mechanic)
        mask |= 1 << Effects[effIndex].Mechanic;
    return mask;
}

uint32 SpellInfo::GetSpellMechanicMaskByEffectMask(uint32 effectMask) const
{
    uint32 mask = 0;
    if (Mechanic)
        mask |= 1 << Mechanic;
    for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
        if ((effectMask & (1 << i)) && Effects[i].Mechanic)
            mask |= 1 << Effects[i].Mechanic;
    return mask;
}

Mechanics SpellInfo::GetEffectMechanic(uint8 effIndex) const
{
    if (Effects[effIndex].IsEffect() && Effects[effIndex].Mechanic)
        return Mechanics(Effects[effIndex].Mechanic);
    if (Mechanic)
        return Mechanics(Mechanic);
    return MECHANIC_NONE;
}

bool SpellInfo::HasAnyEffectMechanic() const
{
    for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
        if (Effects[i].Mechanic)
            return true;
    return false;
}

uint32 SpellInfo::GetDispelMask() const
{
    return GetDispelMask(DispelType(Dispel));
}

uint32 SpellInfo::GetDispelMask(DispelType type)
{
    // If dispel all
    if (type == DISPEL_ALL)
        return DISPEL_ALL_MASK;
    else
        return uint32(1 << type);
}

uint32 SpellInfo::GetExplicitTargetMask() const
{
    return ExplicitTargetMask;
}

AuraStateType SpellInfo::GetAuraState() const
{
    return _auraState;
}

void SpellInfo::_LoadAuraState()
{
    _auraState = [this]() -> AuraStateType
    {
        // Seals
        if (GetSpellSpecific() == SPELL_SPECIFIC_SEAL)
            return AURA_STATE_JUDGEMENT;

        // Conflagrate aura state on Immolate and Shadowflame
        if (SpellFamilyName == SPELLFAMILY_WARLOCK &&
            // Immolate
            ((SpellFamilyFlags[0] & 4) ||
                // Shadowflame
            (SpellFamilyFlags[2] & 2)))
            return AURA_STATE_CONFLAGRATE;

        // Faerie Fire (druid versions)
        if (SpellFamilyName == SPELLFAMILY_DRUID && SpellFamilyFlags[0] & 0x400)
            return AURA_STATE_FAERIE_FIRE;

        // Sting (hunter's pet ability)
        if (GetCategory() == 1133)
            return AURA_STATE_FAERIE_FIRE;

        // Victorious
        if (SpellFamilyName == SPELLFAMILY_WARRIOR &&  SpellFamilyFlags[1] & 0x00040000)
            return AURA_STATE_WARRIOR_VICTORY_RUSH;

        // Swiftmend state on Regrowth & Rejuvenation
        if (SpellFamilyName == SPELLFAMILY_DRUID && SpellFamilyFlags[0] & 0x50)
            return AURA_STATE_SWIFTMEND;

        // Deadly poison aura state
        if (SpellFamilyName == SPELLFAMILY_ROGUE && SpellFamilyFlags[0] & 0x10000)
            return AURA_STATE_DEADLY_POISON;

        // Enrage aura state
        if (Dispel == DISPEL_ENRAGE)
            return AURA_STATE_ENRAGE;

        // Bleeding aura state
        if (GetAllEffectsMechanicMask() & 1 << MECHANIC_BLEED)
            return AURA_STATE_BLEEDING;

        if (GetSchoolMask() & SPELL_SCHOOL_MASK_FROST)
            for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
                if (Effects[i].IsAura() && (Effects[i].ApplyAuraName == SPELL_AURA_MOD_STUN
                    || Effects[i].ApplyAuraName == SPELL_AURA_MOD_ROOT))
                    return AURA_STATE_FROZEN;

        switch (Id)
        {
            case 71465: // Divine Surge
            case 50241: // Evasive Charges
                return AURA_STATE_UNKNOWN22;
            case 9991:  // Touch of Zanzil
            case 35325: // Glowing Blood
            case 35328: // Lambent Blood
            case 35329: // Vibrant Blood
            case 35331: // Black Blood
            case 49163: // Perpetual Instability
                return AURA_STATE_FAERIE_FIRE;
            default:
                break;
        }

        return AURA_STATE_NONE;
    }();
}

SpellSpecificType SpellInfo::GetSpellSpecific() const
{
    return _spellSpecific;
}

void SpellInfo::_LoadSpellSpecific()
{
    _spellSpecific = [this]()->SpellSpecificType
    {
        switch (SpellFamilyName)
        {
            case SPELLFAMILY_GENERIC:
            {
                // Food / Drinks (mostly)
                if (HasAuraInterruptFlag(SpellAuraInterruptFlags::Standing))
                {
                    bool food = false;
                    bool drink = false;
                    for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
                    {
                        if (!Effects[i].IsAura())
                            continue;
                        switch (Effects[i].ApplyAuraName)
                        {
                            // Food
                            case SPELL_AURA_MOD_REGEN:
                            case SPELL_AURA_OBS_MOD_HEALTH:
                                food = true;
                                break;
                                // Drink
                            case SPELL_AURA_MOD_POWER_REGEN:
                            case SPELL_AURA_OBS_MOD_POWER:
                                drink = true;
                                break;
                            default:
                                break;
                        }
                    }

                    if (food && drink)
                        return SPELL_SPECIFIC_FOOD_AND_DRINK;
                    else if (food)
                        return SPELL_SPECIFIC_FOOD;
                    else if (drink)
                        return SPELL_SPECIFIC_DRINK;
                }
                // scrolls effects
                else
                {
                    SpellInfo const* firstRankSpellInfo = GetFirstRankSpell();
                    switch (firstRankSpellInfo->Id)
                    {
                        case 8118: // Strength
                        case 8099: // Stamina
                        case 8112: // Spirit
                        case 8096: // Intellect
                        case 8115: // Agility
                        case 8091: // Armor
                            return SPELL_SPECIFIC_SCROLL;
                        case 12880: // Enrage (Enrage)
                        case 57518: // Enrage (Wrecking Crew)
                            return SPELL_SPECIFIC_WARRIOR_ENRAGE;
                    }
                }
                break;
            }
            case SPELLFAMILY_MAGE:
            {
                // family flags 18(Molten), 25(Frost/Ice), 28(Mage)
                if (SpellFamilyFlags[0] & 0x12040000)
                    return SPELL_SPECIFIC_MAGE_ARMOR;

                // Arcane brillance and Arcane intelect (normal check fails because of flags difference)
                if (SpellFamilyFlags[0] & 0x400)
                    return SPELL_SPECIFIC_MAGE_ARCANE_BRILLANCE;

                if ((SpellFamilyFlags[0] & 0x1000000) && Effects[0].ApplyAuraName == SPELL_AURA_MOD_CONFUSE)
                    return SPELL_SPECIFIC_MAGE_POLYMORPH;

                break;
            }
            case SPELLFAMILY_WARRIOR:
            {
                if (Id == 12292) // Death Wish
                    return SPELL_SPECIFIC_WARRIOR_ENRAGE;

                break;
            }
            case SPELLFAMILY_WARLOCK:
            {
                // Warlock (Bane of Doom | Bane of Agony | Bane of Havoc)
                if (Id == 603 || Id == 980 || Id == 80240)
                    return SPELL_SPECIFIC_BANE;

                // only warlock curses have this
                if (Dispel == DISPEL_CURSE)
                    return SPELL_SPECIFIC_CURSE;

                // Warlock (Demon Armor | Demon Skin | Fel Armor)
                if (SpellFamilyFlags[1] & 0x20000020 || SpellFamilyFlags[2] & 0x00000010)
                    return SPELL_SPECIFIC_WARLOCK_ARMOR;

                //seed of corruption and corruption
                if (SpellFamilyFlags[1] & 0x10 || SpellFamilyFlags[0] & 0x2)
                    return SPELL_SPECIFIC_WARLOCK_CORRUPTION;
                break;
            }
            case SPELLFAMILY_PRIEST:
            {
                // Divine Spirit and Prayer of Spirit
                if (SpellFamilyFlags[0] & 0x20)
                    return SPELL_SPECIFIC_PRIEST_DIVINE_SPIRIT;

                break;
            }
            case SPELLFAMILY_HUNTER:
            {
                // only hunter stings have this
                if (Dispel == DISPEL_POISON)
                    return SPELL_SPECIFIC_STING;

                // only hunter aspects have this (but not all aspects in hunter family)
                if (SpellFamilyFlags.HasFlag(0x00380000, 0x00440000, 0x00001010))
                    return SPELL_SPECIFIC_ASPECT;

                break;
            }
            case SPELLFAMILY_PALADIN:
            {
                // Collection of all the seal family flags. No other paladin spell has any of those.
                if (SpellFamilyFlags[1] & 0xA2000800)
                    return SPELL_SPECIFIC_SEAL;

                if (SpellFamilyFlags[0] & 0x00002190)
                    return SPELL_SPECIFIC_HAND;

                // Judgement
                if (Id == 20271)
                    return SPELL_SPECIFIC_JUDGEMENT;

                // only paladin auras have this (for palaldin class family)
                if (SpellFamilyFlags[2] & 0x00000020)
                    return SPELL_SPECIFIC_AURA;

                break;
            }
            case SPELLFAMILY_SHAMAN:
            {
                // family flags 10 (Lightning), 42 (Earth), 37 (Water), proc shield from T2 8 pieces bonus
                if (SpellFamilyFlags[1] & 0x420
                    || SpellFamilyFlags[0] & 0x00000400
                    || Id == 23552)
                    return SPELL_SPECIFIC_ELEMENTAL_SHIELD;

                break;
            }
            case SPELLFAMILY_DEATHKNIGHT:
                if (Id == 48266 || Id == 48263 || Id == 48265)
                    return SPELL_SPECIFIC_PRESENCE;
                break;
        }

        for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
        {
            if (Effects[i].Effect == SPELL_EFFECT_APPLY_AURA)
            {
                switch (Effects[i].ApplyAuraName)
                {
                    case SPELL_AURA_MOD_CHARM:
                    case SPELL_AURA_MOD_POSSESS:
                    case SPELL_AURA_AOE_CHARM:
                        return SPELL_SPECIFIC_CHARM;
                    case SPELL_AURA_TRACK_CREATURES:
                        /// @workaround For non-stacking tracking spells (We need generic solution)
                        if (Id == 30645) // Gas Cloud Tracking
                            return SPELL_SPECIFIC_NORMAL;
                        [[fallthrough]];
                    case SPELL_AURA_TRACK_RESOURCES:
                    case SPELL_AURA_TRACK_STEALTHED:
                        return SPELL_SPECIFIC_TRACKER;
                }
            }
        }

        return SPELL_SPECIFIC_NORMAL;
    }();
}

void SpellInfo::_LoadSpellDiminishInfo()
{
    auto diminishingGroupCompute = [this](bool triggered) -> DiminishingGroup
    {
        if (IsPositive())
            return DIMINISHING_NONE;

        for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
        {
            if (Effects[i].ApplyAuraName == SPELL_AURA_MOD_TAUNT)
                return DIMINISHING_TAUNT;
        }

        // Explicit Diminishing Groups
        switch (SpellFamilyName)
        {
            case SPELLFAMILY_GENERIC:
            {
                // Pet charge effects (Infernal Awakening, Demon Charge)
                if (SpellVisual[0] == 2816 && SpellIconID == 15)
                    return DIMINISHING_CONTROLLED_STUN;
                // Frost Tomb
                else if (Id == 48400)
                    return DIMINISHING_NONE;
                // Gnaw
                else if (Id == 47481)
                    return DIMINISHING_CONTROLLED_STUN;
                // ToC Icehowl Arctic Breath
                else if (SpellVisual[0] == 14153)
                    return DIMINISHING_NONE;
                // Black Plague
                else if (Id == 64155)
                    return DIMINISHING_NONE;
                // Screams of the Dead (King Ymiron)
                else if (Id == 51750)
                    return DIMINISHING_NONE;
                break;
            }
            // Event spells
            case SPELLFAMILY_UNK1:
                return DIMINISHING_NONE;
            case SPELLFAMILY_MAGE:
            {
                // Frostbite
                if (SpellFamilyFlags[1] & 0x80000000)
                    return DIMINISHING_ROOT;
                // Shattered Barrier
                else if (SpellVisual[0] == 12297)
                    return DIMINISHING_ROOT;
                // Deep Freeze
                else if (SpellIconID == 2939 && SpellVisual[0] == 9963)
                    return DIMINISHING_CONTROLLED_STUN;
                // Frost Nova / Freeze (Water Elemental)
                else if (SpellIconID == 193)
                    return DIMINISHING_CONTROLLED_ROOT;
                // Dragon's Breath
                else if (SpellFamilyFlags[0] & 0x800000)
                    return DIMINISHING_DRAGONS_BREATH;
                break;
            }
            case SPELLFAMILY_WARRIOR:
            {
                // Hamstring - limit duration to 10s in PvP
                if (SpellFamilyFlags[0] & 0x2)
                    return DIMINISHING_LIMITONLY;
                // Charge Stun (own diminishing)
                else if (SpellFamilyFlags[0] & 0x01000000)
                    return DIMINISHING_CHARGE;
                break;
            }
            case SPELLFAMILY_WARLOCK:
            {
                // Curses/etc
                if ((SpellFamilyFlags[0] & 0x80000000) || (SpellFamilyFlags[1] & 0x200))
                    return DIMINISHING_LIMITONLY;
                // Seduction
                else if (SpellFamilyFlags[1] & 0x10000000)
                    return DIMINISHING_FEAR;
                // Sin and Punishment (Priest spell, don't ask)
                else if (SpellIconID == 1869)
                    return DIMINISHING_NONE;
                break;
            }
            case SPELLFAMILY_DRUID:
            {
                // Pounce
                if (SpellFamilyFlags[0] & 0x20000)
                    return DIMINISHING_OPENING_STUN;
                // Cyclone
                else if (SpellFamilyFlags[1] & 0x20)
                    return DIMINISHING_CYCLONE;
                // Entangling Roots
                // Nature's Grasp
                else if (SpellFamilyFlags[0] & 0x00000200)
                    return DIMINISHING_CONTROLLED_ROOT;
                // Faerie Fire
                else if (SpellFamilyFlags[0] & 0x400)
                    return DIMINISHING_LIMITONLY;
                break;
            }
            case SPELLFAMILY_ROGUE:
            {
                // Gouge
                if (SpellFamilyFlags[0] & 0x8)
                    return DIMINISHING_DISORIENT;
                // Blind
                else if (SpellFamilyFlags[0] & 0x1000000)
                    return DIMINISHING_FEAR;
                // Cheap Shot
                else if (SpellFamilyFlags[0] & 0x400)
                    return DIMINISHING_OPENING_STUN;
                // Crippling poison - Limit to 10 seconds in PvP (No SpellFamilyFlags)
                else if (SpellIconID == 163)
                    return DIMINISHING_LIMITONLY;
                break;
            }
            case SPELLFAMILY_HUNTER:
            {
                // Hunter's Mark
                if ((SpellFamilyFlags[0] & 0x400) && SpellIconID == 538)
                    return DIMINISHING_LIMITONLY;
                // Scatter Shot (own diminishing)
                else if ((SpellFamilyFlags[0] & 0x40000) && SpellIconID == 132)
                    return DIMINISHING_SCATTER_SHOT;
                // Entrapment (own diminishing)
                else if (SpellVisual[0] == 7484 && SpellIconID == 20)
                    return DIMINISHING_ENTRAPMENT;
                // Wyvern Sting mechanic is MECHANIC_SLEEP but the diminishing is DIMINISHING_DISORIENT
                else if ((SpellFamilyFlags[1] & 0x1000) && SpellIconID == 1721)
                    return DIMINISHING_DISORIENT;
                // Freezing Arrow
                else if (SpellFamilyFlags[0] & 0x8)
                    return DIMINISHING_DISORIENT;
                break;
            }
            case SPELLFAMILY_PALADIN:
            {
                // Judgement of Justice - limit duration to 10s in PvP
                if (SpellFamilyFlags[0] & 0x100000)
                    return DIMINISHING_LIMITONLY;
                // Turn Evil
                else if ((SpellFamilyFlags[1] & 0x804000) && SpellIconID == 309)
                    return DIMINISHING_FEAR;
                break;
            }
            case SPELLFAMILY_SHAMAN:
            {
                // Storm, Earth and Fire - Earthgrab
                if (SpellFamilyFlags[2] & 0x4000)
                    return DIMINISHING_NONE;
                break;
            }
            case SPELLFAMILY_DEATHKNIGHT:
            {
                // Hungering Cold (no flags)
                if (SpellIconID == 2797)
                    return DIMINISHING_DISORIENT;
                // Mark of Blood
                else if ((SpellFamilyFlags[0] & 0x10000000) && SpellIconID == 2285)
                    return DIMINISHING_LIMITONLY;
                break;
            }
            default:
                break;
        }

        // Lastly - Set diminishing depending on mechanic
        uint32 mechanic = GetAllEffectsMechanicMask();
        if (mechanic & (1 << MECHANIC_CHARM))
            return DIMINISHING_MIND_CONTROL;
        if (mechanic & (1 << MECHANIC_SILENCE))
            return DIMINISHING_SILENCE;
        if (mechanic & (1 << MECHANIC_SLEEP))
            return DIMINISHING_SLEEP;
        if (mechanic & ((1 << MECHANIC_SAPPED) | (1 << MECHANIC_POLYMORPH) | (1 << MECHANIC_SHACKLE)))
            return DIMINISHING_DISORIENT;
        // Mechanic Knockout, except Blast Wave
        if (mechanic & (1 << MECHANIC_KNOCKOUT) && SpellIconID != 292)
            return DIMINISHING_DISORIENT;
        if (mechanic & (1 << MECHANIC_DISARM))
            return DIMINISHING_DISARM;
        if (mechanic & (1 << MECHANIC_FEAR))
            return DIMINISHING_FEAR;
        if (mechanic & (1 << MECHANIC_STUN))
            return triggered ? DIMINISHING_STUN : DIMINISHING_CONTROLLED_STUN;
        if (mechanic & (1 << MECHANIC_BANISH))
            return DIMINISHING_BANISH;
        if (mechanic & (1 << MECHANIC_ROOT))
            return triggered ? DIMINISHING_ROOT : DIMINISHING_CONTROLLED_ROOT;
        if (mechanic & (1 << MECHANIC_HORROR))
            return DIMINISHING_HORROR;

        return DIMINISHING_NONE;
    };

    auto diminishingTypeCompute = [](DiminishingGroup group) -> DiminishingReturnsType
    {
        switch (group)
        {
            case DIMINISHING_TAUNT:
            case DIMINISHING_CONTROLLED_STUN:
            case DIMINISHING_STUN:
            case DIMINISHING_OPENING_STUN:
            case DIMINISHING_CYCLONE:
            case DIMINISHING_CHARGE:
                return DRTYPE_ALL;
            case DIMINISHING_LIMITONLY:
            case DIMINISHING_NONE:
                return DRTYPE_NONE;
            default:
                return DRTYPE_PLAYER;
        }
    };

    auto diminishingMaxLevelCompute = [](DiminishingGroup group) -> DiminishingLevels
    {
        switch (group)
        {
            case DIMINISHING_TAUNT:
                return DIMINISHING_LEVEL_TAUNT_IMMUNE;
            default:
                return DIMINISHING_LEVEL_IMMUNE;
        }
    };

    auto diminishingLimitDurationCompute = [this](DiminishingGroup group) -> int32
    {
        auto isGroupDurationLimited = [group]() -> bool
        {
            switch (group)
            {
                case DIMINISHING_BANISH:
                case DIMINISHING_CONTROLLED_STUN:
                case DIMINISHING_CONTROLLED_ROOT:
                case DIMINISHING_CYCLONE:
                case DIMINISHING_DISORIENT:
                case DIMINISHING_ENTRAPMENT:
                case DIMINISHING_FEAR:
                case DIMINISHING_HORROR:
                case DIMINISHING_MIND_CONTROL:
                case DIMINISHING_OPENING_STUN:
                case DIMINISHING_ROOT:
                case DIMINISHING_STUN:
                case DIMINISHING_SLEEP:
                case DIMINISHING_LIMITONLY:
                    return true;
                default:
                    return false;
            }
        };

        if (!isGroupDurationLimited())
            return 0;

        // Explicit diminishing duration
        switch (SpellFamilyName)
        {
            case SPELLFAMILY_DRUID:
            {
                // Faerie Fire - limit to 40 seconds in PvP (3.1)
                if (SpellFamilyFlags[0] & 0x400)
                    return 40 * IN_MILLISECONDS;
                break;
            }
            case SPELLFAMILY_HUNTER:
            {
                // Wyvern Sting
                if (SpellFamilyFlags[1] & 0x1000)
                    return 6 * IN_MILLISECONDS;
                // Hunter's Mark
                if (SpellFamilyFlags[0] & 0x400)
                    return 30 * IN_MILLISECONDS;
                break;
            }
            case SPELLFAMILY_PALADIN:
            {
                // Repentance - limit to 6 seconds in PvP
                if (SpellFamilyFlags[0] & 0x4)
                    return 6 * IN_MILLISECONDS;
                break;
            }
            case SPELLFAMILY_WARLOCK:
            {
                // Banish - limit to 6 seconds in PvP
                if (SpellFamilyFlags[1] & 0x8000000)
                    return 6 * IN_MILLISECONDS;
                // Curse of Tongues - limit to 12 seconds in PvP
                else if (SpellFamilyFlags[2] & 0x800)
                    return 12 * IN_MILLISECONDS;
                // Curse of Elements - limit to 120 seconds in PvP
                else if (SpellFamilyFlags[1] & 0x200)
                    return 120 * IN_MILLISECONDS;
                break;
            }
            default:
                break;
        }

        return 8 * IN_MILLISECONDS;
    };

    SpellDiminishInfo triggeredInfo, normalInfo;
    triggeredInfo.DiminishGroup = diminishingGroupCompute(true);
    triggeredInfo.DiminishReturnType = diminishingTypeCompute(triggeredInfo.DiminishGroup);
    triggeredInfo.DiminishMaxLevel = diminishingMaxLevelCompute(triggeredInfo.DiminishGroup);
    triggeredInfo.DiminishDurationLimit = diminishingLimitDurationCompute(triggeredInfo.DiminishGroup);

    normalInfo.DiminishGroup = diminishingGroupCompute(false);
    normalInfo.DiminishReturnType = diminishingTypeCompute(normalInfo.DiminishGroup);
    normalInfo.DiminishMaxLevel = diminishingMaxLevelCompute(normalInfo.DiminishGroup);
    normalInfo.DiminishDurationLimit = diminishingLimitDurationCompute(normalInfo.DiminishGroup);

    _diminishInfoTriggered = triggeredInfo;
    _diminishInfoNonTriggered = normalInfo;
}

DiminishingGroup SpellInfo::GetDiminishingReturnsGroupForSpell(bool triggered) const
{
    return triggered ? _diminishInfoTriggered.DiminishGroup : _diminishInfoNonTriggered.DiminishGroup;
}

DiminishingReturnsType SpellInfo::GetDiminishingReturnsGroupType(bool triggered) const
{
    return triggered ? _diminishInfoTriggered.DiminishReturnType : _diminishInfoNonTriggered.DiminishReturnType;
}

DiminishingLevels SpellInfo::GetDiminishingReturnsMaxLevel(bool triggered) const
{
    return triggered ? _diminishInfoTriggered.DiminishMaxLevel : _diminishInfoNonTriggered.DiminishMaxLevel;
}

int32 SpellInfo::GetDiminishingReturnsLimitDuration(bool triggered) const
{
    return triggered ? _diminishInfoTriggered.DiminishDurationLimit : _diminishInfoNonTriggered.DiminishDurationLimit;
}

void SpellInfo::_LoadImmunityInfo()
{
    for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
    {
        uint32 schoolImmunityMask = 0;
        uint32 applyHarmfulAuraImmunityMask = 0;
        uint32 mechanicImmunityMask = 0;
        uint32 dispelImmunity = 0;
        uint32 damageImmunityMask = 0;

        int32 miscVal = Effects[i].MiscValue;
        int32 amount = Effects[i].CalcValue();

        ImmunityInfo& immuneInfo = _immunityInfo[i];

        switch (Effects[i].ApplyAuraName)
        {
            case SPELL_AURA_MECHANIC_IMMUNITY_MASK:
            {
                switch (miscVal)
                {
                    case 96:   // Free Friend, Uncontrollable Frenzy, Warlord's Presence
                    {
                        mechanicImmunityMask |= IMMUNE_TO_MOVEMENT_IMPAIRMENT_AND_LOSS_CONTROL_MASK;

                        immuneInfo.AuraTypeImmune.insert(SPELL_AURA_MOD_STUN);
                        immuneInfo.AuraTypeImmune.insert(SPELL_AURA_MOD_DECREASE_SPEED);
                        immuneInfo.AuraTypeImmune.insert(SPELL_AURA_MOD_ROOT);
                        immuneInfo.AuraTypeImmune.insert(SPELL_AURA_MOD_CONFUSE);
                        immuneInfo.AuraTypeImmune.insert(SPELL_AURA_MOD_FEAR);
                        break;
                    }
                    case 1615: // Incite Rage, Wolf Spirit, Overload, Lightning Tendrils
                    {
                        switch (Id)
                        {
                            case 43292: // Incite Rage
                            case 49172: // Wolf Spirit
                                mechanicImmunityMask |= IMMUNE_TO_MOVEMENT_IMPAIRMENT_AND_LOSS_CONTROL_MASK;

                                immuneInfo.AuraTypeImmune.insert(SPELL_AURA_MOD_STUN);
                                immuneInfo.AuraTypeImmune.insert(SPELL_AURA_MOD_DECREASE_SPEED);
                                immuneInfo.AuraTypeImmune.insert(SPELL_AURA_MOD_ROOT);
                                immuneInfo.AuraTypeImmune.insert(SPELL_AURA_MOD_CONFUSE);
                                immuneInfo.AuraTypeImmune.insert(SPELL_AURA_MOD_FEAR);
                                [[fallthrough]];
                            case 61869: // Overload
                            case 63481:
                            case 61887: // Lightning Tendrils
                            case 63486:
                                mechanicImmunityMask |= (1 << MECHANIC_INTERRUPT) | (1 << MECHANIC_SILENCE);

                                immuneInfo.SpellEffectImmune.insert(SPELL_EFFECT_KNOCK_BACK);
                                immuneInfo.SpellEffectImmune.insert(SPELL_EFFECT_KNOCK_BACK_DEST);
                                break;
                            default:
                                break;
                        }
                        break;
                    }
                    case 679:  // Mind Control, Avenging Fury
                    {
                        if (Id == 57742) // Avenging Fury
                        {
                            mechanicImmunityMask |= IMMUNE_TO_MOVEMENT_IMPAIRMENT_AND_LOSS_CONTROL_MASK;

                            immuneInfo.AuraTypeImmune.insert(SPELL_AURA_MOD_STUN);
                            immuneInfo.AuraTypeImmune.insert(SPELL_AURA_MOD_DECREASE_SPEED);
                            immuneInfo.AuraTypeImmune.insert(SPELL_AURA_MOD_ROOT);
                            immuneInfo.AuraTypeImmune.insert(SPELL_AURA_MOD_CONFUSE);
                            immuneInfo.AuraTypeImmune.insert(SPELL_AURA_MOD_FEAR);
                        }
                        break;
                    }
                    case 1557: // Startling Roar, Warlord Roar, Break Bonds, Stormshield
                    {
                        if (Id == 64187) // Stormshield
                        {
                            mechanicImmunityMask |= (1 << MECHANIC_STUN);
                            immuneInfo.AuraTypeImmune.insert(SPELL_AURA_MOD_STUN);
                        }
                        else
                        {
                            mechanicImmunityMask |= IMMUNE_TO_MOVEMENT_IMPAIRMENT_AND_LOSS_CONTROL_MASK;

                            immuneInfo.AuraTypeImmune.insert(SPELL_AURA_MOD_STUN);
                            immuneInfo.AuraTypeImmune.insert(SPELL_AURA_MOD_DECREASE_SPEED);
                            immuneInfo.AuraTypeImmune.insert(SPELL_AURA_MOD_ROOT);
                            immuneInfo.AuraTypeImmune.insert(SPELL_AURA_MOD_CONFUSE);
                            immuneInfo.AuraTypeImmune.insert(SPELL_AURA_MOD_FEAR);
                        }
                        break;
                    }
                    case 1614: // Fixate
                    case 1694: // Fixated, Lightning Tendrils
                    {
                        immuneInfo.SpellEffectImmune.insert(SPELL_EFFECT_ATTACK_ME);
                        immuneInfo.AuraTypeImmune.insert(SPELL_AURA_MOD_TAUNT);
                        break;
                    }
                    case 1630: // Fervor, Berserk
                    {
                        if (Id == 64112) // Berserk
                        {
                            immuneInfo.SpellEffectImmune.insert(SPELL_EFFECT_ATTACK_ME);
                            immuneInfo.AuraTypeImmune.insert(SPELL_AURA_MOD_TAUNT);
                        }
                        else
                        {
                            mechanicImmunityMask |= IMMUNE_TO_MOVEMENT_IMPAIRMENT_AND_LOSS_CONTROL_MASK;

                            immuneInfo.AuraTypeImmune.insert(SPELL_AURA_MOD_STUN);
                            immuneInfo.AuraTypeImmune.insert(SPELL_AURA_MOD_DECREASE_SPEED);
                            immuneInfo.AuraTypeImmune.insert(SPELL_AURA_MOD_ROOT);
                            immuneInfo.AuraTypeImmune.insert(SPELL_AURA_MOD_CONFUSE);
                            immuneInfo.AuraTypeImmune.insert(SPELL_AURA_MOD_FEAR);
                        }
                        break;
                    }
                    case 477:  // Bladestorm
                    case 1733: // Bladestorm, Killing Spree
                    {
                        if (!amount)
                        {
                            mechanicImmunityMask |= IMMUNE_TO_MOVEMENT_IMPAIRMENT_AND_LOSS_CONTROL_MASK;

                            immuneInfo.SpellEffectImmune.insert(SPELL_EFFECT_KNOCK_BACK);
                            immuneInfo.SpellEffectImmune.insert(SPELL_EFFECT_KNOCK_BACK_DEST);

                            immuneInfo.AuraTypeImmune.insert(SPELL_AURA_MOD_STUN);
                            immuneInfo.AuraTypeImmune.insert(SPELL_AURA_MOD_DECREASE_SPEED);
                            immuneInfo.AuraTypeImmune.insert(SPELL_AURA_MOD_ROOT);
                            immuneInfo.AuraTypeImmune.insert(SPELL_AURA_MOD_CONFUSE);
                            immuneInfo.AuraTypeImmune.insert(SPELL_AURA_MOD_FEAR);
                        }
                        break;
                    }
                    case 878: // Whirlwind, Fog of Corruption, Determination
                    {
                        if (Id == 66092) // Determination
                        {
                            mechanicImmunityMask |= (1 << MECHANIC_SNARE) | (1 << MECHANIC_STUN)
                                | (1 << MECHANIC_DISORIENTED) | (1 << MECHANIC_FREEZE);

                            immuneInfo.AuraTypeImmune.insert(SPELL_AURA_MOD_STUN);
                            immuneInfo.AuraTypeImmune.insert(SPELL_AURA_MOD_DECREASE_SPEED);
                        }
                        break;
                    }
                    default:
                        break;
                }

                if (immuneInfo.AuraTypeImmune.empty())
                {
                    if (miscVal & (1 << 10))
                        immuneInfo.AuraTypeImmune.insert(SPELL_AURA_MOD_STUN);
                    if (miscVal & (1 << 1))
                        immuneInfo.AuraTypeImmune.insert(SPELL_AURA_TRANSFORM);

                    // These flag can be recognized wrong:
                    if (miscVal & (1 << 6))
                        immuneInfo.AuraTypeImmune.insert(SPELL_AURA_MOD_DECREASE_SPEED);
                    if (miscVal & (1 << 0))
                        immuneInfo.AuraTypeImmune.insert(SPELL_AURA_MOD_ROOT);
                    if (miscVal & (1 << 2))
                        immuneInfo.AuraTypeImmune.insert(SPELL_AURA_MOD_CONFUSE);
                    if (miscVal & (1 << 9))
                        immuneInfo.AuraTypeImmune.insert(SPELL_AURA_MOD_FEAR);
                    if (miscVal & (1 << 7))
                        immuneInfo.AuraTypeImmune.insert(SPELL_AURA_MOD_DISARM);
                }
                break;
            }
            case SPELL_AURA_MECHANIC_IMMUNITY:
            {
                switch (Id)
                {
                    case 42292: // PvP trinket
                    case 59752: // Every Man for Himself
                        mechanicImmunityMask |= IMMUNE_TO_MOVEMENT_IMPAIRMENT_AND_LOSS_CONTROL_MASK;
                        immuneInfo.AuraTypeImmune.insert(SPELL_AURA_USE_NORMAL_MOVEMENT_SPEED);
                        break;
                    case 34471: // The Beast Within
                    case 19574: // Bestial Wrath
                    case 53490: // Bullheaded
                        mechanicImmunityMask |= IMMUNE_TO_MOVEMENT_IMPAIRMENT_AND_LOSS_CONTROL_MASK;
                        break;
                    case 54508: // Demonic Empowerment
                        mechanicImmunityMask |= (1 << MECHANIC_SNARE) | (1 << MECHANIC_ROOT) | (1 << MECHANIC_STUN);
                        break;
                    default:
                        if (miscVal < 1)
                            continue;

                        mechanicImmunityMask |= 1 << miscVal;
                        break;
                }
                break;
            }
            case SPELL_AURA_EFFECT_IMMUNITY:
            {
                immuneInfo.SpellEffectImmune.insert(static_cast<SpellEffects>(miscVal));
                break;
            }
            case SPELL_AURA_STATE_IMMUNITY:
            {
                immuneInfo.AuraTypeImmune.insert(static_cast<AuraType>(miscVal));
                break;
            }
            case SPELL_AURA_SCHOOL_IMMUNITY:
            {
                schoolImmunityMask |= uint32(miscVal);
                break;
            }
            case SPELL_AURA_MOD_IMMUNE_AURA_APPLY_SCHOOL:
            {
                applyHarmfulAuraImmunityMask |= uint32(miscVal);
                break;
            }
            case SPELL_AURA_DAMAGE_IMMUNITY:
            {
                damageImmunityMask |= uint32(miscVal);
                break;
            }
            case SPELL_AURA_DISPEL_IMMUNITY:
            {
                dispelImmunity = uint32(miscVal);
                break;
            }
            default:
                break;
        }

        immuneInfo.SchoolImmuneMask = schoolImmunityMask;
        immuneInfo.ApplyHarmfulAuraImmuneMask = applyHarmfulAuraImmunityMask;
        immuneInfo.MechanicImmuneMask = mechanicImmunityMask;
        immuneInfo.DispelImmune = dispelImmunity;
        immuneInfo.DamageSchoolMask = damageImmunityMask;

        immuneInfo.AuraTypeImmune.shrink_to_fit();
        immuneInfo.SpellEffectImmune.shrink_to_fit();

        _allowedMechanicMask |= immuneInfo.MechanicImmuneMask;
    }

    if (HasAttribute(SPELL_ATTR5_USABLE_WHILE_STUNNED))
    {
        switch (Id)
        {
            case 22812: // Barkskin
            case 47585: // Dispersion
                _allowedMechanicMask |=
                    (1 << MECHANIC_STUN) |
                    (1 << MECHANIC_FREEZE) |
                    (1 << MECHANIC_KNOCKOUT) |
                    (1 << MECHANIC_SLEEP);
                break;
            case 49039: // Lichborne, don't allow normal stuns
                break;
            default:
                _allowedMechanicMask |= (1 << MECHANIC_STUN);
                break;
        }
    }

    if (HasAttribute(SPELL_ATTR5_USABLE_WHILE_CONFUSED))
        _allowedMechanicMask |= (1 << MECHANIC_DISORIENTED);

    if (HasAttribute(SPELL_ATTR5_USABLE_WHILE_FEARED))
    {
        switch (Id)
        {
            case 22812: // Barkskin
            case 47585: // Dispersion
                _allowedMechanicMask |= (1 << MECHANIC_FEAR) | (1 << MECHANIC_HORROR);
                break;
            default:
                _allowedMechanicMask |= (1 << MECHANIC_FEAR);
                break;
        }
    }
}

void SpellInfo::ApplyAllSpellImmunitiesTo(Unit* target, uint8 effIndex, bool apply) const
{
    ImmunityInfo const* immuneInfo = _immunityInfo + effIndex;

    if (uint32 schoolImmunity = immuneInfo->SchoolImmuneMask)
    {
        target->ApplySpellImmune(Id, IMMUNITY_SCHOOL, schoolImmunity, apply);

        if (apply && HasAttribute(SPELL_ATTR1_DISPEL_AURAS_ON_IMMUNITY))
        {
            target->RemoveAppliedAuras([this, schoolImmunity](AuraApplication const* aurApp) -> bool
            {
                SpellInfo const* auraSpellInfo = aurApp->GetBase()->GetSpellInfo();
                return ((auraSpellInfo->GetSchoolMask() & schoolImmunity) != 0 && // Check for school mask
                    CanDispelAura(auraSpellInfo) &&
                    (IsPositive() != aurApp->IsPositive()) &&                     // Check spell vs aura possitivity
                    !auraSpellInfo->IsPassive() &&                                // Don't remove passive auras
                    auraSpellInfo->Id != Id);                                     // Don't remove self
            });
        }
        if (apply && schoolImmunity & SPELL_SCHOOL_MASK_NORMAL)
            target->RemoveAurasWithInterruptFlags(SpellAuraInterruptFlags::InvulnerabilityBuff);
    }

    if (uint32 mechanicImmunity = immuneInfo->MechanicImmuneMask)
    {
        for (uint32 i = MECHANIC_CHARM; i < MAX_MECHANIC; ++i)
            if (mechanicImmunity & (1 << i))
                target->ApplySpellImmune(Id, IMMUNITY_MECHANIC, i, apply);

        if(apply && HasAttribute(SPELL_ATTR1_DISPEL_AURAS_ON_IMMUNITY))
            target->RemoveAurasWithMechanic(mechanicImmunity, AuraRemoveFlags::ByDefault, Id);
    }

    if (uint32 dispelImmunity = immuneInfo->DispelImmune)
    {
        target->ApplySpellImmune(Id, IMMUNITY_DISPEL, dispelImmunity, apply);

        if (apply && HasAttribute(SPELL_ATTR1_DISPEL_AURAS_ON_IMMUNITY))
        {
            target->RemoveAppliedAuras([dispelImmunity](AuraApplication const* aurApp) -> bool
            {
                SpellInfo const* spellInfo = aurApp->GetBase()->GetSpellInfo();
                if (spellInfo->Dispel == dispelImmunity)
                    return true;

                return false;
            });
        }
    }

    if (uint32 damageImmunity = immuneInfo->DamageSchoolMask)
    {
        target->ApplySpellImmune(Id, IMMUNITY_DAMAGE, damageImmunity, apply);

        if (apply && damageImmunity & SPELL_SCHOOL_MASK_NORMAL)
            target->RemoveAurasWithInterruptFlags(SpellAuraInterruptFlags::InvulnerabilityBuff);
    }

    for (AuraType auraType : immuneInfo->AuraTypeImmune)
    {
        target->ApplySpellImmune(Id, IMMUNITY_STATE, auraType, apply);
        if (apply && HasAttribute(SPELL_ATTR1_DISPEL_AURAS_ON_IMMUNITY))
            target->RemoveAurasByType(auraType);
    }

    for (SpellEffects effectType : immuneInfo->SpellEffectImmune)
        target->ApplySpellImmune(Id, IMMUNITY_EFFECT, effectType, apply);
}

bool SpellInfo::CanSpellProvideImmunityAgainstAura(SpellInfo const* auraSpellInfo) const
{
    if (!auraSpellInfo)
        return false;

    for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
    {
        ImmunityInfo const* immuneInfo = _immunityInfo + i;

        if (!auraSpellInfo->HasAttribute(SPELL_ATTR1_UNAFFECTED_BY_SCHOOL_IMMUNE) && !auraSpellInfo->HasAttribute(SPELL_ATTR2_UNAFFECTED_BY_AURA_SCHOOL_IMMUNE))
        {
            if (uint32 schoolImmunity = immuneInfo->SchoolImmuneMask)
                if ((auraSpellInfo->SchoolMask & schoolImmunity) != 0)
                    return true;
        }

        if (uint32 mechanicImmunity = immuneInfo->MechanicImmuneMask)
            if ((mechanicImmunity & (1 << auraSpellInfo->Mechanic)) != 0)
                return true;

        if (uint32 dispelImmunity = immuneInfo->DispelImmune)
            if (auraSpellInfo->Dispel == dispelImmunity)
                return true;

        bool immuneToAllEffects = true;
        for (uint8 effIndex = 0; effIndex < MAX_SPELL_EFFECTS; ++effIndex)
        {
            uint32 effectName = auraSpellInfo->Effects[effIndex].Effect;
            if (!effectName)
                continue;

            auto spellImmuneItr = immuneInfo->SpellEffectImmune.find(static_cast<SpellEffects>(effectName));
            if (spellImmuneItr == immuneInfo->SpellEffectImmune.cend())
            {
                immuneToAllEffects = false;
                break;
            }

            if (uint32 mechanic = auraSpellInfo->Effects[effIndex].Mechanic)
            {
                if (!(immuneInfo->MechanicImmuneMask & (1 << mechanic)))
                {
                    immuneToAllEffects = false;
                    break;
                }
            }

            if (!auraSpellInfo->HasAttribute(SPELL_ATTR3_IGNORE_HIT_RESULT))
            {
                if (uint32 auraName = auraSpellInfo->Effects[effIndex].ApplyAuraName)
                {
                    bool isImmuneToAuraEffectApply = false;
                    auto auraImmuneItr = immuneInfo->AuraTypeImmune.find(static_cast<AuraType>(auraName));
                    if (auraImmuneItr != immuneInfo->AuraTypeImmune.cend())
                        isImmuneToAuraEffectApply = true;

                    if (!isImmuneToAuraEffectApply && !auraSpellInfo->IsPositiveEffect(effIndex) && !auraSpellInfo->HasAttribute(SPELL_ATTR2_UNAFFECTED_BY_AURA_SCHOOL_IMMUNE))
                    {
                        if (uint32 applyHarmfulAuraImmunityMask = immuneInfo->ApplyHarmfulAuraImmuneMask)
                            if ((auraSpellInfo->GetSchoolMask() & applyHarmfulAuraImmunityMask) != 0)
                                isImmuneToAuraEffectApply = true;
                    }

                    if (!isImmuneToAuraEffectApply)
                    {
                        immuneToAllEffects = false;
                        break;
                    }
                }
            }
        }

        if (immuneToAllEffects)
            return true;
    }

    return false;
}

// based on client Spell_C::CancelsAuraEffect
bool SpellInfo::SpellCancelsAuraEffect(SpellInfo const* auraSpellInfo, uint8 auraEffIndex) const
{
    if (!HasAttribute(SPELL_ATTR1_DISPEL_AURAS_ON_IMMUNITY))
        return false;

    if (auraSpellInfo->HasAttribute(SPELL_ATTR0_UNAFFECTED_BY_INVULNERABILITY))
        return false;

    for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
    {
        if (Effects[i].Effect != SPELL_EFFECT_APPLY_AURA && Effects[i].Effect != SPELL_EFFECT_APPLY_AURA_2)
            continue;

        uint32 const miscValue = static_cast<uint32>(Effects[i].MiscValue);
        switch (Effects[i].ApplyAuraName)
        {
            case SPELL_AURA_STATE_IMMUNITY:
                if (miscValue != auraSpellInfo->Effects[auraEffIndex].ApplyAuraName)
                    continue;
                break;
            case SPELL_AURA_SCHOOL_IMMUNITY:
            case SPELL_AURA_MOD_IMMUNE_AURA_APPLY_SCHOOL:
                if (auraSpellInfo->HasAttribute(SPELL_ATTR2_UNAFFECTED_BY_AURA_SCHOOL_IMMUNE) || !(auraSpellInfo->SchoolMask & miscValue))
                    continue;
                break;
            case SPELL_AURA_DISPEL_IMMUNITY:
                if (miscValue != auraSpellInfo->Dispel)
                    continue;
                break;
            case SPELL_AURA_MECHANIC_IMMUNITY:
                if (miscValue != auraSpellInfo->Mechanic)
                {
                    if (miscValue != auraSpellInfo->Effects[auraEffIndex].Mechanic)
                        continue;
                }
                break;
            default:
                continue;
        }

        return true;
    }

    return false;
}

uint32 SpellInfo::GetAllowedMechanicMask() const
{
    return _allowedMechanicMask;
}

uint32 SpellInfo::GetMechanicImmunityMask(Unit* caster) const
{
    bool canBeInterrupted = [&]()
    {
        return HasAttribute(SPELL_ATTR7_CAN_ALWAYS_BE_INTERRUPTED)
            || HasChannelInterruptFlag(SpellAuraInterruptFlags::Damage | SpellAuraInterruptFlags::EnteringCombat)
            || (caster->IsPlayer() && InterruptFlags.HasFlag(SpellInterruptFlags::DamageCancelsPlayerOnly))
            || PreventionType & SPELL_PREVENTION_TYPE_SILENCE;
    }();

    uint32 casterMechanicImmunityMask = caster->GetMechanicImmunityMask();
    uint32 mechanicImmunityMask = 0;
    if (canBeInterrupted)
    {
        if (casterMechanicImmunityMask & (1 << MECHANIC_INTERRUPT))
            mechanicImmunityMask |= (1 << MECHANIC_INTERRUPT);

        if (casterMechanicImmunityMask & (1 << MECHANIC_SILENCE))
            mechanicImmunityMask |= (1 << MECHANIC_SILENCE);
    }

    return mechanicImmunityMask;
}

float SpellInfo::GetMinRange(bool positive) const
{
    if (!RangeEntry)
        return 0.0f;
    if (positive)
        return RangeEntry->RangeMin[1];
    return RangeEntry->RangeMin[0];
}

float SpellInfo::GetMaxRange(bool positive, Unit* caster, Spell* spell) const
{
    if (!RangeEntry)
        return 0.0f;
    float range;
    if (positive)
        range = RangeEntry->RangeMax[1];
    else
        range = RangeEntry->RangeMax[0];
    if (caster)
        if (Player* modOwner = caster->GetSpellModOwner())
            modOwner->ApplySpellMod(Id, SPELLMOD_RANGE, range, spell);
    return range;
}

int32 SpellInfo::GetDuration() const
{
    if (!DurationEntry)
        return 0;
    return (DurationEntry->Duration == -1) ? -1 : abs(DurationEntry->Duration);
}

int32 SpellInfo::GetMaxDuration() const
{
    if (!DurationEntry)
        return 0;
    return (DurationEntry->MaxDuration == -1) ? -1 : abs(DurationEntry->MaxDuration);
}

int32 SpellInfo::CalcDuration(Unit* caster, Spell* spell) const
{
    if (IsPassive() && !DurationEntry)
        return -1;

    if (!DurationEntry)
        return 0;

    int32 duration = GetMaxDuration();
    if (duration == -1)
        return -1;

    if (caster != nullptr)
    {
        uint32 level = caster->getLevel();
        if (MaxLevel > 0 && level > MaxLevel)
            level = MaxLevel;
        if (BaseLevel > 0)
            level -= BaseLevel;

        duration = DurationEntry->Duration + DurationEntry->DurationPerLevel * level;
        duration = std::min(GetMaxDuration(), duration);
    }

    if (duration == -1)
        return -1;

    // Increase duration based on combo points
    if (HasAttribute(SPELL_ATTR1_FINISHING_MOVE_DURATION))
    {
        if (uint8 comboPoints = (caster && caster->m_playerMovingMe) ? caster->m_playerMovingMe->GetComboPoints() : 0)
        {
            if (GetDuration() != GetMaxDuration() && GetDuration() != -1)
                duration += int32((GetMaxDuration() - GetDuration()) * comboPoints / 5);
        }
    }

    if (caster != nullptr)
        if (Player* modOwner = caster->GetSpellModOwner())
            modOwner->ApplySpellMod(Id, SPELLMOD_DURATION, duration, spell);

    bool hasteAffectsDuration = HasAttribute(SPELL_ATTR8_HASTE_AFFECTS_DURATION);
    bool spellHasteAffectsPeriodic = HasAttribute(SPELL_ATTR5_SPELL_HASTE_AFFECTS_PERIODIC);
    // That aura is not used as of 4.3.4, but just include it, as it still is technically supported by the client, and so should we.
    bool hasPeriodicHasteAuras = caster != nullptr && caster->HasAuraTypeWithAffectMask(SPELL_AURA_PERIODIC_HASTE, this);

    if (spellHasteAffectsPeriodic || hasteAffectsDuration)
    {
        // This is just a stupid way to find the first periodic effect.
        int32 periodicEffectIndex = 0;
        while ((Effects[periodicEffectIndex].Effect == 0 || Effects[periodicEffectIndex].AuraPeriod == 0))
        {
            ++periodicEffectIndex;
            if (periodicEffectIndex >= MAX_SPELL_EFFECTS)
                return duration;
        }

        if ((spellHasteAffectsPeriodic || hasPeriodicHasteAuras) && !HasAttribute(SPELL_ATTR3_NO_DONE_BONUS))
        {
            float hasteValue = caster ? caster->GetFloatValue(UNIT_MOD_CAST_HASTE) : 0.0f;
            if (hasteValue > 0.0f)
            {
                if (hasteAffectsDuration)
                    return int32(duration * hasteValue);

                int32 effectPeriod = Effects[periodicEffectIndex].CalcPeriod(caster, spell);

                if (effectPeriod > 0)
                {
                    // additional ticks are being added by rounding up, resulting in increased duration.
                    int32 ticks = int32(std::ceilf((float)duration / effectPeriod));
                    duration = std::max(ticks * effectPeriod, duration);
                }
            }
        }
    }

    return duration;
}

uint32 SpellInfo::CalcCastTime(uint8 level, Spell* spell /*= nullptr*/) const
{
    int32 castTime = 0;
    if (!level && spell)
        level = spell->GetCaster()->getLevel();

    // not all spells have cast time index and this is all is pasiive abilities
    if (level && CastTimeMax > 0)
    {
        castTime = CastTimeMax;
        if (CastTimeMaxLevel > level)
            castTime = CastTimeMin + int32(level - 1) * (CastTimeMax - CastTimeMin) / (CastTimeMaxLevel - 1);
    }
    else if (CastTimeEntry)
        castTime = CastTimeEntry->Base;

    if (!castTime)
        return 0;

    if (spell)
        spell->GetCaster()->ModSpellCastTime(this, castTime, spell);

    return (castTime > 0) ? uint32(castTime) : 0;
}

uint32 SpellInfo::GetMaxTicks() const
{
    int32 DotDuration = GetDuration();
    if (DotDuration == 0)
        return 1;

    // 200% limit
    if (DotDuration > 30000)
        DotDuration = 30000;

    for (uint8 x = 0; x < MAX_SPELL_EFFECTS; x++)
    {
        if (Effects[x].Effect == SPELL_EFFECT_APPLY_AURA || Effects[x].Effect == SPELL_EFFECT_APPLY_AURA_2)
            switch (Effects[x].ApplyAuraName)
            {
                case SPELL_AURA_PERIODIC_DAMAGE:
                case SPELL_AURA_PERIODIC_DAMAGE_PERCENT:
                case SPELL_AURA_PERIODIC_HEAL:
                case SPELL_AURA_OBS_MOD_HEALTH:
                case SPELL_AURA_OBS_MOD_POWER:
                case SPELL_AURA_48:
                case SPELL_AURA_POWER_BURN:
                case SPELL_AURA_PERIODIC_LEECH:
                case SPELL_AURA_PERIODIC_MANA_LEECH:
                case SPELL_AURA_PERIODIC_ENERGIZE:
                case SPELL_AURA_PERIODIC_DUMMY:
                case SPELL_AURA_PERIODIC_TRIGGER_SPELL:
                case SPELL_AURA_PERIODIC_TRIGGER_SPELL_WITH_VALUE:
                case SPELL_AURA_PERIODIC_HEALTH_FUNNEL:
                    if (Effects[x].AuraPeriod != 0)
                        return DotDuration / Effects[x].AuraPeriod;
                    break;
            }
    }

    return 6;
}

uint32 SpellInfo::GetRecoveryTime() const
{
    return RecoveryTime > CategoryRecoveryTime ? RecoveryTime : CategoryRecoveryTime;
}

int32 SpellInfo::CalcPowerCost(Unit const* caster, SpellSchoolMask schoolMask, Spell* spell) const
{
    // Spell drain all exist power on cast (Only paladin lay of Hands)
    if (HasAttribute(SPELL_ATTR1_DRAIN_ALL_POWER))
    {
        // If power type - health drain all
        if (PowerType == POWER_HEALTH)
            return caster->GetHealth();
        // Else drain all power
        if (PowerType < MAX_POWERS)
            return caster->GetPower(Powers(PowerType));

        TC_LOG_ERROR("spells", "SpellInfo::CalcPowerCost: Unknown power type '%d' in spell %d", PowerType, Id);
        return 0;
    }

    bool invertSign = false;
    int32 powerCost = 0;

    if (SpellPowerId)
    {
        int32 manaCost = ManaCost;
        powerCost = manaCost;
        float powerCostPct = ManaCostPercentage2 != 0.f ? ManaCostPercentage2 : (float)ManaCostPercentage;

        if (manaCost < 0 || powerCostPct < 0.f)
        {
            manaCost = std::abs(manaCost);
            powerCostPct = std::fabs(powerCostPct);
            invertSign = true;
            powerCost = manaCost;
        }

        // Base calculation
        if (ManaCostPercentage)
        {
            int32 ressource = 0;
            switch (PowerType)
            {
                // health as power used
                case POWER_HEALTH:
                    ressource = caster->GetCreateHealth();
                    break;
                case POWER_MANA:
                    ressource = caster->GetCreateMana();
                    break;
                case POWER_RAGE:
                case POWER_FOCUS:
                case POWER_ENERGY:
                    ressource = caster->GetMaxPower(Powers(PowerType));
                    break;
                case POWER_RUNE:
                case POWER_RUNIC_POWER:
                    TC_LOG_DEBUG("spells", "CalculateManaCost: Not implemented yet!");
                    break;
                default:
                    TC_LOG_ERROR("spells", "CalculateManaCost: Unknown power type '%d' in spell %d", PowerType, Id);
                    return 0;
            }

            manaCost += int32((ressource * powerCostPct) / 100);
            powerCost = manaCost;
        }

        // Scaling
        if (SpellScalingId)
            powerCost = int32(ceilf(GetSpellScalingMultiplier(caster, GetSpellScaling(), true) * (double)powerCost));
        else
        {
            uint32 manaCostPerLevel = invertSign ? -int32(ManaCostPerlevel) : ManaCostPerlevel;
            powerCost = manaCostPerLevel * SpellLevel + manaCost;
        }
    }

    // Flat mod from caster auras by spell school and power type
    Unit::AuraEffectList const& auras = caster->GetAuraEffectsByType(SPELL_AURA_MOD_POWER_COST_SCHOOL);
    for (Unit::AuraEffectList::const_iterator i = auras.begin(); i != auras.end(); ++i)
    {
        if (!((*i)->GetMiscValue() & schoolMask))
            continue;
        if (!((*i)->GetMiscValueB() & (1 << PowerType)))
            continue;
        powerCost += (*i)->GetAmount();
    }

    // Shiv - costs 20 + weaponSpeed*10 energy (apply only to non-triggered spell with energy cost)
    if (HasAttribute(SPELL_ATTR4_SPELL_VS_EXTEND_COST))
    {
        uint32 speed = 0;
        if (SpellShapeshiftFormEntry const* ss = sSpellShapeshiftFormStore.LookupEntry(caster->GetShapeshiftForm()))
            speed = ss->CombatRoundTime;
        else
        {
            WeaponAttackType slot = BASE_ATTACK;
            if (HasAttribute(SPELL_ATTR3_REQ_OFFHAND))
                slot = OFF_ATTACK;

            speed = caster->GetAttackTime(slot);
        }

        powerCost += speed / 100;
    }

    // Apply cost mod by spell
    if (Player* modOwner = caster->GetSpellModOwner())
        modOwner->ApplySpellMod(Id, SPELLMOD_COST, powerCost, spell);

    if (!caster->IsControlledByPlayer())
    {
        if (HasAttribute(SPELL_ATTR0_LEVEL_DAMAGE_CALCULATION))
        {
            GtNPCManaCostScalerEntry const* spellScaler = sGtNPCManaCostScalerStore.LookupEntry(SpellLevel - 1);
            GtNPCManaCostScalerEntry const* casterScaler = sGtNPCManaCostScalerStore.LookupEntry(caster->getLevel() - 1);
            if (spellScaler && spellScaler->ratio > 0.f && casterScaler && casterScaler->ratio > 0.f)
                powerCost *= casterScaler->ratio / spellScaler->ratio;
        }
    }

    if (invertSign)
        powerCost = -powerCost;

    // PCT mod from user auras by spell school and power type
    Unit::AuraEffectList const& aurasPct = caster->GetAuraEffectsByType(SPELL_AURA_MOD_POWER_COST_SCHOOL_PCT);
    for (Unit::AuraEffectList::const_iterator i = aurasPct.begin(); i != aurasPct.end(); ++i)
    {
        if (!((*i)->GetMiscValue() & schoolMask))
            continue;
        if (!((*i)->GetMiscValueB() & (1 << PowerType)))
            continue;
        powerCost += CalculatePct(powerCost, (*i)->GetAmount());
    }
    if (powerCost < 0)
        powerCost = 0;
    return powerCost;
}

float SpellInfo::GetSpellScalingMultiplier(Unit const* caster, SpellScalingEntry const* scalingEntry, bool isPowerCostRelated /*= false*/) const
{
    if (!caster || !scalingEntry)
        return 1.f;

    float multiplier = 1.f;
    float scalingMultiplier = 1.f;

    int32 castTimeMaxLevel = scalingEntry->CastTimeMaxLevel;
    uint8 casterLevel = caster->getLevel();

    if (casterLevel < castTimeMaxLevel && scalingEntry->CastTimeMax)
    {
        float castTimeMax = (float)scalingEntry->CastTimeMax;
        int32 castTime = scalingEntry->CastTimeMin + ((casterLevel - 1) * (scalingEntry->CastTimeMax - scalingEntry->CastTimeMin)) / (castTimeMaxLevel - 1);
        multiplier = castTime / castTimeMax;
        scalingMultiplier = castTime / castTimeMax;
    }

    if (!isPowerCostRelated)
    {
        int32 nerfMaxLevel = scalingEntry->NerfMaxLevel;
        if (casterLevel < nerfMaxLevel)
            scalingMultiplier = ((((1.f - scalingEntry->NerfFactor) * (casterLevel - 1)) / (nerfMaxLevel - 1)) + scalingEntry->NerfFactor) * multiplier;
    }

    return scalingMultiplier;
}

bool SpellInfo::IsRanked() const
{
    return ChainEntry != nullptr;
}

uint8 SpellInfo::GetRank() const
{
    if (!ChainEntry)
        return 1;
    return ChainEntry->rank;
}

SpellInfo const* SpellInfo::GetFirstRankSpell() const
{
    if (!ChainEntry)
        return this;
    return ChainEntry->first;
}
SpellInfo const* SpellInfo::GetLastRankSpell() const
{
    if (!ChainEntry)
        return nullptr;
    return ChainEntry->last;
}
SpellInfo const* SpellInfo::GetNextRankSpell() const
{
    if (!ChainEntry)
        return nullptr;
    return ChainEntry->next;
}
SpellInfo const* SpellInfo::GetPrevRankSpell() const
{
    if (!ChainEntry)
        return nullptr;
    return ChainEntry->prev;
}

SpellInfo const* SpellInfo::GetAuraRankForLevel(uint8 level) const
{
    // ignore passive spells
    if (IsPassive())
        return this;

    // Client ignores spell with these attributes (sub_53D9D0)
    if (HasAttribute(SPELL_ATTR0_NEGATIVE_1) || HasAttribute(SPELL_ATTR2_UNK3))
        return this;

    bool needRankSelection = false;
    for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
    {
        if (IsPositiveEffect(i) &&
            (Effects[i].Effect == SPELL_EFFECT_APPLY_AURA ||
            Effects[i].Effect == SPELL_EFFECT_APPLY_AURA_2 ||
            Effects[i].Effect == SPELL_EFFECT_APPLY_AREA_AURA_PARTY ||
            Effects[i].Effect == SPELL_EFFECT_APPLY_AREA_AURA_RAID) &&
            !Effects[i].ScalingMultiplier)
        {
            needRankSelection = true;
            break;
        }
    }

    // not required
    if (!needRankSelection)
        return this;

    for (SpellInfo const* nextSpellInfo = this; nextSpellInfo != nullptr; nextSpellInfo = nextSpellInfo->GetPrevRankSpell())
    {
        // if found appropriate level
        if (uint32(level + 10) >= nextSpellInfo->SpellLevel)
            return nextSpellInfo;

        // one rank less then
    }

    // not found
    return nullptr;
}

bool SpellInfo::IsRankOf(SpellInfo const* spellInfo) const
{
    return GetFirstRankSpell() == spellInfo->GetFirstRankSpell();
}

bool SpellInfo::IsDifferentRankOf(SpellInfo const* spellInfo) const
{
    if (Id == spellInfo->Id)
        return false;
    return IsRankOf(spellInfo);
}

bool SpellInfo::IsHighRankOf(SpellInfo const* spellInfo) const
{
    if (ChainEntry && spellInfo->ChainEntry)
        if (ChainEntry->first == spellInfo->ChainEntry->first)
            if (ChainEntry->rank > spellInfo->ChainEntry->rank)
                return true;

    return false;
}

void SpellInfo::_InitializeExplicitTargetMask()
{
    bool srcSet = false;
    bool dstSet = false;
    uint32 targetMask = Targets;
    // prepare target mask using effect target entries
    for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
    {
        if (!Effects[i].IsEffect())
            continue;

        targetMask |= Effects[i].TargetA.GetExplicitTargetMask(srcSet, dstSet);
        targetMask |= Effects[i].TargetB.GetExplicitTargetMask(srcSet, dstSet);

        // add explicit target flags based on spell effects which have EFFECT_IMPLICIT_TARGET_EXPLICIT and no valid target provided
        if (Effects[i].GetImplicitTargetType() != EFFECT_IMPLICIT_TARGET_EXPLICIT)
            continue;

        // extend explicit target mask only if valid targets for effect could not be provided by target types
        uint32 effectTargetMask = Effects[i].GetMissingTargetMask(srcSet, dstSet, targetMask);

        // don't add explicit object/dest flags when spell has no max range
        if (GetMaxRange(true) == 0.0f && GetMaxRange(false) == 0.0f)
            effectTargetMask &= ~(TARGET_FLAG_UNIT_MASK | TARGET_FLAG_GAMEOBJECT | TARGET_FLAG_CORPSE_MASK | TARGET_FLAG_DEST_LOCATION);

        targetMask |= effectTargetMask;
    }

    ExplicitTargetMask = targetMask;
}

bool SpellInfo::_IsPositiveEffect(uint8 effIndex, bool deep) const
{
    // not found a single positive spell with this attribute
    if (HasAttribute(SPELL_ATTR0_NEGATIVE_1))
        return false;

    switch (SpellFamilyName)
    {
        case SPELLFAMILY_GENERIC:
            switch (Id)
            {
                case 29214: // Wrath of the Plaguebringer
                case 34700: // Allergic Reaction
                case 54836: // Wrath of the Plaguebringer
                case 41914: // Parasitic Shadowfiend (Illidan)
                case 41917: // Parasitic Shadowfiend (Illidan)
                    return false;
                case 30877: // Tag Murloc
                case 61716: // Rabbit Costume
                case 61734: // Noblegarden Bunny
                case 62344: // Fists of Stone
                case 61819: // Manabonked! (item)
                case 61834: // Manabonked! (minigob)
                    return true;
                default:
                    break;
            }
            break;
        case SPELLFAMILY_MAGE:
            // Impact
            if (SpellIconID == 45)
                return true;
            // Arcane Missiles
            if (SpellFamilyFlags[0] == 0x00000800)
                return false;
            break;
        case SPELLFAMILY_PRIEST:
            switch (Id)
            {
                case 64844: // Divine Hymn
                case 64904: // Hymn of Hope
                case 47585: // Dispersion
                    return true;
                default:
                    break;
            }
            break;
        case SPELLFAMILY_ROGUE:
            switch (Id)
            {
                // Envenom must be considered as a positive effect even though it deals damage
                case 32645: // Envenom
                // Slice and Dice. Prevents breaking Stealth
                case 5171:      // Slice and Dice (Rank 1)
                case 6774:      // Slice and Dice (Rank 2)
                    return true;
                default:
                    break;
            }
            break;
        case SPELLFAMILY_PALADIN:
            break;
        default:
            break;
    }

    switch (Mechanic)
    {
        case MECHANIC_IMMUNE_SHIELD:
            return true;
        default:
            break;
    }

    // Special case: effects which determine positivity of whole spell
    for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
    {
        if (Effects[i].IsAura())
        {
            switch (Effects[i].ApplyAuraName)
            {
                case SPELL_AURA_MOD_STEALTH:
                    return true;
                case SPELL_AURA_CHANNEL_DEATH_ITEM:
                case SPELL_AURA_EMPATHY:
                    return false;
            }
        }
    }

    switch (Effects[effIndex].Effect)
    {
        case SPELL_EFFECT_DUMMY:
            // some explicitly required dummy effect sets
            switch (Id)
            {
                case 28441:
                    return false; // AB Effect 000
                default:
                    break;
            }
            break;
        // always positive effects (check before target checks that provided non-positive result in some case for positive effects)
        case SPELL_EFFECT_HEAL:
        case SPELL_EFFECT_LEARN_SPELL:
        case SPELL_EFFECT_SKILL_STEP:
        case SPELL_EFFECT_HEAL_PCT:
        case SPELL_EFFECT_ENERGIZE_PCT:
            return true;
        case SPELL_EFFECT_APPLY_AREA_AURA_ENEMY:
            return false;

            // non-positive aura use
        case SPELL_EFFECT_APPLY_AURA:
        case SPELL_EFFECT_APPLY_AURA_2:
        case SPELL_EFFECT_APPLY_AREA_AURA_FRIEND:
        {
            switch (Effects[effIndex].ApplyAuraName)
            {
                case SPELL_AURA_MOD_DAMAGE_DONE:            // dependent from bas point sign (negative -> negative)
                case SPELL_AURA_MOD_STAT:
                case SPELL_AURA_MOD_SKILL:
                case SPELL_AURA_MOD_DODGE_PERCENT:
                case SPELL_AURA_MOD_HEALING_PCT:
                case SPELL_AURA_MOD_HEALING_DONE:
                case SPELL_AURA_MOD_DAMAGE_PERCENT_DONE:
                    if (Effects[effIndex].CalcValue() < 0)
                        return false;
                    break;
                case SPELL_AURA_MOD_DAMAGE_TAKEN:           // dependent from bas point sign (positive -> negative)
                    if (Effects[effIndex].CalcValue() > 0)
                        return false;
                    break;
                case SPELL_AURA_MOD_CRIT_PCT:
                case SPELL_AURA_MOD_SPELL_CRIT_CHANCE:
                    if (Effects[effIndex].CalcValue() > 0)
                        return true;                        // some expected positive spells have SPELL_ATTR1_NEGATIVE
                    break;
                case SPELL_AURA_ADD_TARGET_TRIGGER:
                    return true;
                case SPELL_AURA_PERIODIC_TRIGGER_SPELL_WITH_VALUE:
                case SPELL_AURA_PERIODIC_TRIGGER_SPELL:
                    if (!deep)
                    {
                        if (SpellInfo const* spellTriggeredProto = sSpellMgr->GetSpellInfo(Effects[effIndex].TriggerSpell))
                        {
                            // negative targets of main spell return early
                            for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
                            {
                                if (!spellTriggeredProto->Effects[i].Effect)
                                    continue;
                                // if non-positive trigger cast targeted to positive target this main cast is non-positive
                                // this will place this spell auras as debuffs
                                if (_IsPositiveTarget(spellTriggeredProto->Effects[i].TargetA.GetTarget(), spellTriggeredProto->Effects[i].TargetB.GetTarget()) && !spellTriggeredProto->_IsPositiveEffect(i, true))
                                    return false;
                            }
                        }
                    }
                    break;
                case SPELL_AURA_PROC_TRIGGER_SPELL:
                    // many positive auras have negative triggered spells at damage for example and this not make it negative (it can be canceled for example)
                    break;
                case SPELL_AURA_MOD_STUN:                   //have positive and negative spells, we can't sort its correctly at this moment.
                    if (effIndex == 0 && Effects[1].Effect == 0 && Effects[2].Effect == 0)
                        return false;                       // but all single stun aura spells is negative
                    break;
                case SPELL_AURA_MOD_PACIFY_SILENCE:
                    if (Id == 24740)             // Wisp Costume
                        return true;
                    return false;
                case SPELL_AURA_MOD_ROOT:
                case SPELL_AURA_MOD_SILENCE:
                case SPELL_AURA_GHOST:
                case SPELL_AURA_PERIODIC_LEECH:
                case SPELL_AURA_MOD_STALKED:
                case SPELL_AURA_PERIODIC_DAMAGE_PERCENT:
                case SPELL_AURA_PREVENT_RESURRECTION:
                    return false;
                case SPELL_AURA_PERIODIC_DAMAGE:            // used in positive spells also.
                    // part of negative spell if cast at self (prevent cancel)
                    if (Effects[effIndex].TargetA.GetTarget() == TARGET_UNIT_CASTER)
                        return false;
                    break;
                case SPELL_AURA_MOD_DECREASE_SPEED:         // used in positive spells also
                    // part of positive spell if cast at self
                    if (Effects[effIndex].TargetA.GetTarget() != TARGET_UNIT_CASTER)
                        return false;
                    // but not this if this first effect (didn't find better check)
                    if (HasAttribute(SPELL_ATTR0_NEGATIVE_1) && effIndex == 0)
                        return false;
                    break;
                case SPELL_AURA_MECHANIC_IMMUNITY:
                {
                    // non-positive immunities
                    switch (Effects[effIndex].MiscValue)
                    {
                        case MECHANIC_BANDAGE:
                        case MECHANIC_SHIELD:
                        case MECHANIC_MOUNT:
                        case MECHANIC_INVULNERABILITY:
                            return false;
                        default:
                            break;
                    }
                    break;
                }
                case SPELL_AURA_ADD_FLAT_MODIFIER:          // mods
                case SPELL_AURA_ADD_PCT_MODIFIER:
                {
                    // non-positive mods
                    switch (Effects[effIndex].MiscValue)
                    {
                        case SPELLMOD_COST:                 // dependent from bas point sign (negative -> positive)
                            if (Effects[effIndex].CalcValue() > 0)
                            {
                                if (!deep)
                                {
                                    bool negative = true;
                                    for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
                                    {
                                        if (i != effIndex)
                                            if (_IsPositiveEffect(i, true))
                                            {
                                                negative = false;
                                                break;
                                            }
                                    }
                                    if (negative)
                                        return false;
                                }
                            }
                            break;
                        default:
                            break;
                    }
                    break;
                }
                default:
                    break;
            }
            break;
        }
        default:
            break;
    }

    // non-positive targets
    if (!_IsPositiveTarget(Effects[effIndex].TargetA.GetTarget(), Effects[effIndex].TargetB.GetTarget()))
        return false;

    // negative spell if triggered spell is negative
    if (!deep && !Effects[effIndex].ApplyAuraName && Effects[effIndex].TriggerSpell)
    {
        if (SpellInfo const* spellTriggeredProto = sSpellMgr->GetSpellInfo(Effects[effIndex].TriggerSpell))
            if (!spellTriggeredProto->_IsPositiveSpell())
                return false;
    }

    // ok, positive
    return true;
}

bool SpellInfo::_IsPositiveSpell() const
{
    // spells with at least one negative effect are considered negative
    // some self-applied spells have negative effects but in self casting case negative check ignored.
    for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
        if (!_IsPositiveEffect(i, true))
            return false;
    return true;
}

bool SpellInfo::_IsPositiveTarget(uint32 targetA, uint32 targetB)
{
    // non-positive targets
    switch (targetA)
    {
        case TARGET_UNIT_NEARBY_ENEMY:
        case TARGET_UNIT_TARGET_ENEMY:
        case TARGET_UNIT_SRC_AREA_ENEMY:
        case TARGET_UNIT_DEST_AREA_ENEMY:
        case TARGET_UNIT_CONE_ENEMY_24:
        case TARGET_UNIT_CONE_ENEMY_104:
        case TARGET_DEST_DYNOBJ_ENEMY:
        case TARGET_DEST_TARGET_ENEMY:
            return false;
        default:
            break;
    }
    if (targetB)
        return _IsPositiveTarget(targetB, 0);
    return true;
}

SpellTargetRestrictionsEntry const* SpellInfo::GetSpellTargetRestrictions() const
{
    return SpellTargetRestrictionsId ? sSpellTargetRestrictionsStore.LookupEntry(SpellTargetRestrictionsId) : nullptr;
}

SpellEquippedItemsEntry const* SpellInfo::GetSpellEquippedItems() const
{
    return SpellEquippedItemsId ? sSpellEquippedItemsStore.LookupEntry(SpellEquippedItemsId) : nullptr;
}

SpellInterruptsEntry const* SpellInfo::GetSpellInterrupts() const
{
    return SpellInterruptsId ? sSpellInterruptsStore.LookupEntry(SpellInterruptsId) : nullptr;
}

SpellLevelsEntry const* SpellInfo::GetSpellLevels() const
{
    return SpellLevelsId ? sSpellLevelsStore.LookupEntry(SpellLevelsId) : nullptr;
}

SpellPowerEntry const* SpellInfo::GetSpellPower() const
{
    return SpellPowerId ? sSpellPowerStore.LookupEntry(SpellPowerId) : nullptr;
}

SpellReagentsEntry const* SpellInfo::GetSpellReagents() const
{
    return SpellReagentsId ? sSpellReagentsStore.LookupEntry(SpellReagentsId) : nullptr;
}

SpellScalingEntry const* SpellInfo::GetSpellScaling() const
{
    return SpellScalingId ? sSpellScalingStore.LookupEntry(SpellScalingId) : nullptr;
}

SpellShapeshiftEntry const* SpellInfo::GetSpellShapeshift() const
{
    return SpellShapeshiftId ? sSpellShapeshiftStore.LookupEntry(SpellShapeshiftId) : nullptr;
}

SpellTotemsEntry const* SpellInfo::GetSpellTotems() const
{
    return SpellTotemsId ? sSpellTotemsStore.LookupEntry(SpellTotemsId) : nullptr;
}

SpellAuraOptionsEntry const* SpellInfo::GetSpellAuraOptions() const
{
    return SpellAuraOptionsId ? sSpellAuraOptionsStore.LookupEntry(SpellAuraOptionsId) : nullptr;
}

SpellAuraRestrictionsEntry const* SpellInfo::GetSpellAuraRestrictions() const
{
    return SpellAuraRestrictionsId ? sSpellAuraRestrictionsStore.LookupEntry(SpellAuraRestrictionsId) : nullptr;
}

SpellCastingRequirementsEntry const* SpellInfo::GetSpellCastingRequirements() const
{
    return SpellCastingRequirementsId ? sSpellCastingRequirementsStore.LookupEntry(SpellCastingRequirementsId) : nullptr;
}

SpellCategoriesEntry const* SpellInfo::GetSpellCategories() const
{
    return SpellCategoriesId ? sSpellCategoriesStore.LookupEntry(SpellCategoriesId) : nullptr;
}

SpellClassOptionsEntry const* SpellInfo::GetSpellClassOptions() const
{
    return SpellClassOptionsId ? sSpellClassOptionsStore.LookupEntry(SpellClassOptionsId) : nullptr;
}

SpellCooldownsEntry const* SpellInfo::GetSpellCooldowns() const
{
    return SpellCooldownsId ? sSpellCooldownsStore.LookupEntry(SpellCooldownsId) : nullptr;
}

void SpellInfo::_UnloadImplicitTargetConditionLists()
{
    // find the same instances of ConditionList and delete them.
    for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
    {
        ConditionContainer* cur = Effects[i].ImplicitTargetConditions;
        if (!cur)
            continue;
        for (uint8 j = i; j < MAX_SPELL_EFFECTS; ++j)
        {
            if (Effects[j].ImplicitTargetConditions == cur)
                Effects[j].ImplicitTargetConditions = nullptr;
        }
        delete cur;
    }
}
