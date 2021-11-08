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

#include "GameClient.h"
#include "MovementPacketSender.h"
#include "MovementStructures.h"
#include "Player.h"
#include "WorldPacket.h"
#include "WorldSession.h"

// SMSG_SPLINE_SET_* messages are for units controlled by the server.
// SMSG_MOVE_SET_*_SPEED messages are for units controlled by a player and require the client to respond with an ACK message.
// SMSG_MOVE_UPDATE_*_SPEED is used to broadcast changes to observers of units controlled by a player
OpcodeServer const MovementPacketSender::moveTypeToOpcode[MAX_MOVE_TYPE][3] =
{
    { SMSG_SPLINE_MOVE_SET_WALK_SPEED,        SMSG_MOVE_SET_WALK_SPEED,         SMSG_MOVE_UPDATE_WALK_SPEED         },
    { SMSG_SPLINE_MOVE_SET_RUN_SPEED,         SMSG_MOVE_SET_RUN_SPEED,          SMSG_MOVE_UPDATE_RUN_SPEED          },
    { SMSG_SPLINE_MOVE_SET_RUN_BACK_SPEED,    SMSG_MOVE_SET_RUN_BACK_SPEED,     SMSG_MOVE_UPDATE_RUN_BACK_SPEED     },
    { SMSG_SPLINE_MOVE_SET_SWIM_SPEED,        SMSG_MOVE_SET_SWIM_SPEED,         SMSG_MOVE_UPDATE_SWIM_SPEED         },
    { SMSG_SPLINE_MOVE_SET_SWIM_BACK_SPEED,   SMSG_MOVE_SET_SWIM_BACK_SPEED,    SMSG_MOVE_UPDATE_SWIM_BACK_SPEED    },
    { SMSG_SPLINE_MOVE_SET_TURN_RATE,         SMSG_MOVE_SET_TURN_RATE,          SMSG_MOVE_UPDATE_TURN_RATE          },
    { SMSG_SPLINE_MOVE_SET_FLIGHT_SPEED,      SMSG_MOVE_SET_FLIGHT_SPEED,       SMSG_MOVE_UPDATE_FLIGHT_SPEED       },
    { SMSG_SPLINE_MOVE_SET_FLIGHT_BACK_SPEED, SMSG_MOVE_SET_FLIGHT_BACK_SPEED,  SMSG_MOVE_UPDATE_FLIGHT_BACK_SPEED  },
    { SMSG_SPLINE_MOVE_SET_PITCH_RATE,        SMSG_MOVE_SET_PITCH_RATE,         SMSG_MOVE_UPDATE_PITCH_RATE         },
};

MovementChangeType MovementPacketSender::GetChangeTypeByMoveType(UnitMoveType moveType)
{
    switch (moveType)
    {
        case MOVE_WALK:         return MovementChangeType::SPEED_CHANGE_WALK;
        case MOVE_RUN:          return MovementChangeType::SPEED_CHANGE_RUN;
        case MOVE_RUN_BACK:     return MovementChangeType::SPEED_CHANGE_RUN_BACK;
        case MOVE_SWIM:         return MovementChangeType::SPEED_CHANGE_SWIM;
        case MOVE_SWIM_BACK:    return MovementChangeType::SPEED_CHANGE_SWIM_BACK;
        case MOVE_TURN_RATE:    return MovementChangeType::RATE_CHANGE_TURN;
        case MOVE_FLIGHT:       return MovementChangeType::SPEED_CHANGE_FLIGHT_SPEED;
        case MOVE_FLIGHT_BACK:  return MovementChangeType::SPEED_CHANGE_FLIGHT_BACK_SPEED;
        case MOVE_PITCH_RATE:   return MovementChangeType::RATE_CHANGE_PITCH;
        default:
            ASSERT(false, "MovementPacketSender::SendSpeedChangeToMover Unsupported UnitMoveType");
    }
}

void MovementPacketSender::SendSpeedChangeToMover(Unit* unit, UnitMoveType mtype, float newSpeedFlat)
{
    GameClient* controller = unit->GetGameClientMovingMe();

    ASSERT(controller);

    uint32 mCounter = unit->GetMovementCounterAndInc();
    PlayerMovementPendingChange pendingChange;
    pendingChange.movementCounter = mCounter;
    pendingChange.newValue = newSpeedFlat;
    pendingChange.movementChangeType = MovementPacketSender::GetChangeTypeByMoveType(mtype);
    unit->PushPendingMovementChange(pendingChange);

    WorldPacket data;
    data.Initialize(moveTypeToOpcode[mtype][1]);

    static MovementStatusElements const speedVal = MSEExtraFloat;
    Movement::ExtraMovementStatusElement extra(&speedVal);
    extra.Data.floatData = newSpeedFlat;

    unit->WriteMovementInfo(data, &extra, &mCounter);
    controller->SendDirectMessage(&data);
}

void MovementPacketSender::SendSpeedChangeToObservers(Unit* unit, UnitMoveType mtype, float newSpeedFlat)
{
    GameClient* controller = unit->GetGameClientMovingMe();

    ASSERT(controller);

    WorldPacket data;
    data.Initialize(moveTypeToOpcode[mtype][2]);
    static MovementStatusElements const speedVal = MSEExtraFloat;
    Movement::ExtraMovementStatusElement extra(&speedVal);
    extra.Data.floatData = newSpeedFlat;

    unit->WriteMovementInfo(data, &extra);
    unit->SendMessageToSet(&data, controller->GetBasePlayer());
}

void MovementPacketSender::SendSpeedChangeToAll(Unit* unit, UnitMoveType mtype, float newSpeedFlat)
{
    GameClient* controller = unit->GetGameClientMovingMe();

    ASSERT(!controller);

    WorldPacket data;
    data.Initialize(moveTypeToOpcode[mtype][0]);
    static MovementStatusElements const speedVal = MSEExtraFloat;
    Movement::ExtraMovementStatusElement extra(&speedVal);
    extra.Data.floatData = newSpeedFlat;

    unit->WriteMovementInfo(data, &extra);
    unit->SendMessageToSet(&data, true);
}

