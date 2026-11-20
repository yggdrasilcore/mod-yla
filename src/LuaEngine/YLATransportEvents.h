/*
 * Copyright (C) 2010 - 2025 Eluna Lua Engine <https://elunaluaengine.github.io/>
 * This program is free software licensed under GPL version 3
 * Please see the included DOCS/LICENSE.md for more information
 */

#ifndef _YLA_TRANSPORT_EVENTS_H
#define _YLA_TRANSPORT_EVENTS_H

class Transport;
class WorldObject;
class Creature;

// Called by YLA_TransportScript in YLA_SC.cpp when the core reports transport activity.
void YLABroadcastTransportPassengerAdded(Transport* transport, WorldObject* passenger);
void YLABroadcastTransportCreaturePassengerAdded(Transport* transport, Creature* creature);
void YLABroadcastTransportPassengerRemoved(Transport* transport, WorldObject* passenger);
void YLABroadcastTransportCreaturePassengerRemoved(Transport* transport, Creature* creature);

#endif
