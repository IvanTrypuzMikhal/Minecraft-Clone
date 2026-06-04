#pragma once
#include "TSQueue.h"
#include "ChunkPackage.h"

class MeshThread
{
public:
	MeshThread() : m_meshRunning{ true } {
		m_meshThread = std::thread(&MeshThread::asyncMeshLoading, this);
	}
	~MeshThread() {
		m_meshRunning = false;
		m_meshCond.notify_one();
		m_meshThread.join();
	}

	void asyncMeshLoading();
	void notifyThread();

	TSQueue<ChunkPackage>& meshQueue();
	TSQueue<std::pair<int, int>>& finishedMeshChunks();

private:
	std::thread m_meshThread;
	bool m_meshRunning;
	std::mutex m_meshMutex;
	std::condition_variable m_meshCond;
	TSQueue<std::pair<int, int>> m_finishedMeshChunks;
	TSQueue<ChunkPackage> m_meshQueue;

};