#include <iostream>
#include <thread>
#include <chrono>
#include "ThreadPool.h"

using namespace std::chrono_literals;


extern "C" void SetContext(RegisterContext* context);
extern "C" void FillContext(RegisterContext* context);

volatile int x = 0;
RegisterContext savedContext{};

int foo()
{
    std::cout << "Foo" << "\n";
    exit(1);
}

void Execute(TaskJobFunction* f, TaskController* c)
{
    (*f)(c);
    SetContext(&savedContext);
}

int main()
{
    char stack[2 << 12];
    for (char& i: stack)
    {
        i = 0;
    }

    char* sp = (char*) (stack + sizeof(stack));
    sp = (char*) ((uintptr_t) sp & -16L);
    sp -= 128;

    std::cout << (void*) foo << " " << (int64_t) (void*) foo << "\n";
    std::cout << (int64_t) sp << "\n";


    auto ff = new TaskJobFunction([](TaskController* c)
    {
        std::cout << "Task controller: " << c << "\n";
        std::cout << "Task job" << "\n";
    });

    RegisterContext context{};
    auto controller = new TaskController();
    context.SecondIntArgument = (int64_t) controller;
    context.InstructionPointer = (int64_t) (void*) Execute;
    context.StackPointer = (int64_t) sp;
    context.FirstIntArgument = (int64_t) ff;

    FillContext(&savedContext);
    if (x == 0)
    {
        if (x != 1) {
            ++x;
            SetContext(&context);
        }
    }

    delete ff;

    ThreadPool pool{};
    pool.Start();

    TaskJobFunction func = [&pool](TaskController* controller)
    {
        std::cout << "ASDASD" << "\n";

        controller->Yield();

        std::cout << "ASDASD" << "\n";
    };

    pool.Schedule(Task("ASDASD", &func));
    pool.Schedule(Task("ASDASD", &func));
    pool.Schedule(Task("ASDASD", &func));
    pool.Schedule(Task("ASDASD", &func));

    std::this_thread::sleep_for(2000ms);

    pool.Stop();

    return 0;
}