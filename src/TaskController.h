#include <vector>
#include "ExecutionContext.h"

enum TaskExecutionState {
    Uninitialized,
    Created,
    InitiallyQueued,
    StartedExecution,
    ReQueued,
    ReExecuted,
    Finished
};

class TaskController {
private:
    StackManager* myStackManager;
    RegisterContext myInitialRegisterContext;
    ExecutionContext myExecutionContext;
    TaskExecutionState myState = TaskExecutionState::Uninitialized;

    Stack ObtainTaskStack();

public:
    explicit TaskController(StackManager* stackManager);

    void SetInitialRegisterContext(const RegisterContext& context);
    RegisterContext GetInitialRegisterContext() const;

    ExecutionContext CreateExecutionContextAndSetStackPointer(RegisterContext& context);

    void Yield();
    void Cancel();
    void WithExclusiveAccess(Lock* lock, PureJobFunction job);

    ~TaskController() = default;
};