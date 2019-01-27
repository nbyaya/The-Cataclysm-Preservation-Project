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

#include "ScriptMgr.h"
#include "CreatureAI.h"
#include "EventMap.h"
#include "GameObject.h"
#include "InstanceScript.h"
#include "magisters_terrace.h"
#include "Map.h"
#include "MotionMaster.h"
#include "ObjectAccessor.h"
#include "TemporarySummon.h"

/*
0  - Selin Fireheart
1  - Vexallus
2  - Priestess Delrissa
3  - Kael'thas Sunstrider
*/

ObjectData const creatureData[] =
{
    { BOSS_KAELTHAS_SUNSTRIDER,     DATA_KAELTHAS_SUNSTRIDER    },
    { 0,                            0                           } // END
};

DoorData const doorData[] =
{
    { GO_SELIN_DOOR,           DATA_SELIN,                  DOOR_TYPE_PASSAGE   },
    { GO_SELIN_ENCOUNTER_DOOR, DATA_SELIN,                  DOOR_TYPE_ROOM      },
    { GO_VEXALLUS_DOOR,        DATA_VEXALLUS,               DOOR_TYPE_PASSAGE   },
    { GO_DELRISSA_DOOR,        DATA_DELRISSA,               DOOR_TYPE_PASSAGE   },
    { GO_ASYLUM_DOOR,          DATA_KAELTHAS_SUNSTRIDER,    DOOR_TYPE_ROOM      },
    { 0,                       0,                           DOOR_TYPE_ROOM      } // END
};

Position const KalecgosSpawnPos = { 164.3747f, -397.1197f, 2.151798f, 1.66219f };
Position const KaelthasTrashGroupDistanceComparisonPos = { 150.0f, 141.0f, -14.4f };

class instance_magisters_terrace : public InstanceMapScript
{
    public:
        instance_magisters_terrace() : InstanceMapScript(MGTScriptName, 585) { }

        struct instance_magisters_terrace_InstanceMapScript : public InstanceScript
        {
            instance_magisters_terrace_InstanceMapScript(InstanceMap* map) : InstanceScript(map)
            {
                SetHeaders(DataHeader);
                SetBossNumber(EncounterCount);
                LoadObjectData(creatureData, nullptr);
                LoadDoorData(doorData);
                Initialize();
            }

            void Initialize()
            {
                _delrissaDeathCount = 0;
                _kaelthasIntroState = 0;
            }

            uint32 GetData(uint32 type) const override
            {
                switch (type)
                {
                    case DATA_DELRISSA_DEATH_COUNT:
                        return _delrissaDeathCount;
                    case DATA_KAELTHAS_INTRO_STATE:
                        return _kaelthasIntroState;
                    default:
                        break;
                }
                return 0;
            }

            void SetData(uint32 type, uint32 data) override
            {
                switch (type)
                {
                    case DATA_DELRISSA_DEATH_COUNT:
                        if (data == SPECIAL)
                            _delrissaDeathCount++;
                        else
                            _delrissaDeathCount = 0;
                        break;
                    case DATA_KAELTHAS_INTRO_STATE:
                        _kaelthasIntroState = data;
                        SaveToDB();
                        break;
                    default:
                        break;
                }
            }

            void OnCreatureCreate(Creature* creature) override
            {
                InstanceScript::OnCreatureCreate(creature);

                switch (creature->GetEntry())
                {
                    case NPC_SELIN:
                        SelinGUID = creature->GetGUID();
                        break;
                    case NPC_DELRISSA:
                        DelrissaGUID = creature->GetGUID();
                        break;
                    case NPC_KALECGOS:
                    case NPC_HUMAN_KALECGOS:
                        KalecgosGUID = creature->GetGUID();
                        break;
                    case NPC_COILSKAR_WITCH:
                    case NPC_SUNBLADE_WARLOCK:
                    case NPC_SUNBLADE_MAGE_GUARD:
                    case NPC_SISTER_OF_TORMENT:
                    case NPC_ETHEREUM_SMUGGLER:
                    case NPC_SUNBLADE_BLOOD_KNIGHT:
                        if (creature->GetDistance(KaelthasTrashGroupDistanceComparisonPos) < 10.0f)
                            _kaelthasPreTrashGUIDs.insert(creature->GetGUID());
                    default:
                        break;
                }
            }

            void OnUnitDeath(Unit* unit) override
            {
                if (unit->GetTypeId() != TYPEID_UNIT)
                    return;

                switch (unit->GetEntry())
                {
                    case NPC_COILSKAR_WITCH:
                    case NPC_SUNBLADE_WARLOCK:
                    case NPC_SUNBLADE_MAGE_GUARD:
                    case NPC_SISTER_OF_TORMENT:
                    case NPC_ETHEREUM_SMUGGLER:
                    case NPC_SUNBLADE_BLOOD_KNIGHT:
                        if (_kaelthasPreTrashGUIDs.find(unit->GetGUID()) != _kaelthasPreTrashGUIDs.end())
                        {
                            _kaelthasPreTrashGUIDs.erase(unit->GetGUID());
                            if (_kaelthasPreTrashGUIDs.size() == 0)
                            {
                                if (Creature* kaelthas = GetCreature(DATA_KAELTHAS_SUNSTRIDER))
                                {
                                    kaelthas->AI()->SetData(DATA_KAELTHAS_INTRO, IN_PROGRESS);
                                    SetData(DATA_KAELTHAS_INTRO_STATE, DONE);
                                }
                            }
                        }
                        break;
                    default:
                        break;
                }
            }

            void OnGameObjectCreate(GameObject* go) override
            {
                InstanceScript::OnGameObjectCreate(go);

                switch (go->GetEntry())
                {
                    case GO_KAEL_STATUE_1:
                    case GO_KAEL_STATUE_2:
                        _statueGUIDs.push_back(go->GetGUID());
                        break;
                    default:
                        break;
                }
            }

            void OnGameObjectRemove(GameObject* go) override
            {
                InstanceScript::OnGameObjectRemove(go);
            }

            void ProcessEvent(WorldObject* obj, uint32 eventId) override
            {
                if (eventId == EVENT_SPAWN_KALECGOS)
                    if (!ObjectAccessor::GetCreature(*obj, KalecgosGUID) && Events.Empty())
                       Events.ScheduleEvent(EVENT_SPAWN_KALECGOS, Minutes(1));
            }

            void Update(uint32 diff) override
            {
                Events.Update(diff);

                if (Events.ExecuteEvent() == EVENT_SPAWN_KALECGOS)
                    if (Creature* kalecgos = instance->SummonCreature(NPC_KALECGOS, KalecgosSpawnPos))
                    {
                        kalecgos->GetMotionMaster()->MovePath(PATH_KALECGOS_FLIGHT, false);
                        kalecgos->AI()->Talk(SAY_KALECGOS_SPAWN);
                    }
            }

            bool SetBossState(uint32 type, EncounterState state) override
            {
                if (!InstanceScript::SetBossState(type, state))
                    return false;

                switch (type)
                {
                    case DATA_DELRISSA:
                        if (state == IN_PROGRESS)
                            _delrissaDeathCount = 0;
                        break;
                    case DATA_KAELTHAS_SUNSTRIDER:
                        if (state == NOT_STARTED)
                        {
                            for (ObjectGuid guid : _statueGUIDs)
                                HandleGameObject(guid, false);
                        }
                        break;
                    default:
                        break;
                }
                return true;
            }

            ObjectGuid GetGuidData(uint32 type) const override
            {
                switch (type)
                {
                    case DATA_SELIN:
                        return SelinGUID;
                    case DATA_DELRISSA:
                        return DelrissaGUID;
                    case DATA_ESCAPE_ORB:
                        return EscapeOrbGUID;
                    default:
                        break;
                }
                return ObjectGuid::Empty;
            }

            void WriteSaveDataMore(std::ostringstream& data) override
            {
                data << _kaelthasIntroState;
            }

            void ReadSaveDataMore(std::istringstream& data) override
            {
                data >> _kaelthasIntroState;
            }

        protected:
            EventMap Events;
            ObjectGuid SelinGUID;
            ObjectGuid DelrissaGUID;
            ObjectGuid EscapeOrbGUID;
            ObjectGuid KalecgosGUID;
            GuidVector _statueGUIDs;
            GuidSet _kaelthasPreTrashGUIDs;
            uint8 _delrissaDeathCount;
            uint8 _kaelthasIntroState;
        };

        InstanceScript* GetInstanceScript(InstanceMap* map) const override
        {
            return new instance_magisters_terrace_InstanceMapScript(map);
        }
};

void AddSC_instance_magisters_terrace()
{
    new instance_magisters_terrace();
}
