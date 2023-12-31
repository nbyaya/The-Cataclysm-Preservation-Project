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

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "zulaman.h"

namespace ZulAman::Daakara
{
enum Texts
{
};

enum Spells
{
};

enum Events
{
};

struct boss_daakara : public BossAI
{
    boss_daakara(Creature* creature) : BossAI(creature, DATA_DAAKARA) { }

    void Reset() override
    {
        _Reset();
    }

    void JustEngagedWith(Unit* who) override
    {
        BossAI::JustEngagedWith(who);
    }

    void JustDied(Unit* /*killer*/) override
    {
        _JustDied();
    }

    void UpdateAI(uint32 diff) override
    {
        if (!UpdateVictim())
            return;

        events.Update(diff);

        if (me->HasUnitState(UNIT_STATE_CASTING))
            return;
        /*
        while (uint32 eventId = events.ExecuteEvent())
        {
            switch (eventId)
            {
                default:
                    break;
            }
        }
        */

        DoMeleeAttackIfReady();
    }
};
}

void AddSC_boss_daakara()
{
    using namespace ZulAman;
    using namespace ZulAman::Daakara;
    RegisterZulAamanCreatureAI(boss_daakara);
}
