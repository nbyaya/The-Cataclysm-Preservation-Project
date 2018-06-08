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

#include "ObjectMgr.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "GameObject.h"
#include "GameObjectAI.h"
#include "Player.h"
#include "Vehicle.h"
#include "throne_of_the_four_winds.h"

enum Spells
{
    SPELL_JET_STREAM                = 84576,
    SPELL_JET_STREAM_TRIGGERED      = 84610,
    SPELL_CATCH_FALL                = 85268,

    SPELL_CATCH_FALL_RIDE_VEHICLE   = 85282,
    SPELL_EJECT_ALL_PASSENGERS      = 68576,
    SPELL_CATCH_FALL_REMOVAL        = 85274,
};

enum Events
{
    EVENT_EJECT_ALL_PASSENGERS      = 1,
    EVENT_THROW_PASSENGER           = 2,
};

enum VehicleSeats
{
    SEAT_0 = 0
};

class at_totfw_jet_stream : public AreaTriggerScript
{
    public:
        at_totfw_jet_stream() : AreaTriggerScript("at_totfw_jet_stream") { }

        bool OnTrigger(Player* player, AreaTriggerEntry const* /*areaTrigger*/)
        {
            if (!player->HasAura(SPELL_JET_STREAM_TRIGGERED) && !player->GetVehicle())
                if (Creature* trigger = player->FindNearestCreature(NPC_WORLD_TRIGGER, 500.0f, true))
                    trigger->CastSpell(player, SPELL_JET_STREAM, true);

            return true;
        }
};

class at_totfw_catch_fall : public AreaTriggerScript
{
    public:
        at_totfw_catch_fall() : AreaTriggerScript("at_totfw_catch_fall") { }

        bool OnTrigger(Player* player, AreaTriggerEntry const* /*areaTrigger*/)
        {
            if (Creature* trigger = player->FindNearestCreature(NPC_WORLD_TRIGGER, 500.0f, true))
                trigger->CastSpell(player, SPELL_CATCH_FALL, true);

            return true;
        }
};

class npc_totfw_fall_catcher : public CreatureScript
{
    public:
        npc_totfw_fall_catcher() : CreatureScript("npc_totfw_fall_catcher") { }

        struct npc_totfw_fall_catcherAI : public ScriptedAI
        {
            npc_totfw_fall_catcherAI(Creature* creature) : ScriptedAI(creature)
            {
                Initialize();
            }

            void Initialize()
            {
                _guid = ObjectGuid::Empty;
            }

            void IsSummonedBy(Unit* summoner) override
            {
                summoner->CastSpell(me, SPELL_CATCH_FALL_RIDE_VEHICLE, true);
                _events.ScheduleEvent(EVENT_EJECT_ALL_PASSENGERS, Seconds(10) + Milliseconds(400));
            }

            void PassengerBoarded(Unit* passenger, int8 /*seatId*/, bool apply) override
            {
                if (!passenger)
                    return;

                passenger->SetDisableGravity(apply, true);

                if (!apply)
                {
                    _guid = passenger->GetGUID();
                    _events.ScheduleEvent(EVENT_THROW_PASSENGER, Milliseconds(100));
                }
            }

            void UpdateAI(uint32 diff) override
            {
                _events.Update(diff);

                while (uint32 eventId = _events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_EJECT_ALL_PASSENGERS:
                            DoCastSelf(SPELL_EJECT_ALL_PASSENGERS, true);
                            break;
                        case EVENT_THROW_PASSENGER:
                            if (Unit* target = ObjectAccessor::GetUnit(*me, _guid))
                                DoCast(target, SPELL_CATCH_FALL_REMOVAL);
                            break;
                        default:
                            break;
                    }
                }
            }
        private:
            EventMap _events;
            ObjectGuid _guid;
        };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return GetThroneOfTheFourWindsAI<npc_totfw_fall_catcherAI>(creature);
        }
};

class spell_totfw_jet_stream : public SpellScriptLoader
{
    public:
        spell_totfw_jet_stream() : SpellScriptLoader("spell_totfw_jet_stream") { }

        class spell_totfw_jet_stream_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_totfw_jet_stream_SpellScript);

            void HandleForcecast(SpellEffIndex effIndex)
            {
                PreventHitDefaultEffect(effIndex);
                GetHitUnit()->CastSpell(GetHitUnit(), GetSpellInfo()->Effects[effIndex].TriggerSpell, true);
            }

            void Register() override
            {
                OnEffectHitTarget += SpellEffectFn(spell_totfw_jet_stream_SpellScript::HandleForcecast, EFFECT_0, SPELL_EFFECT_FORCE_CAST);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_totfw_jet_stream_SpellScript();
        }
};

class spell_totfw_catch_fall_summon : public SpellScriptLoader
{
    public:
        spell_totfw_catch_fall_summon() : SpellScriptLoader("spell_totfw_catch_fall_summon") { }

        class spell_totfw_catch_fall_summon_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_totfw_catch_fall_summon_SpellScript);

            void SetDestPosition(SpellEffIndex /*effIndex*/)
            {
                float x = GetExplTargetDest()->GetPositionX();
                float y = GetExplTargetDest()->GetPositionY();
                float z = GetExplTargetDest()->GetPositionZ() + 30.0f;

                const_cast<WorldLocation*>(GetExplTargetDest())->Relocate(x, y, z);
                GetHitDest()->Relocate(x, y, z);
            }

            void Register()
            {
                OnEffectLaunch += SpellEffectFn(spell_totfw_catch_fall_summon_SpellScript::SetDestPosition, EFFECT_0, SPELL_EFFECT_SUMMON);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_totfw_catch_fall_summon_SpellScript();
        }
};

void AddSC_throne_of_the_four_winds()
{
    new at_totfw_jet_stream();
    new at_totfw_catch_fall();
    new npc_totfw_fall_catcher();
    new spell_totfw_jet_stream();
    new spell_totfw_catch_fall_summon();
}
