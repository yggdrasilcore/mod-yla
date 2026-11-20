/*
 * Copyright (C) 2010 - 2025 Eluna Lua Engine <https://elunaluaengine.github.io/>
 * This program is free software licensed under GPL version 3
 * Please see the included DOCS/LICENSE.md for more information
 */

#include "Hooks.h"
#include "HookHelpers.h"
#include "LuaEngine.h"
#include "BindingMap.h"
#include "Transport.h"
#include "YLATemplate.h"

using namespace Hooks;

#define START_HOOK(EVENT)\
    if (!YLAConfig::GetInstance().IsYLAEnabled())\
        return;\
    LOCK_YLA_STATE;\
    if (!YLA::IsInitialized())\
        return;\
    auto key = EventKey<TransportEvents>(EVENT);\
    if ((!TransportEventBindings || !TransportEventBindings->HasBindingsFor(key)))\
        return;

void YLA::OnTransportCreate(Transport* transport)
{
    START_HOOK(TRANSPORT_EVENT_ON_CREATE);
    Push(transport);
    CallAllFunctions(TransportEventBindings, key);
}

void YLA::OnTransportDestroy(Transport* transport)
{
    START_HOOK(TRANSPORT_EVENT_ON_DESTROY);
    Push(transport);
    CallAllFunctions(TransportEventBindings, key);
}

void YLA::OnTransportUpdate(Transport* transport, uint32 diff)
{
    START_HOOK(TRANSPORT_EVENT_ON_UPDATE);
    Push(transport);
    Push(diff);
    CallAllFunctions(TransportEventBindings, key);
}

void YLA::OnTransportRelocate(Transport* transport, uint32 waypointId, uint32 mapId, float x, float y, float z)
{
    START_HOOK(TRANSPORT_EVENT_ON_RELOCATE);
    Push(transport);
    Push(waypointId);
    Push(mapId);
    Push(x);
    Push(y);
    Push(z);
    CallAllFunctions(TransportEventBindings, key);
}

void YLA::OnTransportAddPassenger(Transport* transport, WorldObject* passenger)
{
    START_HOOK(TRANSPORT_EVENT_ON_ADD_PASSENGER);
    Push(transport);
    Push(passenger);
    CallAllFunctions(TransportEventBindings, key);
}

void YLA::OnTransportRemovePassenger(Transport* transport, WorldObject* passenger)
{
    START_HOOK(TRANSPORT_EVENT_ON_REMOVE_PASSENGER);
    Push(transport);
    Push(passenger);
    CallAllFunctions(TransportEventBindings, key);
}
