#include <vector>
#include "ExecutionContext.h"
#include "typedefs.h"

enum TaskExecutionState {
    Uninitialized,
    Created,
    InitiallyQueued,
    StartedExecution,
    Yielding,
    ReQueued,
    ReExecuted,
    Finished
};

class ThreadPool;
class Task;

class TaskController {
private:
    StackManager* myStackManager{nullptr};
    RegisterContext myInitialRegisterContext;
    ExecutionContext* myExecutionContext{nullptr};
    TaskExecutionState myState = TaskExecutionState::Uninitialized;
    ThreadPool* myThreadPool{nullptr};
    Task* myTask{nullptr};

    Stack* ObtainTaskStack();
    ExecutionContext CaptureCurrentExecutionContext();

public:
    TaskController(StackManager* stackManager, Task* task);

    [[nodiscard]] RegisterContext GetInitialRegisterContext() const;
    void SetInitialRegisterContext(const RegisterContext& context);

    ExecutionContext* CreateExecutionContext(const TaskJobFunction& job, void* wrapperFunction);
    ExecutionContext* GetExecutionContext();

    void SetState(TaskExecutionState newState);
    TaskExecutionState GetState();

    void Yield();
    void Cancel();
    void WithExclusiveAccess(Lock* lock, PureJobFunction job);

    ~TaskController();
};