/*
 * Copyright (C) 2008-2014 TrinityCore <http://www.trinitycore.org/>
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

#ifndef __BATTLEGROUNDTP_H
#define __BATTLEGROUNDTP_H

#include "Battleground.h"
#include "BattlegroundScore.h"

class BattlegroundTPScore final : public BattlegroundScore
{
    protected:
        BattlegroundTPScore(uint64 playerGuid, uint32 team) : BattlegroundScore(playerGuid, team), BasesAssaulted(0), BasesDefended(0) { }

        void UpdateScore(uint32 type, uint32 value) override
        {
            switch (type)
            {
                case SCORE_BASES_ASSAULTED:
                    BasesAssaulted += value;
                    break;
                case SCORE_BASES_DEFENDED:
                    BasesDefended += value;
                    break;
                default:
                    BattlegroundScore::UpdateScore(type, value);
                    break;
            }
        }

        uint32 BasesAssaulted;
        uint32 BasesDefended;
};

class BattlegroundTP : public Battleground
{
    public:
        BattlegroundTP();
        ~BattlegroundTP();
};

#endif
