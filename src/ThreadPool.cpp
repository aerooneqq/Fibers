#include <thread>
#include "ThreadPool.h"

void ThreadPool::Schedule(const Task& task)
{
    mySingleThread->QueueTask(task);
}

void ThreadPool::Stop()
{
    mySingleThread->Stop();
    myIsActive.store(false, std::memory_order_seq_cst);
}

void ThreadPool::WaitForAllTasksCompletion()
{

}

void ThreadPool::Start()
{
    mySingleThread = new ThreadPoolThread();
    mySingleThread->Start();
    myIsActive.store(true, std::memory_order_seq_cst);
}

ThreadPool::~ThreadPool()
{

}

ThreadPool::ThreadPool()
{
    mySingleThread = new ThreadPoolThread();
    myIsActive = new std::atomic<bool>(false);
}

bool ThreadPool::IsActive()
{
    return myIsActive.load(std::memory_order_seq_cst);
}

TaskNode* TaskNode::GetPrevious()
{
    return myPrevious;
}

Task TaskNode::GetTask()
{
    return myTask;
}

TaskNode::TaskNode(const Task& task) : myTask(task)
{
}

TaskNode* TaskNode::SetPrevious(TaskNode* previous)
{
    myPrevious = previous;
}

void TaskNodeList::PushTask(const Task& task)
{
    auto node = new TaskNode(task);
    while (true)
    {
        auto currentTail = myTail.load(std::memory_order_seq_cst);
        node->SetPrevious(currentTail);
        if (myTail.compare_exchange_strong(currentTail, node))
        {
            break;
        }
    }
}

TaskNode* TaskNodeList::Devastate()
{
    while (true)
    {
        auto currentTail = myTail.load(std::memory_order_seq_cst);
        if (myTail.compare_exchange_strong(currentTail, nullptr))
        {
            return currentTail;
        }
    }
}

void ThreadPoolThread::Start()
{
    if (myThread == nullptr)
    {
        myIsActive.store(true, std::memory_order_seq_cst);
        myThread = new std::thread([=]()
        {
            while (true)
            {
                if (!myIsActive.load(std::memory_order_seq_cst))
                {
                    return;
                }

                auto task = myTasks->Devastate();
                if (task != nullptr)
                {
                    BoolAtomicCookie cookie(&myIsProcessingTasks, true);
                    {
                        while (task != nullptr)
                        {
                            task->GetTask().Execute();
                            task = task->GetPrevious();
                        }
                    }
                }
            }
        });

        myThread->detach();
    }
}

ThreadPoolThread::ThreadPoolThread()
{
    myTasks = new TaskNodeList();
}

void ThreadPoolThread::Stop()
{
    myIsActive.store(false, std::memory_order_seq_cst);
}

void ThreadPoolThread::QueueTask(const Task& task)
{
    myTasks->PushTask(task);
}
