#include "FileIOThread.h"
#include <fstream>

void FileIOThread::asyncFileIO() {
	std::cout << "FileIO thread started!" << std::endl;
	
	while (m_fileIORunning) {
		ChunkSnapshot snapshot;
		std::ofstream file;
		
		{
			std::unique_lock<std::mutex> lock(m_fileIOMutex);

			m_fileIOCond.wait(lock, [this] {
				return !m_saveInMemoryQueue.empty() || !m_fileIORunning;
				});

			if (!m_fileIORunning) break;
			snapshot = m_saveInMemoryQueue.pop();
			std::cout << "Saving chunk to memory: " << snapshot.coords.first << " " << snapshot.coords.second << std::endl;
		}

		// Saving chunk to memory logic here.
		// We get chunks region
		std::string path = "src/regions/regions." + std::to_string(snapshot.coords.first % 32) + "." + std::to_string(snapshot.coords.second % 32) + ".bin";
		file.open(path, std::ios::in | std::ios::out | std::ios::binary);

		if (!file.is_open()) {
			file.clear();
			file.open(path, std::ios::out | std::ios::binary);

			std::vector<char> emptyHeader(4096, 0);
			file.write(emptyHeader.data(), emptyHeader.size());

			file.close();
			file.open(path, std::ios::in | std::ios::out | std::ios::binary);
		}

		if (!file){
			std::cout << "Failed to open file for chunk: " << snapshot.coords.first << " " << snapshot.coords.second << std::endl;
			return;
		}

		int index = (snapshot.coords.second % 32) * 32 + (snapshot.coords.first % 32);
		// First 1024 entries are for chunk coordinates and the number of regions that it occupies
		// Thus the size of the entrie will be 4 bytes or uint32_t

		// We calculate the number of sectors needed to save the chunk data.
		size_t sizeInBytes = snapshot.deltas_counts.count * sizeof(Delta);
		uint8_t sectores = (sizeInBytes + Globals::REGION_BLOCK_SIZE - 1) / Globals::REGION_BLOCK_SIZE;
		
		// We will assume for now static size and that each chunk will occupy 2 sectors.
		uint32_t sectorIndexStart = (index * 2 + 1);
		uint32_t chunkEntryData = (sectorIndexStart << 8) | sectores;
		
		// Save chunks metadata
 		file.seekp(index * 4);
		file.write(reinterpret_cast<char*>(&chunkEntryData), sizeof(uint32_t));

		// Now we save the chunk data to the sector calculated by the index and the number of sectors needed to save the chunk data.
		file.seekp(sectorIndexStart * Globals::REGION_BLOCK_SIZE);
		file.write(reinterpret_cast<char*>(&snapshot.deltas_counts.count), sizeof(size_t));
		file.write(reinterpret_cast<char*>(snapshot.deltas_counts.deltas), sizeInBytes);

		// After saving to memory, we can push the chunk coordinates to the finishedFileIOChunks.
		m_finishedfileIOChunks.push(snapshot);
	}
}


TSQueue<ChunkSnapshot>& FileIOThread::saveInMemoryQueue() {
	return m_saveInMemoryQueue;
}

std::unordered_set<std::pair<int, int>, PairHash>& FileIOThread::mainMemSavedChunks() {
	return m_mainMemSavedChunks;
}

TSQueue<ChunkSnapshot>& FileIOThread::finishedFileIOChunks() {
	return m_finishedfileIOChunks;
}

void FileIOThread::notifyThread() {
	m_fileIOCond.notify_one();
}