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

#ifndef TRINITY_COMMONPREDICATES_H
#define TRINITY_COMMONPREDICATES_H

#include "Define.h"
#include <utility>

class WorldObject;
class Unit;

namespace Trinity
{
    namespace Predicates
    {
        /// Returns true when the container's element is the given attacker's current victim
        class TC_GAME_API IsVictimOf
        {
            public:
                IsVictimOf(Unit const* attacker);

                bool operator()(WorldObject const* obj) const
                {
                    return _victim == obj;
                }
            private:
                Unit const* _victim;
        };

        template <typename PRED>
        class TC_GAME_API Inverter
        {
            public:
                Inverter(PRED&& p) : _child(std::move(p)) { }

                template <typename... Args>
                bool operator()(Args&&... args)
                {
                    return !_child(std::forward<Args>(args)...);
                }

                template <typename... Args>
                bool operator()(Args&&... args) const
                {
                    return !_child(std::forward<Args>(args)...);
                }

            private:
                PRED _child;
        };

        template <typename PRED>
        /// Inverts the given predicate to return the opposite result
        Inverter<PRED> Invert(PRED&& p) { return Inverter<PRED>(std::forward<PRED>(p)); }
    }
}

#endif //TRINITY_COMMONPREDICATES_H
