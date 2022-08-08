#include <string>
#include <atomic>
#include <vector>
#include <functional>

#include "ExecutionContext.h"

class Task;

class TaskController
{
private:
    ExecutionContext* myExecutionContext{nullptr};

public:
    Stack ObtainTaskStack();


    void Yield();
    void Cancel();
    void WithExclusiveAccess(Lock *lock, PureJobFunction job);
    void NestedParallel(const std::vector<Task> &tasks);
    void NestedSequential(const std::vector<Task> &tasks);

    ~TaskController() = default;
};

typedef std::function<void(TaskController*)> TaskJobFunction;

class Task
{
private:
    std::string myName;
    std::atomic<bool> myIsCompleted{false};
    TaskJobFunction* myJob;
    TaskController* myController;
    ExecutionContext* myExecutionContext;

public:
    Task(const Task& other);
    Task(std::string name, TaskJobFunction* job);
    std::string GetName();
    bool IsCompleted();
    void Execute();
};