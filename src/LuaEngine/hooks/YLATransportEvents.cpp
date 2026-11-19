/*
 * Copyright (C) 2010 - 2025 Eluna Lua Engine <https://elunaluaengine.github.io/>
 * This program is free software licensed under GPL version 3
 * Please see the included DOCS/LICENSE.md for more information
 */

#include "YLATransportEvents.h"
#include "Creature.h"
#include "LuaEngine.h"
#include "Player.h"
#include "Transport.h"

void YLABroadcastTransportPassengerAdded(Transport* transport, WorldObject* passenger)
{
    YLA::GetMapStateOrGlobal(transport->GetMapId(), transport->GetInstanceId())->OnTransportAddPassenger(transport, passenger);

    if (Player* player = passenger->ToPlayer())
        YLA::GetMapStateOrGlobal(player->GetMapId(), player->GetInstanceId())->OnPlayerEnterTransport(player, transport);
}

void YLABroadcastTransportCreaturePassengerAdded(Transport* transport, Creature* creature)
{
    YLA::GetMapStateOrGlobal(transport->GetMapId(), transport->GetInstanceId())->OnTransportAddCreaturePassenger(transport, creature);
    YLABroadcastTransportPassengerAdded(transport, creature);
}

void YLABroadcastTransportPassengerRemoved(Transport* transport, WorldObject* passenger)
{
    YLA::GetMapStateOrGlobal(transport->GetMapId(), transport->GetInstanceId())->OnTransportRemovePassenger(transport, passenger);

    if (Player* player = passenger->ToPlayer())
        YLA::GetMapStateOrGlobal(player->GetMapId(), player->GetInstanceId())->OnPlayerLeaveTransport(player, transport);
}
