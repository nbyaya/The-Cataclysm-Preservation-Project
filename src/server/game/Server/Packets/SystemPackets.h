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

#ifndef SystemPackets_h__
#define SystemPackets_h__

#include "Packet.h"
#include "ObjectGuid.h"
#include "Optional.h"

namespace WorldPackets
{
    namespace System
    {
            struct SavedThrottleObjectState
            {
                uint32 MaxTries               = 0;
                uint32 PerMilliseconds        = 0;
                uint32 TryCount               = 0;
                uint32 LastResetTimeBeforeNow = 0;
            };

            struct SessionAlertConfig
            {
                int32 Delay       = 0;
                int32 Period      = 0;
                int32 DisplayTime = 0;
            };

        class FeatureSystemStatus final : public ServerPacket
        {
        public:
            FeatureSystemStatus() : ServerPacket(SMSG_FEATURE_SYSTEM_STATUS, 7) { }

            WorldPacket const* Write() override;

            bool VoiceEnabled = false;
            Optional<SessionAlertConfig> SessionAlert;
            uint32 ScrollOfResurrectionMaxRequestsPerDay = 0;
            bool ScrollOfResurrectionEnabled = false;
            Optional<SavedThrottleObjectState> EuropaTicketSystemStatus;
            uint32 ScrollOfResurrectionRequestsRemaining = 0;
            uint32 CfgRealmID = 0;
            uint8 ComplaintStatus = 0;
            int32 CfgRealmRecID = 0;
            bool ItemRestorationButtonEnabled = false;
            bool TravelPassEnabled = false;
        };
    }
}

#endif // SystemPackets_h__
