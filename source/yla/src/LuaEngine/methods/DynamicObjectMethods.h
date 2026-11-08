/*
* Copyright (C) 2010 - 2025 Eluna Lua Engine <https://elunaluaengine.github.io/>
* This program is free software licensed under GPL version 3
* Please see the included DOCS/LICENSE.md for more information
*/

#ifndef DYNAMICOBJECTMETHODS_H
#define DYNAMICOBJECTMETHODS_H

/***
 * A dynamic object in the world, such as an area aura, a portal or a farsight focus.
 *
 * Inherits all methods from: [Object], [WorldObject]
 */
namespace LuaDynamicObject
{
    /**
     * Returns the spell ID that created the [DynamicObject].
     *
     * @return uint32 spellId
     */
    int GetSpellId(lua_State* L, DynamicObject* dynObj)
    {
        YLA::Push(L, dynObj->GetSpellId());
        return 1;
    }

    /**
     * Returns the [Unit] that cast the [DynamicObject].
     *
     * @return [Unit] caster
     */
    int GetCaster(lua_State* L, DynamicObject* dynObj)
    {
        YLA::Push(L, dynObj->GetCaster());
        return 1;
    }

    /**
     * Returns the radius of the [DynamicObject].
     *
     * @return float radius
     */
    int GetRadius(lua_State* L, DynamicObject* dynObj)
    {
        YLA::Push(L, dynObj->GetRadius());
        return 1;
    }

    /**
     * Returns the remaining duration of the [DynamicObject] in milliseconds.
     *
     * @return int32 duration
     */
    int GetDuration(lua_State* L, DynamicObject* dynObj)
    {
        YLA::Push(L, dynObj->GetDuration());
        return 1;
    }

    /**
     * Returns 'true' if the [DynamicObject] is a farsight focus.
     *
     * @return bool isViewpoint
     */
    int IsViewpoint(lua_State* L, DynamicObject* dynObj)
    {
        YLA::Push(L, dynObj->IsViewpoint());
        return 1;
    }
};
#endif
