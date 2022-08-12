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
    ThreadPool* pool = ThreadPool::GetInstance();
    pool->Start();

    TaskJobFunction func([](TaskController* controller) {
        std::cout << "A" << "\n";
        volatile int x = 0;
        //RegisterContext currentContext{};
//    FillContext(&currentContext);
//
        std::cout << x;
        //controller->Yield();

        std::cout << "B" << "\n";
    });

    auto task = Task("1", func);
    auto task1 = Task("2", func);
    auto task2 = Task("3", func);
    auto task3 = Task("4", func);

    pool->Schedule(task);
//    pool->Schedule(task1);
//    pool->Schedule(task2);
//    pool->Schedule(task3);

    std::this_thread::sleep_for(2000ms);

    pool->StopAndWaitForScheduledTasksCompletion();

    return 0;
}