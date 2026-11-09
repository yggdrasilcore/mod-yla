/*
* Copyright (C) 2010 - 2025 Eluna Lua Engine <https://elunaluaengine.github.io/>
* This program is free software licensed under GPL version 3
* Please see the included DOCS/LICENSE.md for more information
*/

#ifndef TALENTTABENTRYMETHODS_H
#define TALENTTABENTRYMETHODS_H

/***
 * Represents a talent tab entry loaded from `TalentTab.dbc`.
 *
 * Inherits all methods from: none
 */
namespace LuaTalentTabEntry
{
    /**
     * Returns the ID of the [TalentTabEntry].
     *
     * @return uint32 id
     */
    int GetId(lua_State* L, TalentTabEntry* entry)
    {
        YLA::Push(L, entry->TalentTabID);
        return 1;
    }

    /**
     * Returns the class mask of the talent tab.
     *
     * @return uint32 classMask
     */
    int GetClassMask(lua_State* L, TalentTabEntry* entry)
    {
        YLA::Push(L, entry->ClassMask);
        return 1;
    }

    /**
     * Returns the pet talent mask of the talent tab.
     *
     * @return uint32 petTalentMask
     */
    int GetPetTalentMask(lua_State* L, TalentTabEntry* entry)
    {
        YLA::Push(L, entry->petTalentMask);
        return 1;
    }

    /**
     * Returns the page index of the talent tab.
     *
     * @return uint32 tabPage
     */
    int GetTabPage(lua_State* L, TalentTabEntry* entry)
    {
        YLA::Push(L, entry->tabpage);
        return 1;
    }
}
#endif
