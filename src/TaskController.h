#include <vector>
#include "typedefs.h"
#include "util.h"

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
    RegisterContext* myInitialRegisterContext{};
    char* myInitialStackPointer{};

    std::vector<char>* mySavedStack{nullptr};
    RegisterContext mySavedContext;

    TaskExecutionState myState = TaskExecutionState::Uninitialized;
    ThreadPool* myThreadPool{nullptr};
    Task* myTask{nullptr};

public:
    TaskController(Task* task);

    void SetState(TaskExecutionState newState);
    TaskExecutionState GetState();

    void SetInitialContext(RegisterContext* context, char* stackStart);
    void SaveContext(const RegisterContext& context);

    RegisterContext GetInitialRegisterContext();
    bool ShouldContinueFromSavePoint();
    RegisterContext PrepareRestore(char* stackPointer);

    void Yield();
    void Cancel();

    ~TaskController();
};