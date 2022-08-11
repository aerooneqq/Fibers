#include <string>
#include <atomic>
#include <vector>
#include <functional>

#include "TaskController.h"

class Task;

typedef std::function<void(TaskController*)> TaskJobFunction;

class Task {
private:
    std::string myName;
    std::atomic<bool> myIsCompleted{false};
    TaskJobFunction myJob;
    TaskController* myController;

public:
    Task(const Task& other);
    Task(std::string name, const TaskJobFunction& job);
    std::string GetName();
    bool IsCompleted();
    void Execute(const RegisterContext& savedContext);
};