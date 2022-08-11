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

Stack* TaskController::ObtainTaskStack() {
    return myStackManager->AllocateStack();
}

ExecutionContext* TaskController::CreateExecutionContext(const TaskJobFunction& job, void* wrapperFunction) {
    RegisterContext context{};
    context.FirstIntArgument = (int64_t) &job;
    context.SecondIntArgument = (int64_t) this;
    context.InstructionPointer = (int64_t) wrapperFunction;
    auto stack = myStackManager->AllocateStack();
    context.StackPointer = stack->MaterializeStackPointer();
    myExecutionContext = new ExecutionContext(context, stack);
    return myExecutionContext;
}

void TaskController::SetInitialRegisterContext(const RegisterContext& context) {
    myInitialRegisterContext = context;
}

RegisterContext TaskController::GetInitialRegisterContext() const {
    return myInitialRegisterContext;
}

ExecutionContext TaskController::CaptureCurrentExecutionContext() {

}
