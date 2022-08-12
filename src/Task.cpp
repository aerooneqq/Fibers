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
    myController = new TaskController(StackManager::GetInstance(), this);
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

void Task::Execute() {
    volatile int x = 0;
    RegisterContext savedContext;
    FillContext(&savedContext);

    if (x == 0) {
        ++x;
        auto existingExecutionContext = myController->GetExecutionContext();
        if (existingExecutionContext != nullptr) {
            auto context = existingExecutionContext->Restore();
            myController->SetState(TaskExecutionState::ReExecuted);
            SetContext(&context);
            return;
        }

        myController->SetInitialRegisterContext(savedContext);
        TaskJobFunction toExecute([=](TaskController* controller) {
            myJob(controller);
            std::cout << "Finished executing task" << "\n";
            controller->SetState(TaskExecutionState::Finished);
            auto context = controller->GetInitialRegisterContext();
            SetContext(&context);
        });

        auto context = myController->CreateExecutionContext(toExecute, (void*) ExecuteForAsmReference);
        auto registerContext = context->GetRegisterContext();

        myController->SetState(TaskExecutionState::StartedExecution);

        SetContext(&registerContext);
    }

}

TaskController* Task::GetController() const {
    return myController;
}