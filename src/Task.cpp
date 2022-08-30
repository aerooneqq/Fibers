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
    myController = new TaskController(this);
}

Task::Task(const Task& other) {
    myJob = other.myJob;
    myName = other.myName;
    myController = other.myController;
    myIsCompleted.store(other.myIsCompleted.load(std::memory_order_seq_cst));
}

void ExecuteForAsmReference(TaskJobFunction* function, TaskController* controller) {
    (*function)(controller);
    auto ctx = controller->GetInitialRegisterContext();
    SetContext(&ctx);
}

void Task::Execute(char* stackPtr) {
    volatile int x = 0;
    RegisterContext savedContext;
    FillContext(&savedContext);

    if (x == 0) {
        ++x;

        if (myController->ShouldContinueFromSavePoint()) {
            auto ctx = myController->PrepareRestore(stackPtr);
            SetContext(&ctx);
            return;
        }

        myController->SetInitialContext(&savedContext, stackPtr);
        TaskJobFunction toExecute([=](TaskController* controller) {
            myJob(controller);
        });

        RegisterContext context{};
        context.FirstIntArgument = (int64_t) &toExecute;
        context.SecondIntArgument = (int64_t) myController;
        context.InstructionPointer = (int64_t) ExecuteForAsmReference;
        context.StackPointer = (int64_t)stackPtr;
        SetContext(&context);
    }
}

TaskController* Task::GetController() const {
    return myController;
}