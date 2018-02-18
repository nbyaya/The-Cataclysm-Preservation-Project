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
#include "ScriptedCreature.h"
#include "deadmines.h"
#include "Spell.h"
#include "Player.h"
#include "WorldSession.h"
#include "GameObjectAI.h"
#include "GameObject.h"

enum DefiasWatcherSpells
{
    SPELL_ON_FIRE           = 91737,
    SPELL_EXPLOSIVE_SUICIDE = 91738,
    SPELL_ENERGIZE          = 89132,
};

enum DefiasWatcherFaction
{
    FACTION_CONTROLABLE     = 1816
};

class npc_deadmines_defias_watcher : public CreatureScript
{
    public:
        npc_deadmines_defias_watcher() : CreatureScript("npc_deadmines_defias_watcher") { }

        struct npc_deadmines_defias_watcherAI : public ScriptedAI
        {
            npc_deadmines_defias_watcherAI(Creature* creature) : ScriptedAI(creature), _instance(creature->GetInstanceScript())
            {
                Initialize();
            }

            void Initialize()
            {
                _suicideStarted = false;
            }

            void JustEngagedWith(Unit* who) override
            {
            }

            void Reset() override
            {
                me->SetHealth(me->GetMaxHealth());
            }

            void JustDied(Unit* /*killer*/) override
            {
                if (!_suicideStarted)
                    DoCastAOE(SPELL_ENERGIZE, true);
            }

            void UpdateAI(uint32 diff) override
            {
                if (!UpdateVictim())
                    return;

                if (me->GetHealthPct() <= 30.0f && !_suicideStarted)
                {
                    DoCastSelf(SPELL_ON_FIRE, true);
                    DoCastAOE(SPELL_ENERGIZE, true);
                    me->AttackStop();
                    me->SetReactState(REACT_PASSIVE);
                    me->SetFaction(FACTION_CONTROLABLE);
                    me->setRegeneratingHealth(false);
                    _events.Reset();
                    _suicideStarted = true;
                }

                _events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while (uint32 eventId = _events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case 0:
                            break;
                        default:
                            break;
                    }
                }
                DoMeleeAttackIfReady();
            }
        private:
            EventMap _events;
            InstanceScript* _instance;
            bool _suicideStarted;
        };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return GetDeadminesAI<npc_deadmines_defias_watcherAI>(creature);
        }
};

enum DefiasCannon
{
    // GroupID 0 and 1 are used by Foe Reaper 5000
    SAY_ANNOUNCE_CANNON_LOADED = 2
};

class go_deadmines_defias_cannon : public GameObjectScript
{
    public:
        go_deadmines_defias_cannon() : GameObjectScript("go_deadmines_defias_cannon") { }

        struct go_deadmines_defias_cannonAI : public GameObjectAI
        {
            go_deadmines_defias_cannonAI(GameObject* go) : GameObjectAI(go), _instance(go->GetInstanceScript()) { }

            bool OnReportUse(Player* player) override
            {
                if (GameObject* cannon = me->ToGameObject())
                {
                    if (_instance->GetData(DATA_BROKEN_DOOR) != DONE)
                    {
                        cannon->SendCustomAnim(0);
                        if (GameObject* door = _instance->GetGameObject(DATA_IRON_CLAD_DOOR))
                            door->SetGoState(GO_STATE_ACTIVE_ALTERNATIVE);

                        if (Creature* bunny = me->FindNearestCreature(NPC_GENERAL_PURPOSE_DUMMY_JMF, 20.0f, true))
                            bunny->AI()->Talk(SAY_ANNOUNCE_CANNON_LOADED);

                        _instance->SetData(DATA_BROKEN_DOOR, DONE);
                    }
                }
                return true;
            }
        private:
            InstanceScript* _instance;
        };

        GameObjectAI* GetAI(GameObject* go) const override
        {
            return GetDeadminesAI<go_deadmines_defias_cannonAI>(go);
        }
};

class spell_deadmines_on_fire : public SpellScriptLoader
{
    public:
        spell_deadmines_on_fire() : SpellScriptLoader("spell_deadmines_on_fire") { }

        class spell_deadmines_on_fire_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_deadmines_on_fire_AuraScript);

            bool Validate(SpellInfo const* /*spellInfo*/) override
            {
                return ValidateSpellInfo({ SPELL_EXPLOSIVE_SUICIDE });
            }

            void AfterRemove(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* owner = GetOwner()->ToUnit())
                    owner->CastSpell(owner, SPELL_EXPLOSIVE_SUICIDE, true);
            }

            void Register() override
            {
                AfterEffectRemove += AuraEffectRemoveFn(spell_deadmines_on_fire_AuraScript::AfterRemove, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE_PERCENT, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_deadmines_on_fire_AuraScript();
        }
};

void AddSC_deadmines()
{
    new npc_deadmines_defias_watcher();
    new go_deadmines_defias_cannon();
    new spell_deadmines_on_fire();
}
