#include <iostream>
#include <thread>
#include <chrono>
#include "ThreadPool.h"

using namespace std::chrono_literals;
void foo(int x) {
    std::cout << x;
}

int main() {
    ThreadPool* pool = ThreadPool::GetInstance();
    pool->Start();

    int x = 123123;
    TaskJobFunction func([](TaskController* controller) {
        std::cout << "A" << "\n";
        controller->Yield();
        std::cout << "B" << "\n";
    });

    auto task = Task("1", func);
    auto task1 = Task("2", func);
    auto task2 = Task("3", func);
    auto task3 = Task("4", func);
    auto task4 = Task("5", func);
    auto task5 = Task("6", func);
    auto task6 = Task("7", func);

    pool->Schedule(task);
    pool->Schedule(task1);
    pool->Schedule(task2);
    pool->Schedule(task3);
    pool->Schedule(task4);
    pool->Schedule(task5);
    pool->Schedule(task6);


    pool->StopAndWaitForScheduledTasksCompletion();

    return 0;
}