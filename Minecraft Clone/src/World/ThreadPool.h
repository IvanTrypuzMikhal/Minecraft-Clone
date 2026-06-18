#pragma once
#include <vector>
#include <thread>
#include "TSQueue.h"
#include <functional>

class ThreadPool
{
public:
	ThreadPool();
	~ThreadPool();

	TSQueue<std::function<void()>>& taskQueue();
	unsigned int getAdaptivePoolSize() const;


private:
	std::vector<std::thread> m_threads;
	TSQueue<std::function<void()>> m_taskQueue;
};
