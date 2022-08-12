#include <iostream>
#include <thread>
#include <chrono>
#include "ThreadPool.h"

using namespace std::chrono_literals;

int main() {
    ThreadPool* pool = ThreadPool::GetInstance();
    pool->Start();

    int x = 123123;
    TaskJobFunction func([&x](TaskController* controller) {
        std::cout << "A" << "\n";

        controller->Yield();

        std::cout << "B" << "\n";
    });

    auto task = Task("1", func);
    auto task1 = Task("2", func);
    auto task2 = Task("3", func);
    auto task3 = Task("4", func);

    pool->Schedule(task);
    pool->Schedule(task1);

    pool->StopAndWaitForScheduledTasksCompletion();

    return 0;
}