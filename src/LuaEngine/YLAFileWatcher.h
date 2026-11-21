/*
* Copyright (C) 2010 - 2025 Eluna Lua Engine <https://elunaluaengine.github.io/>
* This program is free software licensed under GPL version 3
* Please see the included DOCS/LICENSE.md for more information
*/

#ifndef YLA_FILE_WATCHER_H
#define YLA_FILE_WATCHER_H

#include <thread>
#include <atomic>
#include <cstdint>
#include <filesystem>
#include <map>
#include <string>
#include <utility>
#include <chrono>
#include "Common.h"

class YLAFileWatcher
{
public:
    YLAFileWatcher();
    ~YLAFileWatcher();

    void StartWatching(const std::string& scriptPath, uint32 intervalSeconds = 1);
    void StopWatching();
    bool IsWatching() const { return running.load(); }

private:
    struct FileStamp
    {
        std::filesystem::file_time_type modified{};
        std::uintmax_t size = 0;

        bool operator==(FileStamp const& other) const
        {
            return modified == other.modified && size == other.size;
        }
    };

    typedef std::map<std::string, FileStamp> FileSnapshot;

    void WatchLoop();
    void CheckForChanges();
    void CaptureDirectory(const std::filesystem::path& dir, FileSnapshot& snapshot) const;
    FileSnapshot CaptureSnapshot() const;
    bool IsWatchedFileType(const std::string& filename) const;

    std::thread watcherThread;
    std::atomic<bool> running;
    std::string watchPath;
    uint32 checkInterval;

    // Last state that was acted on. Only advanced after a settled reload.
    FileSnapshot baseline;
    // Successive captures taken while waiting for in-progress writes to end.
    FileSnapshot settleSnapshot;
    bool settling = false;
    bool haveSettleCapture = false;
};

#endif
