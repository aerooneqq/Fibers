#include "Task.h"

#include <utility>

std::string Task::GetName()
{
    return myName;
}

bool Task::IsCompleted()
{
    return myIsCompleted.load(std::memory_order_seq_cst);
}

Task::Task(std::string name, TaskJobFunction* job) : myName(std::move(name))
{
    myJob = job;
}

Task::Task(const Task &other)
{
    myJob = other.myJob;
}

void Task::Execute()
{
    (*myJob)(myController);
}


void TaskController::Yield()
{

}

void TaskController::WithExclusiveAccess(Lock* lock, PureJobFunction job)
{

}

void TaskController::Cancel()
{

}

void TaskController::NestedParallel(const std::vector<Task>& tasks)
{

}

void TaskController::NestedSequential(const std::vector<Task>& tasks)
{

}