#include "Task.h"

#include <utility>

std::string Task::GetName() {
    return myName;
}

bool Task::IsCompleted() {
    return myIsCompleted.load(std::memory_order_seq_cst);
}

Task::Task(std::string name, const TaskJobFunction& job) : myName(std::move(name)) {
    myJob = job;
    myController = new TaskController(StackManager::GetInstance());
}

Task::Task(const Task& other) {
    myJob = other.myJob;
    myName = other.myName;
    myController = other.myController;
    myIsCompleted.store(other.myIsCompleted.load(std::memory_order_seq_cst));
}

void ExecuteForAsmReference(TaskJobFunction* function, TaskController* controller) {
    (*function)(controller);
}

void Task::Execute(const RegisterContext& savedContext) {
    myController->SetInitialRegisterContext(savedContext);

    TaskJobFunction toExecute([=](TaskController* controller) {
        myJob(controller);
        auto contextToRestore = myController->GetInitialRegisterContext();
        SetContext(&contextToRestore);
    });

    auto context = myController->CreateExecutionContext(toExecute, (void*) ExecuteForAsmReference);
    auto registerContext = context->GetRegisterContext();
    SetContext(&registerContext);
}
