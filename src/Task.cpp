#include "Task.h"

#include <utility>

std::string Task::GetName() {
    return myName;
}

bool Task::IsCompleted() {
    return myIsCompleted.load(std::memory_order_seq_cst);
}

Task::Task(std::string name, TaskJobFunction* job) : myName(std::move(name)) {
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
        (*myJob)(controller);
        auto contextToRestore = myController->GetInitialRegisterContext();
        SetContext(&contextToRestore);
    });

    RegisterContext context{};
    context.FirstIntArgument = (int64_t) &toExecute;
    context.SecondIntArgument = (int64_t) myController;
    context.InstructionPointer = (int64_t) (void*) ExecuteForAsmReference;
    myController->CreateExecutionContextAndSetStackPointer(context);
    SetContext(&context);
}
