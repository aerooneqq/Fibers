
#include <queue>
#include <mutex>
#include "Task.h"

class ThreadPool
{
private:
    std::atomic<bool>* myIsActive;
    std::mutex* myMutex;
    std::queue<Task>* myTaskQueue;

public:
    ThreadPool();
    ~ThreadPool();

    bool IsActive();
    void StartBlocking();
    void Schedule(Task task);
    void Stop();
    void WaitForAllTasksCompletion();
};