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
#include "Player.h"
#include "grim_batol.h"

enum Spells
{
    // General Umbriss
    SPELL_BLEEDING_WOUND                = 74846,
    SPELL_BLITZ_SUMMON_TRIGGER          = 74668,
    SPELL_BLITZ                         = 74670,
    SPELL_GROUND_SIEGE_SUMMON_TRIGGER   = 74640,
    SPELL_GROUND_SIEGE                  = 74634,
    SPELL_SUMMON_SKARDYN                = 74859,
    SPELL_FRENZY                        = 74853,

    // Troggs
    SPELL_CLAW_PUNCTURE                 = 76507,
    SPELL_MODGUDS_MALADY                = 74837,
    SPELL_MODGUDS_MALICE                = 74699,
    SPELL_MODGUDS_MALICE_HC             = 90169,
    SPELL_MODGUDS_MALICE_SPREAD         = 90170,

    // Battered Dragon
    SPELL_ENGULFING_FLAMES              = 74040,
    SPELL_ENGULFING_FLAMES_HC           = 90904,
};

enum Events
{
    // General Umbriss
    EVENT_BLEEDING_WOUND = 1,
    EVENT_BLITZ,
    EVENT_GROUND_SIEGE,
    EVENT_SUMMON_SKARDYN,

    // Skardyn
    EVENT_ATTACK_PLAYER,
    EVENT_CLAW_PUNCTURE,
};

enum Texts
{
    SAY_BOMBARDMENT             = 0,
    SAY_AGGRO                   = 1,
    SAY_SUMMON_SKARDYN          = 2,
    SAY_SLAY                    = 3,
    SAY_ANNOUNCE_BLITZ          = 4,
    SAY_ANNOUNCE_GROUND_SIEGE   = 5,
    SAY_ANNOUNCE_FRENZY         = 6,
};

enum MovementPoints
{
    POINT_SKARDYN_SUMMON = 1
};

Position const MalignantTroggSummonPosition = { -705.4689f, -485.3758f, 281.0951f, 1.265874f };
Position const MalignantTroggMovePosition = { -691.993f, -456.1111f, 268.7674f };

Position const TroggDwellerSummonPositions[3] =
{
    { -723.0738f, -488.2023f, 292.2519f, 1.432476f  },
    { -749.5168f, -469.0633f, 309.0818f, 0.5629198f },
    { -697.3342f, -486.9066f, 281.0173f, 1.33856f   }
};

Position const TroggDwellerMovePositions[3] =
{
    { -718.5313f, -443.1129f, 268.7674f },
    { -729.0295f, -443.5573f, 268.7674f },
    { -693.0521f, -458.2517f, 268.7674f }
};

class boss_general_umbriss : public CreatureScript
{
    public:
        boss_general_umbriss() : CreatureScript("boss_general_umbriss") { }

        struct boss_general_umbrissAI : public BossAI
        {
            boss_general_umbrissAI(Creature* creature) : BossAI(creature, DATA_GENERAL_UMBRISS)
            {
                Initialize();
            }

            void Reset() override
            {
                _Reset();
                Initialize();
                me->ApplySpellImmune(0, IMMUNITY_ID, !IsHeroic() ? SPELL_ENGULFING_FLAMES : SPELL_ENGULFING_FLAMES_HC, true);
            }

            void Initialize()
            {
                _frenzyCasted = false;
            }

            void JustEngagedWith(Unit* /*who*/) override
            {
                _JustEngagedWith();
                Talk(SAY_AGGRO);
                instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);
                events.ScheduleEvent(EVENT_SUMMON_SKARDYN, Seconds(6));
                events.ScheduleEvent(EVENT_BLEEDING_WOUND, Seconds(11));
                events.ScheduleEvent(EVENT_GROUND_SIEGE, Seconds(25) + Milliseconds(500));
                events.ScheduleEvent(EVENT_BLITZ, Seconds(16));
            }

            void KilledUnit(Unit* victim) override
            {
                if (victim->GetTypeId() == TYPEID_PLAYER)
                    Talk(SAY_SLAY);
            }

            void JustDied(Unit* /*killer*/) override
            {
                _JustDied();
                summons.DespawnAll();
                instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
            }

            void EnterEvadeMode(EvadeReason /*why*/) override
            {
                _EnterEvadeMode();
                summons.DespawnAll();
                instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
                _DespawnAtEvade();
            }

            void JustSummoned(Creature* summon) override
            {
                summons.Summon(summon);

                switch (summon->GetEntry())
                {
                    case NPC_BLITZ_STALKER:
                        me->StopMoving();
                        me->SetFacingToObject(summon);
                        DoCast(summon, SPELL_BLITZ);
                        break;
                    case NPC_GROUND_SIEGE_STALKER:
                        me->StopMoving();
                        me->SetFacingToObject(summon);
                        DoCast(summon, SPELL_GROUND_SIEGE);
                        Talk(SAY_ANNOUNCE_GROUND_SIEGE);
                        break;
                    default:
                        break;
                }
            }

            void DamageTaken(Unit* /*attacker*/, uint32& /*damage*/) override
            {
                if (me->HealthBelowPct(30) && !_frenzyCasted)
                {
                    _frenzyCasted = true;
                    DoCastSelf(SPELL_FRENZY, true);
                }
            }

            void SpellHit(Unit* /*caster*/, SpellInfo const* spell) override
            {
                if (spell->Id == SPELL_FRENZY)
                    Talk(SAY_ANNOUNCE_FRENZY);
            }

            void UpdateAI(uint32 diff) override
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_BLEEDING_WOUND:
                            DoCastVictim(SPELL_BLEEDING_WOUND);
                            events.Repeat(Seconds(10) + Milliseconds(500));
                            break;
                        case EVENT_BLITZ:
                            DoCastAOE(SPELL_BLITZ_SUMMON_TRIGGER, true);
                            events.Repeat(Seconds(23));
                            break;
                        case EVENT_GROUND_SIEGE:
                            DoCastAOE(SPELL_GROUND_SIEGE_SUMMON_TRIGGER, true);
                            events.Repeat(Seconds(23));
                            break;
                        case EVENT_SUMMON_SKARDYN:
                            summons.RemoveNotExisting();
                            if (!summons.HasEntry(NPC_MALIGNANT_TROGG) && !summons.HasEntry(NPC_TROGG_DWELLER))
                            {
                                Talk(SAY_SUMMON_SKARDYN);
                                DoCastAOE(SPELL_SUMMON_SKARDYN); // sends a event but since event_scripts does not support multiple spawns...
                                if (Creature* malignantTrogg = DoSummon(NPC_MALIGNANT_TROGG, MalignantTroggSummonPosition, 5000))
                                    malignantTrogg->GetMotionMaster()->MovePoint(POINT_SKARDYN_SUMMON, MalignantTroggMovePosition, false);
                                for (uint8 i = 0; i < 3; i++)
                                    if (Creature* troggDweller = DoSummon(NPC_TROGG_DWELLER, TroggDwellerSummonPositions[i], 5000))
                                        troggDweller->GetMotionMaster()->MovePoint(POINT_SKARDYN_SUMMON, TroggDwellerMovePositions[i], false);
                            }
                            events.Repeat(Seconds(23));
                            break;
                        default:
                            break;
                    }
                }
                DoMeleeAttackIfReady();
            }
        private:
            bool _frenzyCasted;
        };


        CreatureAI* GetAI(Creature *creature) const override
        {
            return GetGrimBatolAI<boss_general_umbrissAI>(creature);
        }
};

class npc_umbriss_skardyn : public CreatureScript
{
    public:
        npc_umbriss_skardyn() : CreatureScript("npc_umbriss_skardyn") { }

        struct npc_umbriss_skardynAI : public ScriptedAI
        {
            npc_umbriss_skardynAI(Creature* creature) : ScriptedAI(creature)
            {
                _infected = false;
            }

            void MovementInform(uint32 type, uint32 pointId) override
            {
                if (type == POINT_MOTION_TYPE && pointId == POINT_SKARDYN_SUMMON)
                    _events.ScheduleEvent(EVENT_ATTACK_PLAYER, Milliseconds(1));
            }

            void SpellHit(Unit* /*caster*/, SpellInfo const* spell) override
            {
                if (spell->Id == SPELL_MODGUDS_MALICE || spell->Id == SPELL_MODGUDS_MALICE_HC)
                    _infected = true;
            }

            void JustEngagedWith(Unit* /*victim*/) override
            {
                _events.ScheduleEvent(EVENT_CLAW_PUNCTURE, Seconds(6), Seconds(7));
            }

            void JustDied(Unit* /*killer*/) override
            {
                if (_infected || me->GetEntry() == NPC_MALIGNANT_TROGG)
                {
                    DoCastAOE(SPELL_MODGUDS_MALADY, true);
                    DoCastAOE(SPELL_MODGUDS_MALICE_SPREAD, true);
                }
                me->DespawnOrUnsummon(Seconds(5));
            }

            void UpdateAI(uint32 diff) override
            {
                if (me->IsInCombat() && !UpdateVictim())
                    return;

                _events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while (uint32 eventId = _events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_ATTACK_PLAYER:
                            if (Player* player = me->SelectNearestPlayer(100.0f))
                                me->AI()->AttackStart(player);
                            break;
                        case EVENT_CLAW_PUNCTURE:
                            if (Unit* victim = me->GetVictim())
                                DoCast(victim, SPELL_CLAW_PUNCTURE);
                            _events.Repeat(Seconds(6), Seconds(7));
                            break;
                        default:
                            break;
                    }
                }
                DoMeleeAttackIfReady();
            }
        private:
            EventMap _events;
            bool _infected;
        };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return GetGrimBatolAI<npc_umbriss_skardynAI>(creature);
        }
};

class spell_umbriss_summon_blitz_trigger : public SpellScriptLoader
{
    public:
        spell_umbriss_summon_blitz_trigger() : SpellScriptLoader("spell_umbriss_summon_blitz_trigger") { }

        class spell_umbriss_summon_blitz_trigger_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_umbriss_summon_blitz_trigger_SpellScript);

            void FilterTargets(std::list<WorldObject*>& targets)
            {
                if (targets.empty())
                    return;

                Trinity::Containers::RandomResize(targets, 1);
            }

            void HandleSummon(SpellEffIndex effIndex)
            {
                PreventHitDefaultEffect(effIndex);

                GetHitUnit()->CastSpell(GetHitUnit(), GetSpellInfo()->Effects[effIndex].TriggerSpell, true);
            }

            void HandleScriptEffect(SpellEffIndex /*effIndex*/)
            {
                if (Creature* umbriss = GetCaster()->ToCreature())
                    umbriss->AI()->Talk(SAY_ANNOUNCE_BLITZ, GetHitUnit());
            }

            void Register() override
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_umbriss_summon_blitz_trigger_SpellScript::FilterTargets, EFFECT_ALL, TARGET_UNIT_SRC_AREA_ENEMY);
                OnEffectHitTarget += SpellEffectFn(spell_umbriss_summon_blitz_trigger_SpellScript::HandleSummon, EFFECT_0, SPELL_EFFECT_FORCE_CAST);
                OnEffectHitTarget += SpellEffectFn(spell_umbriss_summon_blitz_trigger_SpellScript::HandleScriptEffect, EFFECT_1, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_umbriss_summon_blitz_trigger_SpellScript();
        }
};

class spell_umbriss_summon_ground_siege_trigger : public SpellScriptLoader
{
    public:
        spell_umbriss_summon_ground_siege_trigger() : SpellScriptLoader("spell_umbriss_summon_ground_siege_trigger") { }

        class spell_umbriss_summon_ground_siege_trigger_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_umbriss_summon_ground_siege_trigger_SpellScript);

            void FilterTargets(std::list<WorldObject*>& targets)
            {
                if (targets.empty())
                    return;

                Trinity::Containers::RandomResize(targets, 1);
            }

            void HandleSummon(SpellEffIndex effIndex)
            {
                PreventHitDefaultEffect(effIndex);
                GetHitUnit()->CastSpell(GetHitUnit(), GetSpellInfo()->Effects[effIndex].TriggerSpell, true);
            }

            void Register() override
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_umbriss_summon_ground_siege_trigger_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
                OnEffectHitTarget += SpellEffectFn(spell_umbriss_summon_ground_siege_trigger_SpellScript::HandleSummon, EFFECT_0, SPELL_EFFECT_FORCE_CAST);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_umbriss_summon_ground_siege_trigger_SpellScript();
        }
};

class spell_umbriss_bleeding_wound : public SpellScriptLoader
{
    public:
        spell_umbriss_bleeding_wound() : SpellScriptLoader("spell_umbriss_bleeding_wound") { }

        class spell_umbriss_bleeding_wound_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_umbriss_bleeding_wound_AuraScript);

            void HandleRemoval(AuraEffect const* aurEff)
            {
                if (Unit* owner = GetOwner()->ToUnit())
                    if (owner->GetHealthPct() >= 90.0f)
                        aurEff->GetBase()->Remove();
            }   

            void Register() override
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_umbriss_bleeding_wound_AuraScript::HandleRemoval, EFFECT_1, SPELL_AURA_PERIODIC_DAMAGE);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_umbriss_bleeding_wound_AuraScript();
        }
};

class spell_umbriss_blitz : public SpellScriptLoader
{
    public:
        spell_umbriss_blitz() : SpellScriptLoader("spell_umbriss_blitz") { }

        class spell_umbriss_blitz_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_umbriss_blitz_SpellScript);

            void HandleCharge(SpellEffIndex /*effIndex*/)
            {
                if (Unit* caster = GetCaster())
                {
                    caster->ClearUnitState(UNIT_STATE_CANNOT_TURN);
                    caster->ClearUnitState(UNIT_STATE_CASTING);
                }
            }

            void Register() override
            {
                OnEffectLaunch += SpellEffectFn(spell_umbriss_blitz_SpellScript::HandleCharge, EFFECT_0, SPELL_EFFECT_CHARGE_DEST);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_umbriss_blitz_SpellScript();
        }
};

void AddSC_boss_general_umbriss()
{
    new boss_general_umbriss();
    new npc_umbriss_skardyn();
    new spell_umbriss_summon_blitz_trigger();
    new spell_umbriss_summon_ground_siege_trigger();
    new spell_umbriss_bleeding_wound();
    new spell_umbriss_blitz();
}
