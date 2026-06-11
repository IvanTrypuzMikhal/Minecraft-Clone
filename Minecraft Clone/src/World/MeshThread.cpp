#include "MeshThread.h"
#include "World.h"

void MeshThread::asyncMeshLoading() {
	std::cout << "Mesh thread started!" << std::endl;
	while (m_meshRunning) {
		ChunkPackage package;

		{
			std::unique_lock<std::mutex> lock(m_meshMutex);

			m_meshCond.wait(lock, [this] {
				return !m_meshQueue.empty() || !m_meshRunning;
				});

			if (!m_meshRunning) break;

			package = m_meshQueue.pop();
			//std::cout << "Creating mesh around chunk: " << package.coords.first << " " << package.coords.second << std::endl;
		}
		package.center->buildMesh(package);
		m_finishedMeshChunks.push(package.coords);
	}
}

void MeshThread::notifyThread() {
	m_meshCond.notify_one();
}

TSQueue<ChunkPackage>& MeshThread::meshQueue() {
	return m_meshQueue;
}
TSQueue<std::pair<int, int>>& MeshThread::finishedMeshChunks() {
	return m_finishedMeshChunks;
}
