#include <thread>
#include "ThreadPool.h"

ThreadPool* ThreadPool::ourInstance = new ThreadPool();

bool ThreadPool::Schedule(const Task& task) {
    if (!myIsActive.load(std::memory_order_seq_cst)) {
        return false;
    }

    return mySingleThread->QueueTask(task);
}

void ThreadPool::StopAndWaitForScheduledTasksCompletion() {
    mySingleThread->Shutdown();
    mySingleThread->WaitForRemainingTasksCompletion();
    myIsActive.store(false, std::memory_order_seq_cst);
}

void ThreadPool::WaitForAllTasksCompletion() {
    mySingleThread->WaitForRemainingTasksCompletion();
}

void ThreadPool::Start() {
    mySingleThread = new ThreadPoolThread();
    mySingleThread->Start();
    myIsActive.store(true, std::memory_order_seq_cst);
}

ThreadPool::~ThreadPool() {
    delete mySingleThread;
}

ThreadPool::ThreadPool() {
    mySingleThread = new ThreadPoolThread();
    myIsActive = new std::atomic<bool>(false);
}

bool ThreadPool::IsActive() {
    return myIsActive.load(std::memory_order_seq_cst);
}

ThreadPool* ThreadPool::GetInstance() {
    return ThreadPool::ourInstance;
}

TaskNode* TaskNode::GetPrevious() {
    return myPrevious;
}

Task TaskNode::GetTask() {
    return myTask;
}

TaskNode::TaskNode(const Task& task) : myTask(task) {
}

void TaskNode::SetPrevious(TaskNode* previous) {
    myPrevious = previous;
}

void TaskNodeList::PushTask(const Task& task) {
    auto node = new TaskNode(task);
    while (true) {
        auto currentTail = myTail.load(std::memory_order_seq_cst);
        node->SetPrevious(currentTail);
        if (myTail.compare_exchange_strong(currentTail, node)) {
            break;
        }
    }
}

TaskNode* TaskNodeList::Devastate() {
    while (true) {
        auto currentTail = myTail.load(std::memory_order_seq_cst);
        if (myTail.compare_exchange_strong(currentTail, nullptr)) {
            return currentTail;
        }
    }
}

bool TaskNodeList::IsEmpty() {
    return myTail.load(std::memory_order_seq_cst) == nullptr;
}

TaskNodeList::~TaskNodeList() {
    auto tail = myTail.load(std::memory_order_seq_cst);
    while (tail != nullptr) {
        auto current = tail;
        tail = current->GetPrevious();
        delete current;
    }
}

void ThreadPoolThread::Start() {
    if (myThread == nullptr) {
        myIsActive.store(true, std::memory_order_release);
        myThread = new std::thread([=]() {
            while (true) {
                if (myWasToldToShutdown.load(std::memory_order_acquire) && myTasks->IsEmpty()) {
                    myIsActive.store(false, std::memory_order_release);
                    return;
                }

                auto task = myTasks->Devastate();
                if (task != nullptr) {
                    BoolAtomicCookie cookie(&myIsProcessingTasks, true);

                    while (task != nullptr) {
                        volatile int x = 0;
                        RegisterContext registerContext{};
                        FillContext(&registerContext);

                        if (x == 0) {
                            ++x;
                            task->GetTask().Execute(registerContext);
                        }

                        auto currentTask = task;
                        task = task->GetPrevious();
                        delete currentTask;
                    }
                }
            }
        });

        myThread->detach();
    }
}

ThreadPoolThread::ThreadPoolThread() {
    myTasks = new TaskNodeList();
}

void ThreadPoolThread::Shutdown() {
    myWasToldToShutdown.store(true, std::memory_order_release);
}

bool ThreadPoolThread::QueueTask(const Task& task) {
    myTasks->PushTask(task);
    return true;
}

void ThreadPoolThread::WaitForRemainingTasksCompletion() {
    while (myIsActive.load(std::memory_order_acquire)) {
        if (!myIsActive.load(std::memory_order_relaxed)) {
            return;
        }
    }
}

ThreadPoolThread::~ThreadPoolThread() {
    delete myTasks;
    delete myThread;
}
