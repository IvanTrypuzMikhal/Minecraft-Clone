#pragma once
#include "TSQueue.h"
#include "ChunkPackage.h"
#include <unordered_set>

class FileIOThread
{
public:
	FileIOThread() {
		m_fileIOThread = std::thread(&FileIOThread::asyncFileIO, this);
	}
	~FileIOThread() {
		m_fileIORunning = false;
		m_fileIOCond.notify_one();
		m_fileIOThread.join();
	}

	void asyncFileIO();
	TSQueue<ChunkSnapshot>& saveInMemoryQueue();
	TSQueue<ChunkSnapshot>& finishedFileIOChunks();
	TSQueue<std::pair<int, int>>& loadFromMemoryQueue();
	TSQueue<ChunkSnapshot>& finishedLoadQueue();
	void notifyThread();
private:
	std::thread m_fileIOThread;
	bool m_fileIORunning{ true };
	std::mutex m_fileIOMutex;
	std::condition_variable m_fileIOCond;

	// Save queues
	TSQueue<ChunkSnapshot> m_finishedfileIOChunks;
	TSQueue<ChunkSnapshot> m_saveInMemoryQueue;

	// Load queues
	TSQueue<std::pair<int, int>> m_loadFromMemoryQueue;
	TSQueue<ChunkSnapshot> m_finishedLoadFromMemoryQueue;
};