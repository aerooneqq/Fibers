#include <queue>
#include <mutex>
#include "Task.h"

class TaskNode {
private:
    Task myTask;
    TaskNode* myPrevious{nullptr};

public:
    explicit TaskNode(const Task& task);
    void SetPrevious(TaskNode* node);
    TaskNode* GetPrevious();
    Task GetTask();
};

class TaskNodeList {
private:
    std::atomic<TaskNode*> myTail{nullptr};

public:
    void PushTask(const Task& task);
    TaskNode* Devastate();
};

class ThreadPoolThread {
private:
    std::atomic<bool> myIsActive{false};
    std::atomic<bool> myIsProcessingTasks{false};
    std::thread* myThread{nullptr};
    TaskNodeList* myTasks;

public:
    ThreadPoolThread();
    void Start();
    void Stop();
    void QueueTask(const Task& task);
};

class ThreadPool {
private:
    static ThreadPool* ourInstance;
    std::atomic<bool> myIsActive{false};
    ThreadPoolThread* mySingleThread;
    ThreadPool();

public:
    static ThreadPool* GetInstance();

    ~ThreadPool();

    bool IsActive();
    void Start();
    void Schedule(const Task& task);
    void Stop();
    void WaitForAllTasksCompletion();
};