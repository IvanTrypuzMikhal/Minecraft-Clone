#include "ThreadPool.h"

ThreadPool::ThreadPool() {
	auto m_processor_count = getAdaptivePoolSize();

	for (int i = 0; i < m_processor_count; ++i) {
		m_threads.emplace_back([this] {
			while (true) {
				auto task = m_taskQueue.pop();
				if (!task) {
					break;
				}
				task();
			}
			});
	}
	std::cout << "Thread pool created with " << m_processor_count << " threads." << std::endl;
}

ThreadPool::~ThreadPool() {

	for (int i = 0; i < m_threads.size(); ++i) {
		m_taskQueue.push(nullptr);
	}

	for (auto& thread : m_threads) {
		if (thread.joinable()) {
			thread.join();
		}
	}
}

unsigned int ThreadPool::getAdaptivePoolSize() const {
	unsigned int hardware_threads = std::thread::hardware_concurrency();

	if (hardware_threads == 0) {
		return 5; // Default to 5 threads if hardware_concurrency cannot determine the number of cores
	}
	unsigned int pool_size = 5;
	if (hardware_threads > 5) {
		pool_size += static_cast<unsigned int>(std::sqrt(hardware_threads - 5) * 2);
	}
	return pool_size;
}


TSQueue<std::function<void()>>& ThreadPool::taskQueue() {
	return m_taskQueue;
}