#include <queue>
#include <mutex>
#include <thread>
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
    ~TaskNodeList();

    void PushTask(const Task& task);
    TaskNode* Devastate();
    bool IsEmpty();
};

class ThreadPoolThread {
private:
    std::atomic<bool> myWasToldToShutdown{false};
    std::atomic<bool> myIsActive{false};
    std::atomic<bool> myIsProcessingTasks{false};
    std::thread* myThread{nullptr};
    TaskNodeList* myTasks;

public:
    ThreadPoolThread();
    ~ThreadPoolThread();

    void Start();
    void Shutdown();
    bool QueueTask(const Task& task);
    void WaitForRemainingTasksCompletion();
};

class ThreadPool {
private:
    static ThreadPool* ourInstance;
    std::atomic<bool> myIsActive{false};
    ThreadPoolThread* mySingleThread{nullptr};
    ThreadPool();

public:
    static ThreadPool* GetInstance();

    ~ThreadPool();

    bool IsActive();
    void Start();
    bool Schedule(const Task& task);
    void StopAndWaitForScheduledTasksCompletion();
    void WaitForAllTasksCompletion();
};