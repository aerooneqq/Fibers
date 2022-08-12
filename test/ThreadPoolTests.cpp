#include <gtest/gtest.h>
#include "../src/ThreadPool.h"

void ExecuteWithThreadPool(const std::function<void(ThreadPool*)>& function);

TEST(ThreadPoolTests, TestQueueOneTask) {
    auto x = 0;

    ExecuteWithThreadPool([&x](ThreadPool* pool) {
        pool->Schedule(Task("TestQueueOneTask", [&x](TaskController* controller) {
            ++x;
        }));
    });

    ASSERT_EQ(1, x);
}

TEST(ThreadPoolTests, TestQueueSeveralTasks) {
    auto x = 0;
    const int TasksCount = 10;

    ExecuteWithThreadPool([&x](ThreadPool* pool) {
        for (int i = 0; i < TasksCount; ++i) {
            pool->Schedule(Task("TestQueueSeveralTasks", [&x](TaskController* controller) {
                ++x;
            }));
        }
    });

    ASSERT_EQ(x, TasksCount);
}

void ExecuteWithThreadPool(const std::function<void(ThreadPool*)>& function) {
    ThreadPool* pool = ThreadPool::GetInstance();
    pool->Start();

    function(pool);

    pool->StopAndWaitForScheduledTasksCompletion();
}