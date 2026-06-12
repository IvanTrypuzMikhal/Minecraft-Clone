#include "FileIOThread.h"
#include <fstream>

void FileIOThread::asyncFileIO() {
	std::cout << "FileIO thread started!" << std::endl;
	
	while (m_fileIORunning) {
		ChunkSnapshot snapshot;
		bool isThereWork = false;

		{
			std::unique_lock<std::mutex> lock(m_fileIOMutex);

			m_fileIOCond.wait(lock, [this] {
				return !m_saveInMemoryQueue.empty() || !m_loadFromMemoryQueue.empty() || !m_fileIORunning;
				}); 
		}
		
		if (!m_fileIORunning) break;

		if (!m_saveInMemoryQueue.empty()) {
			snapshot = m_saveInMemoryQueue.pop();
			isThereWork = true;
		}

		if (isThereWork) {
			std::fstream file;

			// Region coords
			int regionX = snapshot.coords.first >> 5;
			int regionZ = snapshot.coords.second >> 5;

			// Local chunk coords in the region
			int localX = snapshot.coords.first & 31;
			int localZ = snapshot.coords.second & 31;

			// Saving chunk to memory logic here.
			// We get chunks region
			std::string path = "src/regions/regions." + std::to_string(regionX) + "." + std::to_string(regionZ) + ".bin";
			file.open(path, std::ios::in | std::ios::out | std::ios::binary);

			if (!file.is_open()) {
				file.clear();
				file.open(path, std::ios::out | std::ios::binary);

				std::vector<char> emptyHeader(4096, 0);
				file.write(emptyHeader.data(), emptyHeader.size());

				file.close();
				file.open(path, std::ios::in | std::ios::out | std::ios::binary);
			}

			if (!file) {
				std::cout << "Failed to open file for chunk: " << snapshot.coords.first << " " << snapshot.coords.second << std::endl;
				return;
			}

			int index = localZ * 32 + localX;

			std::cout << "Chunk coords: " << snapshot.coords.first << ", " << snapshot.coords.second << std::endl;
			std::cout << "File path: " << path << std::endl;
			std::cout << "Chunk index in region: " << localX << ", " << localZ << std::endl;
			std::cout << "Chunk entry index: " << index << std::endl;

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
		


		// Loading chunks data from memory here
		while (!m_loadFromMemoryQueue.empty()) {
			std::pair<int, int> coords = m_loadFromMemoryQueue.pop();
			std::cout << "Loading chunk from memory: " << coords.first << " " << coords.second << std::endl;

			int regionX = coords.first >> 5;
			int regionZ = coords.second >> 5;
			int localX = coords.first & 31;
			int localZ = coords.second & 31;

			std::string path = "src/regions/regions." + std::to_string(regionX) + "." + std::to_string(regionZ) + ".bin";
			std::ifstream file(path, std::ios::binary);
			if (!file) {
				std::cout << "Failed to open region file for reading: " << path << std::endl;
				continue;
			}

			int index = localZ * 32 + localX;

			uint32_t chunkEntryData = 0;
			file.seekg(index * 4);
			file.read(reinterpret_cast<char*>(&chunkEntryData), sizeof(uint32_t));

			uint32_t sectorIndexStart = chunkEntryData >> 8;

			file.seekg(sectorIndexStart * Globals::REGION_BLOCK_SIZE);

			size_t count = 0;
			file.read(reinterpret_cast<char*>(&count), sizeof(size_t));

			Delta* deltas = (Delta*)malloc(count * sizeof(Delta));
			file.read(reinterpret_cast<char*>(deltas), count * sizeof(Delta));

			ChunkSnapshot snapshot;
			snapshot.coords = coords;
			snapshot.deltas_counts.deltas = deltas;
			snapshot.deltas_counts.count = count;

			m_finishedLoadFromMemoryQueue.push(snapshot);
		}
		
	}
}


TSQueue<ChunkSnapshot>& FileIOThread::saveInMemoryQueue() {
	return m_saveInMemoryQueue;
}

TSQueue<ChunkSnapshot>& FileIOThread::finishedFileIOChunks() {
	return m_finishedfileIOChunks;
}
TSQueue<std::pair<int, int>>& FileIOThread::loadFromMemoryQueue() {
	return m_loadFromMemoryQueue;
}	
TSQueue<ChunkSnapshot>& FileIOThread::finishedLoadQueue() {
	return m_finishedLoadFromMemoryQueue;
}

void FileIOThread::notifyThread() {
	m_fileIOCond.notify_one();
}