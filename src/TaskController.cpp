
#include "TaskController.h"

TaskController::TaskController(StackManager* stackManager) {
    myStackManager = stackManager;
    myState = TaskExecutionState::Created;
}

void TaskController::Yield() {
}

void TaskController::WithExclusiveAccess(Lock* lock, PureJobFunction job) {

}

void TaskController::Cancel() {

}

Stack TaskController::ObtainTaskStack() {
    return myStackManager->AllocateStack();
}

ExecutionContext TaskController::CreateExecutionContextAndSetStackPointer(RegisterContext& context) {
    auto stack = myStackManager->AllocateStack();
    context.StackPointer = stack.MaterializeStackPointer();
    myExecutionContext = ExecutionContext(context, stack);
    return myExecutionContext;
}

void TaskController::SetInitialRegisterContext(const RegisterContext& context) {
    myInitialRegisterContext = context;
}

RegisterContext TaskController::GetInitialRegisterContext() const {
    return myInitialRegisterContext;
}
