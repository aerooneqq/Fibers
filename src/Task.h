#include <string>
#include <atomic>
#include <vector>
#include <functional>

#include "TaskController.h"
#include "typedefs.h"

class Task {
private:
    std::string myName;
    std::atomic<bool> myIsCompleted{false};
    TaskJobFunction myJob;
    TaskController* myController{nullptr};

public:
    Task(const Task& other);
    Task(std::string name, const TaskJobFunction& job);

    std::string GetName();

    bool IsCompleted();
    void Execute(const RegisterContext& savedContext);

    [[nodiscard]] TaskController* GetController() const;
};