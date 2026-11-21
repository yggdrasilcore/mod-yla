/*
* Copyright (C) 2010 - 2025 Eluna Lua Engine <https://elunaluaengine.github.io/>
* This program is free software licensed under GPL version 3
* Please see the included DOCS/LICENSE.md for more information
*/

#include "YLAFileWatcher.h"
#include "LuaEngine.h"
#include "YLAUtility.h"
#include <system_error>

YLAFileWatcher::YLAFileWatcher() : running(false), checkInterval(1)
{
}

YLAFileWatcher::~YLAFileWatcher()
{
    StopWatching();
}

void YLAFileWatcher::StartWatching(const std::string& scriptPath, uint32 intervalSeconds)
{
    if (running.load())
    {
        YLA_LOG_DEBUG("[YLAFileWatcher]: Already watching files");
        return;
    }

    if (scriptPath.empty())
    {
        YLA_LOG_ERROR("[YLAFileWatcher]: Cannot start watching - script path is empty");
        return;
    }

    watchPath = scriptPath;
    checkInterval = intervalSeconds;

    baseline = CaptureSnapshot();
    settleSnapshot.clear();
    settling = false;
    haveSettleCapture = false;

    running.store(true);

    watcherThread = std::thread(&YLAFileWatcher::WatchLoop, this);
    
    YLA_LOG_INFO("[YLAFileWatcher]: Started watching '{}' (interval: {}s)", watchPath, checkInterval);
}

void YLAFileWatcher::StopWatching()
{
    if (!running.load())
        return;

    running.store(false);

    if (watcherThread.joinable())
        watcherThread.join();

    baseline.clear();
    settleSnapshot.clear();
    settling = false;
    haveSettleCapture = false;
    
    YLA_LOG_INFO("[YLAFileWatcher]: Stopped watching files");
}

void YLAFileWatcher::WatchLoop()
{
    while (running.load())
    {
        try
        {
            CheckForChanges();
        }
        catch (const std::exception& e)
        {
            YLA_LOG_ERROR("[YLAFileWatcher]: Error during file watching: {}", e.what());
        }

        std::this_thread::sleep_for(std::chrono::seconds(checkInterval));
    }
}

bool YLAFileWatcher::IsWatchedFileType(const std::string& filename) const
{
    return (filename.length() >= 4 && filename.substr(filename.length() - 4) == ".lua") ||
        (filename.length() >= 4 && filename.substr(filename.length() - 4) == ".ext") ||
        (filename.length() >= 5 && filename.substr(filename.length() - 5) == ".moon");
}

void YLAFileWatcher::CaptureDirectory(const std::filesystem::path& dir, FileSnapshot& snapshot) const
{
    std::error_code ec;
    std::filesystem::directory_iterator dir_iter(dir, ec);
    std::filesystem::directory_iterator end_iter;

    while (!ec && dir_iter != end_iter)
    {
        const std::filesystem::directory_entry& entry = *dir_iter;

        std::error_code entryEc;
        if (entry.is_directory(entryEc))
        {
            if (!entryEc)
                CaptureDirectory(entry.path(), snapshot);
        }
        else if (!entryEc && entry.is_regular_file(entryEc) && !entryEc)
        {
            std::string filename = entry.path().filename().generic_string();

            if (IsWatchedFileType(filename))
            {
                FileStamp stamp;
                stamp.modified = std::filesystem::last_write_time(entry.path(), entryEc);
                if (!entryEc)
                    stamp.size = std::filesystem::file_size(entry.path(), entryEc);

                if (!entryEc)
                    snapshot[entry.path().generic_string()] = stamp;
            }
        }

        dir_iter.increment(ec);
    }
}

YLAFileWatcher::FileSnapshot YLAFileWatcher::CaptureSnapshot() const
{
    FileSnapshot snapshot;

    std::error_code ec;
    if (!std::filesystem::is_directory(watchPath, ec) || ec)
        return snapshot;

    CaptureDirectory(watchPath, snapshot);
    return snapshot;
}

void YLAFileWatcher::CheckForChanges()
{
    FileSnapshot current = CaptureSnapshot();

    if (!settling)
    {
        if (current == baseline)
            return;

        // Something changed. Do not reload yet: wait one tick, capture the
        // files, wait another tick, capture again. Only when both captures
        // match is the write considered finished.
        settling = true;
        haveSettleCapture = false;
        YLA_LOG_INFO("[YLAFileWatcher]: Lua script changes detected, waiting for writes to settle");
        return;
    }

    if (!haveSettleCapture)
    {
        settleSnapshot = std::move(current);
        haveSettleCapture = true;
        return;
    }

    if (current != settleSnapshot)
    {
        // Still being written; keep capturing until two consecutive
        // captures are identical.
        settleSnapshot = std::move(current);
        return;
    }

    YLA_LOG_INFO("[YLAFileWatcher]: Lua script changes settled - triggering reload");
    baseline = settleSnapshot;
    settling = false;
    haveSettleCapture = false;
    YLA::ReloadYLA();
}
