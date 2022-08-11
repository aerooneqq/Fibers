#include <iostream>
#include <thread>
#include <chrono>
#include "ThreadPool.h"

using namespace std::chrono_literals;

volatile int x = 0;
RegisterContext savedContext{};

int foo() {
    std::cout << "Foo" << "\n";
    exit(1);
}

void Execute(TaskJobFunction* function, TaskController* controller) {
    (*function)(controller);
    SetContext(&savedContext);
}

int main() {
    TaskJobFunction ff = [](TaskController* c) {
        std::cout << "Task controller: " << c << "\n";
        std::cout << "Task job" << "\n";
    };

    char stack[2 << 12];
    for (char& i: stack) {
        i = 0;
    }

    char* sp = (char*) (stack + sizeof(stack));
    sp = (char*) ((uintptr_t) sp & -16L);
    sp -= 128;

    std::cout << (void*) foo << " " << (int64_t) (void*) foo << "\n";
    std::cout << (int64_t) sp << "\n";

    RegisterContext context{};
    auto controller = new TaskController(StackManager::GetInstance());
    context.FirstIntArgument = (int64_t) &ff;
    context.SecondIntArgument = (int64_t) controller;
    context.InstructionPointer = (int64_t) (void*) Execute;
    context.StackPointer = (int64_t) sp;

    FillContext(&savedContext);
    if (x == 0) {
        if (x != 1) {
            ++x;
            SetContext(&context);
        }
    }

    ThreadPool* pool = ThreadPool::GetInstance();
    pool->Start();

    TaskJobFunction func([](TaskController* controller) {
        std::cout << "1" << "\n";

        controller->Yield();

        std::cout << "2" << "\n";
    });

    pool->Schedule(Task("ASDASD", func));
    pool->Schedule(Task("ASDASD", func));
    pool->Schedule(Task("ASDASD", func));
    pool->Schedule(Task("ASDASD", func));

    std::this_thread::sleep_for(2000ms);

    pool->StopAndWaitForScheduledTasksCompletion();

    return 0;
}