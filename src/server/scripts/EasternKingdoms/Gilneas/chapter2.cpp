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

#include "gilneas.h"
#include "ScriptMgr.h"
#include "CombatAI.h"
#include "MotionMaster.h"
#include "ObjectAccessor.h"
#include "Player.h"
#include "PassiveAI.h"
#include "PhasingHandler.h"
#include "ScriptedCreature.h"
#include "SpellInfo.h"
#include "SpellScript.h"
#include "TemporarySummon.h"
#include "Vehicle.h"
#include "GameObjectAI.h"
#include "GameObject.h"

/*######
## Quest 14320 - In Need of Ingredients
######*/

enum GilneasInvasionCamera
{
    CINEMATIC_FORSAKEN_INVASION = 168
};

class go_gilneas_invasion_camera : public GameObjectScript
{
    public:
        go_gilneas_invasion_camera() : GameObjectScript("go_gilneas_invasion_camera") { }

        struct go_gilneas_invasion_cameraAI : public GameObjectAI
        {
            go_gilneas_invasion_cameraAI(GameObject* go) : GameObjectAI(go) { }

            bool OnReportUse(Player* player) override
            {
                player->SendCinematicStart(CINEMATIC_FORSAKEN_INVASION);
                return true;
            }
        };

        GameObjectAI* GetAI(GameObject* go) const override
        {
            return new go_gilneas_invasion_cameraAI(go);
        }
};

void AddSC_gilneas_c2()
{
    new go_gilneas_invasion_camera();
}
