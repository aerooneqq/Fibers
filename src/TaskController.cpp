#include "TaskController.h"

void TaskController::Cancel() {
}

void TaskController::SetState(TaskExecutionState newState) {
    myState = newState;
}

TaskExecutionState TaskController::GetState() {
    return myState;
}

TaskController::~TaskController() {
}

void TaskController::SetInitialContext(RegisterContext* context, char* stackStart) {
    myInitialRegisterContext = context;
    myInitialStackPointer = stackStart;
}

void TaskController::SaveContext(const RegisterContext& context) {
    delete mySavedStack;

    mySavedStack = new std::vector<char>();
    for (auto ptr = myInitialStackPointer; ptr !=(char*)context.StackPointer ; --ptr) {
        mySavedStack->push_back(*ptr);
    }

    mySavedContext = context;
}

RegisterContext TaskController::GetInitialRegisterContext() {
    return *myInitialRegisterContext;
}

bool TaskController::ShouldContinueFromSavePoint() {
    return mySavedStack != nullptr;
}

RegisterContext TaskController::PrepareRestore(char* stackPointer) {
    for (auto c : *mySavedStack) {
        *stackPointer = c;
        --stackPointer;
    }

    delete mySavedStack;
    mySavedStack = nullptr;
    mySavedContext.StackPointer = (int64_t)stackPointer;
    return mySavedContext;
}
