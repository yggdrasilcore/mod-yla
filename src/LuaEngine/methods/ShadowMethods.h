/*
 * Copyright (C) 2010 - 2026 YLA Lua Engine <https://github.com/YggdrasilWotLK/mod-yla>
 * This program is free software licensed under GPL version 3.
 * Please see the included LICENSE file for more information.
 *
 * Shadow (mod-shadows bot) bindings for YLA.
 *
 * A `Shadow` is a sub-class of `Player` in Lua: every Player/Object/
 * WorldObject/Unit method is available on it, plus the bot-specific methods
 * below. Use `Player:ToShadow()` to view a bot Player as a Shadow, and
 * `Shadow:ToPlayer()` to go back.
 *
 * All bot-specific methods degrade gracefully when mod-shadows is not
 * compiled in (MOD_SHADOWS undefined): predicates return false, getters
 * return nil, and commands return empty tables.
 */

#ifndef SHADOWMETHODS_H
#define SHADOWMETHODS_H

#include "ShadowPlayer.h"
#include "YlaDefer.h"

#ifdef MOD_SHADOWS
#include "Chat.h"
#include "ObjectAccessor.h"
#include "ShadowAI.h"
#include "ShadowMgr.h"
#endif

/***
 * Inherits all methods from: [Object], [WorldObject], [Unit], [Player]
 */
namespace LuaShadow
{
    /**
     * Returns the [Shadow] as a [Player].
     *
     * @return [Player] player
     */
    inline int ToPlayer(lua_State* L, ShadowPlayer* shadow)
    {
        if (!shadow || !ShadowPlayer::IsLive(shadow))
        {
            YLA::Push(L, (Player*)nullptr);
            return 1;
        }
        YLA::Push(L, ShadowPlayer::ToPlayer(shadow));
        return 1;
    }

    /**
     * Returns the master [Player] of the [Shadow] bot, or nil if it has none
     * (random bot, offline master, or mod-shadows unavailable).
     *
     * @return [Player] master
     */
    inline int GetMaster(lua_State* L, ShadowPlayer* shadow)
    {
#ifdef MOD_SHADOWS
        if (!shadow || !ShadowPlayer::IsLive(shadow))
        {
            YLA::Push(L, (Player*)nullptr);
            return 1;
        }
        Player* bot = ShadowPlayer::ToPlayer(shadow);
        if (auto ai = sShadowsMgr->GetShadowAI(bot))
        {
            // GetMaster() revalidates the stored pointer against the registry.
            YLA::Push(L, ai->GetMaster());
            return 1;
        }
        YLA::Push(L, (Player*)nullptr);
        return 1;
#else
        (void)shadow;
        YLA::Push(L, (Player*)nullptr);
        return 1;
#endif
    }

    /**
     * Returns `true` if the [Shadow] currently has a live bot AI, `false` otherwise.
     *
     * @return bool hasAI
     */
    inline int HasAI(lua_State* L, ShadowPlayer* shadow)
    {
#ifdef MOD_SHADOWS
        if (!shadow || !ShadowPlayer::IsLive(shadow))
        {
            YLA::Push(L, false);
            return 1;
        }
        YLA::Push(L, sShadowsMgr->GetShadowAI(ShadowPlayer::ToPlayer(shadow)) != nullptr);
        return 1;
#else
        (void)shadow;
        YLA::Push(L, false);
        return 1;
#endif
    }

    /**
     * Returns `true` if the [Shadow] is a real player (master == bot), `false` otherwise.
     *
     * @return bool isRealPlayer
     */
    inline int IsRealPlayer(lua_State* L, ShadowPlayer* shadow)
    {
#ifdef MOD_SHADOWS
        if (!shadow || !ShadowPlayer::IsLive(shadow))
        {
            YLA::Push(L, false);
            return 1;
        }
        if (auto ai = sShadowsMgr->GetShadowAI(ShadowPlayer::ToPlayer(shadow)))
        {
            YLA::Push(L, ai->IsRealPlayer());
            return 1;
        }
        YLA::Push(L, false);
        return 1;
#else
        (void)shadow;
        YLA::Push(L, false);
        return 1;
#endif
    }

    /**
     * Returns `true` if the [Shadow] has a real-player master, `false` otherwise.
     *
     * @return bool hasMaster
     */
    inline int HasRealPlayerMaster(lua_State* L, ShadowPlayer* shadow)
    {
#ifdef MOD_SHADOWS
        if (!shadow || !ShadowPlayer::IsLive(shadow))
        {
            YLA::Push(L, false);
            return 1;
        }
        if (auto ai = sShadowsMgr->GetShadowAI(ShadowPlayer::ToPlayer(shadow)))
        {
            YLA::Push(L, ai->HasRealPlayerMaster());
            return 1;
        }
        YLA::Push(L, false);
        return 1;
#else
        (void)shadow;
        YLA::Push(L, false);
        return 1;
#endif
    }

    /**
     * Returns `true` if the [Shadow] has an actively-playing master, `false` otherwise.
     *
     * @return bool hasActiveMaster
     */
    inline int HasActivePlayerMaster(lua_State* L, ShadowPlayer* shadow)
    {
#ifdef MOD_SHADOWS
        if (!shadow || !ShadowPlayer::IsLive(shadow))
        {
            YLA::Push(L, false);
            return 1;
        }
        if (auto ai = sShadowsMgr->GetShadowAI(ShadowPlayer::ToPlayer(shadow)))
        {
            YLA::Push(L, ai->HasActivePlayerMaster());
            return 1;
        }
        YLA::Push(L, false);
        return 1;
#else
        (void)shadow;
        YLA::Push(L, false);
        return 1;
#endif
    }

    /**
     * Returns `true` if the [Shadow] is summoned as an alt of a player, `false` otherwise.
     *
     * @return bool isAlt
     */
    inline int IsAlt(lua_State* L, ShadowPlayer* shadow)
    {
#ifdef MOD_SHADOWS
        if (!shadow || !ShadowPlayer::IsLive(shadow))
        {
            YLA::Push(L, false);
            return 1;
        }
        if (auto ai = sShadowsMgr->GetShadowAI(ShadowPlayer::ToPlayer(shadow)))
        {
            YLA::Push(L, ai->IsAlt());
            return 1;
        }
        YLA::Push(L, false);
        return 1;
#else
        (void)shadow;
        YLA::Push(L, false);
        return 1;
#endif
    }

    /**
     * Returns the bot AI state: 0 = combat, 1 = non-combat, 2 = dead, or -1 when unavailable.
     *
     * @return int state
     */
    inline int GetState(lua_State* L, ShadowPlayer* shadow)
    {
#ifdef MOD_SHADOWS
        if (!shadow || !ShadowPlayer::IsLive(shadow))
        {
            YLA::Push(L, -1);
            return 1;
        }
        if (auto ai = sShadowsMgr->GetShadowAI(ShadowPlayer::ToPlayer(shadow)))
        {
            YLA::Push(L, static_cast<int>(ai->GetState()));
            return 1;
        }
        YLA::Push(L, -1);
        return 1;
#else
        (void)shadow;
        YLA::Push(L, -1);
        return 1;
#endif
    }

    /**
     * Sends a chat command to the [Shadow] bot AI (e.g. "follow", "attack", "stay").
     * The sender defaults to the bot's master and falls back to the bot itself.
     * Named SendCommand (not BotCommand) so it never collides with the
     * master-side [Player]:BotCommand(".bot" manager command).
     *
     * @param string command : bot chat command text
     * @param [Player] from = nil : optional sender override
     */
    inline int SendCommand(lua_State* L, ShadowPlayer* shadow)
    {
#ifdef MOD_SHADOWS
        std::string cmd = YLA::CHECKVAL<std::string>(L, 2);
        if (!shadow || !ShadowPlayer::IsLive(shadow))
            return 0;
        Player* bot = ShadowPlayer::ToPlayer(shadow);
        auto ai = sShadowsMgr->GetShadowAI(bot);
        if (!ai)
            return 0;
        Player* from = YLA::CHECKOBJ<Player>(L, 3, false);
        if (!from)
            from = ai->GetMaster();
        if (!from)
            from = bot;
        ai->HandleCommand(CHAT_MSG_WHISPER, cmd, from);
        return 0;
#else
        (void)shadow;
        return 0;
#endif
    }

    /**
     * Returns `true` if the [Shadow] bot AI currently uses the named strategy.
     *
     * @param string name : strategy name
     * @param int state = 1 : 0 = combat, 1 = non-combat, 2 = dead
     * @return bool hasStrategy
     */
    inline int HasStrategy(lua_State* L, ShadowPlayer* shadow)
    {
#ifdef MOD_SHADOWS
        std::string name = YLA::CHECKVAL<std::string>(L, 2);
        int state = YLA::CHECKVAL<int>(L, 3, 1);
        if (!shadow || !ShadowPlayer::IsLive(shadow))
        {
            YLA::Push(L, false);
            return 1;
        }
        if (auto ai = sShadowsMgr->GetShadowAI(ShadowPlayer::ToPlayer(shadow)))
        {
            YLA::Push(L, ai->HasStrategy(name, static_cast<BotState>(state)));
            return 1;
        }
        YLA::Push(L, false);
        return 1;
#else
        (void)shadow;
        YLA::Push(L, false);
        return 1;
#endif
    }

    /**
     * Returns a table of strategy names active on the [Shadow] for the given state.
     *
     * @param int state = 1 : 0 = combat, 1 = non-combat, 2 = dead
     * @return table strategies
     */
    inline int GetStrategies(lua_State* L, ShadowPlayer* shadow)
    {
#ifdef MOD_SHADOWS
        int state = YLA::CHECKVAL<int>(L, 2, 1);
        lua_createtable(L, 0, 0);
        int tbl = lua_gettop(L);
        uint32 i = 0;
        if (shadow && ShadowPlayer::IsLive(shadow))
        {
            if (auto ai = sShadowsMgr->GetShadowAI(ShadowPlayer::ToPlayer(shadow)))
            {
                for (std::string const& name : ai->GetStrategies(static_cast<BotState>(state)))
                {
                    YLA::Push(L, name);
                    lua_rawseti(L, tbl, ++i);
                }
            }
        }
        lua_settop(L, tbl);
        return 1;
#else
        (void)shadow;
        lua_createtable(L, 0, 0);
        return 1;
#endif
    }

    /**
     * Triggers a named bot action on the [Shadow] (e.g. "follow", "flee", "eat").
     *
     * @param string action : action name
     * @return bool executed
     */
    inline int DoAction(lua_State* L, ShadowPlayer* shadow)
    {
#ifdef MOD_SHADOWS
        std::string action = YLA::CHECKVAL<std::string>(L, 2);
        if (!shadow || !ShadowPlayer::IsLive(shadow))
        {
            YLA::Push(L, false);
            return 1;
        }
        if (auto ai = sShadowsMgr->GetShadowAI(ShadowPlayer::ToPlayer(shadow)))
        {
            YLA::Push(L, ai->DoSpecificAction(action));
            return 1;
        }
        YLA::Push(L, false);
        return 1;
#else
        (void)shadow;
        YLA::Push(L, false);
        return 1;
#endif
    }

    /**
     * Enables a spell for the [Shadow] by removing it from the bot's
     * ignored-spell list (the same list used by the `ss` chat command).
     *
     * @param uint32 spellId : spell ID
     * @return bool changed
     */
    inline int EnableSpell(lua_State* L, ShadowPlayer* shadow)
    {
#ifdef MOD_SHADOWS
        uint32 spellId = YLA::CHECKVAL<uint32>(L, 2);
        if (!shadow || !ShadowPlayer::IsLive(shadow))
        {
            YLA::Push(L, false);
            return 1;
        }
        if (auto ai = sShadowsMgr->GetShadowAI(ShadowPlayer::ToPlayer(shadow)))
        {
            YLA::Push(L, ai->SetSpellEnabled(spellId, true));
            return 1;
        }
        YLA::Push(L, false);
        return 1;
#else
        (void)shadow;
        YLA::Push(L, false);
        return 1;
#endif
    }

    /**
     * Disables a spell for the [Shadow] by adding it to the bot's
     * ignored-spell list (the same list used by the `ss` chat command).
     *
     * @param uint32 spellId : spell ID
     * @return bool changed
     */
    inline int DisableSpell(lua_State* L, ShadowPlayer* shadow)
    {
#ifdef MOD_SHADOWS
        uint32 spellId = YLA::CHECKVAL<uint32>(L, 2);
        if (!shadow || !ShadowPlayer::IsLive(shadow))
        {
            YLA::Push(L, false);
            return 1;
        }
        if (auto ai = sShadowsMgr->GetShadowAI(ShadowPlayer::ToPlayer(shadow)))
        {
            YLA::Push(L, ai->SetSpellEnabled(spellId, false));
            return 1;
        }
        YLA::Push(L, false);
        return 1;
#else
        (void)shadow;
        YLA::Push(L, false);
        return 1;
#endif
    }

    /**
     * Returns `true` if the [Shadow] is allowed to cast the spell, `false`
     * otherwise (including when the bot/AI is unavailable).
     *
     * @param uint32 spellId : spell ID
     * @return bool enabled
     */
    inline int IsSpellEnabled(lua_State* L, ShadowPlayer* shadow)
    {
#ifdef MOD_SHADOWS
        uint32 spellId = YLA::CHECKVAL<uint32>(L, 2);
        if (!shadow || !ShadowPlayer::IsLive(shadow))
        {
            YLA::Push(L, false);
            return 1;
        }
        if (auto ai = sShadowsMgr->GetShadowAI(ShadowPlayer::ToPlayer(shadow)))
        {
            YLA::Push(L, ai->IsSpellEnabled(spellId));
            return 1;
        }
        YLA::Push(L, false);
        return 1;
#else
        (void)shadow;
        YLA::Push(L, false);
        return 1;
#endif
    }

    /**
     * Returns the strategies active on the [Shadow] for every state.
     * The result is keyed by state: 0 = combat, 1 = non-combat, 2 = dead.
     *
     * @return table strategiesByState
     */
    inline int GetAllStrategies(lua_State* L, ShadowPlayer* shadow)
    {
#ifdef MOD_SHADOWS
        lua_createtable(L, BOT_STATE_MAX, 0);
        int tbl = lua_gettop(L);
        if (shadow && ShadowPlayer::IsLive(shadow))
        {
            if (auto ai = sShadowsMgr->GetShadowAI(ShadowPlayer::ToPlayer(shadow)))
            {
                for (int state = 0; state < BOT_STATE_MAX; ++state)
                {
                    lua_createtable(L, 0, 0);
                    int list = lua_gettop(L);
                    uint32 i = 0;
                    for (std::string const& name : ai->GetStrategies(static_cast<BotState>(state)))
                    {
                        YLA::Push(L, name);
                        lua_rawseti(L, list, ++i);
                    }
                    lua_rawseti(L, tbl, state);
                }
            }
        }
        lua_settop(L, tbl);
        return 1;
#else
        (void)shadow;
        lua_createtable(L, 0, 0);
        return 1;
#endif
    }

    /**
     * Changes the combat strategies of the [Shadow], using the same syntax as
     * the `co` command: `+name` adds, `-name` removes and `~name` replaces the
     * currently active strategies. Multiple entries can be comma-separated.
     *
     * @param string strategies : strategy change text
     * @return bool changed
     */
    inline int ToggleCombatStrategy(lua_State* L, ShadowPlayer* shadow)
    {
#ifdef MOD_SHADOWS
        std::string strategies = YLA::CHECKVAL<std::string>(L, 2);
        if (!shadow || !ShadowPlayer::IsLive(shadow))
        {
            YLA::Push(L, false);
            return 1;
        }
        if (auto ai = sShadowsMgr->GetShadowAI(ShadowPlayer::ToPlayer(shadow)))
        {
            ai->ChangeStrategy(strategies, BOT_STATE_COMBAT);
            YLA::Push(L, true);
            return 1;
        }
        YLA::Push(L, false);
        return 1;
#else
        (void)shadow;
        YLA::Push(L, false);
        return 1;
#endif
    }

    /**
     * Changes the non-combat strategies of the [Shadow], using the same syntax
     * as the `nc` command: `+name` adds, `-name` removes and `~name` replaces
     * the currently active strategies. Multiple entries can be comma-separated.
     *
     * @param string strategies : strategy change text
     * @return bool changed
     */
    inline int ToggleNonCombatStrategy(lua_State* L, ShadowPlayer* shadow)
    {
#ifdef MOD_SHADOWS
        std::string strategies = YLA::CHECKVAL<std::string>(L, 2);
        if (!shadow || !ShadowPlayer::IsLive(shadow))
        {
            YLA::Push(L, false);
            return 1;
        }
        if (auto ai = sShadowsMgr->GetShadowAI(ShadowPlayer::ToPlayer(shadow)))
        {
            ai->ChangeStrategy(strategies, BOT_STATE_NON_COMBAT);
            YLA::Push(L, true);
            return 1;
        }
        YLA::Push(L, false);
        return 1;
#else
        (void)shadow;
        YLA::Push(L, false);
        return 1;
#endif
    }

    /**
     * Changes the dead strategies of the [Shadow], using the same syntax as the
     * `co` / `nc` commands: `+name` adds, `-name` removes and `~name` replaces
     * the currently active strategies. Multiple entries can be comma-separated.
     *
     * @param string strategies : strategy change text
     * @return bool changed
     */
    inline int ToggleDeadStrategy(lua_State* L, ShadowPlayer* shadow)
    {
#ifdef MOD_SHADOWS
        std::string strategies = YLA::CHECKVAL<std::string>(L, 2);
        if (!shadow || !ShadowPlayer::IsLive(shadow))
        {
            YLA::Push(L, false);
            return 1;
        }
        if (auto ai = sShadowsMgr->GetShadowAI(ShadowPlayer::ToPlayer(shadow)))
        {
            ai->ChangeStrategy(strategies, BOT_STATE_DEAD);
            YLA::Push(L, true);
            return 1;
        }
        YLA::Push(L, false);
        return 1;
#else
        (void)shadow;
        YLA::Push(L, false);
        return 1;
#endif
    }

    /**
     * Suppresses all mod-shadows movement handling for the given duration so
     * Lua can drive the bot's movement completely. Any movement currently in
     * progress is stopped. Passing 0 clears the suppression immediately.
     *
     * @param uint32 durationMs : duration in milliseconds (0 = clear)
     * @return bool applied
     */
    inline int SuppressMovement(lua_State* L, ShadowPlayer* shadow)
    {
#ifdef MOD_SHADOWS
        uint32 durationMs = YLA::CHECKVAL<uint32>(L, 2);
        if (!shadow || !ShadowPlayer::IsLive(shadow))
        {
            YLA::Push(L, false);
            return 1;
        }
        if (auto ai = sShadowsMgr->GetShadowAI(ShadowPlayer::ToPlayer(shadow)))
        {
            ai->SuppressMovement(durationMs);
            YLA::Push(L, true);
            return 1;
        }
        YLA::Push(L, false);
        return 1;
#else
        (void)shadow;
        YLA::Push(L, false);
        return 1;
#endif
    }

    /**
     * Clears any active movement suppression for the [Shadow] immediately.
     *
     * @return bool cleared
     */
    inline int ClearMovementSuppression(lua_State* L, ShadowPlayer* shadow)
    {
#ifdef MOD_SHADOWS
        if (!shadow || !ShadowPlayer::IsLive(shadow))
        {
            YLA::Push(L, false);
            return 1;
        }
        if (auto ai = sShadowsMgr->GetShadowAI(ShadowPlayer::ToPlayer(shadow)))
        {
            ai->ClearMovementSuppression();
            YLA::Push(L, true);
            return 1;
        }
        YLA::Push(L, false);
        return 1;
#else
        (void)shadow;
        YLA::Push(L, false);
        return 1;
#endif
    }

    /**
     * Returns `true` while mod-shadows movement handling is suppressed for the
     * [Shadow], `false` otherwise.
     *
     * @return bool suppressed
     */
    inline int IsMovementSuppressed(lua_State* L, ShadowPlayer* shadow)
    {
#ifdef MOD_SHADOWS
        if (!shadow || !ShadowPlayer::IsLive(shadow))
        {
            YLA::Push(L, false);
            return 1;
        }
        if (auto ai = sShadowsMgr->GetShadowAI(ShadowPlayer::ToPlayer(shadow)))
        {
            YLA::Push(L, ai->IsMovementSuppressed());
            return 1;
        }
        YLA::Push(L, false);
        return 1;
#else
        (void)shadow;
        YLA::Push(L, false);
        return 1;
#endif
    }
}

/***
 * Master-side bot helpers registered onto [Player].
 */
namespace LuaPlayerShadow
{
    /**
     * Views this [Player] as a [Shadow] bot. Returns nil when the player is not
     * a bot (or mod-shadows is unavailable).
     *
     * @return [Shadow] shadow
     */
    inline int ToShadow(lua_State* L, Player* player)
    {
        if (!player)
        {
            YLA::Push(L, (ShadowPlayer*)nullptr);
            return 1;
        }
#ifdef MOD_SHADOWS
        WorldSession* session = player->GetSession();
        bool isBot = session && session->IsBot();
        if (!isBot)
            isBot = sShadowsMgr->GetShadowAI(player) != nullptr;
        if (!isBot)
        {
            YLA::Push(L, (ShadowPlayer*)nullptr);
            return 1;
        }
        YLA::Push(L, ShadowPlayer::FromPlayer(player));
        return 1;
#else
        (void)player;
        YLA::Push(L, (ShadowPlayer*)nullptr);
        return 1;
#endif
    }

    /**
     * Returns a table of [Shadow] bots owned by this [Player].
     *
     * Snapshot under lock: safe from any state/thread. Pushed objects
     * are liveness-checked on use as usual.
     *
     * @return table bots
     */
    inline int GetBots(lua_State* L, Player* player)
    {
        lua_createtable(L, 0, 0);
        int tbl = lua_gettop(L);
#ifdef MOD_SHADOWS
        if (player)
        {
            if (auto mgr = sShadowsMgr->GetShadowMgr(player))
            {
                std::vector<Player*> bots = mgr->GetShadowsSnapshot();
                uint32 i = 0;
                for (Player* bot : bots)
                {
                    if (!bot)
                        continue;
                    YLA::Push(L, ShadowPlayer::FromPlayer(bot));
                    // Push() leaves nil for null; skip it to keep a dense list.
                    if (lua_isnil(L, -1))
                    {
                        lua_pop(L, 1);
                        continue;
                    }
                    lua_rawseti(L, tbl, ++i);
                }
            }
        }
#else
        (void)player;
#endif
        lua_settop(L, tbl);
        return 1;
    }

    /**
     * Returns the number of [Shadow] bots owned by this [Player].
     *
     * @return uint32 count
     */
    inline int GetBotsCount(lua_State* L, Player* player)
    {
#ifdef MOD_SHADOWS
        if (player)
        {
            if (auto mgr = sShadowsMgr->GetShadowMgr(player))
            {
                YLA::Push(L, mgr->GetShadowsCount());
                return 1;
            }
        }
        YLA::Push(L, (uint32)0);
        return 1;
#else
        (void)player;
        YLA::Push(L, (uint32)0);
        return 1;
#endif
    }

    /**
     * Runs a `.bot` shadows command as this [Player] (e.g. "list", "add Name",
     * "remove Name") and returns each output line as a table entry.
     *
     * @param string command
     * @return table output
     */
    inline int BotCommand(lua_State* L, Player* player)
    {
        lua_createtable(L, 0, 0);
        int tbl = lua_gettop(L);
#ifdef MOD_SHADOWS
        std::string cmd = YLA::CHECKVAL<std::string>(L, 2);
        if (player)
        {
            if (auto mgr = sShadowsMgr->GetShadowMgr(player))
            {
                std::vector<std::string> out = mgr->HandleShadowCommand(cmd.c_str(), player);
                uint32 i = 0;
                for (std::string const& line : out)
                {
                    YLA::Push(L, line);
                    lua_rawseti(L, tbl, ++i);
                }
            }
        }
#else
        (void)player;
#endif
        lua_settop(L, tbl);
        return 1;
    }

    /**
     * Adds a bot by character name (equivalent to `.bot add <name>`).
     *
     * @param string name : bot character name
     * @return table output
     */
    inline int AddBot(lua_State* L, Player* player)
    {
        std::string name = YLA::CHECKVAL<std::string>(L, 2);
        lua_createtable(L, 0, 0);
        int tbl = lua_gettop(L);
#ifdef MOD_SHADOWS
        if (player)
        {
            if (auto mgr = sShadowsMgr->GetShadowMgr(player))
            {
                std::string cmd = "add " + name;
                std::vector<std::string> out = mgr->HandleShadowCommand(cmd.c_str(), player);
                uint32 i = 0;
                for (std::string const& line : out)
                {
                    YLA::Push(L, line);
                    lua_rawseti(L, tbl, ++i);
                }
            }
        }
#else
        (void)player;
#endif
        lua_settop(L, tbl);
        return 1;
    }

    /**
     * Removes (logs out) a bot by character name (equivalent to `.bot remove <name>`).
     *
     * @param string name : bot character name
     * @return table output
     */
    inline int RemoveBot(lua_State* L, Player* player)
    {
        std::string name = YLA::CHECKVAL<std::string>(L, 2);
        lua_createtable(L, 0, 0);
        int tbl = lua_gettop(L);
#ifdef MOD_SHADOWS
        if (player)
        {
            if (auto mgr = sShadowsMgr->GetShadowMgr(player))
            {
                std::string cmd = "remove " + name;
                std::vector<std::string> out = mgr->HandleShadowCommand(cmd.c_str(), player);
                uint32 i = 0;
                for (std::string const& line : out)
                {
                    YLA::Push(L, line);
                    lua_rawseti(L, tbl, ++i);
                }
            }
        }
#else
        (void)player;
#endif
        lua_settop(L, tbl);
        return 1;
    }
}

#endif // SHADOWMETHODS_H
