
#include <mutex>
#include <thread>
#include "ThreadPool.h"

void ThreadPool::Schedule(Task task)
{
    std::unique_lock lock(myMutex);
    myTaskQueue->push(task);
}

void ThreadPool::Stop()
{
    myIsActive.store(false, std::memory_order_seq_cst);
}

void ThreadPool::WaitForAllTasksCompletion()
{

}

void ThreadPool::StartBlocking()
{
    myIsActive.store(true, std::memory_order_seq_cst);

    std::thread([=]() {
        while (true)
        {
            if (!myIsActive.load(std::memory_order_seq_cst))
            {
                return;
            }

            std::unique_lock lock(myMutex);
            std::vector<Task> tasks;
            while (!myTaskQueue->empty())
            {
                tasks.push_back(myTaskQueue->front());
                myTaskQueue->pop();
            }

            lock.release();

            if (!tasks.empty())
            {
                for (auto task: tasks)
                {
                    task.Execute();
                }
            }
        }
    }).join();
}

ThreadPool::~ThreadPool()
{

}

ThreadPool::ThreadPool()
{
    myTaskQueue = new std::queue<Task>();
    myIsActive = new std::atomic<bool>(false);
}

bool ThreadPool::IsActive()
{
    return myIsActive.load(std::memory_order_seq_cst);
}
