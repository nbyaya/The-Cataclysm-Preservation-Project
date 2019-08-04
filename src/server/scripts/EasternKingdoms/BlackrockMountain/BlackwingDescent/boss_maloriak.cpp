/*
 * Copyright (C) 2008-2019 TrinityCore <https://www.trinitycore.org/>
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
#include "CommonPredicates.h"
#include "GridNotifiers.h"
#include "PassiveAI.h"
#include "ScriptedCreature.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "SpellMgr.h"
#include "blackwing_descent.h"

enum Spells
{
    // Maloriak
    SPELL_ARCANE_STORM                  = 77896,
    SPELL_REMEDY                        = 77912,
    SPELL_THROW_RED_BOTTLE              = 77925,
    SPELL_THROW_BLUE_BOTTLE             = 77932,
    SPELL_THROW_GREEN_BOTTLE            = 77937,
    SPELL_DRINK_RED_BOTTLE              = 88699,
    SPELL_DRINK_BLUE_BOTTLE             = 88700,
    SPELL_DRINK_BLACK_BOTTLE            = 92828,
    SPELL_FIRE_IMBUED                   = 78896,
    SPELL_FROST_IMBUED                  = 78895,
    SPELL_SHADOW_IMBUED                 = 92716,
    SPELL_SLIME_IMBUED                  = 92917,
    SPELL_THROW_RED_BOTTLE_TRIGGERED    = 77928,
    SPELL_THROW_BLUE_BOTTLE_TRIGGERED   = 77934,
    SPELL_THROW_GREEN_BOTTLE_TRIGGERED  = 77938,
    SPELL_THROW_BLACK_BOTTLE_TRIGGERED  = 92837,
    SPELL_RELEASE_ABERRATIONS           = 77569,
    SPELL_RELEASE_ALL_MINIONS           = 77991,
    SPELL_SCORCHING_BLAST               = 77679,
    SPELL_BITING_CHILL                  = 77760,
    SPELL_FLASH_FREEZE_TARGETING        = 97693,

    // Cauldron Trigger
    SPELL_DEBILITATING_SLIME_CAST       = 77602,
    SPELL_DEBILITATING_SLIME_KNOCKBACK  = 77948,
    SPELL_DEBILITATING_SLIME_DEBUFF     = 77615,

    // Flash Freeze
    SPELL_FLASH_FREEZE_VISUAL           = 77712,
    SPELL_SHATTER                       = 77715,

    // Experiments
    SPELL_DROWNED_STATE                 = 77564,
    SPELL_GROWTH_CATALYST               = 77987,

    // Player
    SPELL_FLASH_FREEZE_SUMMON           = 77711,
    SPELL_FLASH_FREEZE_DUMMY            = 77716,
    SPELL_FLASH_FREEZE_STUN_NORMAL      = 77699

};

#define SPELL_FLASH_FREEZE_STUN RAID_MODE<uint32>(77699, 92978, 92979, 92980)
#define SPELL_CONSUMING_FLAMES RAID_MODE<uint32>(77786, 92971, 92972, 92973)

enum Events
{
    // Maloriak
    EVENT_ARCANE_STORM = 1,
    EVENT_REMEDY,
    EVENT_RELEASE_ABERRATIONS,
    EVENT_FACE_TO_CAULDRON,
    EVENT_THROW_VIAL,
    EVENT_MOVE_TO_CAULDRON,
    EVENT_DRINK_BOTTLE,
    EVENT_IMBUED_BUFF,
    EVENT_EXPLODE_CAULDRON,
    EVENT_ATTACK_PLAYERS,
    EVENT_CONSUMING_FLAMES,
    EVENT_SCORCHING_BLAST,
    EVENT_BITING_CHILL,
    EVENT_FLASH_FREEZE,

    // Experiments
    EVENT_LEAP_OUT_OF_CHAMBER,
};

enum Actions
{
    ACTION_SCHEDULE_EVENTS_FOR_PHASE    = 1,
    ACTION_RELEASE_EXPERIMENT           = 1
};

enum MovePoints
{
    // Maloriak
    POINT_CAULDRON  = 1,

    // Experiments
    POINT_GROUND    = 1
};

enum Texts
{
    // Maloriak
    SAY_AGGRO               = 0,
    SAY_RED_VIAL            = 1,
    SAY_ANNOUNCE_RED_VIAL   = 2,
    SAY_BLUE_VIAL           = 3,
    SAY_ANNOUNCE_BLUE_VIAL  = 4,
    SAY_GREEN_VIAL          = 5,
    SAY_ANNOUNCE_GREEN_VIAL = 6,
    SAY_RELEASE_ABERRATIONS = 7,
};

enum Vials
{
    VIAL_RED    = 0,
    VIAL_BLUE   = 1,
    VIAL_GREEN  = 2
};

enum GameObjectCustomAnim
{
    CUSTOM_ANIM_RED_CAULDRON    = 0,
    CUSTOM_ANIM_BLUE_CAULDRON   = 1,
    CUSTOM_ANIM_GREEN_CAULDRON  = 2,
    CUSTOM_ANIM_BLACK_CAULDRON  = 3
};

Position const CauldronMovePosition = { -106.6782f, -475.4438f, 73.45684f };

struct VialData
{
    uint8 SayTextId;
    uint8 AnnounceTextId;
    uint32 ThrowSpellId;
    uint32 DrinkSpellId;
    uint32 ImbuedSpellId;
};

enum Misc
{
    SUMMON_GROUP_EXPERIMENTS    = 0,
    SPAWN_GROUP_GROWTH_CHAMBERS = 401
};

std::unordered_map<uint8, VialData> vialData =
{
    { VIAL_RED,    { SAY_RED_VIAL,   SAY_ANNOUNCE_RED_VIAL,      SPELL_THROW_RED_BOTTLE,     SPELL_DRINK_RED_BOTTLE,    SPELL_FIRE_IMBUED   }},
    { VIAL_BLUE,   { SAY_BLUE_VIAL,  SAY_ANNOUNCE_BLUE_VIAL,     SPELL_THROW_BLUE_BOTTLE,    SPELL_DRINK_BLUE_BOTTLE,   SPELL_FROST_IMBUED  }},
    { VIAL_GREEN,  { SAY_GREEN_VIAL, SAY_ANNOUNCE_GREEN_VIAL,    SPELL_THROW_GREEN_BOTTLE,   0,                         SPELL_SLIME_IMBUED  }},
};

struct boss_maloriak : public BossAI
{
    boss_maloriak(Creature* creature) : BossAI(creature, DATA_MALORIAK),
        _currentVial(urand(VIAL_RED, VIAL_BLUE)), _usedVialsCount(0), _releasedAberrationsCount(0) { }

    void Reset() override
    {
        _Reset();
        me->MakeInterruptable(false);
    }

    void JustAppeared() override
    {
        instance->instance->SpawnGroupSpawn(SPAWN_GROUP_GROWTH_CHAMBERS, true);
        me->SummonCreatureGroup(SUMMON_GROUP_EXPERIMENTS);
    }

    void JustEngagedWith(Unit* /*who*/) override
    {
        _JustEngagedWith();
        Talk(SAY_AGGRO);
        instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);
        events.ScheduleEvent(EVENT_FACE_TO_CAULDRON, 15s + 500ms);
    }

    void EnterEvadeMode(EvadeReason /*why*/) override
    {
        _EnterEvadeMode();
        instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
        CleanupEncounter();
        _DespawnAtEvade();
    }

    void JustDied(Unit* /*killer*/) override
    {
        // Talk(SAY_DEATH);
        instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
        CleanupEncounter();
        _JustDied();
    }

    void OnSpellCastInterrupt(SpellInfo const* spell) override
    {
        switch (spell->Id)
        {
            case SPELL_ARCANE_STORM:
                me->MakeInterruptable(false);
                me->m_Events.KillAllEvents(true);
                break;
            case SPELL_RELEASE_ABERRATIONS:
                me->MakeInterruptable(false);
                break;
            default:
                break;
        }
    }

    void OnSuccessfulSpellCast(SpellInfo const* spell) override
    {
        switch (spell->Id)
        {
            case SPELL_RELEASE_ABERRATIONS:
                Talk(SAY_RELEASE_ABERRATIONS);
                me->MakeInterruptable(false);
                break;
            default:
                break;
        }
    }

    void MovementInform(uint32 motionType, uint32 pointId) override
    {
        if (motionType != POINT_MOTION_TYPE && motionType != EFFECT_MOTION_TYPE)
            return;

        switch (pointId)
        {
            case POINT_CAULDRON:
                events.ScheduleEvent(EVENT_DRINK_BOTTLE, 1s + 500ms);
                break;
            default:
                break;
        }
    }

    void DoAction(int32 action) override
    {
        switch (action)
        {
            case ACTION_SCHEDULE_EVENTS_FOR_PHASE:
                events.ScheduleEvent(EVENT_ATTACK_PLAYERS, _currentVial != VIAL_GREEN ? 1ms : 4s);

                if (_currentVial == VIAL_RED || _currentVial == VIAL_BLUE)
                {
                    events.ScheduleEvent(EVENT_ARCANE_STORM, 14s + 500ms);
                    events.ScheduleEvent(EVENT_REMEDY, 21s + 500ms);
                    events.ScheduleEvent(EVENT_RELEASE_ABERRATIONS, 11s);
                }
                else if (_currentVial == VIAL_GREEN)
                {
                    if (Creature* cauldron = instance->GetCreature(DATA_CAULDRON_TRIGGER))
                        cauldron->CastSpell(cauldron, SPELL_DEBILITATING_SLIME_DEBUFF);

                    events.ScheduleEvent(EVENT_ARCANE_STORM, 8s);
                    events.ScheduleEvent(EVENT_REMEDY, 33s + 500ms);
                    events.ScheduleEvent(EVENT_RELEASE_ABERRATIONS, 15s);
                }

                if (_currentVial == VIAL_RED)
                {
                    events.ScheduleEvent(EVENT_CONSUMING_FLAMES, 7s + 500ms);
                    events.ScheduleEvent(EVENT_SCORCHING_BLAST, 22s);
                }
                else if (_currentVial == VIAL_BLUE)
                {
                    events.ScheduleEvent(EVENT_BITING_CHILL, 13s + 500ms);
                    events.ScheduleEvent(EVENT_FLASH_FREEZE, 22s);
                }

                events.ScheduleEvent(EVENT_FACE_TO_CAULDRON, 40s);
                break;
            default:
                break;
        }
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
                case EVENT_ARCANE_STORM:
                {
                    me->MakeInterruptable(true);
                    DoCastSelf(SPELL_ARCANE_STORM);
                    Creature* maloriak = me;
                    me->m_Events.AddEventAtOffset([maloriak]() { maloriak->MakeInterruptable(false); }, 6s + 500ms);
                    events.Repeat(13s + 500ms);
                    break;
                }
                case EVENT_REMEDY:
                    DoCastSelf(SPELL_REMEDY);
                    break;
                case EVENT_RELEASE_ABERRATIONS:
                    if (_releasedAberrationsCount < 6)
                    {
                        me->MakeInterruptable(true);
                        DoCastAOE(SPELL_RELEASE_ABERRATIONS);
                        events.Repeat(24s + 500ms);
                        _releasedAberrationsCount++;
                    }
                    break;
                case EVENT_FACE_TO_CAULDRON:
                    events.Reset();
                    if (Creature* cauldron = instance->GetCreature(DATA_CAULDRON_TRIGGER))
                    {
                        me->AttackStop();
                        me->SetReactState(REACT_PASSIVE);
                        me->SetFacingToObject(cauldron);

                        _currentVial = _usedVialsCount < 2 ? _currentVial == VIAL_BLUE ? VIAL_RED : VIAL_BLUE : VIAL_GREEN;
                        Talk(vialData[_currentVial].SayTextId);
                        _usedVialsCount = _usedVialsCount < 2 ? _usedVialsCount + 1 : 0;
                        events.ScheduleEvent(EVENT_THROW_VIAL, 1s + 300ms);
                    }
                    break;
                case EVENT_THROW_VIAL:
                    DoCastSelf(vialData[_currentVial].ThrowSpellId);
                    Talk(vialData[_currentVial].AnnounceTextId);
                    events.ScheduleEvent(EVENT_MOVE_TO_CAULDRON, 1s);
                    break;
                case EVENT_MOVE_TO_CAULDRON:
                    me->GetMotionMaster()->MovePoint(POINT_CAULDRON, CauldronMovePosition);
                    break;
                case EVENT_DRINK_BOTTLE:
                    DoCastSelf(vialData[_currentVial].DrinkSpellId);
                    events.ScheduleEvent(EVENT_IMBUED_BUFF, 2s + 500ms);

                    if (_currentVial == VIAL_GREEN)
                        events.ScheduleEvent(EVENT_EXPLODE_CAULDRON, 1s);
                    break;
                case EVENT_IMBUED_BUFF:
                    DoCastSelf(vialData[_currentVial].ImbuedSpellId);
                    DoAction(ACTION_SCHEDULE_EVENTS_FOR_PHASE);
                    break;
                case EVENT_EXPLODE_CAULDRON:
                    if (Creature* cauldron = instance->GetCreature(DATA_CAULDRON_TRIGGER))
                    {
                        cauldron->CastSpell(cauldron, SPELL_DEBILITATING_SLIME_CAST);
                        cauldron->CastSpell(cauldron, SPELL_DEBILITATING_SLIME_KNOCKBACK);
                    }
                    break;
                case EVENT_ATTACK_PLAYERS:
                    me->SetReactState(REACT_AGGRESSIVE);
                    break;
                case EVENT_CONSUMING_FLAMES:
                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 60.0f, true))
                        DoCast(target, SPELL_CONSUMING_FLAMES);
                    events.Repeat(17s);
                    break;
                case EVENT_SCORCHING_BLAST:
                    DoCastSelf(SPELL_SCORCHING_BLAST);
                    events.Repeat(17s);
                    break;
                case EVENT_BITING_CHILL:
                   if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 60.0f, true))
                        DoCast(target, SPELL_BITING_CHILL);
                    events.Repeat(11s);
                    break;
                case EVENT_FLASH_FREEZE:
                    DoCastAOE(SPELL_FLASH_FREEZE_TARGETING);
                    events.Repeat(17s);
                    break;
                default:
                    break;
            }
        }

        DoMeleeAttackIfReady();
    }
private:
    uint8 _currentVial;
    uint8 _usedVialsCount;
    uint8 _releasedAberrationsCount;

    void CleanupEncounter()
    {
        instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_FLASH_FREEZE_STUN);
        instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_BITING_CHILL);
        instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_CONSUMING_FLAMES);
        instance->instance->SpawnGroupDespawn(SPAWN_GROUP_GROWTH_CHAMBERS);
        summons.DespawnAll();
    }
};

struct npc_maloriak_flash_freeze : public NullCreatureAI
{
    npc_maloriak_flash_freeze(Creature* creature) : NullCreatureAI(creature) { }

    void JustAppeared() override
    {
        DoCastSelf(SPELL_FLASH_FREEZE_VISUAL);
        Creature* creature = me;
        me->m_Events.AddEventAtOffset([creature]() { creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE); }, 1s);
    }

    void JustDied(Unit* /*killer*/) override
    {
        if (TempSummon* summon = me->ToTempSummon())
            if (Unit* owner = summon->GetSummoner())
                owner->CastSpell(owner, SPELL_FLASH_FREEZE_DUMMY, true);

        DoCastAOE(SPELL_SHATTER, true);
        me->DespawnOrUnsummon(4s);
    }
};

struct npc_maloriak_experiment : public ScriptedAI
{
    npc_maloriak_experiment(Creature* creature) : ScriptedAI(creature)
    {
        Initialize();
    }

    void Initialize()
    {
        me->SetReactState(REACT_PASSIVE);
        me->AddUnitMovementFlag(MOVEMENTFLAG_DISABLE_GRAVITY);
    }

    void JustDied(Unit* /*killer*/) override
    {
        me->DespawnOrUnsummon(5s);
    }

    void DoAction(int32 action) override
    {
        switch (action)
        {
            case ACTION_RELEASE_EXPERIMENT:
            {
                me->RemoveAurasDueToSpell(SPELL_DROWNED_STATE);
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);

                // The chambers have super weird spawn points so FindNearestGameObject wont work here.
                std::list<GameObject*> gameObjectList;
                uint32 entry = me->GetEntry() == NPC_ABERRATION ? GO_GROWTH_CHAMBER : GO_LARGE_GROWTH_CHAMBER;
                me->GetGameObjectListWithEntryInGrid(gameObjectList, entry, 2.0f);
                float z = me->GetPositionZ();

                if (me->GetEntry() == NPC_ABERRATION)
                {
                    gameObjectList.remove_if([z](GameObject const* go)
                    {
                        if (go->GetPositionZ() > z)
                            return true;

                        if (std::abs(go->GetPositionZ() - z) > 7.0f)
                            return true;

                        return false;
                    });
                }

                if (gameObjectList.empty())
                    break;

                for (GameObject* chamber : gameObjectList)
                {
                    chamber->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_IN_USE);
                    chamber->SetGoState(GO_STATE_ACTIVE_ALTERNATIVE);
                }

                _events.ScheduleEvent(EVENT_LEAP_OUT_OF_CHAMBER, 1s + 700ms);
                break;
            }
            default:
                break;
        }
    }

    void MovementInform(uint32 motionType, uint32 pointId) override
    {
        if (motionType != EFFECT_MOTION_TYPE)
            return;

        switch (pointId)
        {
            case POINT_GROUND:
                me->SetDisableGravity(false);
                me->SendSetPlayHoverAnim(false);
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
                me->SetReactState(REACT_AGGRESSIVE);
                DoZoneInCombat();
                DoCastSelf(SPELL_GROWTH_CATALYST);
                break;
            default:
                break;
        }
    }

    void UpdateAI(uint32 diff) override
    {
        // No return here. Maloriak handles the despawn.
        UpdateVictim();

        _events.Update(diff);

        while (uint32 eventId = _events.ExecuteEvent())
        {
            switch (eventId)
            {
                case EVENT_LEAP_OUT_OF_CHAMBER:
                {
                    Position pos = me->GetPosition();
                    pos.m_positionX += cos(me->GetOrientation()) * 11.64f;
                    pos.m_positionY += sin(me->GetOrientation()) * 11.64f;
                    pos.m_positionZ = me->GetMapHeight(pos.GetPositionX(), pos.GetPositionY(), me->GetPositionZ());
                    me->GetMotionMaster()->MoveJump(pos, 21.0f, 15.0f, POINT_GROUND);
                    break;
                }
                default:
                    break;
            }
        }

        DoMeleeAttackIfReady();
    }
private:
    EventMap _events;
};

class spell_maloriak_throw_bottle : public SpellScript
{
    PrepareSpellScript(spell_maloriak_throw_bottle);

    void HandleDummyEffect(SpellEffIndex effIndex)
    {
        if (Unit* caster = GetCaster())
            caster->CastSpell(GetHitUnit(), GetSpellInfo()->Effects[effIndex].BasePoints, true);
    }

    void Register() override
    {
        OnEffectLaunchTarget += SpellEffectFn(spell_maloriak_throw_bottle::HandleDummyEffect, EFFECT_0, SPELL_EFFECT_DUMMY);
    }
};

class spell_maloriak_throw_bottle_triggered : public SpellScript
{
    PrepareSpellScript(spell_maloriak_throw_bottle_triggered);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo(
            {
                SPELL_THROW_RED_BOTTLE_TRIGGERED,
                SPELL_THROW_BLUE_BOTTLE_TRIGGERED,
                SPELL_THROW_GREEN_BOTTLE_TRIGGERED,
                SPELL_THROW_BLACK_BOTTLE_TRIGGERED
            });
    }

    void HandleDummyEffect(SpellEffIndex /*effIndex*/)
    {
        InstanceScript* instance = GetHitUnit()->GetInstanceScript();
        if (!instance)
            return;

        if (GameObject* cauldron = instance->GetGameObject(DATA_CAULDRON))
        {
            switch (GetSpellInfo()->Id)
            {
                case SPELL_THROW_RED_BOTTLE_TRIGGERED:
                    cauldron->SendCustomAnim(CUSTOM_ANIM_RED_CAULDRON);
                    break;
                case SPELL_THROW_BLUE_BOTTLE_TRIGGERED:
                    cauldron->SendCustomAnim(CUSTOM_ANIM_BLUE_CAULDRON);
                    break;
                case SPELL_THROW_GREEN_BOTTLE_TRIGGERED:
                    cauldron->SendCustomAnim(CUSTOM_ANIM_GREEN_CAULDRON);
                    break;
                case SPELL_THROW_BLACK_BOTTLE_TRIGGERED:
                    cauldron->SendCustomAnim(CUSTOM_ANIM_BLACK_CAULDRON);
                    break;
                default:
                    break;
            }
        }
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_maloriak_throw_bottle_triggered::HandleDummyEffect, EFFECT_0, SPELL_EFFECT_DUMMY);
    }
};

class spell_maloriak_consuming_flames: public AuraScript
{
    PrepareAuraScript(spell_maloriak_consuming_flames);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetSpellInfo() && eventInfo.GetDamageInfo();
    }

    void HandleProc(AuraEffect const* /*aurEff*/, ProcEventInfo& eventInfo)
    {
        AuraEffect* effect = GetEffect(EFFECT_0);
        effect->SetAmount(effect->GetAmount() + CalculatePct(eventInfo.GetDamageInfo()->GetDamage(), 50));
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_maloriak_consuming_flames::HandleProc, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
    }
};

class spell_maloriak_flash_freeze_targeting : public SpellScript
{
    PrepareSpellScript(spell_maloriak_flash_freeze_targeting);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_FLASH_FREEZE_SUMMON });
    }

    void FilterTargets(std::list<WorldObject*>& targets)
    {
        if (targets.size() <= 1)
            return;

        targets.remove_if(Trinity::Predicates::IsVictimOf(GetCaster()));

        if (targets.empty())
            return;

        targets.remove_if([](WorldObject* obj)
        {
            Unit const* target = obj->ToUnit();
            if (!target)
                return true;

            for (Unit* attacker : target->getAttackers())
                if (attacker->GetEntry() == NPC_ABERRATION && attacker->GetVictim() == target)
                    return true;

            return false;
        });

        if (!targets.empty())
            Trinity::Containers::RandomResize(targets, 1);
    }

    void HandleDummyEffect(SpellEffIndex effIndex)
    {
        if (Unit* caster = GetCaster())
        {
            caster->CastSpell(GetHitUnit(), GetSpellInfo()->Effects[effIndex].BasePoints, true);
            GetHitUnit()->CastSpell(GetHitUnit(), SPELL_FLASH_FREEZE_SUMMON, true);
        }
    }

    void Register() override
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_maloriak_flash_freeze_targeting::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
        OnEffectHitTarget += SpellEffectFn(spell_maloriak_flash_freeze_targeting::HandleDummyEffect, EFFECT_0, SPELL_EFFECT_DUMMY);
    }
};

class spell_maloriak_flash_freeze_dummy : public SpellScript
{
    PrepareSpellScript(spell_maloriak_flash_freeze_dummy);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_FLASH_FREEZE_STUN_NORMAL });
    }

    void FilterTargets(std::list<WorldObject*>& targets)
    {
        Unit* caster = GetCaster();
        if (targets.empty())
        {
            caster->RemoveAurasDueToSpell(sSpellMgr->GetSpellIdForDifficulty(SPELL_FLASH_FREEZE_STUN_NORMAL, caster));
            return;
        }

        targets.remove_if([caster](WorldObject* obj)
        {
            Unit* target = obj->ToUnit();
            if (!target)
                return true;

            return target->isDead() || !target->ToTempSummon() || target->ToTempSummon()->GetSummoner() != caster;
        });

       if (targets.empty())
           caster->RemoveAurasDueToSpell(sSpellMgr->GetSpellIdForDifficulty(SPELL_FLASH_FREEZE_STUN_NORMAL, caster));
    }

    void Register() override
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_maloriak_flash_freeze_dummy::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENTRY);
    }
};

class spell_maloriak_release_experiments : public SpellScript
{
    PrepareSpellScript(spell_maloriak_release_experiments);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo(
            {
                SPELL_RELEASE_ABERRATIONS,
                SPELL_RELEASE_ALL_MINIONS
            });
    }

    void FilterTargets(std::list<WorldObject*>& targets)
    {
        if (targets.empty())
            return;

        Unit* caster = GetCaster();
        targets.remove_if(Trinity::UnitAuraCheck(false, SPELL_DROWNED_STATE));

        if (!targets.empty() && GetSpellInfo()->Id == SPELL_RELEASE_ABERRATIONS)
            Trinity::Containers::RandomResize(targets, 3);
    }

    void HandleDummyEffect(SpellEffIndex /*effIndex*/)
    {
        if (Creature* target = GetHitCreature())
            if (target->IsAIEnabled)
                target->AI()->DoAction(ACTION_RELEASE_EXPERIMENT);
    }

    void Register() override
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_maloriak_release_experiments::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENTRY);
        OnEffectHitTarget += SpellEffectFn(spell_maloriak_release_experiments::HandleDummyEffect, EFFECT_0, SPELL_EFFECT_DUMMY);
        if (m_scriptSpellId == SPELL_RELEASE_ALL_MINIONS)
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_maloriak_release_experiments::FilterTargets, EFFECT_1, TARGET_UNIT_SRC_AREA_ENTRY);
            OnEffectHitTarget += SpellEffectFn(spell_maloriak_release_experiments::HandleDummyEffect, EFFECT_1, SPELL_EFFECT_DUMMY);
        }
    }
};

void AddSC_boss_maloriak()
{
    RegisterBlackwingDescentCreatureAI(boss_maloriak);
    RegisterBlackwingDescentCreatureAI(npc_maloriak_flash_freeze);
    RegisterBlackwingDescentCreatureAI(npc_maloriak_experiment);
    RegisterSpellScript(spell_maloriak_throw_bottle);
    RegisterSpellScript(spell_maloriak_throw_bottle_triggered);
    RegisterAuraScript(spell_maloriak_consuming_flames);
    RegisterSpellScript(spell_maloriak_flash_freeze_targeting);
    RegisterSpellScript(spell_maloriak_flash_freeze_dummy);
    RegisterSpellScript(spell_maloriak_release_experiments);
}
