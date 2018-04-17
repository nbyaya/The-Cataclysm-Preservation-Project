/*
* Copyright (C) 2008-2017 TrinityCore <http://www.trinitycore.org/>
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
#include "PassiveAI.h"
#include "GameObject.h"
#include "GameObjectAI.h"
#include "Player.h"
#include "bastion_of_twilight.h"

enum Spells
{
    // Halfus Wyrmbreaker
    SPELL_FRENZIED_ASSAULT      = 83693,
    SPELL_MALEVOLENT_STRIKES    = 39171,
    SPELL_SHADOW_NOVA           = 83703,
    SPELL_BIND_WILL_TRIGGERED   = 83432,
    SPELL_PARALYSIS             = 84030,
    SPELL_FURIOUS_ROAR          = 83710,

    // Proto Behemoth
    SPELL_DRAGONS_VENGEANCE     = 87683,
    SPELL_DANCING_FLAMES        = 84106,
    SPELL_SUPERHEATED_BREATH    = 83956,
    SPELL_FIREBALL              = 86058,
    SPELL_FIREBALL_BARRAGE      = 83706,
    SPELL_FIREBALL_BARRAGE_FAST = 83720,
    SPELL_FIREBALL_BARRAGE_SLOW = 83733,
    SPELL_SCORCHING_BREATH      = 83707,

    // Enslaved Dragons
    SPELL_UNRESPONSIVE_DRAGON   = 86003,
    SPELL_UNRESPONSIVE_WHELP    = 86022,
    SPELL_NETHER_BLINDNESS      = 83611,
    SPELL_STONE_TOUCH           = 83603,
    SPELL_CYCLONE_WINDS         = 83612,
    SPELL_TIME_DILATION         = 83601,
    SPELL_ATROPHIC_POISON       = 83609,
    SPELL_BIND_WILL             = 84246,
};

enum Events
{
    // Halfus Wyrmbreaker
    EVENT_SHADOW_NOVA = 1,
    EVENT_FURIOUS_ROAR,
    EVENT_RESET_ACHIEVEMT,

    // Proto-Behemoth
    EVENT_FIREBALL,
    EVENT_FIREBALL_BARRAGE,
    EVENT_SCORCHING_BREATH,

    // Enslaved Dragons
    EVENT_LIFTOFF,
    EVENT_CAST_DEBUFF,
    EVENT_BIND_TO_HALFUS_WILL,
    EVENT_MOVE_OUT_OF_CAGE
};

enum AchievementData
{
    DATA_THE_ONLY_ESCAPE = 1,
};

enum Texts
{
    // Halfus Wyrmbreaker
    SAY_AGGRO                   = 0,
    SAY_SLAY                    = 1,
    SAY_ANNOUNCE_BIND_DRAGON    = 2,
    SAY_ANNOUNCE_FURIOUS_ROAR   = 3
};

class boss_halfus_wyrmbreaker : public CreatureScript
{
    public:
        boss_halfus_wyrmbreaker() : CreatureScript("boss_halfus_wyrmbreaker") { }

        struct boss_halfus_wyrmbreakerAI : public BossAI
        {
            boss_halfus_wyrmbreakerAI(Creature* creature) : BossAI(creature, DATA_HALFUS_WYRMBREAKER)
            {
                Initialize();
            }

            void Initialize()
            {
                _allowShadowNovaCasts = false;
                _announcedOrphanedEmeraldWhelpBinding = false;
                _furiousRoarEnabled = false;
                _furiousRoarCount = 0;
                _theOnlyEscapeAchievementState = NOT_STARTED;
            }

            void Reset() override
            {
                _Reset();
                Initialize();
                instance->SetData(DATA_CAST_DRAGON_BUFFS, DRAGON_BUFFS_HALFUS_WYRMBREAKER);
            }

            void JustEngagedWith(Unit* /*who*/) override
            {
                _JustEngagedWith();
                Talk(SAY_AGGRO);
                instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);

                if (Creature* protoBehemoth = instance->GetCreature(DATA_PROTO_BEHEMOTH))
                    protoBehemoth->SetInCombatWithZone();

                if (_allowShadowNovaCasts)
                    events.ScheduleEvent(EVENT_SHADOW_NOVA, Seconds(7));
            }

            void KilledUnit(Unit* victim) override
            {
                if (victim->GetTypeId() == TYPEID_PLAYER)
                    Talk(SAY_SLAY);
            }

            void JustDied(Unit* /*killer*/) override
            {
                _JustDied();
                instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
            }

            void EnterEvadeMode(EvadeReason /*why*/) override
            {
                _EnterEvadeMode();
                instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
                _DespawnAtEvade();
            }

            void SpellHit(Unit* caster, SpellInfo const* spell) override
            {
                if (!caster)
                    return;

                switch (spell->Id)
                {
                    case SPELL_BIND_WILL:
                        if (caster->GetEntry() == NPC_ORPHANED_EMERALD_WELP && !_announcedOrphanedEmeraldWhelpBinding)
                        {
                            Talk(SAY_ANNOUNCE_BIND_DRAGON, caster);
                            _announcedOrphanedEmeraldWhelpBinding = true;
                        }
                        else if (caster->GetEntry() != NPC_ORPHANED_EMERALD_WELP)
                            Talk(SAY_ANNOUNCE_BIND_DRAGON, caster);
                        break;
                    case SPELL_DRAGONS_VENGEANCE:
                        if (_theOnlyEscapeAchievementState == NOT_STARTED)
                        {
                            _theOnlyEscapeAchievementState = IN_PROGRESS;
                            events.ScheduleEvent(EVENT_RESET_ACHIEVEMT, Seconds(10));
                        }
                        else if (_theOnlyEscapeAchievementState == IN_PROGRESS)
                        {
                            _theOnlyEscapeAchievementState = DONE;
                            events.CancelEvent(EVENT_RESET_ACHIEVEMT);
                        }
                        break;
                    default:
                        break;
                }
            }

            void DamageTaken(Unit* /*attacker*/, uint32& /*damage*/) override
            {
                if (me->HealthBelowPct(50) && !_furiousRoarEnabled)
                {
                    events.ScheduleEvent(EVENT_FURIOUS_ROAR, Milliseconds(1));
                    _furiousRoarEnabled = true;
                }
            }

            void DoAction(int32 action)
            {
                switch (action)
                {
                    case ACTION_ENABLE_MALEVOLENT_STRIKES:
                        me->AddAura(SPELL_MALEVOLENT_STRIKES, me);
                        break;
                    case ACTION_ENABLE_FRENZIED_ASSAULT:
                        me->AddAura(SPELL_FRENZIED_ASSAULT, me);
                        break;
                    case ACTION_ENABLE_SHADOW_NOVA:
                        _allowShadowNovaCasts = true;
                        break;
                    default:
                        break;
                }
            }

            uint32 GetData(uint32 type) const override
            {
                if (type == DATA_THE_ONLY_ESCAPE)
                    return _theOnlyEscapeAchievementState;

                return 0;
            }

            void UpdateAI(uint32 diff) override
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING) || me->HasUnitState(UNIT_STATE_STUNNED))
                    return;

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_SHADOW_NOVA:
                            DoCastAOE(SPELL_SHADOW_NOVA);
                            events.Repeat(Seconds(8));
                            break;
                        case EVENT_FURIOUS_ROAR:
                            if (_furiousRoarCount == 0)
                                Talk(SAY_ANNOUNCE_FURIOUS_ROAR);

                            if (_furiousRoarCount != 2)
                            {
                                DoCastAOE(SPELL_FURIOUS_ROAR);
                                events.Repeat(Seconds(3));
                                _furiousRoarCount++;
                            }
                            else
                            {
                                DoCastAOE(SPELL_FURIOUS_ROAR);
                                events.Repeat(Seconds(25));
                                _furiousRoarCount = 0;
                            }
                            break;
                        case EVENT_RESET_ACHIEVEMT:
                            _theOnlyEscapeAchievementState = NOT_STARTED;
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }

        private:
            bool _allowShadowNovaCasts;
            bool _announcedOrphanedEmeraldWhelpBinding;
            bool _furiousRoarEnabled;
            uint8 _theOnlyEscapeAchievementState;
            uint8 _furiousRoarCount;
        };

        CreatureAI* GetAI(Creature *creature) const override
        {
            return GetBastionOfTwilightAI<boss_halfus_wyrmbreakerAI>(creature);
        }
};

class npc_halfus_proto_behemoth : public CreatureScript
{
    public:
        npc_halfus_proto_behemoth() : CreatureScript("npc_halfus_proto_behemoth") { }

        struct npc_halfus_proto_behemothAI : public PassiveAI
        {
            npc_halfus_proto_behemothAI(Creature* creature) : PassiveAI(creature), _instance(creature->GetInstanceScript()) { }

            void JustEngagedWith(Unit* /*who*/) override
            {
                _events.ScheduleEvent(EVENT_FIREBALL, Seconds(1));

                if (me->HasAura(SPELL_DANCING_FLAMES))
                    _events.ScheduleEvent(EVENT_FIREBALL_BARRAGE, Seconds(13));

                if (me->HasAura(SPELL_SUPERHEATED_BREATH))
                    _events.ScheduleEvent(EVENT_SCORCHING_BREATH, Seconds(24));
            }

            void Reset() override
            {
                _events.Reset();
                _instance->SetData(DATA_CAST_DRAGON_BUFFS, DRAGON_BUFFS_PROTO_BEHEMOTH);
            }

            void DoAction(int32 action) override
            {
                switch (action)
                {
                    case ACTION_CAST_DRAGONS_VENGEANCE:
                        DoCastSelf(SPELL_DRAGONS_VENGEANCE, true);
                        break;
                    case ACTION_ENABLE_SCORCHING_BREATH:
                        me->AddAura(SPELL_SUPERHEATED_BREATH, me);
                        break;
                    case ACTION_ENABLE_FIREBALL_BARRAGE:
                        me->AddAura(SPELL_DANCING_FLAMES, me);
                        break;
                    default:
                        break;
                }
            }

            void UpdateAI(uint32 diff) override
            {
                _events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while (uint32 eventId = _events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_FIREBALL:
                            DoCastAOE(SPELL_FIREBALL);
                            _events.Repeat(Seconds(2) + Milliseconds(500));
                            break;
                        case EVENT_FIREBALL_BARRAGE:
                            DoCastSelf(SPELL_FIREBALL_BARRAGE);
                            _events.Repeat(Seconds(31));
                            break;
                        case EVENT_SCORCHING_BREATH:
                            DoCastSelf(SPELL_SCORCHING_BREATH);
                            _events.Repeat(Seconds(31));
                            break;
                        default:
                            break;
                    }
                }
            }

        private:
            EventMap _events;
            InstanceScript* _instance;
        };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return GetBastionOfTwilightAI<npc_halfus_proto_behemothAI>(creature);
        }
};

class npc_halfus_enslaved_dragon : public CreatureScript
{
    public:
        npc_halfus_enslaved_dragon() : CreatureScript("npc_halfus_enslaved_dragon") { }

        struct npc_halfus_enslaved_dragonAI : public ScriptedAI
        {
            npc_halfus_enslaved_dragonAI(Creature* creature) : ScriptedAI(creature), _instance(creature->GetInstanceScript()) { }

            void Reset() override
            {
                _events.Reset();

                if (_instance->GetData(DATA_UNRESPONSIVE_DRAGON_FIRST) == me->GetEntry()
                    || _instance->GetData(DATA_UNRESPONSIVE_DRAGON_SECOND) == me->GetEntry())
                {
                    me->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                    if (me->GetEntry() == NPC_ORPHANED_EMERALD_WELP)
                        DoCastSelf(SPELL_UNRESPONSIVE_WHELP, true);
                    else
                        DoCastSelf(SPELL_UNRESPONSIVE_DRAGON, true);
                }
            }

            void JustDied(Unit* /*killer*/) override
            {
                me->SetDisableGravity(false);
                me->SendSetPlayHoverAnim(false);

                if (me->GetEntry() != NPC_ORPHANED_EMERALD_WELP)
                    if (Creature* protoBehemoth = _instance->GetCreature(DATA_PROTO_BEHEMOTH))
                        protoBehemoth->AI()->DoAction(ACTION_CAST_DRAGONS_VENGEANCE);
            }

            void SpellHit(Unit* /*caster*/, SpellInfo const* spell) override
            {
                if (spell->HasEffect(SPELL_EFFECT_SEND_EVENT))
                {
                    me->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);
                    me->RemoveAllAuras();
                    _events.ScheduleEvent(EVENT_LIFTOFF, Milliseconds(1));
                }
                else if (spell->Id == SPELL_BIND_WILL_TRIGGERED)
                    if (Player* player = me->SelectNearestPlayer(100.0f))
                        me->AI()->AttackStart(player);
            }

            void DoAction(int32 action) override
            {
                if (action == ACTION_MOVE_OUT_OF_CAGE)
                    _events.ScheduleEvent(EVENT_MOVE_OUT_OF_CAGE, Milliseconds(250));
            }

            void UpdateAI(uint32 diff) override
            {
                _events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while (uint32 eventId = _events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_LIFTOFF:
                        {
                            Position pos = me->GetPosition();
                            me->SetDisableGravity(true);
                            me->SendSetPlayHoverAnim(true); // TODO: fix unsetting hover on respawn.
                            me->GetMotionMaster()->MoveTakeoff(0, pos);
                            _events.ScheduleEvent(EVENT_CAST_DEBUFF, Seconds(1) + Milliseconds(750));
                            break;
                        }
                        case EVENT_MOVE_OUT_OF_CAGE:
                        {
                            float angle = me->GetOrientation();
                            float x = me->GetPositionX() + cos(angle) * 20.0f;
                            float y = me->GetPositionY() + sin(angle) * 20.0f;
                            float z = 888.1f;
                            me->GetMotionMaster()->MovePoint(0, x, y, z, true);
                            _events.ScheduleEvent(EVENT_CAST_DEBUFF, Seconds(2) + Milliseconds(600));
                            break;
                        }
                        case EVENT_CAST_DEBUFF:
                        {
                            Creature* halfus = _instance->GetCreature(DATA_HALFUS_WYRMBREAKER);
                            Creature* protoBehemoth = _instance->GetCreature(DATA_PROTO_BEHEMOTH);

                            if (!halfus || !protoBehemoth)
                                break;

                            switch (me->GetEntry())
                            {
                                case NPC_NETHER_SCION:
                                    me->SetFacingToObject(halfus);
                                    DoCastSelf(SPELL_NETHER_BLINDNESS, true);
                                    break;
                                case NPC_SLATE_DRAGON:
                                    me->SetFacingToObject(halfus);
                                    DoCastSelf(SPELL_STONE_TOUCH, true);
                                    break;
                                case NPC_STORM_RIDER:
                                    me->SetFacingToObject(halfus);
                                    DoCastSelf(SPELL_CYCLONE_WINDS, true);
                                    break;
                                case NPC_TIME_WARDEN:
                                    me->SetFacingToObject(protoBehemoth);
                                    DoCastSelf(SPELL_TIME_DILATION, true);
                                    break;
                                case NPC_ORPHANED_EMERALD_WELP:
                                    me->SetFacingToObject(protoBehemoth);
                                    DoCastSelf(SPELL_ATROPHIC_POISON, true);
                                    break;
                                default:
                                    break;
                            }
                            _events.ScheduleEvent(EVENT_BIND_TO_HALFUS_WILL, Seconds(1) + Milliseconds(850));
                            break;
                        }
                        case EVENT_BIND_TO_HALFUS_WILL:
                            if (Creature* halfus = _instance->GetCreature(DATA_HALFUS_WYRMBREAKER))
                            {
                                _instance->SetData(DATA_DRAGON_TO_BIND, me->GetEntry());
                                DoCastSelf(SPELL_BIND_WILL, true);
                            }
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
        };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return GetBastionOfTwilightAI<npc_halfus_enslaved_dragonAI>(creature);
        }
};

class go_halfus_whelp_cage : public GameObjectScript
{
    public:
        go_halfus_whelp_cage() : GameObjectScript("go_halfus_whelp_cage") { }

        struct go_halfus_whelp_cageAI : public GameObjectAI
        {
            go_halfus_whelp_cageAI(GameObject* go) : GameObjectAI(go), _instance(go->GetInstanceScript()) { }

            bool OnReportUse(Player* /*player*/) override
            {
                me->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_NOT_SELECTABLE);
                _instance->SetData(DATA_OPEN_ORPHANED_EMERALD_WHELP_CAGE, DONE);
                return true;
            }

        private:
            InstanceScript* _instance;
        };

        GameObjectAI* GetAI(GameObject* go) const override
        {
            return GetBastionOfTwilightAI<go_halfus_whelp_cageAI>(go);
        }
};

class spell_halfus_bind_will : public SpellScriptLoader
{
    public:
        spell_halfus_bind_will() : SpellScriptLoader("spell_halfus_bind_will") { }

        class spell_halfus_bind_will_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_halfus_bind_will_SpellScript);

            void HandleHit(SpellEffIndex effIndex)
            {
                if (Unit* caster = GetCaster())
                    if (Unit* target = GetHitUnit())
                        target->CastSpell(caster, GetSpellInfo()->Effects[effIndex].BasePoints, true);
            }

            void Register() override
            {
                OnEffectHitTarget += SpellEffectFn(spell_halfus_bind_will_SpellScript::HandleHit, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_halfus_bind_will_SpellScript();
        }
};

class spell_halfus_fireball : public SpellScriptLoader
{
    public:
        spell_halfus_fireball() : SpellScriptLoader("spell_halfus_fireball") { }

        class spell_halfus_fireball_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_halfus_fireball_SpellScript);

            bool Validate(SpellInfo const* /*spellInfo*/) override
            {
                return ValidateSpellInfo(
                    {
                        SPELL_FIREBALL_BARRAGE_FAST,
                        SPELL_FIREBALL_BARRAGE_SLOW
                    });
            }

            void FilterTargets(std::list<WorldObject*>& targets)
            {
                if (targets.empty())
                    return;

                Trinity::Containers::RandomResize(targets, 1);
            }

            void HandleHit(SpellEffIndex effIndex)
            {
                if (Unit* caster = GetCaster())
                {
                    if (!caster->HasAura(SPELL_TIME_DILATION))
                        caster->CastSpell(GetHitUnit(), SPELL_FIREBALL_BARRAGE_FAST, true);
                    else
                        caster->CastSpell(GetHitUnit(), SPELL_FIREBALL_BARRAGE_SLOW, true);
                }
            }

            void Register() override
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_halfus_fireball_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ENEMY);
                OnEffectHitTarget += SpellEffectFn(spell_halfus_fireball_SpellScript::HandleHit, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_halfus_fireball_SpellScript();
        }
};

class spell_halfus_stone_touch : public SpellScriptLoader
{
    public:
        spell_halfus_stone_touch() : SpellScriptLoader("spell_halfus_stone_touch") { }

        class spell_halfus_stone_touch_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_halfus_stone_touch_AuraScript);

            bool Validate(SpellInfo const* /*spellInfo*/) override
            {
                return ValidateSpellInfo({ SPELL_PARALYSIS });
            }

            void HandlePeriodic(AuraEffect const* /*aurEff*/)
            {
                if (Unit* caster = GetTarget())
                    caster->CastSpell(caster, SPELL_PARALYSIS, true);
            }

            void Register() override
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_halfus_stone_touch_AuraScript::HandlePeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_halfus_stone_touch_AuraScript();
        }
};

class spell_halfus_cyclone_winds : public SpellScriptLoader
{
    public:
        spell_halfus_cyclone_winds() : SpellScriptLoader("spell_halfus_cyclone_winds") { }

        class spell_halfus_cyclone_winds_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_halfus_cyclone_winds_SpellScript);

            void SetDestPosition(SpellEffIndex /*effIndex*/)
            {
                if (Unit* caster = GetCaster())
                {
                    float angle = caster->GetOrientation();
                    float x = caster->GetPositionX() + cos(angle) * 7.0f;
                    float y = caster->GetPositionY() + sin(angle) * 7.0f;
                    float z = caster->GetPositionZ();

                    const_cast<WorldLocation*>(GetExplTargetDest())->Relocate(x, y, z);
                    GetHitDest()->Relocate(x, y, z);
                }
            }

            void Register()
            {
                OnEffectLaunch += SpellEffectFn(spell_halfus_cyclone_winds_SpellScript::SetDestPosition, EFFECT_0, SPELL_EFFECT_SUMMON);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_halfus_cyclone_winds_SpellScript();
        }
};

class DancingFlamesDistanceCheck
{
    public:
        DancingFlamesDistanceCheck(Unit* caster) : _caster(caster) { }

        bool operator()(WorldObject* object)
        {
            return (object->GetDistance2d(_caster) <= 40.0f || object->GetDistance2d(_caster) >= 100.0f);
        }
    private:
        Unit* _caster;
};

class spell_halfus_dancing_flames : public SpellScriptLoader
{
    public:
        spell_halfus_dancing_flames() : SpellScriptLoader("spell_halfus_dancing_flames") { }

        class spell_halfus_dancing_flames_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_halfus_dancing_flames_SpellScript);

            void FilterTargets(std::list<WorldObject*>& targets)
            {
                if (targets.empty())
                    return;

                if (Unit* caster = GetCaster())
                    targets.remove_if(DancingFlamesDistanceCheck(caster));

                if (targets.empty())
                    return;

                Trinity::Containers::RandomResize(targets, 1);
            }

            void Register() override
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_halfus_dancing_flames_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENTRY);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_halfus_dancing_flames_SpellScript();
        }
};

class achievement_the_only_escape : public AchievementCriteriaScript
{
    public:
        achievement_the_only_escape() : AchievementCriteriaScript("achievement_the_only_escape") { }

        bool OnCheck(Player* /*source*/, Unit* target)
        {
            if (target && target->IsAIEnabled)
                return (target->GetAI()->GetData(DATA_THE_ONLY_ESCAPE) == DONE);

            return false;
        }
};

void AddSC_boss_halfus_wyrmbreaker()
{
    new boss_halfus_wyrmbreaker();
    new npc_halfus_proto_behemoth();
    new npc_halfus_enslaved_dragon();
    new go_halfus_whelp_cage();
    new spell_halfus_bind_will();
    new spell_halfus_fireball();
    new spell_halfus_stone_touch();
    new spell_halfus_cyclone_winds();
    new spell_halfus_dancing_flames();
    new achievement_the_only_escape();
}
