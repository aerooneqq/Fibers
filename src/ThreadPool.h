
#include <queue>
#include <mutex>
#include "Task.h"

class ThreadPool
{
private:
    std::atomic<bool> myIsExecutingTasks{false};
    std::atomic<bool> myIsActive{false};
    std::mutex myMutex{};
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