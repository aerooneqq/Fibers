#include <iostream>
#include <thread>
#include <chrono>
#include "ThreadPool.h"


extern "C" void SetContext(RegisterContext* context);
extern "C" void FillContext(RegisterContext* context);

volatile int x = 0;

int main()
{
//    RegisterContext context{};
//    FillContext(&context);
//
//    int xx = 1;
//    std::cout << xx << "\n";
//    ++xx;
//
//    std::cout << context.ReturnAddress << " " << context.StackPointer << "\n";
//    std::cout << "SADASD" << "\n";
//
//    if (x == 0)
//    {
//        ++x;
//        SetContext(&context);
//    }

    ThreadPool pool{};

    std::function<void(TaskController*)> func = [&pool](TaskController* controller)
    {
        std::cout << "ASDASD" << "\n";

        controller->Yield();

        std::cout << "ASDASD" << "\n";
        pool.Stop();
    };


    pool.Schedule(Task("ASDASD", &func));

    pool.StartBlocking();
    return 0;
}