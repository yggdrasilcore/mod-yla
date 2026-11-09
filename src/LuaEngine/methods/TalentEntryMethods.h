/*
* Copyright (C) 2010 - 2025 Eluna Lua Engine <https://elunaluaengine.github.io/>
* This program is free software licensed under GPL version 3
* Please see the included DOCS/LICENSE.md for more information
*/

#ifndef TALENTENTRYMETHODS_H
#define TALENTENTRYMETHODS_H

/***
 * Represents a talent entry loaded from `Talent.dbc`.
 *
 * Inherits all methods from: none
 */
namespace LuaTalentEntry
{
    /**
     * Returns the ID of the [TalentEntry].
     *
     * @return uint32 id
     */
    int GetId(lua_State* L, TalentEntry* entry)
    {
        YLA::Push(L, entry->TalentID);
        return 1;
    }

    /**
     * Returns the ID of the talent tab this talent belongs to.
     *
     * @return uint32 talentTabId
     */
    int GetTab(lua_State* L, TalentEntry* entry)
    {
        YLA::Push(L, entry->TalentTab);
        return 1;
    }

    /**
     * Returns the row of the talent within its tab.
     *
     * @return uint32 row
     */
    int GetRow(lua_State* L, TalentEntry* entry)
    {
        YLA::Push(L, entry->Row);
        return 1;
    }

    /**
     * Returns the column of the talent within its tab.
     *
     * @return uint32 column
     */
    int GetCol(lua_State* L, TalentEntry* entry)
    {
        YLA::Push(L, entry->Col);
        return 1;
    }

    /**
     * Returns the spell ID for a rank of this talent.
     *
     * @param uint32 rank : 0-based rank index
     * @return uint32 spellId
     */
    int GetRank(lua_State* L, TalentEntry* entry)
    {
        uint32 rank = YLA::CHECKVAL<uint32>(L, 2);
        if (rank >= MAX_TALENT_RANK)
            return 0;

        YLA::Push(L, entry->RankID[rank]);
        return 1;
    }

    /**
     * Returns the spell IDs for all ranks of this talent.
     *
     * @return table rankSpellIds : array of spell IDs, indexed from 1
     */
    int GetRanks(lua_State* L, TalentEntry* entry)
    {
        lua_createtable(L, MAX_TALENT_RANK, 0);
        for (uint32 i = 0; i < MAX_TALENT_RANK; ++i)
        {
            YLA::Push(L, entry->RankID[i]);
            lua_rawseti(L, -2, i + 1);
        }
        return 1;
    }

    /**
     * Returns the ID of the talent that must be learned first.
     *
     * @return uint32 dependsOnTalentId
     */
    int GetDependsOn(lua_State* L, TalentEntry* entry)
    {
        YLA::Push(L, entry->DependsOn);
        return 1;
    }

    /**
     * Returns the rank required in the prerequisite talent.
     *
     * @return uint32 dependsOnRank
     */
    int GetDependsOnRank(lua_State* L, TalentEntry* entry)
    {
        YLA::Push(L, entry->DependsOnRank);
        return 1;
    }

    /**
     * Returns whether the talent is added to the spell book.
     *
     * @return uint32 addToSpellBook
     */
    int GetAddToSpellBook(lua_State* L, TalentEntry* entry)
    {
        YLA::Push(L, entry->addToSpellBook);
        return 1;
    }
}
#endif
